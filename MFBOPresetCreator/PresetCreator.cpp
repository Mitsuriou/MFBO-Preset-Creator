#include "PresetCreator.h"

PresetCreator::PresetCreator(QWidget* aParent, const Struct::Settings& aSettings)
  : QWidget(aParent)
  , mSettings(aSettings)
  , mMinimumFirstColumnWidth(300)
{
  // Very first container in which the scroll area will be added
  auto lBaseLayout{new QVBoxLayout(this)};
  lBaseLayout->setContentsMargins(0, 0, 0, 0);

  // Create a scroll area
  auto lScrollArea{new QScrollArea(this)};
  lScrollArea->setObjectName("scrollable_zone");
  lScrollArea->setContentsMargins(0, 0, 0, 0);
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->setWidgetResizable(true);
  // Remove the borders of the scroll area
  lScrollArea->setStyleSheet("QScrollArea{border: none;}");

  // Add a QFrame to permit automatic expanding of the content inside the scroll area
  auto lMainWidget{new QFrame(this)};

  // Main container
  auto lMainLayout{new QVBoxLayout(lMainWidget)};
  lMainLayout->setContentsMargins(10, 10, 10, 10);

  lScrollArea->setWidget(lMainWidget);
  lBaseLayout->addWidget(lScrollArea);

  // Setup all the different GUI components
  this->setupBodyMeshesGUI(*lMainLayout);
  this->setupSkeletonGUI(*lMainLayout);
  this->setupBodySlideGUI(*lMainLayout);
  this->setupOutputGUI(*lMainLayout);
  this->setupRemainingGUI(*lMainLayout);

  // Update the GUI based on the values entered
  this->refreshAllPreviewFields();

  // Cursor change for the scroll bar
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderPressed, this, &PresetCreator::scrollbarPressed);
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderReleased, this, &PresetCreator::scrollbarReleased);

  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderPressed, this, &PresetCreator::scrollbarPressed);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderReleased, this, &PresetCreator::scrollbarReleased);
}

void PresetCreator::updateSettings(Struct::Settings aSettings)
{
  this->mSettings = aSettings;
}

void PresetCreator::fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList)
{
  // Set the preset name
  this->findChild<QLineEdit*>("names_osp_xml_input")->setText(aPresetName);
  this->findChild<QLineEdit*>("names_bodyslide_input")->setText(aPresetName);

  for (const auto& lResult : aResultsList)
  {
    auto lRole{static_cast<AssistedConversionRole>(lResult.role)};
    switch (lRole)
    {
      case AssistedConversionRole::Body:
        this->findChild<QLineEdit*>("meshes_path_input_femalebody")->setText(lResult.path);
        this->findChild<QLineEdit*>("body_mesh_name_input")->setText(lResult.name);
        break;
      case AssistedConversionRole::Feet:
        this->findChild<QLineEdit*>("meshes_path_input_femalefeet")->setText(lResult.path);
        this->findChild<QLineEdit*>("feet_mesh_name_input")->setText(lResult.name);
        break;
      case AssistedConversionRole::Hands:
        this->findChild<QLineEdit*>("meshes_path_input_femalehands")->setText(lResult.path);
        this->findChild<QLineEdit*>("hands_mesh_name_input")->setText(lResult.name);
        break;
      case AssistedConversionRole::Skeleton:
        auto lNeedCustomSkeleton{this->findChild<QCheckBox*>("use_custom_skeleton")};
        if (!lNeedCustomSkeleton->isChecked())
        {
          lNeedCustomSkeleton->setChecked(true);
        }

        this->findChild<QLineEdit*>("skeleton_path_directory")->setText(lResult.path);
        this->findChild<QLineEdit*>("skeleton_name")->setText(lResult.name);
        break;
    }
  }
}

void PresetCreator::setupBodyMeshesGUI(QVBoxLayout& aLayout)
{
  // CBBE body meshes group box
  auto lMeshesGroupBox{new QGroupBox(tr("Original mod's body meshes"), this)};
  aLayout.addWidget(lMeshesGroupBox);

  // Grid layout
  auto lMeshesGridLayout{new QGridLayout(lMeshesGroupBox)};
  lMeshesGridLayout->setSpacing(10);
  lMeshesGridLayout->setContentsMargins(15, 20, 15, 15);
  lMeshesGridLayout->setAlignment(Qt::AlignTop);
  lMeshesGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Body meshes names
  auto lMeshestitlePreview{new QLabel(tr("Meshes paths and names:"), this)};
  lMeshesGridLayout->addWidget(lMeshestitlePreview, 1, 0, 3, 1);

  // femalebody
  auto lMeshesPathFemaleBodyLineEdit{new QLineEdit(this)};
  lMeshesPathFemaleBodyLineEdit->setObjectName("meshes_path_input_femalebody");
  lMeshesPathFemaleBodyLineEdit->setPlaceholderText("meshes/");
  lMeshesGridLayout->addWidget(lMeshesPathFemaleBodyLineEdit, 1, 1);

  auto lBodyMeshNameInput{new QLineEdit(this)};
  lBodyMeshNameInput->setObjectName("body_mesh_name_input");
  lMeshesGridLayout->addWidget(lBodyMeshNameInput, 1, 2);
  lBodyMeshNameInput->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_ -]+"), this));
  lBodyMeshNameInput->setText("femalebody");
  lBodyMeshNameInput->setPlaceholderText("femalebody");

  auto lBodyMeshNameLabel1{new QLabel(tr("_0.nif/_1.nif"), this)};
  lBodyMeshNameLabel1->setObjectName("body_mesh_name_extension");
  lMeshesGridLayout->addWidget(lBodyMeshNameLabel1, 1, 3);

  // femalefeet
  auto lMeshesPathFemaleFeetLineEdit{new QLineEdit(this)};
  lMeshesPathFemaleFeetLineEdit->setObjectName("meshes_path_input_femalefeet");
  lMeshesPathFemaleFeetLineEdit->setPlaceholderText("meshes/");
  lMeshesGridLayout->addWidget(lMeshesPathFemaleFeetLineEdit, 2, 1);

  auto lFeetMeshNameInput{new QLineEdit(this)};
  lFeetMeshNameInput->setObjectName("feet_mesh_name_input");
  lMeshesGridLayout->addWidget(lFeetMeshNameInput, 2, 2);
  lFeetMeshNameInput->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_ -]+"), this));
  lFeetMeshNameInput->setText("femalefeet");
  lFeetMeshNameInput->setPlaceholderText("femalefeet");

  auto lBodyMeshNameLabel2{new QLabel(tr("_0.nif/_1.nif"), this)};
  lBodyMeshNameLabel2->setObjectName("feet_mesh_name_extension");
  lMeshesGridLayout->addWidget(lBodyMeshNameLabel2, 2, 3);

  // femalehands
  auto lMeshesPathFemaleHandsLineEdit{new QLineEdit(this)};
  lMeshesPathFemaleHandsLineEdit->setObjectName("meshes_path_input_femalehands");
  lMeshesPathFemaleHandsLineEdit->setPlaceholderText("meshes/");
  lMeshesGridLayout->addWidget(lMeshesPathFemaleHandsLineEdit, 3, 1);

  auto lHandsMeshNameInput{new QLineEdit(this)};
  lHandsMeshNameInput->setObjectName("hands_mesh_name_input");
  lMeshesGridLayout->addWidget(lHandsMeshNameInput, 3, 2);
  lHandsMeshNameInput->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_ -]+"), this));
  lHandsMeshNameInput->setText("femalehands");
  lHandsMeshNameInput->setPlaceholderText("femalehands");

  auto lBodyMeshNameLabel3{new QLabel(tr("_0.nif/_1.nif"), this)};
  lBodyMeshNameLabel3->setObjectName("hands_mesh_name_extension");
  lMeshesGridLayout->addWidget(lBodyMeshNameLabel3, 3, 3);

  // Preview
  auto lLabelPreview{new QLabel(this)};
  lLabelPreview->setText(tr("Preview:"));
  lMeshesGridLayout->addWidget(lLabelPreview, 4, 0);

  auto lMeshesPreview{new QLabel(this)};
  lMeshesPreview->setObjectName("meshes_preview");
  lMeshesPreview->setAutoFillBackground(true);
  lMeshesGridLayout->addWidget(lMeshesPreview, 4, 1, 1, 2);

  // Beast hands
  auto lLabelBeastHands{new QLabel(tr("Use beast hands?"), this)};
  lLabelBeastHands->setObjectName("label_use_beast_hands");
  lMeshesGridLayout->addWidget(lLabelBeastHands, 5, 0);

  auto lNeedBeastHands{new QCheckBox(tr("Check this box if the follower or NPC uses beast hands."), this)};
  lNeedBeastHands->setCursor(Qt::PointingHandCursor);
  lNeedBeastHands->setObjectName("use_beast_hands");
  lMeshesGridLayout->addWidget(lNeedBeastHands, 5, 1, 1, 2);

  // Event binding
  this->connect(lMeshesPathFemaleBodyLineEdit, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lMeshesPathFemaleFeetLineEdit, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lMeshesPathFemaleHandsLineEdit, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lBodyMeshNameInput, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lFeetMeshNameInput, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lHandsMeshNameInput, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lNeedBeastHands, &QCheckBox::clicked, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
}

