#include "RetargetingTool.h"
#include "BodySlideFiltersEditor.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Enum.h"
#include "SliderFileBuilder.h"
#include "Utils.h"
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QTextStream>

RetargetingTool::RetargetingTool(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  this->mHasUserDoneSomething = false;

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.bodySlidePresetsRetargetingDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void RetargetingTool::closeEvent(QCloseEvent* aEvent)
{
  if (!this->mHasUserDoneSomething)
  {
    aEvent->accept();
    emit modalClosed();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the retargeting tool window"),
                                    "",
                                    this->mSettings.dangerColor,
                                    this->mSettings.successColor,
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

void RetargetingTool::reject()
{
  this->close();
}

void RetargetingTool::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("BodySlide Presets' Retargeting"));
  this->setWindowIcon(QIcon(QPixmap(":/black/arrow-up")));
}

void RetargetingTool::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QGridLayout(this)};
  lMainVertical->setRowStretch(0, 0);
  lMainVertical->setRowStretch(1, 0);
  lMainVertical->setRowStretch(2, 2);
  this->setupInterface(*lMainVertical);
}

void RetargetingTool::setupInterface(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // General group box
  auto lGeneralGroupBox{new QGroupBox(tr("General").append("  "), this)};
  Utils::AddIconToGroupBox(lGeneralGroupBox, lIconFolder, "tune", this->mSettings.font.size);
  this->connect(lGeneralGroupBox, &QGroupBox::toggled, this, &RetargetingTool::groupBoxChecked);
  Utils::SetGroupBoxState(lGeneralGroupBox, false);
  aLayout.addWidget(lGeneralGroupBox, 0, 0);

  // Grid layout
  auto lGeneralGridLayout{new QGridLayout(lGeneralGroupBox)};
  lGeneralGridLayout->setSpacing(10);
  lGeneralGridLayout->setContentsMargins(15, 20, 15, 15);
  lGeneralGridLayout->setAlignment(Qt::AlignTop);

  // Targeted body and version
  auto lDefaultBodyVersionSettings{DataLists::GetSplittedNameVersionFromBodyVersion(mSettings.defaultRetargetingToolBody)};

  lGeneralGridLayout->addWidget(new QLabel(tr("Targeted body and version:"), this), 0, 0);

  auto lBodyNameVersionWrapper{new QHBoxLayout(lGeneralGroupBox)};
  lBodyNameVersionWrapper->setMargin(0);
  lGeneralGridLayout->addLayout(lBodyNameVersionWrapper, 0, 1, 1, 2);

  // Body Name
  auto lBodyNameSelector{new QComboBox(this)};
  lBodyNameSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyNameSelector->setCursor(Qt::PointingHandCursor);
  lBodyNameSelector->addItems(DataLists::GetBodiesNames());
  lBodyNameSelector->setCurrentIndex(lDefaultBodyVersionSettings.first);
  lBodyNameSelector->setObjectName(QString("body_selector_name"));
  lBodyNameVersionWrapper->addWidget(lBodyNameSelector);

  // Body Version
  auto lBodyVersionSelector{new QComboBox(this)};
  lBodyVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyVersionSelector->setCursor(Qt::PointingHandCursor);
  lBodyVersionSelector->addItems(DataLists::GetVersionsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lBodyVersionSelector->setCurrentIndex(lDefaultBodyVersionSettings.second);
  lBodyVersionSelector->setObjectName(QString("body_selector_version"));
  lBodyNameVersionWrapper->addWidget(lBodyVersionSelector);

  // Feet mod chooser
  auto lFeetSelector{new QComboBox(this)};
  lFeetSelector->setItemDelegate(new QStyledItemDelegate());
  lFeetSelector->setCursor(Qt::PointingHandCursor);
  lFeetSelector->addItems(DataLists::GetFeetModsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lFeetSelector->setCurrentIndex(mSettings.defaultMainFeetMod);
  lFeetSelector->setObjectName(QString("feet_mod_selector"));
  lBodyNameVersionWrapper->addWidget(lFeetSelector);

  lBodyNameVersionWrapper->addStretch();

  // Input path
  lGeneralGridLayout->addWidget(new QLabel(tr("Input path:"), this), 1, 0);

  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setObjectName(QString("input_path_directory"));
  lGeneralGridLayout->addWidget(lInputPathLineEdit, 1, 1);

  auto lInputPathChooser{ComponentFactory::CreateButton(this, tr("Choose a directory..."), "", "folder", lIconFolder, "", false, true)};
  lGeneralGridLayout->addWidget(lInputPathChooser, 1, 2);

  // Filters
  lGeneralGridLayout->addWidget(new QLabel(tr("BodySlide filters:"), this), 2, 0);

  auto lFiltersWrapper{new QHBoxLayout(lGeneralGroupBox)};
  lFiltersWrapper->setMargin(0);
  lGeneralGridLayout->addLayout(lFiltersWrapper, 2, 1, 1, 2);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  lFiltersWrapper->addWidget(lFiltersListChooser);

  auto lFiltersList{new QLabel("", this)};
  lFiltersList->setObjectName(QString("bodyslide_filters"));
  lFiltersList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  lFiltersList->setWordWrap(true);
  lFiltersWrapper->addWidget(lFiltersList);

  auto lEditFilters{ComponentFactory::CreateButton(this, tr("Edit BodySlide filters sets"), "", "filter", lIconFolder, "edit_filters")};
  lFiltersWrapper->addWidget(lEditFilters);

  // Backup group box
  auto lBackupGroupBox{new QGroupBox(tr("Backup").append("  "), this)};
  Utils::AddIconToGroupBox(lBackupGroupBox, lIconFolder, "restore", this->mSettings.font.size);
  this->connect(lBackupGroupBox, &QGroupBox::toggled, this, &RetargetingTool::groupBoxChecked);
  Utils::SetGroupBoxState(lBackupGroupBox, false);
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

  auto lBackupPathLineEdit{new QLineEdit("", this)};
  lBackupPathLineEdit->setReadOnly(true);
  lBackupPathLineEdit->setObjectName(QString("backup_path_directory"));
  lBackupGridLayout->addWidget(lBackupPathLineEdit, 1, 1);

  auto lBackupPathChooser{ComponentFactory::CreateButton(this, tr("Choose a directory..."), "", "folder", lIconFolder, "backup_dir_chooser", false, true)};
  lBackupGridLayout->addWidget(lBackupPathChooser, 1, 2);

  // Backup subdirectory name/path
  auto lLabelSubDirectoryBackupPath{new QLabel(tr("Backup subdirectory name/path:"), this)};
  lLabelSubDirectoryBackupPath->setObjectName(QString("backup_subdir_label"));
  lBackupGridLayout->addWidget(lLabelSubDirectoryBackupPath, 2, 0);

  auto lBackupSubpathLineEdit{new QLineEdit("", this)};
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
  auto lGenerateButton{ComponentFactory::CreateButton(this, tr("Retarget all the files under the input path"), "", "arrow-up", lIconFolder, "objectname", false, true)};
  aLayout.addWidget(lGenerateButton, 2, 0, Qt::AlignBottom);

  // Event bindings for user actions (disconnected the first time the user does an action in the GUI)
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&RetargetingTool::userHasDoneAnAction));
  this->connect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&RetargetingTool::userHasDoneAnAction));

  // Event binding
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &RetargetingTool::updateAvailableBodyVersions);
  this->connect(lFeetSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &RetargetingTool::updateBodySlideFiltersListPreview);
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &RetargetingTool::chooseInputDirectory);
  this->connect(lKeepBackup, &QCheckBox::stateChanged, this, &RetargetingTool::updateBackupState);
  lKeepBackup->setChecked(true);

  this->connect(lBackupPathChooser, &QPushButton::clicked, this, &RetargetingTool::chooseBackupDirectory);
  this->connect(lBackupSubpathLineEdit, &QLineEdit::textChanged, this, &RetargetingTool::updateBackupPreview);
  this->connect(lGenerateButton, &QPushButton::clicked, this, &RetargetingTool::launchUpDownGradeProcess);
  this->connect(lFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &RetargetingTool::updateBodySlideFiltersListPreview);
  this->connect(lEditFilters, &QPushButton::clicked, this, &RetargetingTool::openBodySlideFiltersEditor);

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
  this->disconnect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&RetargetingTool::userHasDoneAnAction));

  // Body version selector
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("body_selector_version"))};
  this->disconnect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&RetargetingTool::userHasDoneAnAction));
}

