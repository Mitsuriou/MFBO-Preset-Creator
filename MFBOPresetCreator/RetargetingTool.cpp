#include "RetargetingTool.h"
#include "BodySlideFiltersEditor.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Enum.h"
#include "LineEdit.h"
#include "SliderFileBuilder.h"
#include "TargetMeshesPicker.h"
#include "Utils.h"
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QLabel>
#include <QProgressBar>
#include <QProgressDialog>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QTextStream>

RetargetingTool::RetargetingTool(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, tr("BodySlide Presets' Retargeting"), "arrow-up", aSettings, aLastPaths, 800)
  , mFileWatcher(new QFileSystemWatcher())
  , mTargetBodyMesh(aSettings.presetsRetargeting.defaultBodyFeet.bodyMesh)
  , mTargetFeetMesh(aSettings.presetsRetargeting.defaultBodyFeet.feetMesh)
{
  // Build the window's interface
  this->initializeGUI();

  QObject::connect(this->mFileWatcher, &QFileSystemWatcher::directoryChanged, this, &RetargetingTool::updateBackupPreview);

  this->mHasUserDoneSomething = false;

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.display.bodySlidePresetsRetargetingDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

RetargetingTool::~RetargetingTool()
{
  this->mFileWatcher->removePaths(this->mFileWatcher->files());
  delete this->mFileWatcher;
}

void RetargetingTool::closeEvent(QCloseEvent* aEvent)
{
  if (!this->mHasUserDoneSomething)
  {
    aEvent->accept();
    emit modalClosed();
    return;
  }

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    this->getThemedResourcePath(),
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the retargeting tool window"),
                                    "",
                                    this->settings().display.dangerColor,
                                    this->settings().display.successColor,
                                    "",
                                    false)
      == ButtonClicked::YES)
  {
    aEvent->accept();
  }
  else
  {
    aEvent->ignore();
  }

  emit modalClosed();
}

void RetargetingTool::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QGridLayout(this)};
  lMainVertical->setRowStretch(0, 0);
  lMainVertical->setRowStretch(1, 0);
  lMainVertical->setRowStretch(2, 2);
  this->setupInterface(*lMainVertical);

  this->getCentralWidget()->setLayout(lMainVertical);
}

