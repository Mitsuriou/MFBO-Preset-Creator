#include "RetargetingTool.h"

RetargetingTool::RetargetingTool(QWidget* parent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(parent)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  this->mHasUserDoneSomething = false;

  // Show the window when it's completely built
  this->adjustSize();
  this->exec();
}

void RetargetingTool::closeEvent(QCloseEvent* aEvent)
{
  if (!this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  if (Utils::displayQuestionMessage(this, tr("Closing"), tr("Do you want to close the window?"), lIconFolder, "help-circle", tr("Close the window"), tr("Go back to the retargeting tool window"), this->mSettings.dangerColor, this->mSettings.successColor, false)
      == ButtonClicked::Yes)
  {
    aEvent->accept();
  }
  else
  {
    aEvent->ignore();
  }
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
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
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
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // General group box
  auto lGeneralGroupBox{new QGroupBox(tr("General").append("  "), this)};
  Utils::addIconToGroupBox(lGeneralGroupBox, lIconFolder, "tune");
  this->connect(lGeneralGroupBox, &QGroupBox::toggled, this, &RetargetingTool::groupBoxChecked);
  Utils::setGroupBoxState(lGeneralGroupBox, false);
  aLayout.addWidget(lGeneralGroupBox, 0, 0);

  // Grid layout
  auto lGeneralGridLayout{new QGridLayout(lGeneralGroupBox)};
  lGeneralGridLayout->setColumnStretch(0, 0);
  lGeneralGridLayout->setColumnStretch(1, 1);
  lGeneralGridLayout->setColumnStretch(2, 1);
  lGeneralGridLayout->setColumnStretch(3, 2);
  lGeneralGridLayout->setColumnStretch(4, 0);
  lGeneralGridLayout->setSpacing(10);
  lGeneralGridLayout->setContentsMargins(15, 20, 15, 15);
  lGeneralGridLayout->setAlignment(Qt::AlignTop);

  // Targeted body and version
  auto lDefaultBodyVersionSettings{DataLists::getSplittedNameVersionFromBodyVersion(mSettings.defaultRetargetingToolBody)};

  lGeneralGridLayout->addWidget(new QLabel(tr("Targeted body and version:"), this), 0, 0);

  auto lBodyNameSelector{new QComboBox(this)};
  lBodyNameSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyNameSelector->setCursor(Qt::PointingHandCursor);
  lBodyNameSelector->addItems(DataLists::getBodiesNames());
  lBodyNameSelector->setCurrentIndex(lDefaultBodyVersionSettings.first);
  lBodyNameSelector->setObjectName(QString("body_selector_name"));
  lGeneralGridLayout->addWidget(lBodyNameSelector, 0, 1);

  auto lBodyVersionSelector{new QComboBox(this)};
  lBodyVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyVersionSelector->setCursor(Qt::PointingHandCursor);
  lBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lBodyVersionSelector->setCurrentIndex(lDefaultBodyVersionSettings.second);
  lBodyVersionSelector->setObjectName(QString("body_selector_version"));
  lGeneralGridLayout->addWidget(lBodyVersionSelector, 0, 2);

  // Feet mod chooser
  auto lFeetSelector{new QComboBox(this)};
  lFeetSelector->setItemDelegate(new QStyledItemDelegate());
  lFeetSelector->setCursor(Qt::PointingHandCursor);
  lFeetSelector->addItems(DataLists::getFeetModsEntries());
  lFeetSelector->setCurrentIndex(mSettings.defaultRetargetingToolFeetMod);
  lFeetSelector->setObjectName(QString("feet_selector_version"));
  lGeneralGridLayout->addWidget(lFeetSelector, 0, 3);

  // Input path
  lGeneralGridLayout->addWidget(new QLabel(tr("Input path:"), this), 1, 0);

  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  lGeneralGridLayout->addWidget(lInputPathLineEdit, 1, 1, 1, 3);

  auto lInputPathChooser{ComponentFactory::createButton(this, tr("Choose a directory..."), "", "folder", lIconFolder, "", false, true)};
  lGeneralGridLayout->addWidget(lInputPathChooser, 1, 4);

  // BodySlide filters
  lGeneralGridLayout->addWidget(new QLabel(tr("BodySlide filters:"), this), 2, 0);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  lGeneralGridLayout->addWidget(lFiltersListChooser, 2, 1);

  auto lFiltersList{new QLabel("", this)};
  lFiltersList->setObjectName("bodyslide_filters");
  lFiltersList->setWordWrap(true);
  lGeneralGridLayout->addWidget(lFiltersList, 2, 2, 1, 2);

  auto lEditFilters{ComponentFactory::createButton(this, tr("Edit BodySlide filters sets"), "", "filter", lIconFolder, "edit_filters", false, true)};
  lGeneralGridLayout->addWidget(lEditFilters, 2, 4);

  // Backup group box
  auto lBackupGroupBox{new QGroupBox(tr("Backup").append("  "), this)};
  Utils::addIconToGroupBox(lBackupGroupBox, lIconFolder, "restore");
  this->connect(lBackupGroupBox, &QGroupBox::toggled, this, &RetargetingTool::groupBoxChecked);
  Utils::setGroupBoxState(lBackupGroupBox, false);
  aLayout.addWidget(lBackupGroupBox, 1, 0);

  // Grid layout
  auto lBackupGridLayout{new QGridLayout(lBackupGroupBox)};
  lBackupGridLayout->setSpacing(10);
  lBackupGridLayout->setContentsMargins(15, 20, 15, 15);
  lBackupGridLayout->setAlignment(Qt::AlignTop);

  // Keep backup checkbox
  lBackupGridLayout->addWidget(new QLabel(tr("Keep a backup?"), this), 0, 0);

  auto lKeepBackup{new QCheckBox(tr("You should always check this box to avoid any data loss or corruption."), this)};
  lKeepBackup->setCursor(Qt::PointingHandCursor);
  lKeepBackup->setObjectName("keep_backup");
  lBackupGridLayout->addWidget(lKeepBackup, 0, 1, 1, 2);

  // Backup directory path
  auto lBackupPathLabel{new QLabel(tr("Backup directory path:"), this)};
  lBackupPathLabel->setObjectName("backup_path_label");
  lBackupGridLayout->addWidget(lBackupPathLabel, 1, 0);

  auto lBackupPathLineEdit{new QLineEdit("", this)};
  lBackupPathLineEdit->setReadOnly(true);
  lBackupPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lBackupPathLineEdit->setObjectName("backup_path_directory");
  lBackupGridLayout->addWidget(lBackupPathLineEdit, 1, 1);

  auto lBackupPathChooser{ComponentFactory::createButton(this, tr("Choose a directory..."), "", "folder", lIconFolder, "backup_dir_chooser", false, true)};
  lBackupGridLayout->addWidget(lBackupPathChooser, 1, 2);

  // Backup subdirectory name/path
  auto lLabelSubDirectoryBackupPath{new QLabel(tr("Backup subdirectory name/path:"), this)};
  lLabelSubDirectoryBackupPath->setObjectName("backup_subdir_label");
  lBackupGridLayout->addWidget(lLabelSubDirectoryBackupPath, 2, 0);

  auto lBackupSubpathLineEdit{new QLineEdit("", this)};
  lBackupSubpathLineEdit->setObjectName("backup_path_subdirectory");
  lBackupGridLayout->addWidget(lBackupSubpathLineEdit, 2, 1);

  // Backup preview
  auto lBackupPathPreviewLabel{new QLabel(tr("Preview:"), this)};
  lBackupPathPreviewLabel->setObjectName("backup_path_preview_label");
  lBackupPathPreviewLabel->setAlignment(Qt::AlignTop);
  lBackupGridLayout->addWidget(lBackupPathPreviewLabel, 3, 0);

  auto lBackupPathsPreview{new QLabel("", this)};
  lBackupPathsPreview->setObjectName("backup_path_preview");
  lBackupPathsPreview->setAlignment(Qt::AlignTop);
  lBackupGridLayout->addWidget(lBackupPathsPreview, 3, 1, 1, 2);

  // Generate button
  auto lGenerateButton{ComponentFactory::createButton(this, tr("Retarget all the files under the input path"), "", "arrow-up", lIconFolder, "objectname", false, true)};
  aLayout.addWidget(lGenerateButton, 2, 0, Qt::AlignBottom);

  // Event bindings for user actions (disconnected the first time the user does an action in the GUI)
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&RetargetingTool::userHasDoneAnAction));
  this->connect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&RetargetingTool::userHasDoneAnAction));

  // Event binding
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &RetargetingTool::updateAvailableBodyVersions);
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &RetargetingTool::chooseInputDirectory);
  this->connect(lKeepBackup, &QCheckBox::stateChanged, this, &RetargetingTool::updateBackupState);
  lKeepBackup->setChecked(true);

  this->connect(lBackupPathChooser, &QPushButton::clicked, this, &RetargetingTool::chooseBackupDirectory);
  this->connect(lBackupSubpathLineEdit, &QLineEdit::textChanged, this, &RetargetingTool::updateBackupPreview);
  this->connect(lGenerateButton, &QPushButton::clicked, this, &RetargetingTool::launchUpDownGradeProcess);
  this->connect(lFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&RetargetingTool::updateBodySlideFiltersListPreview));
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
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("body_selector_version"))};
  lBodyVersionSelector->clear();
  lBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(lBodyName));
  lBodyVersionSelector->setCurrentIndex(0);
}