void PresetCreator::setupBodySlideGUI(QVBoxLayout& aLayout)
{
  // BodySlide defined names group box
  auto lBodyslideGroupBox{new QGroupBox(tr("BodySlide output"), this)};
  aLayout.addWidget(lBodyslideGroupBox);

  // Grid layout
  auto lBodyslideGridLayout{new QGridLayout(lBodyslideGroupBox)};
  lBodyslideGridLayout->setColumnStretch(0, 0);
  lBodyslideGridLayout->setColumnStretch(1, 1);
  lBodyslideGridLayout->setColumnStretch(2, 0);
  lBodyslideGridLayout->setSpacing(10);
  lBodyslideGridLayout->setContentsMargins(15, 20, 15, 15);
  lBodyslideGridLayout->setAlignment(Qt::AlignTop);
  lBodyslideGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // CBBE 3BBB version
  auto lBodyLabel{new QLabel(tr("Targeted body and version:"), this)};
  lBodyslideGridLayout->addWidget(lBodyLabel, 0, 0);

  auto lBodySelector{new QComboBox(this)};
  lBodySelector->setItemDelegate(new QStyledItemDelegate());
  lBodySelector->setCursor(Qt::PointingHandCursor);
  lBodySelector->addItems(DataLists::getBodiesNameVersions());
  lBodySelector->setCurrentIndex(static_cast<int>(mSettings.defaultMainWindowBody));
  lBodySelector->setObjectName(QString("body_selector"));
  lBodyslideGridLayout->addWidget(lBodySelector, 0, 1, 1, 2);

  // Second line
  auto lOSPXMLNames{new QLabel(tr("Bodyslide files names:"), this)};
  lBodyslideGridLayout->addWidget(lOSPXMLNames, 1, 0);

  auto lOSPXMLNamesLineEdit{new QLineEdit(this)};
  lOSPXMLNamesLineEdit->setObjectName("names_osp_xml_input");
  lOSPXMLNamesLineEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_ -]+"), this));
  lBodyslideGridLayout->addWidget(lOSPXMLNamesLineEdit, 1, 1, 1, 2);

  // Third line
  auto lLabelOspXmlNames{new QLabel(tr("Preview:"), this)};
  lBodyslideGridLayout->addWidget(lLabelOspXmlNames, 2, 0);

  auto lPathsNamesOspXmlNames{new QLabel("", this)};
  lPathsNamesOspXmlNames->setObjectName("names_osp_xml_preview");
  lPathsNamesOspXmlNames->setAutoFillBackground(true);
  lBodyslideGridLayout->addWidget(lPathsNamesOspXmlNames, 2, 1, 1, 2);

  // Fourth line
  auto lNamesInApp{new QLabel(this)};
  lNamesInApp->setTextFormat(Qt::RichText);
  lNamesInApp->setText(tr("Preset names: &#128712;"));
  lNamesInApp->setToolTip(QString(tr("This field represents the name under which the preset will be listed in the BodySlide application.")));
  lBodyslideGridLayout->addWidget(lNamesInApp, 3, 0);

  auto lNamesInAppLineEdit{new QLineEdit(this)};
  lNamesInAppLineEdit->setObjectName("names_bodyslide_input");
  lBodyslideGridLayout->addWidget(lNamesInAppLineEdit, 3, 1, 1, 2);

  // Fifth line
  auto lLabelNamesInApp{new QLabel(tr("Preview:"), this)};
  lBodyslideGridLayout->addWidget(lLabelNamesInApp, 4, 0);

  auto lResultNamesInApp{new QLabel("", this)};
  lResultNamesInApp->setObjectName("names_bodyslide_preview");
  lBodyslideGridLayout->addWidget(lResultNamesInApp, 4, 1, 1, 2);

  // Sixth line
  auto lLabelFilters{new QLabel(tr("BodySlide filters:"), this)};
  lBodyslideGridLayout->addWidget(lLabelFilters, 5, 0);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  lBodyslideGridLayout->addWidget(lFiltersListChooser, 5, 1);

  auto lFiltersList{new QLabel("", this)};
  lFiltersList->setObjectName("bodyslide_filters");
  lFiltersList->setWordWrap(true);
  lBodyslideGridLayout->addWidget(lFiltersList, 5, 2);

  auto lEditFilters{new QPushButton(this)};
  lEditFilters->setText("Edit custom BodySlide filters");
  lEditFilters->setCursor(Qt::PointingHandCursor);
  lEditFilters->setObjectName("edit_filters");
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};
  lEditFilters->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/filter")));
  lBodyslideGridLayout->addWidget(lEditFilters, 5, 3);

  // Pre-bind initialization functions
  this->updateOSPXMLPreview(QString(""));
  this->updateBodyslideNamesPreview(QString(""));

  // Event binding
  this->connect(lBodySelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lOSPXMLNamesLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateOSPXMLPreview);
  this->connect(lNamesInAppLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateBodyslideNamesPreview);
  this->connect(lFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&PresetCreator::updateBodySlideFiltersListPreview));
  this->connect(lEditFilters, &QPushButton::clicked, this, &PresetCreator::openBodySlideFiltersEditor);

  // Post-bind initialization functions
  this->initBodySlideFiltersList();
}