void RetargetingTool::setupInterface(QGridLayout& aLayout)
{
  // General group box
  auto lGeneralGroupBox{ComponentFactory::CreateGroupBox(this, tr("General"), "tune", this->getThemedResourcePath(), this->settings().display.font.pointSize)};
  aLayout.addWidget(lGeneralGroupBox, 0, 0);

  // Grid layout
  auto lGeneralGridLayout{new QGridLayout(lGeneralGroupBox)};
  lGeneralGridLayout->setSpacing(10);
  lGeneralGridLayout->setContentsMargins(15, 20, 15, 15);
  lGeneralGridLayout->setAlignment(Qt::AlignTop);

  // Targeted body and version
  auto lTargetMeshesPicker{ComponentFactory::CreateTargetMeshesPickerLine(this,
                                                                          *lGeneralGridLayout,
                                                                          true,
                                                                          0,
                                                                          this->getThemedResourcePath(),
                                                                          QString("target_meshes_picker_button"),
                                                                          QString("currently_targeted_body_feet"))};

  // Input path
  lGeneralGridLayout->addWidget(new QLabel(tr("Input path:"), this), 1, 0);

  auto lInputPathLineEdit{new LineEdit(this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setObjectName(QString("input_path_directory"));
  lGeneralGridLayout->addWidget(lInputPathLineEdit, 1, 1);

  auto lInputPathChooser{ComponentFactory::CreateButton(this, tr("Choose a directory..."), "", "folder", this->getThemedResourcePath(), "", false, true)};
  lGeneralGridLayout->addWidget(lInputPathChooser, 1, 2);

  // Filters
  lGeneralGridLayout->addWidget(new QLabel(tr("BodySlide filters:"), this), 2, 0);

  auto lFiltersWrapper{new QHBoxLayout(lGeneralGroupBox)};
  lFiltersWrapper->setContentsMargins(0, 0, 0, 0);
  lGeneralGridLayout->addLayout(lFiltersWrapper, 2, 1, 1, 2);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  lFiltersListChooser->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
  lFiltersWrapper->addWidget(lFiltersListChooser);

  auto lFiltersList{new QLabel("", this)};
  lFiltersList->setObjectName(QString("bodyslide_filters"));
  lFiltersList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  lFiltersList->setWordWrap(true);
  lFiltersWrapper->addWidget(lFiltersList);

  auto lEditFilters{ComponentFactory::CreateButton(this, tr("Edit BodySlide filters sets"), "", "filter", this->getThemedResourcePath(), "edit_filters")};
  lFiltersWrapper->addWidget(lEditFilters);

  // Backup group box
  auto lBackupGroupBox{ComponentFactory::CreateGroupBox(this, tr("Backup"), "restore", this->getThemedResourcePath(), this->settings().display.font.pointSize)};
  aLayout.addWidget(lBackupGroupBox, 1, 0);

  // Grid layout
  auto lBackupGridLayout{new QGridLayout(lBackupGroupBox)};
  lBackupGridLayout->setSpacing(10);
  lBackupGridLayout->setContentsMargins(15, 20, 15, 15);
  lBackupGridLayout->setAlignment(Qt::AlignTop);

  // Keep backup checkbox
  lBackupGridLayout->addWidget(new QLabel(tr("Keep a backup?"), this), 0, 0);

  auto lKeepBackup{ComponentFactory::CreateCheckBox(this, tr("You should always check this box to avoid any data loss or corruption"), "", "keep_backup")};
  lBackupGridLayout->addWidget(lKeepBackup, 0, 1, 1, 2);

  // Backup directory path
  auto lBackupPathLabel{new QLabel(tr("Backup directory path:"), this)};
  lBackupPathLabel->setObjectName(QString("backup_path_label"));
  lBackupGridLayout->addWidget(lBackupPathLabel, 1, 0);

  auto lBackupPathLineEdit{new LineEdit(this)};
  lBackupPathLineEdit->setReadOnly(true);
  lBackupPathLineEdit->setObjectName(QString("backup_path_directory"));
  lBackupGridLayout->addWidget(lBackupPathLineEdit, 1, 1);

  auto lBackupPathChooser{ComponentFactory::CreateButton(this,
                                                         tr("Choose a directory..."),
                                                         "",
                                                         "folder",
                                                         this->getThemedResourcePath(),
                                                         "backup_dir_chooser",
                                                         false,
                                                         true)};
  lBackupGridLayout->addWidget(lBackupPathChooser, 1, 2);

  // Backup subdirectory name/path
  auto lLabelSubDirectoryBackupPath{new QLabel(tr("Backup subdirectory name/path:"), this)};
  lLabelSubDirectoryBackupPath->setObjectName(QString("backup_subdir_label"));
  lBackupGridLayout->addWidget(lLabelSubDirectoryBackupPath, 2, 0);

  auto lBackupSubpathLineEdit{new LineEdit(this)};
  lBackupSubpathLineEdit->setObjectName(QString("backup_path_subdirectory"));
  lBackupGridLayout->addWidget(lBackupSubpathLineEdit, 2, 1);

  // Backup preview
  auto lBackupPathPreviewLabel{new QLabel(tr("Preview:"), this)};
  lBackupPathPreviewLabel->setObjectName(QString("backup_path_preview_label"));
  lBackupPathPreviewLabel->setAlignment(Qt::AlignTop);
  lBackupGridLayout->addWidget(lBackupPathPreviewLabel, 3, 0);

  auto lBackupPathsPreview{new QLabel("", this)};
  lBackupPathsPreview->setObjectName(QString("backup_path_preview"));
  lBackupPathsPreview->setAlignment(Qt::AlignTop);
  lBackupGridLayout->addWidget(lBackupPathsPreview, 3, 1, 1, 2);

  // Generate button
  auto lGenerateButton{ComponentFactory::CreateButton(this,
                                                      tr("Retarget all the files under the input path"),
                                                      "",
                                                      "arrow-up",
                                                      this->getThemedResourcePath(),
                                                      "objectname",
                                                      false,
                                                      true)};
  aLayout.addWidget(lGenerateButton, 2, 0, Qt::AlignBottom);

  // Pre-bind initialization functions
  this->targetMeshesChanged(this->mTargetBodyMesh, this->mTargetFeetMesh);

  // Event binding
  QObject::connect(lTargetMeshesPicker, &QPushButton::clicked, this, &RetargetingTool::openTargetMeshesPicker);
  QObject::connect(lInputPathChooser, &QPushButton::clicked, this, &RetargetingTool::chooseInputDirectory);
  QObject::connect(lKeepBackup, &QCheckBox::stateChanged, this, &RetargetingTool::updateBackupState);
  lKeepBackup->setChecked(true);

  QObject::connect(lBackupPathChooser, &QPushButton::clicked, this, &RetargetingTool::chooseBackupDirectory);
  QObject::connect(lBackupSubpathLineEdit, &QLineEdit::textChanged, this, &RetargetingTool::updateBackupPreview);
  QObject::connect(lGenerateButton, &QPushButton::clicked, this, &RetargetingTool::launchUpDownGradeProcess);
  QObject::connect(lFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &RetargetingTool::updateBodySlideFiltersListPreview);
  QObject::connect(lEditFilters, &QPushButton::clicked, this, &RetargetingTool::openBodySlideFiltersEditor);

  // Post-bind initialization functions
  this->initBodySlideFiltersList();
}

void RetargetingTool::userHasDoneAnAction()
{
  if (!this->mHasUserDoneSomething)
  {
    this->mHasUserDoneSomething = true;
  }
}

void RetargetingTool::userHasDoneAnAction(int)
{
  this->mHasUserDoneSomething = true;

  // Body name selector
  auto lBodyNameSelector{this->findChild<QComboBox*>(QString("body_selector_name"))};
  QObject::disconnect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&RetargetingTool::userHasDoneAnAction));

  // Body version selector
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("body_selector_version"))};
  QObject::disconnect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&RetargetingTool::userHasDoneAnAction));
}