void RetargetingTool::chooseInputDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("input_path_directory")};
  const auto& lContextPath{Utils::getPathFromKey(this->mLastPaths, "retargetingToolInput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::updatePathAtKey(this->mLastPaths, "retargetingToolInput", lPath);
  this->updateBackupPreview();

  if (!this->mHasUserDoneSomething && lPath.size() > 0)
  {
    this->userHasDoneAnAction();
  }
}

void RetargetingTool::chooseBackupDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("backup_path_directory")};
  const auto& lContextPath{Utils::getPathFromKey(this->mLastPaths, "retargetingToolOutput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::updatePathAtKey(this->mLastPaths, "retargetingToolOutput", lPath);
  this->updateBackupPreview();

  if (!this->mHasUserDoneSomething && lPath.size() > 0)
  {
    this->userHasDoneAnAction();
  }
}

void RetargetingTool::updateBackupState(int aState)
{
  auto lBackupPathLabel{this->findChild<QLabel*>("backup_path_label")};
  auto lBackupPathLineEdit{this->findChild<QLineEdit*>("backup_path_directory")};
  auto lBackupPathChooser{this->findChild<QPushButton*>("backup_dir_chooser")};
  auto lLabelSubDirectoryBackupPath{this->findChild<QLabel*>("backup_subdir_label")};
  auto lBackupSubpathLineEdit{this->findChild<QLineEdit*>("backup_path_subdirectory")};
  auto lOutputPathsPreviewLabel{this->findChild<QLabel*>("backup_path_preview_label")};
  auto lOutputPathsPreview{this->findChild<QLabel*>("backup_path_preview")};

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
  auto lEventSource{qobject_cast<QLineEdit*>(sender())};
  auto lSubdirLineEdit{this->findChild<QLineEdit*>("backup_path_subdirectory")};

  if (lEventSource == lSubdirLineEdit)
  {
    this->userHasDoneAnAction();
  }

  // Get main directory
  auto lMainDirTextEdit{this->findChild<QLineEdit*>("backup_path_directory")};
  auto lMainDirectory{lMainDirTextEdit->text().trimmed()};
  Utils::cleanPathString(lMainDirectory);

  // Get subdirectory
  auto lSubDirectory{this->findChild<QLineEdit*>("backup_path_subdirectory")->text().trimmed()};
  Utils::cleanPathString(lSubDirectory);
  auto lIsValidPath{true};

  // Construct full path
  auto lFullPath(QString(""));
  if (lMainDirectory.length() > 0 && lSubDirectory.length() > 0)
  {
    lFullPath = lMainDirectory + "/" + lSubDirectory;
    lMainDirTextEdit->setDisabled(false);
  }
  else if (lMainDirectory.length() > 0 && lSubDirectory.length() == 0)
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
  auto lOutputPathsPreview{this->findChild<QLabel*>("backup_path_preview")};

  auto lNewTextColor{this->mSettings.successColor};

  if (lIsValidPath)
  {
    // Check if the user is trying to backup the directory into itself
    if (lFullPath.startsWith(this->findChild<QLineEdit*>("input_path_directory")->text()))
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
  auto lRootDir{this->findChild<QLineEdit*>("input_path_directory")->text()};

  // Selected body
  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  auto lBodySelected{DataLists::getBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected)};

  // Selected feet
  auto lFeetModIndex{this->findChild<QComboBox*>(QString("feet_selector_version"))->currentIndex()};

  // Check if the input path has been given by the user
  if (lRootDir.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no path path given for the retargeting."));
    return;
  }

  // Manage the backup part
  auto lMustKeepBackup{this->findChild<QCheckBox*>("keep_backup")->isChecked()};

  if (lMustKeepBackup)
  {
    // Backup paths
    auto lMainDirectory{this->findChild<QLineEdit*>("backup_path_directory")->text().trimmed()};
    auto lSubDirectory{this->findChild<QLineEdit*>("backup_path_subdirectory")->text().trimmed()};
    Utils::cleanPathString(lSubDirectory);

    // Full extract path
    const auto& lDirtyFullBackupDirectory{(lSubDirectory.length() == 0 ? lMainDirectory : (lMainDirectory + QDir::separator() + lSubDirectory))};
    auto lFullBackupDirectory{Utils::cleanPathString(lDirtyFullBackupDirectory)};

    // Check if the full extract path has been given by the user
    if (lFullBackupDirectory.length() == 0)
    {
      Utils::displayWarningMessage(tr("Error: no path given to backup the files."));
      return;
    }

    // Check if the user is trying to backup the directory into itself
    auto lInputPath{this->findChild<QLineEdit*>("input_path_directory")->text()};

    if (lFullBackupDirectory.startsWith(lInputPath))
    {
      Utils::displayWarningMessage(tr("Error: it is not possible to backup a directory inside itself. Choose another backup location."));
      return;
    }

    // Check if the path could be valid
    if (lFullBackupDirectory.startsWith("/"))
    {
      Utils::displayWarningMessage(tr("Error: the path given to backup the files seems to be invalid."));
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
    if (!Utils::copyRecursively(lRootDir, lFullBackupDirectory))
    {
      Utils::displayWarningMessage(tr("Error: the backup could not be created. Please try again."));
      return;
    }
  }

  // Scan the number of files to treat
  auto lNumberOSPFiles{Utils::getNumberFilesByExtensionRecursiveIgnoringFOMOD(lRootDir, "*.osp")};
  auto lNumberXMLFiles{Utils::getNumberFilesByExtensionRecursiveIgnoringFOMOD(lRootDir, "*.xml")};
  auto lTreatedFiles{0};

  // Progress bar
  auto lProgressbar{new QProgressBar(this)};
  lProgressbar->setFormat("%v / %m");
  lProgressbar->setValue(0);
  lProgressbar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog("", tr("Cancel treatment"), 0, 0, this);
  lProgressDialog.setBar(lProgressbar);
  lProgressDialog.setWindowFlags(lProgressDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
  lProgressDialog.show();

  // Iterate through all the files
  std::vector<QPair<QString, QString>> lNamesBuffer;

  auto lAbsFilePath{QString()};
  auto lRelativeDirs{QString()};
  std::vector<Struct::SliderSet> lParsedSliderSets;
  auto lRessourcesFolder{Utils::getBodyRessourceFolder(lBodySelected)};

  if (lRessourcesFolder.length() == 0)
  {
    // It should not be possible to reach this statement, but in case an update is buggy, keep this security
    return;
  }

  lProgressDialog.setLabelText(tr("Parsing XML files. Please wait..."));
  lProgressbar->setRange(0, lNumberXMLFiles);
  lTreatedFiles = 0;

  QDirIterator it(lRootDir, QStringList() << QString("*.xml"), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::displayWarningMessage(tr("Process aborted by the user."));
      return;
    }

    // Navigate to the next file
    it.next();

    // Ignore FOMOD directory
    lAbsFilePath = it.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.mid(0).remove(lRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("fomod", Qt::CaseInsensitive))
    {
      continue;
    }

    // Build the names buffer
    lNamesBuffer.push_back(QPair<QString, QString>(it.fileInfo().completeBaseName(), Utils::getPresetNameFromXMLFile(lAbsFilePath)));

    if (lNamesBuffer.at(lNamesBuffer.size() - 1).second == "")
    {
      Utils::displayWarningMessage(tr("Error while trying to parse the XML file \"%1\". Aborting process.").arg(lAbsFilePath));
      return;
    }

    lProgressDialog.setValue(++lTreatedFiles);
    qApp->processEvents();
  }

  lProgressDialog.setLabelText(tr("Parsing and patching OSP files. Please wait..."));
  lProgressbar->setRange(0, lNumberOSPFiles);
  lTreatedFiles = 0;

  std::vector<QPair<QString, QString>> lOSPBuffer;
  for (const auto& lPair : lNamesBuffer)
  {
    lOSPBuffer.push_back(lPair);
  }

  QDirIterator it2(lRootDir, QStringList() << QString("*.osp"), QDir::Files, QDirIterator::Subdirectories);
  while (it2.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::displayWarningMessage(tr("Process aborted by the user."));
      return;
    }

    // Navigate to the next file
    it2.next();

    // Ignore FOMOD directory
    lAbsFilePath = it2.fileInfo().absoluteFilePath();

    lRelativeDirs = lAbsFilePath.mid(0).remove(lRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("fomod", Qt::CaseInsensitive))
    {
      continue;
    }

    auto lMustUseBeastHands{Utils::isPresetUsingBeastHands(lAbsFilePath)};

    // Check the file extension
    auto lFileName{it2.fileInfo().completeBaseName()};

    lParsedSliderSets = Utils::getOutputPathsFromOSPFile(lAbsFilePath);

    if (lParsedSliderSets.size() == 0)
    {
      Utils::displayWarningMessage(tr("Error while trying to parse the OSP file \"%1\". Aborting process.").arg(lAbsFilePath));
      return;
    }

    // Search for the preset name in the buffer
    auto lPresetName{QString()};
    auto lBufferLocationToRemove{-1};

    auto lOSPBufSize{lOSPBuffer.size()};
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
      Utils::displayWarningMessage(tr("No data found from the associated XML file. The file \"%1\" was not modified.").arg(it2.fileInfo().absoluteFilePath()));
      continue;
    }

    // Check, if the preset is using beast hands, if the chosen body does not support beast hands
    auto lSkipBeastHands{false};
    if (lMustUseBeastHands && !Utils::isBodySupportingBeastHands(lBodySelected))
    {
      Utils::displayWarningMessage(tr("The chosen body/version does not support beast hands. The retargeting of the OSP file \"%1\" has been skipped.").arg(it2.fileInfo().absoluteFilePath()));
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

      // TODO: Update the OSP file generation code block below, based on the PresetCreator.cpp file:

      // Copy the OSP file
      if (lMustUseBeastHands)
      {
        if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_beast_hands_osp", lAbsFilePath))
        {
          Utils::displayWarningMessage(tr("The OSP file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions. Aborting process."));
          return;
        }
      }
      else
      {
        if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_osp", lAbsFilePath))
        {
          Utils::displayWarningMessage(tr("The OSP file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions. Aborting process."));
          return;
        }
      }

      // Read the created OSP file
      QFile lOSPFile(lAbsFilePath);
      lOSPFile.setPermissions(QFile::WriteUser);

      QByteArray lOSPFileContent;

      if (lOSPFile.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        lOSPFileContent = lOSPFile.readAll();
        lOSPFile.close();
      }
      else
      {
        Utils::displayWarningMessage(tr("Error while trying to read the OSP file \"%1\". Aborting process.").arg(lAbsFilePath));
        return;
      }

      // Replace the custom tags in the file
      if (lOSPFileContent.length() > 0)
      {
        if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
          auto lTextToParse{static_cast<QString>(lOSPFileContent)};
          lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), lPresetName);

          for (const auto& lSliderSet : lParsedSliderSets)
          {
            if (lSliderSet.meshPart == "Body")
            {
              lTextToParse.replace(QString("{%%body_output_path%%}"), lSliderSet.outputPath);
              lTextToParse.replace(QString("{%%body_output_file%%}"), lSliderSet.outputFile);
            }
            else if (lSliderSet.meshPart == "Feet")
            {
              lTextToParse.replace(QString("{%%feet_output_path%%}"), lSliderSet.outputPath);
              lTextToParse.replace(QString("{%%feet_output_file%%}"), lSliderSet.outputFile);
            }
            else if (lSliderSet.meshPart == "Hands")
            {
              lTextToParse.replace(QString("{%%hands_output_path%%}"), lSliderSet.outputPath);
              lTextToParse.replace(QString("{%%hands_output_file%%}"), lSliderSet.outputFile);
            }
          }

          QTextStream lTextStream(&lOSPFile);
          lTextStream << lTextToParse;
          lTextStream.flush();

          lOSPFile.close();
        }
        else
        {
          Utils::displayWarningMessage(tr("Error while trying to write in the OSP file \"%1\". Aborting process.").arg(lAbsFilePath));
          return;
        }
      }
      else
      {
        Utils::displayWarningMessage(tr("Error while trying to parse the OSP file \"%1\". Aborting process.").arg(lAbsFilePath));
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
  lProgressbar->setRange(0, lNumberOSPFiles);
  lTreatedFiles = 0;

  QDirIterator it3(lRootDir, QStringList() << QString("*.xml"), QDir::Files, QDirIterator::Subdirectories);
  while (it3.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::displayWarningMessage(tr("Process aborted by the user."));
      return;
    }

    // Navigate to the next file
    it3.next();

    // Ignore FOMOD directory
    lAbsFilePath = it3.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.mid(0).remove(lRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("fomod", Qt::CaseInsensitive))
    {
      continue;
    }

    auto lMustUseBeastHands{Utils::isPresetUsingBeastHands(lAbsFilePath)};

    // Check the file extension
    auto lFileName{it3.fileInfo().completeBaseName()};

    // Check if the OSP file has skiped the parsing
    auto lSkipXMLLastTreatment{false};
    auto lOSPBufSize{lOSPBuffer.size()};
    for (int i = 0; i < lOSPBufSize; i++)
    {
      if (lFileName == lOSPBuffer.at(i).first)
      {
        Utils::displayWarningMessage(tr("Since the associated OSP file has not been modified, the file \"%1\" has not been modified.").arg(it3.fileInfo().absoluteFilePath()));
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
    auto lNamesBufSize{lNamesBuffer.size()};

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

    // Copy the XML file
    auto lUserFilters{Utils::splitString(this->findChild<QLabel*>("bodyslide_filters")->text(), " ; ")};
    auto lUserFiltersListSize{lUserFilters.size()};

    if (lUserFilters.size() == 0)
    {
      lUserFilters = Utils::getXMLDefaultFiltersFromBody(static_cast<BodyNameVersion>(lBodySelected));
    }

    if (!QFile::copy(":/ressources/bodyslide_xml_custom", lAbsFilePath))
    {
      Utils::displayWarningMessage(tr("The XML file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions. Aborting process."));
      return;
    }

    QFile lXMLFile(lAbsFilePath);
    lXMLFile.setPermissions(QFile::WriteUser);

    QByteArray lTempXMLContent;

    if (lXMLFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      lTempXMLContent = lXMLFile.readAll();
      lXMLFile.close();
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to read the XML file \"%1\". Aborting process.").arg(lAbsFilePath));
      return;
    }

    if (lTempXMLContent.length() > 0)
    {
      if (lXMLFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
      {
        auto lTextToParse{static_cast<QString>(lTempXMLContent)};

        // Custom BodySlide filters
        if (lUserFiltersListSize > 0)
        {
          auto lUserFiltersConcat{QString()};

          for (const auto& lUserFilter : lUserFilters)
          {
            lUserFiltersConcat += Utils::getXMLFilterBlockFromBody(lUserFilter, static_cast<int>(lBodySelected), lMustUseBeastHands, lFeetModIndex);
          }

          lTextToParse.replace(QString("{%%bodyslide_filters_block%%}"), lUserFiltersConcat);
        }

        // BodySlide preset name
        lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), lPresetName);

        QTextStream lTextStream(&lXMLFile);
        lTextStream << lTextToParse;
        lTextStream.flush();

        lXMLFile.close();
      }
      else
      {
        Utils::displayWarningMessage(tr("Error while trying to write in the XML file \"%1\". Aborting process.").arg(lAbsFilePath));
        return;
      }
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to parse the XML file \"%1\". Aborting process.").arg(lAbsFilePath));
      return;
    }

    lProgressDialog.setValue(++lTreatedFiles);
    qApp->processEvents();
  }

  QMessageBox lConfirmationBox(QMessageBox::Icon::Information, tr("Retargeting successful"), tr("All the files have been correctly re-targeted. You can now close this window!"), QMessageBox::StandardButton::NoButton, this);
  lConfirmationBox.setIconPixmap(QPixmap(":/icons/green-info-circle").scaledToHeight(48, Qt::SmoothTransformation));

  auto lOKButton{lConfirmationBox.addButton(tr("OK"), QMessageBox::ButtonRole::AcceptRole)};
  lOKButton->setCursor(Qt::PointingHandCursor);
  lConfirmationBox.setDefaultButton(lOKButton);
  lConfirmationBox.exec();
}