void PresetCreator::setupSkeletonGUI(QVBoxLayout& aLayout)
{
  // Custom skeleton group box
  auto lSkeletonGroupBox{new QGroupBox(tr("Skeleton"), this)};
  aLayout.addWidget(lSkeletonGroupBox);

  auto lSkeletonGridLayout{new QGridLayout(lSkeletonGroupBox)};
  lSkeletonGridLayout->setSpacing(10);
  lSkeletonGridLayout->setContentsMargins(15, 20, 15, 15);
  lSkeletonGridLayout->setAlignment(Qt::AlignTop);
  lSkeletonGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Skeleton
  auto lLabelSkeleton{new QLabel(this)};
  lLabelSkeleton->setTextFormat(Qt::RichText);
  lLabelSkeleton->setText(tr("Use a custom skeleton? &#128712;"));
  lLabelSkeleton->setToolTip(QString(tr("Note: not overriding a custom skeleton would cause breasts collision and physics to be inaccurate.")));
  lSkeletonGridLayout->addWidget(lLabelSkeleton, 0, 0);

  auto lNeedCustomSkeleton{new QCheckBox(tr("Check this box if the follower or NPC uses a custom skeleton."), this)};
  lNeedCustomSkeleton->setCursor(Qt::PointingHandCursor);
  lNeedCustomSkeleton->setObjectName("use_custom_skeleton");
  lSkeletonGridLayout->addWidget(lNeedCustomSkeleton, 0, 1);

  // Choose the skeleton file
  auto lLabelSkeletonChooser{new QLabel(tr("Skeleton file:"), this)};
  lLabelSkeletonChooser->setObjectName("label_skeleton_chooser");
  lSkeletonGridLayout->addWidget(lLabelSkeletonChooser, 1, 0);

  auto lSkeletonChooser{new QComboBox(this)};
  lSkeletonChooser->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooser->setCursor(Qt::PointingHandCursor);
  lSkeletonChooser->setObjectName("skeleton_chooser");
  lSkeletonGridLayout->addWidget(lSkeletonChooser, 1, 1);

  this->populateSkeletonChooser();

  auto lSkeletonRefresher{new QPushButton(this)};
  lSkeletonRefresher->setCursor(Qt::PointingHandCursor);
  lSkeletonRefresher->setObjectName("skeleton_chooser_refresher");
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};
  lSkeletonRefresher->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/reload")));
  lSkeletonGridLayout->addWidget(lSkeletonRefresher, 1, 2);

  // Skeleton path
  auto lLabelSkeletonPath{new QLabel(tr("Skeleton path:"), this)};
  lLabelSkeletonPath->setObjectName("label_skeleton_path_directory");
  lSkeletonGridLayout->addWidget(lLabelSkeletonPath, 2, 0);

  auto lSkeletonPathLineEdit{new QLineEdit(this)};
  lSkeletonPathLineEdit->setObjectName("skeleton_path_directory");
  lSkeletonPathLineEdit->setPlaceholderText("meshes/");
  lSkeletonGridLayout->addWidget(lSkeletonPathLineEdit, 2, 1);

  // Skeleton name
  auto lSkeletonNameLabel{new QLabel(tr("Skeleton file name:"), this)};
  lSkeletonNameLabel->setObjectName("label_skeleton_female");
  lSkeletonGridLayout->addWidget(lSkeletonNameLabel, 3, 0);

  auto lSkeletonName{new QLineEdit(this)};
  lSkeletonName->setObjectName("skeleton_name");
  lSkeletonName->setPlaceholderText("skeleton_female");
  lSkeletonName->setText("skeleton_female");
  lSkeletonGridLayout->addWidget(lSkeletonName, 3, 1);

  auto lSkeletonNameExtension{new QLabel(tr(".nif"), this)};
  lSkeletonNameExtension->setObjectName("skeleton_name_extension");
  lSkeletonGridLayout->addWidget(lSkeletonNameExtension, 3, 2);

  // Skeleton path preview
  auto lSkeletontitlePreview{new QLabel(tr("Preview:"), this)};
  lSkeletontitlePreview->setObjectName("label_skeleton_path_preview");
  lSkeletonGridLayout->addWidget(lSkeletontitlePreview, 4, 0);

  auto lSkeletonPathsPreview{new QLabel(this)};
  lSkeletonPathsPreview->setObjectName("skeleton_path_preview");
  lSkeletonGridLayout->addWidget(lSkeletonPathsPreview, 4, 1);

  // Initialization functions
  this->updateSkeletonPreview();

  // Event binding
  this->connect(lNeedCustomSkeleton, &QCheckBox::stateChanged, this, &PresetCreator::updateSkeletonPathState);
  lNeedCustomSkeleton->setChecked(true);
  lNeedCustomSkeleton->setChecked(false);

  this->connect(lSkeletonPathLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateSkeletonPreview);
  this->connect(lSkeletonRefresher, &QPushButton::clicked, this, &PresetCreator::populateSkeletonChooser);
  this->connect(lSkeletonName, &QLineEdit::textChanged, this, &PresetCreator::updateSkeletonPreview);
}

void PresetCreator::setupOutputGUI(QVBoxLayout& aLayout)
{
  // Output group box
  auto lOutputGroupBox{new QGroupBox(tr("Files generation's output location"), this)};
  aLayout.addWidget(lOutputGroupBox);

  // Grid layout
  auto lOutputGridLayout{new QGridLayout(lOutputGroupBox)};
  lOutputGridLayout->setSpacing(10);
  lOutputGridLayout->setContentsMargins(15, 20, 15, 15);
  lOutputGridLayout->setAlignment(Qt::AlignTop);
  lOutputGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // First line
  auto lOutputPathLabel{new QLabel(tr("Output directory path:"), this)};
  lOutputGridLayout->addWidget(lOutputPathLabel, 0, 0);

  auto lOutputPathLineEdit{new QLineEdit(this)};
  lOutputPathLineEdit->setReadOnly(true);
  lOutputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lOutputPathLineEdit->setObjectName("output_path_directory");
  lOutputPathLineEdit->setText(mSettings.mainWindowOutputPath);
  lOutputGridLayout->addWidget(lOutputPathLineEdit, 0, 1);

  auto lOutputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lOutputPathChooser->setCursor(Qt::PointingHandCursor);
  lOutputGridLayout->addWidget(lOutputPathChooser, 0, 2);

  // Second line
  auto lLabelSubDirectoryPath{new QLabel(tr("Output subdirectory name/path:"), this)};
  lOutputGridLayout->addWidget(lLabelSubDirectoryPath, 1, 0);

  auto lOutputSubpathLineEdit{new QLineEdit(this)};
  lOutputSubpathLineEdit->setObjectName("output_path_subdirectory");
  lOutputGridLayout->addWidget(lOutputSubpathLineEdit, 1, 1);

  // Third line
  auto lOutputtitlePreview{new QLabel(tr("Preview:"), this)};
  lOutputGridLayout->addWidget(lOutputtitlePreview, 2, 0);

  auto lOutputPathsPreview{new QLabel("", this)};
  lOutputPathsPreview->setObjectName("output_path_preview");
  lOutputPathsPreview->setAutoFillBackground(true);
  lOutputGridLayout->addWidget(lOutputPathsPreview, 2, 1);

  // Event binding
  this->connect(lOutputPathChooser, &QPushButton::clicked, this, &PresetCreator::chooseExportDirectory);
  this->connect(lOutputSubpathLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateOutputPreview);

  // Pre-filled data
  this->updateOutputPreview();
}