void RetargetingTool::chooseInputDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->lastPaths(),
                                                 "retargetingToolInput",
                                                 lLineEdit->text(),
                                                 this->settings().general.eachButtonSavesItsLastUsedPath)};
  const auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->lastPaths(), "retargetingToolInput", lPath);

  if (!this->mHasUserDoneSomething && lPath.size() > 0)
  {
    this->userHasDoneAnAction();
  }

  this->updateBackupPreview();
}

void RetargetingTool::chooseBackupDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("backup_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->lastPaths(),
                                                 "retargetingToolOutput",
                                                 lLineEdit->text(),
                                                 this->settings().general.eachButtonSavesItsLastUsedPath)};
  const auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->lastPaths(), "retargetingToolOutput", lPath);
  this->updateBackupPreview();

  if (!this->mHasUserDoneSomething && lPath.size() > 0)
  {
    this->userHasDoneAnAction();
  }
}

void RetargetingTool::updateBackupState(int aState)
{
  auto lBackupPathLabel{this->findChild<QLabel*>(QString("backup_path_label"))};
  auto lBackupPathLineEdit{this->findChild<QLineEdit*>(QString("backup_path_directory"))};
  auto lBackupPathChooser{this->findChild<QPushButton*>(QString("backup_dir_chooser"))};
  auto lLabelSubDirectoryBackupPath{this->findChild<QLabel*>(QString("backup_subdir_label"))};
  auto lBackupSubpathLineEdit{this->findChild<QLineEdit*>(QString("backup_path_subdirectory"))};
  auto lOutputPathsPreviewLabel{this->findChild<QLabel*>(QString("backup_path_preview_label"))};
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("backup_path_preview"))};

  switch (aState)
  {
    case Qt::Unchecked:
      lOutputPathsPreview->setStyleSheet("");

      lBackupPathLabel->setDisabled(true);
      lBackupPathLineEdit->setDisabled(true);
      lBackupPathChooser->setDisabled(true);
      lLabelSubDirectoryBackupPath->setDisabled(true);
      lBackupSubpathLineEdit->setDisabled(true);
      lOutputPathsPreviewLabel->setDisabled(true);
      lOutputPathsPreview->setDisabled(true);
      break;
    case Qt::Checked:
      lBackupPathLabel->setDisabled(false);
      lBackupPathLineEdit->setDisabled(false);
      lBackupPathChooser->setDisabled(false);
      lLabelSubDirectoryBackupPath->setDisabled(false);
      lBackupSubpathLineEdit->setDisabled(false);
      lOutputPathsPreviewLabel->setDisabled(false);
      lOutputPathsPreview->setDisabled(false);

      this->updateBackupPreview();
  }
}

