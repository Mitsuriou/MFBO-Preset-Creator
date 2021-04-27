#include "BatchConversion.h"

BatchConversion::BatchConversion(QWidget* parent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
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

void BatchConversion::closeEvent(QCloseEvent* aEvent)
{
  if (!this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  if (Utils::displayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the batch conversion window"),
                                    this->mSettings.dangerColor,
                                    this->mSettings.successColor,
                                    false)
      == ButtonClicked::Yes)
  {
    aEvent->accept();
  }
  else
  {
    aEvent->ignore();
  }
}

void BatchConversion::reject()
{
  this->close();
}

void BatchConversion::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(800);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Batch Conversion"));
  this->setWindowIcon(QIcon(QPixmap(":/black/reorder")));
}

void BatchConversion::initializeGUI()
{
  // Main window layout
  auto lMainGrid{new QGridLayout(this)};
  lMainGrid->setColumnStretch(0, 0);
  lMainGrid->setColumnStretch(1, 1);
  lMainGrid->setColumnStretch(2, 1);
  lMainGrid->setColumnStretch(3, 2);
  lMainGrid->setColumnStretch(4, 0);
  lMainGrid->setSpacing(10);
  lMainGrid->setContentsMargins(15, 20, 15, 15);
  lMainGrid->setAlignment(Qt::AlignTop);
  this->setLayout(lMainGrid);

  this->setupInterface(lMainGrid);
}