void PresetCreator::setupRemainingGUI(QVBoxLayout& aLayout)
{
  // Generate button
  auto lGenerateButton{new QPushButton(tr("Generate the files on my computer"), this)};
  lGenerateButton->setCursor(Qt::PointingHandCursor);
  aLayout.addWidget(lGenerateButton);

  // Event binding
  this->connect(lGenerateButton, &QPushButton::clicked, this, &PresetCreator::generateDirectoryStructure);
}

void PresetCreator::updateGUIOnBodyChange()
{
  // Get the GUI components
  auto lLabelBeastHands{this->findChild<QLabel*>("label_use_beast_hands")};
  auto lMustUseBeastHands{this->findChild<QCheckBox*>("use_beast_hands")};
  auto lMeshesPathFeet{this->findChild<QLineEdit*>("meshes_path_input_femalefeet")};
  auto lMeshesPathHands{this->findChild<QLineEdit*>("meshes_path_input_femalehands")};
  auto lFeetName{this->findChild<QLineEdit*>("feet_mesh_name_input")};
  auto lHandsName{this->findChild<QLineEdit*>("hands_mesh_name_input")};
  auto lFeetExtension{this->findChild<QLabel*>("feet_mesh_name_extension")};
  auto lHandsExtension{this->findChild<QLabel*>("hands_mesh_name_extension")};

  // The new "disabled" state to apply
  auto lDisableBeastHands{false};
  auto lDisableFeet{false};
  auto lDisableHands{false};

  auto lBodySelected{static_cast<BodyNameVersion>(this->findChild<QComboBox*>(QString("body_selector"))->currentIndex())};
  switch (lBodySelected)
  {
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      lDisableBeastHands = true;
      lDisableFeet = true;
      lDisableHands = true;
      break;
    case BodyNameVersion::BHUNP_3BBB_2_13:
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_13:
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_13:
    case BodyNameVersion::BHUNP_BBP_2_13:
    case BodyNameVersion::BHUNP_BBP_Advanced_2_13:
    case BodyNameVersion::BHUNP_TBBP_2_13:
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_13:
      lDisableBeastHands = true;
      break;
  }

  lLabelBeastHands->setDisabled(lDisableBeastHands);
  lMustUseBeastHands->setDisabled(lDisableBeastHands);
  lMeshesPathFeet->setDisabled(lDisableFeet);
  lMeshesPathHands->setDisabled(lDisableHands);
  lFeetName->setDisabled(lDisableFeet);
  lHandsName->setDisabled(lDisableHands);
  lFeetExtension->setDisabled(lDisableFeet);
  lHandsExtension->setDisabled(lDisableHands);
}

bool PresetCreator::generateXMLFile(const QString& aEntryDirectory,
                                    const bool& aGenerateFilesInExistingMainDirectory,
                                    const QString& aOSPXMLNames,
                                    const bool& aMustUseBeastHands,
                                    const QString& aRessourcesFolder,
                                    const int& aBodySelected,
                                    const QString& aBodyslideSlidersetsNames)
{
  // Create the SliderGroups directory
  auto lSliderGroupsDirectory{aEntryDirectory + QDir::separator() + "CalienteTools" + QDir::separator() + "BodySlide" + QDir::separator() + "SliderGroups"};
  if (!QDir(lSliderGroupsDirectory).exists())
  {
    QDir().mkpath(lSliderGroupsDirectory);
  }
  else if (!aGenerateFilesInExistingMainDirectory)
  {
    Utils::displayWarningMessage(tr("Error while creating the meshes directory: \"") + lSliderGroupsDirectory + tr("\" already exists."));
    return false;
  }

  // Copy the XML file
  auto lXMLPathName{lSliderGroupsDirectory + QDir::separator() + aOSPXMLNames + ".xml"};
  auto lUserFilters{Utils::splitString(this->findChild<QLabel*>("bodyslide_filters")->text(), " ; ")};
  auto lUserFiltersListSize{lUserFilters.size()};

  auto lRessourcePath{QString("")};

  // Use custom filters
  if (lUserFiltersListSize > 0)
  {
    lRessourcePath = ":/ressources/bodyslide_xml_custom";
  }
  // Use beast hands
  else if (aMustUseBeastHands)
  {
    lRessourcePath = QString(":/%1/%2").arg(aRessourcesFolder).arg("bodyslide_beast_hands_xml");
  }
  // Use classic XML
  else
  {
    lRessourcePath = QString(":/%1/%2").arg(aRessourcesFolder).arg("bodyslide_xml");
  }

  if (!QFile::copy(lRessourcePath, lXMLPathName))
  {
    Utils::displayWarningMessage(tr("The XML file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions."));
    return false;
  }

  QFile lXMLFile(lXMLPathName);
  lXMLFile.setPermissions(QFile::WriteUser);

  QByteArray lTempXMLContent;

  if (lXMLFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    lTempXMLContent = lXMLFile.readAll();
    lXMLFile.close();
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lXMLPathName + tr("\"."));
    return false;
  }

  if (lTempXMLContent.length() > 0)
  {
    if (lXMLFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
      auto lTextToParse{static_cast<QString>(lTempXMLContent)};

      // Custom BodySlide filters
      if (lUserFiltersListSize > 0)
      {
        auto lUserFiltersConcat{QString("")};

        for (const auto& lUserFilter : lUserFilters)
        {
          lUserFiltersConcat += Utils::getXMLFilterBlockFromBody(aBodySelected, aMustUseBeastHands, lUserFilter);
        }

        lTextToParse.replace(QString("{%%bodyslide_filters_block%%}"), lUserFiltersConcat);
      }

      // BodySlide preset name
      lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), aBodyslideSlidersetsNames);

      QTextStream lTextStream(&lXMLFile);
      lTextStream << lTextToParse;
      lTextStream.flush();

      lXMLFile.close();
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lXMLPathName + tr("\"."));
      return false;
    }
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to parse the XML BodySlide file."));
    return false;
  }

  return true;
}