void RetargetingTool::updateBackupPreview()
{
  // Check if the user has typed in the output subdir qlineedit
  auto lEventSource{qobject_cast<QLineEdit*>(this->sender())};
  auto lSubdirLineEdit{this->findChild<QLineEdit*>(QString("backup_path_subdirectory"))};

  if (lEventSource == lSubdirLineEdit)
  {
    this->userHasDoneAnAction();
  }

  // Get main directory
  auto lMainDirTextEdit{this->findChild<QLineEdit*>(QString("backup_path_directory"))};
  auto lMainDirectory{lMainDirTextEdit->text().trimmed()};
  Utils::CleanPathString(lMainDirectory);

  // Get subdirectory
  auto lSubDirectory{this->findChild<QLineEdit*>(QString("backup_path_subdirectory"))->text().trimmed()};
  Utils::CleanPathString(lSubDirectory);
  auto lIsValidPath{true};

  // Construct full path
  QString lFullPath;
  if (!lMainDirectory.isEmpty() && !lSubDirectory.isEmpty())
  {
    lFullPath = lMainDirectory + "/" + lSubDirectory;
    lMainDirTextEdit->setDisabled(false);
  }
  else if (!lMainDirectory.isEmpty() && lSubDirectory.isEmpty())
  {
    lFullPath = lMainDirectory;
    lMainDirTextEdit->setDisabled(false);
  }
  else
  {
    lFullPath = tr("No path given or invalid path given.");
    lMainDirTextEdit->setDisabled(true);
    lIsValidPath = false;
  }

  // Clear all the watched paths
  this->mFileWatcher->removePaths(this->mFileWatcher->files());

  // Set the full path value in the preview label
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("backup_path_preview"))};
  auto lNewTextColor{this->settings().display.successColor};

  if (lIsValidPath)
  {
    const auto lInputPath{Utils::CleanPathString(this->findChild<QLineEdit*>(QString("input_path_directory"))->text())};
    const auto lFullPathConst{lFullPath};

    // Check if the user is trying to backup the directory into itself
    if (Utils::CleanPathString(lFullPathConst).compare(lInputPath, Qt::CaseInsensitive) == 0
        || Utils::CleanPathString(lFullPathConst + QDir::separator()).startsWith(Utils::CleanPathString(lInputPath + QDir::separator()), Qt::CaseInsensitive))
    {
      lNewTextColor = this->settings().display.dangerColor;
    }
    // Check if the wanted backup directory already exists
    else if (QDir(lFullPath).exists())
    {
      lNewTextColor = this->settings().display.warningColor;
    }

    // Add a new path to watch to
    this->mFileWatcher->addPath(lFullPath);
  }
  else
  {
    lNewTextColor = this->settings().display.dangerColor;
  }

  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lFullPath);
}