void RetargetingTool::openBodySlideFiltersEditor()
{
  auto lEditor{new BodySlideFiltersEditor(this, this->mSettings, this->mFiltersList)};
  this->connect(lEditor, &BodySlideFiltersEditor::listEdited, this, &RetargetingTool::updateBodySlideFiltersList);
}

void RetargetingTool::initBodySlideFiltersList()
{
  // Load and save the filters list
  this->mFiltersList = Utils::loadFiltersFromFile();

  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};

  // Disable the combobox if there is not any available filter
  if (this->mFiltersList.size() == 0)
  {
    lChooser->setDisabled(true);
    return;
  }

  // Fill the combobox
  for (const auto& lPair : this->mFiltersList)
  {
    lChooser->addItem(lPair.first);
  }

  lChooser->setCurrentIndex(0);
}

void RetargetingTool::updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList)
{
  this->mFiltersList = aFilterList;
  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};
  auto lLabel{this->findChild<QLabel*>("bodyslide_filters")};
  Utils::updateComboBoxBodyslideFiltersList(this->mFiltersList, lChooser, lLabel);
}

void RetargetingTool::updateBodySlideFiltersListPreview(int aIndex)
{
  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};
  auto LFiltersLabel{this->findChild<QLabel*>("bodyslide_filters")};

  auto lText{QString()};
  if (aIndex != -1)
  {
    lText = this->mFiltersList.find(lChooser->itemText(aIndex))->second.join(QString(" ; "));
  }

  LFiltersLabel->setText(lText);

  this->userHasDoneAnAction();
}

void RetargetingTool::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);

  // Resize the window
  this->adjustSize();
}