void BatchConversion::setupInterface(QGridLayout* aLayout)
{
  // TODO: Split the code below into smaller functions
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // First line
  aLayout->addWidget(new QLabel(tr("Input path:"), this), 0, 0);

  // Input label
  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  lInputPathLineEdit->setDisabled(true);
  aLayout->addWidget(lInputPathLineEdit, 0, 1, 1, 3);

  // Input chooser
  auto lInputPathChooser{ComponentFactory::createButton(this, tr("Choose a directory..."), "", "folder", lIconFolder)};
  aLayout->addWidget(lInputPathChooser, 0, 4);

  // Targeted body and version
  auto lDefaultBodyVersionSettings{DataLists::getSplittedNameVersionFromBodyVersion(mSettings.defaultRetargetingToolBody)};

  aLayout->addWidget(new QLabel(tr("Targeted body and version:"), this), 1, 0);

  auto lBodyNameSelector{new QComboBox(this)};
  lBodyNameSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyNameSelector->setCursor(Qt::PointingHandCursor);
  lBodyNameSelector->addItems(DataLists::getBodiesNames());
  lBodyNameSelector->setCurrentIndex(lDefaultBodyVersionSettings.first);
  lBodyNameSelector->setObjectName(QString("body_selector_name"));
  aLayout->addWidget(lBodyNameSelector, 1, 1, 1, 3);

  auto lBodyVersionSelector{new QComboBox(this)};
  lBodyVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyVersionSelector->setCursor(Qt::PointingHandCursor);
  lBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lBodyVersionSelector->setCurrentIndex(lDefaultBodyVersionSettings.second);
  lBodyVersionSelector->setObjectName(QString("body_selector_version"));
  aLayout->addWidget(lBodyVersionSelector, 1, 4);

  // Human skeleton file
  aLayout->addWidget(new QLabel(tr("Skeleton file (human):"), this), 2, 0);

  auto lSkeletonChooserHuman{new QComboBox(this)};
  lSkeletonChooserHuman->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooserHuman->setCursor(Qt::PointingHandCursor);
  lSkeletonChooserHuman->setObjectName("skeleton_chooser_human");
  aLayout->addWidget(lSkeletonChooserHuman, 2, 1, 1, 3);

  auto lSkeletonRefresherHuman{ComponentFactory::createButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  aLayout->addWidget(lSkeletonRefresherHuman, 2, 4);

  // Beast skeleton file
  aLayout->addWidget(new QLabel(tr("Skeleton file (beast):"), this), 3, 0);

  auto lSkeletonChooserBeast{new QComboBox(this)};
  lSkeletonChooserBeast->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooserBeast->setCursor(Qt::PointingHandCursor);
  lSkeletonChooserBeast->setObjectName("skeleton_chooser_beast");
  aLayout->addWidget(lSkeletonChooserBeast, 3, 1, 1, 3);

  this->populateSkeletonChooser();

  auto lSkeletonRefresherBeast{ComponentFactory::createButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  aLayout->addWidget(lSkeletonRefresherBeast, 3, 4);

  // BodySlide filters
  aLayout->addWidget(new QLabel(tr("BodySlide filters:"), this), 4, 0);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  aLayout->addWidget(lFiltersListChooser, 4, 1);

  auto lFiltersList{new QLabel("", this)};
  lFiltersList->setObjectName("bodyslide_filters");
  lFiltersList->setWordWrap(true);
  aLayout->addWidget(lFiltersList, 4, 2, 1, 2);

  auto lEditFilters{ComponentFactory::createButton(this, tr("Edit BodySlide filters sets"), "", "filter", lIconFolder, "edit_filters")};
  aLayout->addWidget(lEditFilters, 4, 4);

  // BodySlide files names pattern
  aLayout->addWidget(new QLabel(tr("BodySlide files names:"), this), 5, 0);

  auto lOSPXMLNamesLineEdit{new QLineEdit(this)};
  lOSPXMLNamesLineEdit->setObjectName("names_osp_xml_input");
  aLayout->addWidget(lOSPXMLNamesLineEdit, 5, 1, 1, 4);

  // BodySlide presets names pattern
  auto lNamesInApp{new QLabel(this)};
  lNamesInApp->setTextFormat(Qt::RichText);
  auto lText{tr("Presets names:")};
  auto lRichText{
    QStringLiteral("<p style=\"text-align: left; padding: 0px; margin: 0px;\">"
                   "<img src=\":/%1/info-circle-smaller\" alt=\"~info icon~\" style=\"vertical-align: baseline;\">"
                   " %2"
                   "</p>")
      .arg(lIconFolder)
      .arg(lText)};
  lNamesInApp->setText(lRichText);
  lNamesInApp->setToolTip(QString(tr("This field represents the names under which the presets will be listed in the BodySlide application.")));
  aLayout->addWidget(lNamesInApp, 6, 0);

  auto lNamesInAppLineEdit{new QLineEdit(this)};
  lNamesInAppLineEdit->setObjectName("names_bodyslide_input");
  aLayout->addWidget(lNamesInAppLineEdit, 6, 1, 1, 4);

  // Output location
  this->setupOutputGUI(aLayout);

  // Launch search button
  auto lGenerateButton{ComponentFactory::createButton(this, tr("Batch generate the files on my computer"), "", "build", lIconFolder, "launch_search_button", true, true)};
  aLayout->addWidget(lGenerateButton, 8, 0, 1, 5, Qt::AlignTop);

  // Event bindings for user actions (disconnected the first time the user does an action in the GUI)
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&BatchConversion::userHasDoneAnAction));
  this->connect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&BatchConversion::userHasDoneAnAction));

  // Event binding
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &BatchConversion::updateAvailableBodyVersions);
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &BatchConversion::chooseInputDirectory);

  this->connect(lSkeletonRefresherHuman, &QPushButton::clicked, this, &BatchConversion::populateSkeletonChooser);
  this->connect(lSkeletonRefresherBeast, &QPushButton::clicked, this, &BatchConversion::populateSkeletonChooser);

  this->connect(lGenerateButton, &QPushButton::clicked, this, &BatchConversion::launchSearchProcess);
  this->connect(lFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&BatchConversion::updateBodySlideFiltersListPreview));
  this->connect(lEditFilters, &QPushButton::clicked, this, &BatchConversion::openBodySlideFiltersEditor);

  // Post-bind initialization functions
  this->initBodySlideFiltersList();
}

