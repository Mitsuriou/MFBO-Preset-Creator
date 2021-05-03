#include "BatchConversion.h"

BatchConversion::BatchConversion(QWidget* parent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mMinimumFirstColumnWidth(300)
{
  // Build the window's interface
  this->setWindowProperties();

  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::createScrollAreaWindowLayout(this)};

  // Setup all the different GUI components
  this->setupGeneralGUI(lMainLayout);
  this->setupSkeletonGUI(lMainLayout);
  this->setupBodySlideGUI(lMainLayout);
  this->setupOutputGUI(lMainLayout);
  this->setupRemainingGUI(lMainLayout);

  // Cursor change for the scroll bar
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderPressed, this, &BatchConversion::scrollbarPressed);
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderReleased, this, &BatchConversion::scrollbarReleased);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderPressed, this, &BatchConversion::scrollbarPressed);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderReleased, this, &BatchConversion::scrollbarReleased);

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
  this->setMinimumWidth(900);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Batch Conversion"));
  this->setWindowIcon(QIcon(QPixmap(":/black/reorder")));
}

void BatchConversion::setupGeneralGUI(QGridLayout* aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // General group box
  auto lGeneralGroupBox{new QGroupBox(tr("General").append("  "), this)};
  Utils::addIconToGroupBox(lGeneralGroupBox, lIconFolder, "tune");
  this->connect(lGeneralGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);
  Utils::setGroupBoxState(lGeneralGroupBox, false);
  aLayout->addWidget(lGeneralGroupBox, 0, 0);

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
  lGeneralGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // First line
  lGeneralGridLayout->addWidget(new QLabel(tr("Input path:"), this), 0, 0);

  // Input label
  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  lInputPathLineEdit->setDisabled(true);
  lGeneralGridLayout->addWidget(lInputPathLineEdit, 0, 1, 1, 3);

  // Input chooser
  auto lInputPathChooser{ComponentFactory::createButton(this, tr("Choose a directory..."), "", "folder", lIconFolder)};
  lGeneralGridLayout->addWidget(lInputPathChooser, 0, 4);

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &BatchConversion::chooseInputDirectory);
}

