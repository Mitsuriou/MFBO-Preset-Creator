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

  if (Utils::displayQuestionMessage(this, tr("Closing"), tr("Do you want to close the window?"), lIconFolder, "help-circle", tr("Close the window"), tr("Go back to the batch conversion window"), this->mSettings.dangerColor, this->mSettings.successColor, false)
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
  lNamesInApp->setText(QString("<p style=\"text-align: left; padding: 0px; margin: 0px;\">"
                               "<img src=\":/%1/info-circle-smaller\" alt=\"~info icon~\" style=\"vertical-align: baseline;\"> %2</p>")
                         .arg(lIconFolder)
                         .arg(tr("Presets names:")));
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

  // Create the group box
  ComponentFactory::createOutputBox(this, aLayout, 7, 0, lIconFolder);
  auto lOutputGroupBox{this->findChild<QGroupBox*>("output_group_box")};
  this->connect(lOutputGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);

  // Event binding
  auto lOutputPathChooser{this->findChild<QPushButton*>("output_path_chooser")};
  this->connect(lOutputPathChooser, &QPushButton::clicked, this, &BatchConversion::chooseExportDirectory);

  auto lOutputSubpathLineEdit{this->findChild<QLineEdit*>("output_path_subdirectory")};
  this->connect(lOutputSubpathLineEdit, &QLineEdit::textChanged, this, &BatchConversion::updateOutputPreview);

  auto lUseOnlySubdir{this->findChild<QCheckBox*>("only_use_subdirectory")};
  this->connect(lUseOnlySubdir, &QCheckBox::stateChanged, this, &BatchConversion::useOnlySubdirStateChanged);

  // Pre-filled data
  this->updateOutputPreview();
}

void BatchConversion::userHasDoneAnAction()
{
  if (!this->mHasUserDoneSomething)
  {
    this->mHasUserDoneSomething = true;
  }
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
  // TODO: this function needs to be written
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

void BatchConversion::useOnlySubdirStateChanged(int)
{
  this->updateOutputPreview();
}

void BatchConversion::updateOutputPreview()
{
  this->mHasUserDoneSomething = true;

  // Get main directory
  auto lMainDirTextEdit{this->findChild<QLineEdit*>("output_path_directory")};
  auto lMainDirectory{lMainDirTextEdit->text().trimmed()};
  Utils::cleanPathString(lMainDirectory);

  // Get subdirectory
  auto lSubDirectory{this->findChild<QLineEdit*>("output_path_subdirectory")->text().trimmed()};
  Utils::cleanPathString(lSubDirectory);
  auto lIsValidPath{true};

  // Does the user want to define the path only through the secondary path?
  auto lUseOnlySubdir{this->findChild<QCheckBox*>("only_use_subdirectory")->isChecked()};

  // Construct full path
  auto lFullPath(QString(""));
  if (lUseOnlySubdir)
  {
    lMainDirTextEdit->setDisabled(true);

    if (lSubDirectory.length() > 0)
    {
      lFullPath = lSubDirectory;
    }
    else
    {
      lFullPath = tr("No path given or invalid path given.");
      lIsValidPath = false;
    }
  }
  else
  {
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
    else if (lMainDirectory.length() == 0 && lSubDirectory.length() > 0)
    {
      lFullPath = tr("You must choose a directory through the file chooser. Current path defined: \" /%1\".").arg(lSubDirectory);
      lMainDirTextEdit->setDisabled(true);
      lIsValidPath = false;
    }
    else
    {
      lFullPath = tr("No path given or invalid path given.");
      lMainDirTextEdit->setDisabled(true);
      lIsValidPath = false;
    }
  }

  // Set the full path value in the preview label
  auto lOutputPathsPreview{this->findChild<QLabel*>("output_path_preview")};

  auto lNewTextColor{this->mSettings.successColor};

  if (lIsValidPath)
  {
    if (QDir(lFullPath).exists() || lUseOnlySubdir)
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

void BatchConversion::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("output_path_directory")};
  const auto& lContextPath{Utils::getPathFromKey(this->mLastPaths, "batchConversionOutput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::updatePathAtKey(this->mLastPaths, "batchConversionOutput", lPath);
  this->updateOutputPreview();
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
  this->mFiltersList = aFilterList;
  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};
  auto lLabel{this->findChild<QLabel*>("bodyslide_filters")};
  Utils::updateComboBoxBodyslideFiltersList(this->mFiltersList, lChooser, lLabel);
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