bool PresetCreator::generateOSPFile(const QString& aEntryDirectory,
                                    const bool& aGenerateFilesInExistingMainDirectory,
                                    const QString& aOSPXMLNames,
                                    const bool& aMustUseBeastHands,
                                    const QString& aRessourcesFolder,
                                    const QString& aBodyslideSlidersetsNames,
                                    QString aMeshesPathBody,
                                    QString aMeshesPathFeet,
                                    QString aMeshesPathHands,
                                    const QString& aBodyName,
                                    const QString& aFeetName,
                                    const QString& aHandsName)
{
  // Create the SliderSets directory
  auto lSliderSetsDirectory{aEntryDirectory + QDir::separator() + "CalienteTools" + QDir::separator() + "BodySlide" + QDir::separator() + "SliderSets"};

  if (!QDir(lSliderSetsDirectory).exists())
  {
    QDir().mkpath(lSliderSetsDirectory);
  }
  else if (!aGenerateFilesInExistingMainDirectory)
  {
    Utils::displayWarningMessage(tr("Error while creating the meshes directory: \"") + lSliderSetsDirectory + tr("\" already exists."));
    return false;
  }

  // Copy the OSP file
  auto lOSPPathName(lSliderSetsDirectory + QDir::separator() + aOSPXMLNames + ".osp");
  if (aMustUseBeastHands)
  {
    if (!QFile::copy(":/" + aRessourcesFolder + "/bodyslide_beast_hands_osp", lOSPPathName))
    {
      Utils::displayWarningMessage(tr("The OSP file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions."));
      return false;
    }
  }
  else
  {
    if (!QFile::copy(":/" + aRessourcesFolder + "/bodyslide_osp", lOSPPathName))
    {
      Utils::displayWarningMessage(tr("The OSP file could not be created. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions."));
      return false;
    }
  }

  QFile lOSPFile(lOSPPathName);
  lOSPFile.setPermissions(QFile::WriteUser);

  QByteArray lTempOSPContent;

  if (lOSPFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    lTempOSPContent = lOSPFile.readAll();
    lOSPFile.close();
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lOSPPathName + tr("\"."));
    return false;
  }

  // Replace the slider sets names
  if (lTempOSPContent.length() > 0)
  {
    if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
      auto lTextToParse{static_cast<QString>(lTempOSPContent)};
      lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), aBodyslideSlidersetsNames);
      lTextToParse.replace(QString("{%%body_output_path%%}"), aMeshesPathBody.replace("/", "\\"));
      lTextToParse.replace(QString("{%%feet_output_path%%}"), aMeshesPathFeet.replace("/", "\\"));
      lTextToParse.replace(QString("{%%hands_output_path%%}"), aMeshesPathHands.replace("/", "\\"));

      lTextToParse.replace(QString("{%%body_output_file%%}"), aBodyName.length() > 0 ? aBodyName : "femalebody");
      lTextToParse.replace(QString("{%%feet_output_file%%}"), aFeetName.length() > 0 ? aFeetName : "femalefeet");
      lTextToParse.replace(QString("{%%hands_output_file%%}"), aHandsName.length() > 0 ? aHandsName : "femalehands");

      QTextStream lTextStream(&lOSPFile);
      lTextStream << lTextToParse;
      lTextStream.flush();

      lOSPFile.close();
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lOSPPathName + tr("\"."));
      return false;
    }
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to parse the OSP BodySlide file."));
    return false;
  }

  return true;
}

bool PresetCreator::generateSkeletonFile(const QString& aEntryDirectory, const QString& aSkeletonPath)
{
  if (aSkeletonPath.length() > 0)
  {
    auto lSkeletonPath{Utils::cleanPathString(aSkeletonPath)};
    auto lSkeletonDirectory{aEntryDirectory + QDir::separator() + lSkeletonPath};
    QDir().mkpath(lSkeletonDirectory);

    auto lSkeletonName{this->findChild<QLineEdit*>("skeleton_name")->text()};

    // Custom skeleton chooser
    auto lSkeletonChooser{this->findChild<QComboBox*>("skeleton_chooser")};
    auto lPath{QString("%1/assets/skeletons/%2").arg(QCoreApplication::applicationDirPath()).arg(lSkeletonChooser->currentText())};

    auto lSkeletonWriteLocation{QString("%1%2%3.nif").arg(lSkeletonDirectory).arg(QDir::separator()).arg(lSkeletonName)};

    if (!QFile::copy(lPath, lSkeletonWriteLocation))
    {
      Utils::displayWarningMessage(tr("The custom skeleton file was not found or could not be copied. The application will take with the default XPMSSE (v4.72) skeleton instead..."));

      // Fallback option if the custom skeleton could not be copied
      if (!QFile::copy(":/ressources/skeleton_female", lSkeletonWriteLocation))
      {
        Utils::displayWarningMessage(tr("The skeleton file could not be created even using the default skeleton. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions."));
        return false;
      }
    }
  }
  else
  {
    Utils::displayWarningMessage(tr("Error: no path given for the custom skeleton."));
    return false;
  }

  return true;
}

void PresetCreator::populateSkeletonChooser()
{
  auto lRootDir{QCoreApplication::applicationDirPath() + "/assets/skeletons/"};
  auto lRelativeDirs{QString("")};
  auto lAvailableSkeletons{QStringList()};

  // Search for all "*.nif" files
  QDirIterator it(lRootDir, QStringList() << QString("*.nif"), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
    lAvailableSkeletons.push_back(it.fileInfo().absoluteFilePath().remove(lRootDir, Qt::CaseInsensitive));
  }

  // Clear the combo box and add the found files to it
  auto lSkeletonChooser{this->findChild<QComboBox*>("skeleton_chooser")};
  lSkeletonChooser->clear();
  lSkeletonChooser->addItems(lAvailableSkeletons);
}

void PresetCreator::updateMeshesPreview()
{
  // Body meshes path and name
  auto lMeshesPathBody{this->findChild<QLineEdit*>("meshes_path_input_femalebody")->text().trimmed()};
  Utils::cleanPathString(lMeshesPathBody);
  auto lBodyName{this->findChild<QLineEdit*>("body_mesh_name_input")->text().trimmed()};

  // Feet meshes path and name
  auto lMeshesPathFeet{this->findChild<QLineEdit*>("meshes_path_input_femalefeet")->text().trimmed()};
  Utils::cleanPathString(lMeshesPathFeet);
  auto lFeetName{this->findChild<QLineEdit*>("feet_mesh_name_input")->text().trimmed()};

  // Hands meshes path and name
  auto lMeshesPathHands{this->findChild<QLineEdit*>("meshes_path_input_femalehands")->text().trimmed()};
  Utils::cleanPathString(lMeshesPathHands);
  auto lHandsName{this->findChild<QLineEdit*>("hands_mesh_name_input")->text().trimmed()};

  // Get preview label
  auto lPreviewLabel{this->findChild<QLabel*>("meshes_preview")};
  auto lFullPreview(QString(""));
  auto lIsValidPath{true};

  if (lMeshesPathBody == "")
  {
    lMeshesPathBody = "*";
    lIsValidPath = false;
  }

  if (lMeshesPathFeet == "")
  {
    lMeshesPathFeet = "*";
    lIsValidPath = false;
  }

  if (lMeshesPathHands == "")
  {
    lMeshesPathHands = "*";
    lIsValidPath = false;
  }

  if (lBodyName == "")
  {
    lBodyName = "femalebody";
  }

  if (lFeetName == "")
  {
    lFeetName = "femalefeet";
  }

  if (lHandsName == "")
  {
    lHandsName = "femalehands";
  }

  lFullPreview += QStringLiteral("[...]/Skyrim Special Edition/Data/%1/%2_[0/1].nif\n").arg(lMeshesPathBody).arg(lBodyName);

  auto lBodySelected{static_cast<BodyNameVersion>(this->findChild<QComboBox*>(QString("body_selector"))->currentIndex())};
  if (lBodySelected == BodyNameVersion::CBBE_SMP_3BBB_1_2_0)
  {
    lFullPreview.append("  -\n  -");
  }
  else
  {
    lFullPreview.append(QString("[...]/Skyrim Special Edition/Data/%1/%2_[0/1].nif\n").arg(lMeshesPathFeet).arg(lFeetName));
    lFullPreview.append(QString("[...]/Skyrim Special Edition/Data/%1/%2_[0/1].nif").arg(lMeshesPathHands).arg(lHandsName));
  }

  auto lNewTextColor{QString("hsl(141, 53%, 53%)")};

  if (lIsValidPath)
  {
    if ((!lMeshesPathBody.startsWith("meshes/", Qt::CaseInsensitive) || (lMeshesPathBody.startsWith("meshes/", Qt::CaseInsensitive) && lMeshesPathBody.length() < 8))
        || (!lMeshesPathFeet.startsWith("meshes/", Qt::CaseInsensitive) || (lMeshesPathFeet.startsWith("meshes/", Qt::CaseInsensitive) && lMeshesPathFeet.length() < 8))
        || (!lMeshesPathHands.startsWith("meshes/", Qt::CaseInsensitive) || (lMeshesPathHands.startsWith("meshes/", Qt::CaseInsensitive) && lMeshesPathHands.length() < 8)))
    {
      lNewTextColor = "hsl(33, 100%, 71%)";
    }
  }
  else
  {
    lNewTextColor = "hsl(4, 90%, 58%)";
  }

  lPreviewLabel->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lPreviewLabel->setText(lFullPreview);
}