void RetargetingTool::launchUpDownGradeProcess()
{
  // Input Directory
  auto lRootDir{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};

  // Check if the input path has been given by the user
  if (lRootDir.isEmpty())
  {
    Utils::DisplayWarningMessage(tr("Error: no path path given for the retargeting."));
    return;
  }

  // Manage the backup part
  auto lMustKeepBackup{this->findChild<QCheckBox*>(QString("keep_backup"))->isChecked()};

  if (lMustKeepBackup)
  {
    // Backup paths
    auto lMainDirectory{this->findChild<QLineEdit*>(QString("backup_path_directory"))->text().trimmed()};
    auto lSubDirectory{this->findChild<QLineEdit*>(QString("backup_path_subdirectory"))->text().trimmed()};
    Utils::CleanPathString(lSubDirectory);

    // Full extract path
    const auto& lDirtyFullBackupDirectory{(lSubDirectory.isEmpty() ? lMainDirectory : (lMainDirectory + QDir::separator() + lSubDirectory))};
    auto lFullBackupDirectory{Utils::CleanPathString(lDirtyFullBackupDirectory)};

    // Check if the full extract path has been given by the user
    if (lFullBackupDirectory.isEmpty())
    {
      Utils::DisplayWarningMessage(tr("Error: no path given to backup the files."));
      return;
    }

    // Check if the user is trying to backup the directory into itself
    auto lInputPath{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};
    const auto lFullPathConst{lFullBackupDirectory};

    // if (lFullBackupDirectory.startsWith())
    if (Utils::CleanPathString(lFullPathConst).compare(lInputPath, Qt::CaseInsensitive) == 0
        || Utils::CleanPathString(lFullPathConst + QDir::separator()).startsWith(Utils::CleanPathString(lInputPath + QDir::separator()), Qt::CaseInsensitive))
    {
      Utils::DisplayWarningMessage(tr("Error: it is not possible to backup a directory inside itself. Choose another backup location."));
      return;
    }

    // Check if the path could be valid
    if (lFullBackupDirectory.startsWith("/"))
    {
      Utils::DisplayWarningMessage(tr("Error: the path given to backup the files seems to be invalid."));
      return;
    }

    // Display a message to the user that the backup is being created
    QDialog lDialog(this);
    QVBoxLayout lLayout;
    lDialog.setLayout(&lLayout);
    QLabel lLabel(tr("The backup is currently being created. Depending on your computer, it can take a certain time. Please wait..."), this);
    lLayout.addWidget(&lLabel);
    lDialog.show();

    QCoreApplication::processEvents();

    // Copy the directory and its content
    if (!Utils::CopyRecursively(lRootDir, lFullBackupDirectory))
    {
      Utils::DisplayWarningMessage(tr("Error: the backup could not be created. Please try again."));
      return;
    }
  }

  // Scan the number of files to treat
  auto lNumberOSPFiles{Utils::GetNumberFilesByExtensionRecursiveIgnoringFOMOD(lRootDir, "*.osp")};
  auto lNumberXMLFiles{Utils::GetNumberFilesByExtensionRecursiveIgnoringFOMOD(lRootDir, "*.xml")};
  auto lTreatedFiles{0};

  // Progress bar
  auto lProgressBar{new QProgressBar(this)};
  lProgressBar->setFormat("%v / %m");
  lProgressBar->setRange(0, lNumberXMLFiles);
  lProgressBar->setValue(0);
  lProgressBar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Parsing XML files. Please wait..."), tr("Cancel treatment"), 0, 0, this);
  lProgressDialog.setBar(lProgressBar);
  lProgressDialog.setModal(true);
  lProgressDialog.show();

  // Iterate through all the files
  std::vector<QPair<QString, QString>> lNamesBuffer;

  QString lAbsFilePath;
  QString lRelativeDirs;
  std::vector<Struct::SliderSet> lParsedSliderSets;

  lTreatedFiles = 0;

  QDirIterator it(lRootDir, QStringList() << QString("*.xml"), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::DisplayWarningMessage(tr("Process aborted by the user."));
      return;
    }

    // Navigate to the next file
    it.next();

    // Ignore FOMOD directory
    lAbsFilePath = it.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.mid(0).remove(lRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("/fomod/info.xml", Qt::CaseInsensitive) || lRelativeDirs.contains("/fomod/ModuleConfig.xml", Qt::CaseInsensitive))
    {
      continue;
    }

    // Build the names buffer
    lNamesBuffer.push_back(QPair<QString, QString>(it.fileInfo().completeBaseName(), Utils::GetPresetNameFromXMLFile(lAbsFilePath)));

    if (lNamesBuffer.at(lNamesBuffer.size() - 1).second.isEmpty())
    {
      Utils::DisplayWarningMessage(tr("Error while trying to parse the XML file \"%1\". Aborting process.").arg(lAbsFilePath));
      return;
    }

    lProgressDialog.setValue(++lTreatedFiles);
    QCoreApplication::processEvents();
  }

  lProgressDialog.setLabelText(tr("Parsing and patching OSP files. Please wait..."));
  lProgressBar->setRange(0, lNumberOSPFiles);
  lTreatedFiles = 0;

  std::vector<QPair<QString, QString>> lOSPBuffer;
  for (const auto& lPair : lNamesBuffer)
  {
    lOSPBuffer.push_back(lPair);
  }

  std::map<QString, unsigned char> lOSPUsedSliders;

  QDirIterator it2(lRootDir, QStringList() << QString("*.osp"), QDir::Files, QDirIterator::Subdirectories);
  while (it2.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::DisplayWarningMessage(tr("Process aborted by the user."));
      return;
    }

    // Navigate to the next file
    it2.next();

    lAbsFilePath = it2.fileInfo().absoluteFilePath();

    auto lMustUseBeastHands{Utils::IsPresetUsingBeastHands(lAbsFilePath)};

    // Check the file extension
    const auto lFileName{it2.fileInfo().completeBaseName()};

    lParsedSliderSets = Utils::ReadOSPFileInformation(lAbsFilePath);

    if (lParsedSliderSets.size() == 0 || lParsedSliderSets.size() > 3)
    {
      Utils::DisplayWarningMessage(tr("Error while trying to parse the OSP file \"%1\". Aborting process.").arg(lAbsFilePath));
      return;
    }

    // Search for the preset name in the buffer
    QString lPresetName;
    auto lBufferLocationToRemove{-1};

    auto lOSPBufSize{static_cast<int>(lOSPBuffer.size())};
    for (int i = 0; i < lOSPBufSize; i++)
    {
      if (lFileName == lOSPBuffer.at(i).first)
      {
        lPresetName = lOSPBuffer.at(i).second;
        lBufferLocationToRemove = i;
        break;
      }
    }

    if (lPresetName.isEmpty())
    {
      Utils::DisplayWarningMessage(tr("No data found from the associated XML file. The file \"%1\" was not modified.").arg(it2.fileInfo().absoluteFilePath()));
      continue;
    }

    // Check, if the preset is using beast hands, if the chosen body does not support beast hands
    auto lSkipBeastHands{false};
    if (lMustUseBeastHands && !Utils::IsBodySupportingBeastHands(this->mTargetBodyMesh))
    {
      Utils::DisplayWarningMessage(tr("The chosen body/version does not support beast hands. The retargeting of the OSP file \"%1\" has been skipped.").arg(it2.fileInfo().absoluteFilePath()));
      lSkipBeastHands = true;
    }

    // If the preset is using beast hands but the chosen body does not support beast hands, skip this OSP file's treatment
    if (!lSkipBeastHands)
    {
      // Remove the file once all data has been read
      if (QFile::exists(lAbsFilePath))
      {
        QFile::remove(lAbsFilePath);
      }

      // Construct the file content
      unsigned char lOptions{0};
      for (const auto& lSliderSet : lParsedSliderSets)
      {
        switch (lSliderSet.getMeshPart())
        {
          case MeshPartType::UNKNOWN:
            break;
          case MeshPartType::BODY:
            lOptions += 100;
            break;
          case MeshPartType::FEET:
            lOptions += 10;
            break;
          case MeshPartType::HANDS:
          case MeshPartType::BEAST_HANDS:
            lOptions += 1;
            break;
        }
      }
      lOSPUsedSliders.insert(std::make_pair(lFileName, lOptions));

      auto lOSPFileContent{SliderFileBuilder::BuildOSPFileContent(lPresetName, this->mTargetBodyMesh, this->mTargetFeetMesh, lMustUseBeastHands, lOptions)};

      // Fill the custom variables
      for (const auto& lSliderSet : lParsedSliderSets)
      {
        switch (lSliderSet.getMeshPart())
        {
          case MeshPartType::UNKNOWN:
            break;
          case MeshPartType::BODY:
            lOSPFileContent.replace(QString("{%%body_output_path%%}"), lSliderSet.getOutputPath());
            lOSPFileContent.replace(QString("{%%body_output_file%%}"), lSliderSet.getOutputFile());
            break;
          case MeshPartType::FEET:
            lOSPFileContent.replace(QString("{%%feet_output_path%%}"), lSliderSet.getOutputPath());
            lOSPFileContent.replace(QString("{%%feet_output_file%%}"), lSliderSet.getOutputFile());
            break;
          case MeshPartType::HANDS:
          case MeshPartType::BEAST_HANDS:
            lOSPFileContent.replace(QString("{%%hands_output_path%%}"), lSliderSet.getOutputPath());
            lOSPFileContent.replace(QString("{%%hands_output_file%%}"), lSliderSet.getOutputFile());
            break;
        }
      }

      // Create the OSP file on disk
      QFile lOSPFile(lAbsFilePath);
      if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
      {
        QTextStream lTextStream(&lOSPFile);
        lTextStream << lOSPFileContent;
        lTextStream.flush();

        lOSPFile.close();
      }
      else
      {
        Utils::DisplayWarningMessage(tr("Error while trying to create the OSP file \"%1\". Aborting process.").arg(lAbsFilePath));
        return;
      }

      if (lBufferLocationToRemove != -1)
      {
        lOSPBuffer.erase(lOSPBuffer.begin() + lBufferLocationToRemove);
      }
    }

    lProgressDialog.setValue(++lTreatedFiles);
    QCoreApplication::processEvents();
  }

  lProgressDialog.setLabelText(tr("Patching XML files. Please wait..."));
  lProgressBar->setRange(0, lNumberOSPFiles);
  lTreatedFiles = 0;

  QDirIterator it3(lRootDir, QStringList() << QString("*.xml"), QDir::Files, QDirIterator::Subdirectories);
  while (it3.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::DisplayWarningMessage(tr("Process aborted by the user."));
      return;
    }

    // Navigate to the next file
    it3.next();

    // Ignore FOMOD directory
    lAbsFilePath = it3.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.mid(0).remove(lRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("/fomod/info.xml", Qt::CaseInsensitive) || lRelativeDirs.contains("/fomod/ModuleConfig.xml", Qt::CaseInsensitive))
    {
      continue;
    }

    auto lMustUseBeastHands{Utils::IsPresetUsingBeastHands(lAbsFilePath)};

    // Check the file extension
    auto lFileName{it3.fileInfo().completeBaseName()};

    // Check if the OSP file has skiped the parsing
    auto lSkipXMLLastTreatment{false};
    auto lOSPBufSize{static_cast<int>(lOSPBuffer.size())};
    for (int i = 0; i < lOSPBufSize; i++)
    {
      if (lFileName == lOSPBuffer.at(i).first)
      {
        Utils::DisplayWarningMessage(tr("Since the associated OSP file has not been modified, the file \"%1\" has not been modified.").arg(it3.fileInfo().absoluteFilePath()));
        lSkipXMLLastTreatment = true;
        break;
      }
    }

    if (lSkipXMLLastTreatment)
    {
      lProgressDialog.setValue(++lTreatedFiles);
      QCoreApplication::processEvents();
      continue;
    }

    // Searching for the preset name in the buffer
    QString lPresetName;
    auto lNamesBufSize{static_cast<int>(lNamesBuffer.size())};

    for (int i = 0; i < lNamesBufSize; i++)
    {
      if (lFileName == lNamesBuffer.at(i).first)
      {
        lPresetName = lNamesBuffer.at(i).second;
        break;
      }
    }

    // Remove the file once all data has been read
    if (QFile::exists(lAbsFilePath))
    {
      QFile::remove(lAbsFilePath);
    }

    // Construct the file content
    auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
    auto lUserFilters{Utils::GetFiltersForExport(this->mFiltersList,
                                                 lFiltersListChooser->itemText(lFiltersListChooser->currentIndex()),
                                                 this->mTargetBodyMesh,
                                                 this->mTargetFeetMesh)};
    auto lXMLFileContent{SliderFileBuilder::BuildXMLFileContent(lPresetName,
                                                                lUserFilters,
                                                                this->mTargetBodyMesh,
                                                                this->mTargetFeetMesh,
                                                                lMustUseBeastHands,
                                                                lOSPUsedSliders.find(lFileName)->second)};

    // Create the OSP file on disk
    QFile lXMLFile(lAbsFilePath);
    if (lXMLFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
      QTextStream lTextStream(&lXMLFile);
      lTextStream << lXMLFileContent;
      lTextStream.flush();

      lXMLFile.close();
    }
    else
    {
      Utils::DisplayWarningMessage(tr("Error while trying to create the XML file \"%1\". Aborting process.").arg(lAbsFilePath));
      return;
    }

    lProgressDialog.setValue(++lTreatedFiles);
    QCoreApplication::processEvents();
  }

  const auto& lTitle{tr("Retargeting successful")};
  const auto& lMessage{tr("All the files have been correctly retargeted. You can now close this window!")};

  // Open the directory where the file structure has been created
  if (this->settings().presetsRetargeting.automaticallyOpenFinalDirectory)
  {
    Utils::DisplayInfoMessage(this, lTitle, lMessage, "icons", "green-info", tr("Open the retargeted directory"));
    QDesktopServices::openUrl(QUrl::fromLocalFile(lRootDir));
  }
  else if (Utils::DisplayQuestionMessage(this,
                                         lTitle,
                                         lMessage,
                                         "icons",
                                         "green-info",
                                         tr("Open the retargeted directory"),
                                         tr("OK"),
                                         "",
                                         "",
                                         "",
                                         "",
                                         false)
           == ButtonClicked::YES)
  {
    QDesktopServices::openUrl(QUrl::fromLocalFile(lRootDir));
  }
}