void BatchConversion::setupOutputGUI(QGridLayout* aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Output group box
  auto lOutputGroupBox{new QGroupBox(tr("Files generation's output location").append("  "), this)};
  Utils::addIconToGroupBox(lOutputGroupBox, lIconFolder, "file-tree");
  this->connect(lOutputGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);
  Utils::setGroupBoxState(lOutputGroupBox, false);
  aLayout->addWidget(lOutputGroupBox, 7, 0, 1, 5);

  // Grid layout
  auto lOutputGridLayout{new QGridLayout(lOutputGroupBox)};
  lOutputGridLayout->setSpacing(10);
  lOutputGridLayout->setContentsMargins(15, 20, 15, 15);
  lOutputGridLayout->setAlignment(Qt::AlignTop);

  // Main directory
  lOutputGridLayout->addWidget(new QLabel(tr("Output directory path:"), this), 0, 0);

  auto lOutputPathLineEdit{new QLineEdit(this)};
  lOutputPathLineEdit->setReadOnly(true);
  lOutputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lOutputPathLineEdit->setObjectName("output_path_directory");
  lOutputPathLineEdit->setText(mSettings.mainWindowOutputPath);
  lOutputGridLayout->addWidget(lOutputPathLineEdit, 0, 1);

  // Main directory's file chooser button
  auto lOutputPathChooser{ComponentFactory::createButton(this, tr("Choose a directory..."), "", "folder", lIconFolder)};
  lOutputGridLayout->addWidget(lOutputPathChooser, 0, 2);

  // Subdirectory
  lOutputGridLayout->addWidget(new QLabel(tr("Output subdirectory name/path:"), this), 1, 0);

  auto lOutputSubpathLineEdit{new QLineEdit(this)};
  lOutputSubpathLineEdit->setObjectName("output_path_subdirectory");
  lOutputGridLayout->addWidget(lOutputSubpathLineEdit, 1, 1);

  // Use only subdirectory path
  lOutputGridLayout->addWidget(new QLabel(tr("Use only subdirectory path?"), this), 2, 0);

  auto lUseOnlySubdir{new QCheckBox(tr("Check this box to define the export as only the subdirectory field (use at your own risk)."))};
  lUseOnlySubdir->setCursor(Qt::PointingHandCursor);
  lUseOnlySubdir->setObjectName("only_use_subdirectory");
  lOutputGridLayout->addWidget(lUseOnlySubdir, 2, 1, 1, 2);

  // Preview
  lOutputGridLayout->addWidget(new QLabel(tr("Preview:"), this), 3, 0);

  auto lOutputPathsPreview{new QLabel("", this)};
  lOutputPathsPreview->setObjectName("output_path_preview");
  lOutputPathsPreview->setAutoFillBackground(true);
  lOutputGridLayout->addWidget(lOutputPathsPreview, 3, 1);

  // TODO: bind these events
  //// Event binding
  //this->connect(lOutputPathChooser, &QPushButton::clicked, this, &BatchConversion::chooseExportDirectory);
  //this->connect(lOutputSubpathLineEdit, &QLineEdit::textChanged, this, &BatchConversion::updateOutputPreview);
  //this->connect(lUseOnlySubdir, &QCheckBox::stateChanged, this, &BatchConversion::useOnlySubdirStateChanged);

  //// Pre-filled data
  //this->updateOutputPreview();
}

void BatchConversion::userHasDoneAnAction()
{
  if (!this->mHasUserDoneSomething)
  {
    this->mHasUserDoneSomething = true;
  }
}

int BatchConversion::getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const
{
  auto lNumber{0};
  auto lAbsFilePath{QString()};
  auto lRelativeDirs{QString()};

  QDirIterator it(aRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();

    // Ignore FOMOD directory
    lAbsFilePath = it.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.remove(aRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("fomod", Qt::CaseInsensitive))
    {
      continue;
    }

    lNumber++;
  }

  return lNumber;
}

void BatchConversion::userHasDoneAnAction(int)
{
  this->mHasUserDoneSomething = true;

  // Body name selector
  auto lBodyNameSelector{this->findChild<QComboBox*>(QString("body_selector_name"))};
  this->disconnect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&BatchConversion::userHasDoneAnAction));

  // Body version selector
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("body_selector_version"))};
  this->disconnect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&BatchConversion::userHasDoneAnAction));
}

void BatchConversion::updateAvailableBodyVersions()
{
  auto lBodyName{static_cast<BodyName>(this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex())};
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("body_selector_version"))};
  lBodyVersionSelector->clear();
  lBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(lBodyName));
  lBodyVersionSelector->setCurrentIndex(0);
}