void PresetCreator::updateOutputPreview()
{
  // Get main directory
  auto lMainDirTextEdit{this->findChild<QLineEdit*>("output_path_directory")};
  auto lMainDirectory{lMainDirTextEdit->text().trimmed()};
  Utils::cleanPathString(lMainDirectory);

  // Get subdirectory
  auto lSubDirectory{this->findChild<QLineEdit*>("output_path_subdirectory")->text().trimmed()};
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
  auto lOutputPathsPreview{this->findChild<QLabel*>("output_path_preview")};

  auto lNewTextColor{QString("hsl(141, 53%, 53%)")};

  if (lIsValidPath)
  {
    if (QDir(lFullPath).exists())
    {
      lNewTextColor = "hsl(33, 100%, 71%)";
    }
  }
  else
  {
    lNewTextColor = "hsl(4, 90%, 58%)";
  }

  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lFullPath);
}

void PresetCreator::updateOSPXMLPreview(QString aText)
{
  auto lOutputPathsPreview{this->findChild<QLabel*>("names_osp_xml_preview")};

  auto lIsValidPath{true};

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
    lIsValidPath = false;
  }

  auto lConstructedPreviewText(
    QStringLiteral(
      "[...]/Skyrim Special Edition/Data/CalienteTools/BodySlide/SliderGroups/%1.xml\n"
      "[...]/Skyrim Special Edition/Data/CalienteTools/BodySlide/SliderSets/%1.osp")
      .arg(aText));

  auto lNewTextColor{QString("hsl(141, 53%, 53%)")};

  if (!lIsValidPath)
  {
    lNewTextColor = "hsl(4, 90%, 58%)";
  }

  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lConstructedPreviewText);
}

void PresetCreator::updateBodyslideNamesPreview(QString aText)
{
  // Beast hands
  auto lMustUseBeastHands{this->findChild<QCheckBox*>("use_beast_hands")->isChecked()};

  // Path
  auto lIsValidPath{true};

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
    lIsValidPath = false;
  }

  auto lBodySelected{static_cast<BodyNameVersion>(this->findChild<QComboBox*>(QString("body_selector"))->currentIndex())};
  auto lConstructedPreviewText{QString("")};

  switch (lBodySelected)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_40:
      if (lMustUseBeastHands)
        lConstructedPreviewText = QStringLiteral("%1 - 3BBB Body Amazing\n%1 - Feet\n%1 - Beast Hands").arg(aText);
      else
        lConstructedPreviewText = QStringLiteral("%1 - 3BBB Body Amazing\n%1 - Feet\n%1 - Hands ").arg(aText);
      break;
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
      if (lMustUseBeastHands)
        lConstructedPreviewText = QStringLiteral("%1 - CBBE 3BBB Body Amazing\n%1 - CBBE 3BBB Feet\n%1 - CBBE Beast Hands").arg(aText);
      else
        lConstructedPreviewText = QStringLiteral("%1 - CBBE 3BBB Body Amazing\n%1 - CBBE 3BBB Feet\n%1 - CBBE 3BBB Hands").arg(aText);
      break;
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_and_1_52:
      if (lMustUseBeastHands)
        lConstructedPreviewText = QStringLiteral("%1 - CBBE 3BBB Body Amazing\n%1 - CBBE 3BBB Feet\n%1 - CBBE 3BBB Hands Beast").arg(aText);
      else
        lConstructedPreviewText = QStringLiteral("%1 - CBBE 3BBB Body Amazing\n%1 - CBBE 3BBB Feet\n%1 - CBBE 3BBB Hands").arg(aText);
      break;
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      lConstructedPreviewText = QStringLiteral("%1 - CBBE Body SMP (3BBB)\n  -\n  -").arg(aText);
      break;
    case BodyNameVersion::BHUNP_3BBB_2_13:
      lConstructedPreviewText = QStringLiteral("%1 - BHUNP 3BBB\n%1 - BHUNP 3BBB Advanced Feet\n%1 - BHUNP 3BBB Advanced Hands").arg(aText);
      break;
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_13:
      lConstructedPreviewText = QStringLiteral("%1 - BHUNP 3BBB Advanced\n%1 - BHUNP 3BBB Advanced Feet\n%1 - BHUNP 3BBB Advanced Hands").arg(aText);
      break;
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_13:
      lConstructedPreviewText = QStringLiteral("%1 - BHUNP 3BBB Advanced Ver 2\n%1 - BHUNP 3BBB Advanced Feet\n%1 - BHUNP 3BBB Advanced Hands").arg(aText);
      break;
    case BodyNameVersion::BHUNP_BBP_2_13:
      lConstructedPreviewText = QStringLiteral("%1 - BHUNP BBP\n%1 - BHUNP 3BBB Advanced Feet\n%1 - BHUNP 3BBB Advanced Hands").arg(aText);
      break;
    case BodyNameVersion::BHUNP_BBP_Advanced_2_13:
      lConstructedPreviewText = QStringLiteral("%1 - BHUNP BBP Advanced\n%1 - BHUNP 3BBB Advanced Feet\n%1 - BHUNP 3BBB Advanced Hands").arg(aText);
      break;
    case BodyNameVersion::BHUNP_TBBP_2_13:
      lConstructedPreviewText = QStringLiteral("%1 - BHUNP TBBP\n%1 - BHUNP 3BBB Advanced Feet\n%1 - BHUNP 3BBB Advanced Hands").arg(aText);
      break;
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_13:
      lConstructedPreviewText = QStringLiteral("%1 - BHUNP TBBP Advanced\n%1 - BHUNP 3BBB Advanced Feet\n%1 - BHUNP 3BBB Advanced Hands").arg(aText);
      break;
  }

  auto lNewTextColor{QString("hsl(141, 53%, 53%)")};

  if (!lIsValidPath)
  {
    lNewTextColor = "hsl(4, 90%, 58%)";
  }

  auto lOutputPathsPreview{this->findChild<QLabel*>("names_bodyslide_preview")};
  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lConstructedPreviewText);
}