void RetargetingTool::updateAvailableBodyVersions()
{
  auto lBodyName{static_cast<BodyName>(this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex())};

  // Version
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("body_selector_version"))};
  lBodyVersionSelector->clear();
  lBodyVersionSelector->addItems(DataLists::GetVersionsFromBodyName(lBodyName));
  lBodyVersionSelector->setCurrentIndex(0);

  // Feet mod
  auto lFeetSelector{this->findChild<QComboBox*>(QString("feet_mod_selector"))};
  lFeetSelector->clear();
  lFeetSelector->addItems(DataLists::GetFeetModsFromBodyName(lBodyName));
  lFeetSelector->setCurrentIndex(0);
}

void RetargetingTool::chooseInputDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "retargetingToolInput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "retargetingToolInput", lPath);

  if (!this->mHasUserDoneSomething && lPath.size() > 0)
  {
    this->userHasDoneAnAction();
  }
}

void RetargetingTool::chooseBackupDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("backup_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "retargetingToolOutput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "retargetingToolOutput", lPath);
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
  auto lFullPath{QString()};
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

  // Set the full path value in the preview label
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("backup_path_preview"))};
  auto lNewTextColor{this->mSettings.successColor};

  if (lIsValidPath)
  {
    auto lInputPath{Utils::CleanPathString(this->findChild<QLineEdit*>(QString("input_path_directory"))->text())};
    const auto lFullPathConst{lFullPath};

    // Check if the user is trying to backup the directory into itself
    if (Utils::CleanPathString(lFullPathConst).compare(lInputPath, Qt::CaseInsensitive) == 0
        || Utils::CleanPathString(lFullPathConst + QDir::separator()).startsWith(Utils::CleanPathString(lInputPath + QDir::separator()), Qt::CaseInsensitive))
    {
      lNewTextColor = this->mSettings.dangerColor;
    }
    // Check if the wanted backup directory already exists
    else if (QDir(lFullPath).exists())
    {
      lNewTextColor = this->mSettings.warningColor;
    }
  }
  else
  {
    lNewTextColor = this->mSettings.dangerColor;
  }

  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lFullPath);
}