void BatchConversion::setupSkeletonGUI(QGridLayout* aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Custom skeleton group box
  auto lSkeletonGroupBox{new QGroupBox(tr("Skeleton").append("  "), this)};
  Utils::addIconToGroupBox(lSkeletonGroupBox, lIconFolder, "vector-polyline");
  this->connect(lSkeletonGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);
  Utils::setGroupBoxState(lSkeletonGroupBox, false);
  aLayout->addWidget(lSkeletonGroupBox, 1, 0);

  auto lSkeletonGridLayout{new QGridLayout(lSkeletonGroupBox)};
  lSkeletonGridLayout->setColumnStretch(0, 0);
  lSkeletonGridLayout->setColumnStretch(1, 1);
  lSkeletonGridLayout->setColumnStretch(2, 0);
  lSkeletonGridLayout->setSpacing(10);
  lSkeletonGridLayout->setContentsMargins(15, 20, 15, 15);
  lSkeletonGridLayout->setAlignment(Qt::AlignTop);
  lSkeletonGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Human skeleton file
  lSkeletonGridLayout->addWidget(new QLabel(tr("Skeleton file (human):"), this), 1, 0);

  auto lSkeletonChooserHuman{new QComboBox(this)};
  lSkeletonChooserHuman->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooserHuman->setCursor(Qt::PointingHandCursor);
  lSkeletonChooserHuman->setObjectName("skeleton_chooser_human");
  lSkeletonGridLayout->addWidget(lSkeletonChooserHuman, 1, 1);

  auto lSkeletonRefresherHuman{ComponentFactory::createButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  lSkeletonGridLayout->addWidget(lSkeletonRefresherHuman, 1, 2);

  // Beast skeleton file
  lSkeletonGridLayout->addWidget(new QLabel(tr("Skeleton file (beast):"), this), 2, 0);

  auto lSkeletonChooserBeast{new QComboBox(this)};
  lSkeletonChooserBeast->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooserBeast->setCursor(Qt::PointingHandCursor);
  lSkeletonChooserBeast->setObjectName("skeleton_chooser_beast");
  lSkeletonGridLayout->addWidget(lSkeletonChooserBeast, 2, 1);

  auto lSkeletonRefresherBeast{ComponentFactory::createButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  lSkeletonGridLayout->addWidget(lSkeletonRefresherBeast, 2, 2);

  this->populateSkeletonChoosers();

  // Event binding
  this->connect(lSkeletonRefresherHuman, &QPushButton::clicked, this, &BatchConversion::populateSkeletonChoosers);
  this->connect(lSkeletonRefresherBeast, &QPushButton::clicked, this, &BatchConversion::populateSkeletonChoosers);
}

void BatchConversion::setupBodySlideGUI(QGridLayout* aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // BodySlide output settings group box
  auto lBodyslideGroupBox{new QGroupBox(tr("BodySlide").append("  "), this)};
  Utils::addIconToGroupBox(lBodyslideGroupBox, lIconFolder, "bodyslide-logo");
  this->connect(lBodyslideGroupBox, &QGroupBox::toggled, this, &BatchConversion::groupBoxChecked);
  Utils::setGroupBoxState(lBodyslideGroupBox, false);
  aLayout->addWidget(lBodyslideGroupBox, 2, 0);

  // Grid layout
  auto lBodyslideGridLayout{new QGridLayout(lBodyslideGroupBox)};
  lBodyslideGridLayout->setColumnStretch(0, 0);
  lBodyslideGridLayout->setColumnStretch(1, 1);
  lBodyslideGridLayout->setColumnStretch(2, 1);
  lBodyslideGridLayout->setColumnStretch(3, 2);
  lBodyslideGridLayout->setColumnStretch(4, 0);
  lBodyslideGridLayout->setSpacing(10);
  lBodyslideGridLayout->setContentsMargins(15, 20, 15, 15);
  lBodyslideGridLayout->setAlignment(Qt::AlignTop);
  lBodyslideGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Targeted body and version
  auto lDefaultBodyVersionSettings{DataLists::getSplittedNameVersionFromBodyVersion(mSettings.defaultBatchConversionBody)};

  lBodyslideGridLayout->addWidget(new QLabel(tr("Targeted body and version:"), this), 0, 0);

  auto lBodyNameSelector{new QComboBox(this)};
  lBodyNameSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyNameSelector->setCursor(Qt::PointingHandCursor);
  lBodyNameSelector->addItems(DataLists::getBodiesNames());
  lBodyNameSelector->setCurrentIndex(lDefaultBodyVersionSettings.first);
  lBodyNameSelector->setObjectName(QString("body_selector_name"));
  lBodyslideGridLayout->addWidget(lBodyNameSelector, 0, 1);

  auto lBodyVersionSelector{new QComboBox(this)};
  lBodyVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyVersionSelector->setCursor(Qt::PointingHandCursor);
  lBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lBodyVersionSelector->setCurrentIndex(lDefaultBodyVersionSettings.second);
  lBodyVersionSelector->setObjectName(QString("body_selector_version"));
  lBodyslideGridLayout->addWidget(lBodyVersionSelector, 0, 2);

  //
  lBodyslideGridLayout->addWidget(new QLabel(tr("BodySlide files names:"), this), 1, 0);

  auto lOSPXMLNamesLineEdit{new QLineEdit(this)};
  lOSPXMLNamesLineEdit->setObjectName("names_osp_xml_input");
  lBodyslideGridLayout->addWidget(lOSPXMLNamesLineEdit, 1, 1, 1, 4);

  //
  auto lNamesInApp{new QLabel(this)};
  lNamesInApp->setTextFormat(Qt::RichText);
  lNamesInApp->setText(QString("<p style=\"text-align: left; padding: 0px; margin: 0px;\">"
                               "<img src=\":/%1/info-circle-smaller\" alt=\"~info icon~\" style=\"vertical-align: baseline;\"> %2</p>")
                         .arg(lIconFolder)
                         .arg(tr("Presets names:")));
  lNamesInApp->setToolTip(QString(tr("This field represents the names under which the presets will be listed in the BodySlide application.")));
  lBodyslideGridLayout->addWidget(lNamesInApp, 2, 0);

  auto lNamesInAppLineEdit{new QLineEdit(this)};
  lNamesInAppLineEdit->setObjectName("names_bodyslide_input");
  lBodyslideGridLayout->addWidget(lNamesInAppLineEdit, 2, 1, 1, 4);

  // BodySlide filters
  lBodyslideGridLayout->addWidget(new QLabel(tr("BodySlide filters:"), this), 3, 0);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  lBodyslideGridLayout->addWidget(lFiltersListChooser, 3, 1);

  auto lFiltersList{new QLabel("", this)};
  lFiltersList->setObjectName("bodyslide_filters");
  lFiltersList->setWordWrap(true);
  lBodyslideGridLayout->addWidget(lFiltersList, 3, 2, 1, 2);

  auto lEditFilters{ComponentFactory::createButton(this, tr("Edit BodySlide filters sets"), "", "filter", lIconFolder, "edit_filters")};
  lBodyslideGridLayout->addWidget(lEditFilters, 3, 4);

  // Event binding
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &BatchConversion::updateAvailableBodyVersions);
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
  ComponentFactory::createOutputBox(this, aLayout, 3, 0, lIconFolder, mSettings.batchConversionOutputPath, this->mMinimumFirstColumnWidth);
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

void BatchConversion::setupRemainingGUI(QGridLayout* aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Batch generate button
  auto lGenerateButton{ComponentFactory::createButton(this, tr("Batch generate the files on my computer"), "", "build", lIconFolder)};
  aLayout->addWidget(lGenerateButton, 4, 0, Qt::AlignBottom);
  aLayout->setRowStretch(4, 1);

  // Event binding
  this->connect(lGenerateButton, &QPushButton::clicked, this, &BatchConversion::launchSearchProcess);
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
}

void BatchConversion::launchSearchProcess()
{
  // TODO: this function needs to be written
}

void BatchConversion::populateSkeletonChoosers()
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

void BatchConversion::scrollbarPressed()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::ClosedHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::ClosedHandCursor);
}

void BatchConversion::scrollbarReleased()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
}

void BatchConversion::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);
}