void PresetCreator::updateSkeletonPathState(int aState)
{
  // Chooser
  auto lLabelChooser{this->findChild<QLabel*>("label_skeleton_chooser")};
  auto lChooser{this->findChild<QComboBox*>("skeleton_chooser")};
  auto lChooserRefresher{this->findChild<QPushButton*>("skeleton_chooser_refresher")};

  // Directory
  auto lLabelPathDir{this->findChild<QLabel*>("label_skeleton_path_directory")};
  auto lPathDir{this->findChild<QLineEdit*>("skeleton_path_directory")};

  // Name
  auto lLabelSkeleton{this->findChild<QLabel*>("label_skeleton_female")};
  auto lSkeleton{this->findChild<QLineEdit*>("skeleton_name")};
  auto lSkeletonExtension{this->findChild<QLabel*>("skeleton_name_extension")};

  // Preview
  auto lLabelPreview{this->findChild<QLabel*>("label_skeleton_path_preview")};
  auto lPreview{this->findChild<QLabel*>("skeleton_path_preview")};

  switch (aState)
  {
    case Qt::Unchecked:
      // Remove any dynamically applied color
      lPreview->setStyleSheet("");

      // Disable every graphical components
      lLabelChooser->setDisabled(true);
      lChooser->setDisabled(true);
      lChooserRefresher->setDisabled(true);
      lLabelPathDir->setDisabled(true);
      lPathDir->setDisabled(true);
      lLabelSkeleton->setDisabled(true);
      lSkeleton->setDisabled(true);
      lSkeletonExtension->setDisabled(true);
      lLabelPreview->setDisabled(true);
      lPreview->setDisabled(true);
      break;
    case Qt::Checked:
      // Enable back every graphical components
      lLabelChooser->setDisabled(false);
      lChooser->setDisabled(false);
      lChooserRefresher->setDisabled(false);
      lLabelPathDir->setDisabled(false);
      lPathDir->setDisabled(false);
      lLabelSkeleton->setDisabled(false);
      lSkeleton->setDisabled(false);
      lSkeletonExtension->setDisabled(false);
      lLabelPreview->setDisabled(false);
      lPreview->setDisabled(false);

      // Re-apply colors dynamically
      this->updateSkeletonPreview();
  }
}

void PresetCreator::updateSkeletonPreview()
{
  auto lSkeletonPath{this->findChild<QLineEdit*>("skeleton_path_directory")->text()};
  Utils::cleanPathString(lSkeletonPath);
  auto lSkeletonName{this->findChild<QLineEdit*>("skeleton_name")->text()};
  auto lIsValidPath{true};

  if (lSkeletonPath.trimmed().length() == 0)
  {
    lSkeletonPath = QString::fromStdString("*");
    lIsValidPath = false;
  }

  if (lSkeletonName.trimmed().length() == 0)
  {
    lSkeletonName = "skeleton_female";
  }

  auto lConstructedPath(QStringLiteral("[...]/Skyrim Special Edition/Data/%1/%2.nif").arg(lSkeletonPath).arg(lSkeletonName));
  auto lOutputPathPreview{this->findChild<QLabel*>("skeleton_path_preview")};

  auto lNewTextColor{QString("")};

  if (lIsValidPath)
  {
    auto lStart{lSkeletonPath.startsWith("meshes/", Qt::CaseInsensitive)};

    if (!lStart || (lStart && lSkeletonPath.length() < 8))
    {
      lNewTextColor = "hsl(33, 100%, 71%)";
    }
    else
    {
      lNewTextColor = "hsl(141, 53%, 53%)";
    }
  }
  else if (this->findChild<QCheckBox*>("use_custom_skeleton")->isChecked())
  {
    lNewTextColor = "hsl(4, 90%, 58%)";
  }

  lOutputPathPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathPreview->setText(lConstructedPath);
}

void PresetCreator::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("output_path_directory")};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lLineEdit->text().size() > 0 ? lLineEdit->text() : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))};
  lLineEdit->setText(lPath);
  this->updateOutputPreview();
}