void RetargetingTool::launchUpDownGradeProcess()
{
  // Input Directory
  auto lRootDir{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};

  // Selected body
  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  auto lBodySelected{DataLists::GetBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected)};

  // Selected feet
  auto lFeetModIndex{this->findChild<QComboBox*>(QString("feet_mod_selector"))->currentIndex()};

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

    //if (lFullBackupDirectory.startsWith())
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
    lDialog.setWindowFlags(lDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QVBoxLayout lLayout;
    lDialog.setLayout(&lLayout);
    QLabel lLabel(tr("The backup is currently being created. Depending on your computer, it can take a certain time. Please wait..."), this);
    lLayout.addWidget(&lLabel);
    lDialog.show();

    qApp->processEvents();

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
  lProgressBar->setValue(0);
  lProgressBar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Parsing XML files. Please wait..."), tr("Cancel treatment"), 0, 0, this);
  lProgressDialog.setBar(lProgressBar);
  lProgressDialog.setWindowFlags(lProgressDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
  lProgressDialog.setModal(true);
  lProgressDialog.show();

  // Iterate through all the files
  std::vector<QPair<QString, QString>> lNamesBuffer;

  auto lAbsFilePath{QString()};
  auto lRelativeDirs{QString()};
  std::vector<Struct::SliderSet> lParsedSliderSets;

  lProgressBar->setRange(0, lNumberXMLFiles);
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

    if (lNamesBuffer.at(lNamesBuffer.size() - 1).second == "")
    {
      Utils::DisplayWarningMessage(tr("Error while trying to parse the XML file \"%1\". Aborting process.").arg(lAbsFilePath));
      return;
    }

    lProgressDialog.setValue(++lTreatedFiles);
    qApp->processEvents();
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
    auto lFileName{it2.fileInfo().completeBaseName()};

    lParsedSliderSets = Utils::GetOutputPathsFromOSPFile(lAbsFilePath);

    if (lParsedSliderSets.size() == 0)
    {
      Utils::DisplayWarningMessage(tr("Error while trying to parse the OSP file \"%1\". Aborting process.").arg(lAbsFilePath));
      return;
    }

    // Search for the preset name in the buffer
    auto lPresetName{QString()};
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

    if (lPresetName == "")
    {
      Utils::DisplayWarningMessage(tr("No data found from the associated XML file. The file \"%1\" was not modified.").arg(it2.fileInfo().absoluteFilePath()));
      continue;
    }

    // Check, if the preset is using beast hands, if the chosen body does not support beast hands
    auto lSkipBeastHands{false};
    if (lMustUseBeastHands && !Utils::IsBodySupportingBeastHands(lBodySelected))
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
        if (lSliderSet.getMeshPart() == "Body")
        {
          lOptions += 100;
        }
        else if (lSliderSet.getMeshPart() == "Feet")
        {
          lOptions += 10;
        }
        else if (lSliderSet.getMeshPart() == "Hands")
        {
          lOptions += 1;
        }
      }
      lOSPUsedSliders.insert(std::make_pair(lFileName, lOptions));

      auto lOSPFileContent{SliderFileBuilder::BuildOSPFileContent(lPresetName, lBodySelected, lMustUseBeastHands, lFeetModIndex, lOptions)};

      // Fill the custom variables
      for (const auto& lSliderSet : lParsedSliderSets)
      {
        if (lSliderSet.getMeshPart() == "Body")
        {
          lOSPFileContent.replace(QString("{%%body_output_path%%}"), lSliderSet.getOutputPath());
          lOSPFileContent.replace(QString("{%%body_output_file%%}"), lSliderSet.getOutputFile());
        }
        else if (lSliderSet.getMeshPart() == "Feet")
        {
          lOSPFileContent.replace(QString("{%%feet_output_path%%}"), lSliderSet.getOutputPath());
          lOSPFileContent.replace(QString("{%%feet_output_file%%}"), lSliderSet.getOutputFile());
        }
        else if (lSliderSet.getMeshPart() == "Hands")
        {
          lOSPFileContent.replace(QString("{%%hands_output_path%%}"), lSliderSet.getOutputPath());
          lOSPFileContent.replace(QString("{%%hands_output_file%%}"), lSliderSet.getOutputFile());
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
    qApp->processEvents();
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
      qApp->processEvents();
      continue;
    }

    // Searching for the preset name in the buffer
    auto lPresetName{QString()};
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
    auto lUserFilters{Utils::GetFiltersForExport(this->mFiltersList, lFiltersListChooser->itemText(lFiltersListChooser->currentIndex()), lBodySelected, lFeetModIndex)};
    auto lXMLFileContent{SliderFileBuilder::BuildXMLFileContent(lPresetName, lUserFilters, lBodySelected, lMustUseBeastHands, lFeetModIndex, lOSPUsedSliders.find(lFileName)->second)};

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
    qApp->processEvents();
  }

  auto lTitle{tr("Retargeting successful")};
  auto lMessage{tr("All the files have been correctly retargeted. You can now close this window!")};

  if (mSettings.retargetingToolAutomaticallyOpenGeneratedDirectory)
  {
    QMessageBox lConfirmationBox(QMessageBox::Icon::Information, lTitle, lMessage, QMessageBox::StandardButton::NoButton, this);
    lConfirmationBox.setIconPixmap(QPixmap(":/icons/green-info-circle").scaledToHeight(17 * 2)); // TODO: Multiply the size by the DPI scale

    auto lOKButton{lConfirmationBox.addButton(tr("Open the retargeted directory"), QMessageBox::ButtonRole::AcceptRole)};
    lOKButton->setCursor(Qt::PointingHandCursor);
    lConfirmationBox.setDefaultButton(lOKButton);
    lConfirmationBox.exec();

    // Open the directory where the file structure has been created
    QDesktopServices::openUrl(QUrl::fromLocalFile(lRootDir));
  }
  else
  {
    if (Utils::DisplayQuestionMessage(this,
                                      lTitle,
                                      lMessage,
                                      "icons",
                                      "green-info-circle",
                                      tr("Open the retargeted directory"),
                                      tr("OK"),
                                      "",
                                      "",
                                      "",
                                      "",
                                      false)
        == ButtonClicked::YES)
    {
      // Open the directory where the file structure has been created
      QDesktopServices::openUrl(QUrl::fromLocalFile(lRootDir));
    }
  }
}

void RetargetingTool::openBodySlideFiltersEditor()
{
  auto lEditor{new BodySlideFiltersEditor(this, this->mSettings, this->mFiltersList)};
  this->connect(lEditor, &BodySlideFiltersEditor::listEdited, this, &RetargetingTool::updateBodySlideFiltersList);
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

void RetargetingTool::updateBodySlideFiltersListPreview(int)
{
  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  auto lBodySelected{DataLists::GetBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected)};
  auto lFeetModIndex{this->findChild<QComboBox*>(QString("feet_mod_selector"))->currentIndex()};

  // Take custom MSF filter
  auto lAdditionalFilter{Utils::GetAdditionalFeetFilter(lBodySelected, lFeetModIndex)};

  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lFiltersList{this->findChild<QLabel*>(QString("bodyslide_filters"))};

  auto lText{QString()};
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

void RetargetingTool::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::SetGroupBoxState(lGroupBox, !aIsChecked);
}