void RetargetingTool::openTargetMeshesPicker()
{
  auto lDialog{new TargetMeshesPicker(this, this->settings(), this->lastPaths(), this->mTargetBodyMesh, this->mTargetFeetMesh)};
  QObject::connect(lDialog, &TargetMeshesPicker::valuesChosen, this, &RetargetingTool::targetMeshesChanged);
}

void RetargetingTool::targetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet)
{
  // Update the class members
  this->mTargetBodyMesh = aBody;
  this->mTargetFeetMesh = aFeet;

  // Update the "targeted body mesh" text content
  const auto lBodyText{
    QString("%1 [v.%2]").arg(DataLists::GetBodyVariantsList(DataLists::GetName(aBody), DataLists::GetVersionIndex(aBody)).at(DataLists::GetVariantIndex(aBody)), DataLists::GetVersionString(aBody))};

  // Update the "targeted feet mesh" text content
  const auto lFeetText{
    QString("%1 [v.%2]").arg(DataLists::GetFeetVariantsList(DataLists::GetName(aFeet), DataLists::GetVersionIndex(aFeet), Utils::IsCBBEBasedBody(aBody)).at(DataLists::GetVariantIndex(aFeet)), DataLists::GetVersionString(aBody, aFeet))};

  auto lCurrentlyTargetedBody{this->findChild<QLabel*>("currently_targeted_body_feet")};
  lCurrentlyTargetedBody->setText(tr("Targeted body: %1\nTargeted feet: %2").arg(lBodyText, lFeetText));

  // Force the refresh of the filters list preview
  this->updateBodySlideFiltersListPreview();
}