void PresetCreator::generateDirectoryStructure()
{
  // Selected body
  auto lBodySelected{this->findChild<QComboBox*>(QString("body_selector"))->currentIndex()};

  // Beast hands
  auto lCheckboxUseBeastHands{this->findChild<QCheckBox*>("use_beast_hands")};
  auto lMustUseBeastHands{lCheckboxUseBeastHands->isEnabled() && lCheckboxUseBeastHands->isChecked()};

  // Body meshes path and name
  auto lMeshesPathBody{this->findChild<QLineEdit*>("meshes_path_input_femalebody")->text().trimmed()};
  Utils::cleanPathString(lMeshesPathBody);
  auto lBodyName{this->findChild<QLineEdit*>("body_mesh_name_input")->text().trimmed()};

  // Feet meshes path and name
  auto lInputMeshesPathFeet{this->findChild<QLineEdit*>("meshes_path_input_femalefeet")};
  auto lSkipFeetCheck{!lInputMeshesPathFeet->isEnabled()};
  auto lMeshesPathFeet{lInputMeshesPathFeet->text().trimmed()};
  Utils::cleanPathString(lMeshesPathFeet);
  auto lFeetName{this->findChild<QLineEdit*>("feet_mesh_name_input")->text().trimmed()};

  // Hands meshes path and name
  auto lInputMeshesPathHands{this->findChild<QLineEdit*>("meshes_path_input_femalehands")};
  auto lSkipHandsCheck{!lInputMeshesPathHands->isEnabled()};
  auto lMeshesPathHands{lInputMeshesPathHands->text().trimmed()};
  Utils::cleanPathString(lMeshesPathHands);
  auto lHandsName{this->findChild<QLineEdit*>("hands_mesh_name_input")->text().trimmed()};

  // BodySlide names
  auto lOSPXMLNames{this->findChild<QLineEdit*>("names_osp_xml_input")->text().trimmed()};
  auto lBodyslideSlidersetsNames{this->findChild<QLineEdit*>("names_bodyslide_input")->text().trimmed()};

  // Output paths
  auto lMainDirectory{this->findChild<QLineEdit*>("output_path_directory")->text().trimmed()};
  auto lSubDirectory{this->findChild<QLineEdit*>("output_path_subdirectory")->text().trimmed()};
  Utils::cleanPathString(lSubDirectory);

  // Full extract path
  const auto& lEntryDirectory{(lSubDirectory.length() == 0 ? lMainDirectory : (lMainDirectory + QDir::separator() + lSubDirectory))};

  // Check if the full extract path has been given by the user
  if (lEntryDirectory.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no path given to export the files."));
    return;
  }

  // Check if the path could be valid
  if (lEntryDirectory.startsWith(QDir::separator()))
  {
    Utils::displayWarningMessage(tr("Error: the path given to export the files seems to be invalid."));
    return;
  }

  // Create main directory
  auto lGenerateFilesInExistingMainDirectory{false};
  if (!QDir(lEntryDirectory).exists())
  {
    // Wait to know the result of the mkdir()
    if (!QDir().mkdir(lEntryDirectory))
    {
      Utils::displayWarningMessage(tr("Error while creating the main directory: \"") + lEntryDirectory + tr("\" could not be created on your computer. Be sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions."));
      return;
    }
  }
  else
  {
    // Since the directory already exist, ask the user to generate another preset in it
    QMessageBox lConfirmationBox(QMessageBox::Icon::Question,
                                 tr("Already existing directory"),
                                 tr("The main directory \"%1\" already exists on your computer. Do you still want to continue the files generation in this directory?").arg(lEntryDirectory),
                                 QMessageBox::StandardButton::NoButton,
                                 this);

    auto lContinueButton{lConfirmationBox.addButton(tr("Continue the files generation"), QMessageBox::ButtonRole::YesRole)};
    lContinueButton->setCursor(Qt::PointingHandCursor);
    lContinueButton->setStyleSheet("color: hsl(33, 100%, 71%);");

    auto lStopButton{lConfirmationBox.addButton(tr("Cancel the files generation"), QMessageBox::ButtonRole::NoRole)};
    lStopButton->setCursor(Qt::PointingHandCursor);
    lStopButton->setStyleSheet("color: hsl(4, 90%, 58%);");

    lConfirmationBox.setDefaultButton(lContinueButton);
    lConfirmationBox.exec();

    if (lConfirmationBox.clickedButton() != lContinueButton)
    {
      return;
    }

    lGenerateFilesInExistingMainDirectory = true;
  }

  // Export the meshes
  if (lMeshesPathBody.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no path has been given for the body meshes."));
    return;
  }

  if (lMeshesPathFeet.length() == 0 && !lSkipFeetCheck)
  {
    Utils::displayWarningMessage(tr("Error: no path has been given for the feet meshes."));
    return;
  }

  if (lMeshesPathHands.length() == 0 && !lSkipHandsCheck)
  {
    Utils::displayWarningMessage(tr("Error: no path has been given for the hands meshes."));
    return;
  }

  // Check if a name has been given for the OSP and XML files
  if (lOSPXMLNames.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no name given for the BodySlide files."));
    return;
  }

  // Check if a name has been given for the presets
  if (lBodyslideSlidersetsNames.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no name given for the slider sets (names that appear in the BodySlide application)."));
    return;
  }

  // Get the virtual ressources' directory
  auto lRessourcesFolder{Utils::getBodyRessourceFolder(static_cast<BodyNameVersion>(lBodySelected))};
  if (lRessourcesFolder.length() == 0)
  {
    return;
  }

  // XML file
  if (!this->generateXMLFile(lEntryDirectory, lGenerateFilesInExistingMainDirectory, lOSPXMLNames, lMustUseBeastHands, lRessourcesFolder, lBodySelected, lBodyslideSlidersetsNames))
  {
    return;
  }

  // OSP file
  if (!this->generateOSPFile(lEntryDirectory, lGenerateFilesInExistingMainDirectory, lOSPXMLNames, lMustUseBeastHands, lRessourcesFolder, lBodyslideSlidersetsNames, lMeshesPathBody, lMeshesPathFeet, lMeshesPathHands, lBodyName, lFeetName, lHandsName))
  {
    return;
  }

  // Skeleton
  auto lMustCopySkeleton{this->findChild<QCheckBox*>("use_custom_skeleton")->isChecked()};

  if (lMustCopySkeleton)
  {
    auto lSkeletonPath{this->findChild<QLineEdit*>("skeleton_path_directory")->text().trimmed()};
    Utils::cleanPathString(lSkeletonPath);

    if (!this->generateSkeletonFile(lEntryDirectory, lSkeletonPath))
    {
      return;
    }
  }

  // Update the color of the output directory preview
  this->updateOutputPreview();

  QMessageBox lConfirmationBox(QMessageBox::Icon::Information, tr("Generation successful"), tr("Every file has been correctly generated. You can now exit the application or create another conversion!"), QMessageBox::StandardButton::NoButton, this);

  auto lOKButton{lConfirmationBox.addButton(tr("OK"), QMessageBox::ButtonRole::AcceptRole)};
  lOKButton->setCursor(Qt::PointingHandCursor);
  lConfirmationBox.setDefaultButton(lOKButton);
  lConfirmationBox.exec();

  if (mSettings.mainWindowAutomaticallyOpenGeneratedDirectory)
  {
    // Open the directory where the file structure has been created
    QDesktopServices::openUrl(QUrl::fromLocalFile(lEntryDirectory));
  }
}

void PresetCreator::refreshAllPreviewFields(int)
{
  this->refreshAllPreviewFields();
}

void PresetCreator::refreshAllPreviewFields()
{
  // Update the GUI based on the available
  this->updateGUIOnBodyChange();

  // Refresh the preview of the body meshes parts
  this->updateMeshesPreview();

  // Refresh the names in the bodyslide application
  auto lBodyslideSlidersetsNames{this->findChild<QLineEdit*>("names_bodyslide_input")->text().trimmed()};
  this->updateBodyslideNamesPreview(lBodyslideSlidersetsNames);
}

void PresetCreator::openBodySlideFiltersEditor()
{
  auto lCurrentKey{this->findChild<QComboBox*>("bodyslide_filters_chooser")->currentText()};
  auto lEditor{new BodySlideFiltersEditor(this, this->mSettings, this->mFiltersList, lCurrentKey)};
  this->connect(lEditor, &BodySlideFiltersEditor::listEdited, this, &PresetCreator::updateBodySlideFiltersList);
}

void PresetCreator::initBodySlideFiltersList()
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

void PresetCreator::updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList)
{
  // Update the filters list
  this->mFiltersList = aFilterList;

  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};

  // Disable the combobox if there is not any available filter
  if (this->mFiltersList.size() == 0)
  {
    lChooser->setDisabled(true);
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
    auto lLoop{0};
    auto lPrevIndex{-1};
    for (const auto& lPair : this->mFiltersList)
    {
      if (lPair.first.compare(lPrevKey, Qt::CaseSensitive) == 0)
      {
        lPrevIndex = lLoop;
        break;
      }

      lLoop++;
    }

    lChooser->setCurrentIndex(lPrevIndex);
  }
}

void PresetCreator::updateBodySlideFiltersListPreview(int aIndex)
{
  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};
  auto LFiltersLabel{this->findChild<QLabel*>("bodyslide_filters")};

  auto lText{QString("")};
  if (aIndex != -1)
  {
    lText = this->mFiltersList.find(lChooser->itemText(aIndex))->second.join(QString(" ; "));
  }

  LFiltersLabel->setText(lText);
}

void PresetCreator::scrollbarPressed()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::ClosedHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::ClosedHandCursor);
}

void PresetCreator::scrollbarReleased()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
}