void BatchConversion::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>("launch_search_button")};
  auto lLineEdit{this->findChild<QLineEdit*>("input_path_directory")};

  // Open a directory chooser dialog
  const auto& lContextPath{Utils::getPathFromKey(this->mLastPaths, "batchConversionInput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::updatePathAtKey(this->mLastPaths, "batchConversionInput", lPath);

  if (!this->mHasUserDoneSomething && lPath.compare("") != 0)
  {
    this->mHasUserDoneSomething = true;
  }

  // Enable or disable path viewer label and launch button
  auto lMustDisableButton{lPath.compare("", Qt::CaseInsensitive) == 0};
  lLineEdit->setDisabled(lMustDisableButton);
  lLaunchSearchButton->setDisabled(lMustDisableButton);
}

void BatchConversion::launchSearchProcess()
{
  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  auto lBodySelected{DataLists::getBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected)};
  auto lRootDir{this->findChild<QLineEdit*>("input_path_directory")->text()};

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
  auto lNumberOSPFiles{this->getNumberFilesByExtension(lRootDir, "*.osp")};
  auto lNumberXMLFiles{this->getNumberFilesByExtension(lRootDir, "*.xml")};
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

    auto lRessourcePath{QString()};

    // Use custom filters
    if (lUserFiltersListSize > 0)
    {
      lRessourcePath = ":/ressources/bodyslide_xml_custom";
    }
    // Use beast hands
    else if (lMustUseBeastHands)
    {
      lRessourcePath = QString(":/%1/%2").arg(lRessourcesFolder).arg("bodyslide_beast_hands_xml");
    }
    // Use classic XML
    else
    {
      lRessourcePath = QString(":/%1/%2").arg(lRessourcesFolder).arg("bodyslide_xml");
    }

    if (!QFile::copy(lRessourcePath, lAbsFilePath))
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
            lUserFiltersConcat += Utils::getXMLFilterBlockFromBody(static_cast<int>(lBodySelected), lMustUseBeastHands, lUserFilter);
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

void BatchConversion::populateSkeletonChooser()
{
  auto lRootDir{Utils::getAppDataPathFolder() + "assets/skeletons/"};
  Utils::cleanPathString(lRootDir);
  auto lAvailableSkeletons{QStringList()};

  // Search for all "*.nif" files
  QDirIterator it(lRootDir, QStringList() << QString("*.nif"), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
    lAvailableSkeletons.push_back(it.fileInfo().absoluteFilePath().remove(lRootDir, Qt::CaseInsensitive));
  }

  // Clear the combo box and add the found files to it
  auto lSkeletonChooserHuman{this->findChild<QComboBox*>("skeleton_chooser_human")};
  lSkeletonChooserHuman->clear();
  lSkeletonChooserHuman->addItems(lAvailableSkeletons);

  auto lSkeletonChooserBeast{this->findChild<QComboBox*>("skeleton_chooser_beast")};
  lSkeletonChooserBeast->clear();
  lSkeletonChooserBeast->addItems(lAvailableSkeletons);
}

void BatchConversion::openBodySlideFiltersEditor()
{
  auto lEditor{new BodySlideFiltersEditor(this, this->mSettings, this->mFiltersList)};
  this->connect(lEditor, &BodySlideFiltersEditor::listEdited, this, &BatchConversion::updateBodySlideFiltersList);
}

void BatchConversion::initBodySlideFiltersList()
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

void BatchConversion::updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList)
{
  // Update the filters list
  this->mFiltersList = aFilterList;

  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};

  // Disable the combobox if there is not any available filter
  if (this->mFiltersList.size() == 0)
  {
    lChooser->clear();
    lChooser->setDisabled(true);
    this->findChild<QLabel*>("bodyslide_filters")->setText("");
    return;
  }

  lChooser->setDisabled(false);

  auto lPrevKey{lChooser->itemText(lChooser->currentIndex())};

  // Fill the combobox
  lChooser->clear();
  for (const auto& lPair : this->mFiltersList)
  {
    lChooser->addItem(lPair.first);
  }

  if (lPrevKey == "" || this->mFiltersList.count(lPrevKey) == 0)
  {
    lChooser->setCurrentIndex(0);
  }
  else
  {
    auto lPrevIndex{0};
    for (const auto& lPair : this->mFiltersList)
    {
      if (lPair.first.compare(lPrevKey, Qt::CaseSensitive) == 0)
      {
        break;
      }
      lPrevIndex++;
    }

    if (lPrevIndex == this->mFiltersList.size())
    {
      lPrevIndex = 0;
    }

    lChooser->setCurrentIndex(lPrevIndex);
  }
}

void BatchConversion::updateBodySlideFiltersListPreview(int aIndex)
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

void BatchConversion::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);

  // Resize the window
  this->adjustSize();
}