void RetargetingTool::openBodySlideFiltersEditor()
{
  auto lEditor{new BodySlideFiltersEditor(this, this->settings(), this->lastPaths(), this->mFiltersList)};
  QObject::connect(lEditor, &BodySlideFiltersEditor::listEdited, this, &RetargetingTool::updateBodySlideFiltersList);
}

void RetargetingTool::initBodySlideFiltersList()
{
  // Load and save the filters list
  this->mFiltersList = Utils::LoadFiltersFromFile();

  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};

  // Disable the combobox if there is not any available filter
  if (this->mFiltersList.size() == 0)
  {
    lFiltersListChooser->setDisabled(true);
    return;
  }

  // Fill the combobox
  for (const auto& lPair : this->mFiltersList)
  {
    lFiltersListChooser->addItem(lPair.first);
  }

  lFiltersListChooser->setCurrentIndex(0);
}

void RetargetingTool::updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList)
{
  // Do not update anything if the list is the same as the one already set
  if (this->mFiltersList == aFilterList)
  {
    return;
  }

  this->mFiltersList = aFilterList;
  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lFiltersList{this->findChild<QLabel*>(QString("bodyslide_filters"))};
  Utils::UpdateComboBoxBodyslideFiltersList(this->mFiltersList, lFiltersListChooser, lFiltersList);
}

void RetargetingTool::updateBodySlideFiltersListPreview()
{
  // Get the GUI widgets
  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lFiltersList{this->findChild<QLabel*>(QString("bodyslide_filters"))};

  // Get any eventual additional filters
  auto lAdditionalFilter{Utils::GetAdditionalFeetFilter(this->mTargetBodyMesh, this->mTargetFeetMesh)};
  QString lText;

  if (lFiltersListChooser->currentIndex() != -1)
  {
    lText = this->mFiltersList.find(lFiltersListChooser->itemText(lFiltersListChooser->currentIndex()))->second.join(QString(" ; "));

    if (!lAdditionalFilter.isEmpty())
    {
      lText.append(" ; ");
      lText.append(lAdditionalFilter);
      lText.append(tr(" (feet only)"));
    }
  }

  lFiltersList->setText(lText);

  this->userHasDoneAnAction();
}
