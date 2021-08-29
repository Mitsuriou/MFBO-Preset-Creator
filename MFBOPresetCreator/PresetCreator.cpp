#include "PresetCreator.h"
#include "BodySlideFiltersEditor.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Settings.h"
#include "SliderFileBuilder.h"
#include "Utils.h"
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QGroupBox>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushbutton>
#include <QScrollArea>
#include <QStandardPaths>
#include <QStyledItemDelegate>

PresetCreator::PresetCreator(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QWidget(aParent)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mMinimumFirstColumnWidth(275)
{
  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(this)};
  auto lButtonLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};

  // Setup all the different GUI components
  this->setupBodyMeshesGUI(*lMainLayout);
  this->setupSkeletonGUI(*lMainLayout);
  this->setupBodySlideGUI(*lMainLayout);
  this->setupOutputGUI(*lMainLayout);
  this->setupButtons(*lButtonLayout);

  // Update the GUI based on the values entered
  this->refreshAllPreviewFields();

  this->setHasUserDoneSomething(false);
}

void PresetCreator::loadProject(const QString& lFilePath, const bool aSkipFileChooser)
{
  QString lFileToLoad{lFilePath};
  if (!aSkipFileChooser)
  {
    // Open a file chooser dialog
    const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "lastLoadedProject", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), this->mSettings.eachButtonSavesItsLastUsedPath)};
    lFileToLoad = QFileDialog::getOpenFileName(this, "", lContextPath, "Preset Creator Project (*.pcp *.json)");
  }

  if (lFileToLoad.compare("") == 0)
  {
    return;
  }

  Utils::UpdatePathAtKey(this->mLastPaths, "lastLoadedProject", lFileToLoad);

  if (this->hasUserDoneSomething())
  {
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Unsaved data"),
                                      tr("It seems that you have some unsaved data. Do you still want to open the project file \"%1\"?").arg(lFileToLoad),
                                      Utils::GetIconRessourceFolder(this->mSettings.appTheme),
                                      "help-circle",
                                      tr("Open the project without saving"),
                                      tr("Cancel the file opening"),
                                      "",
                                      this->mSettings.dangerColor,
                                      this->mSettings.successColor,
                                      "",
                                      false)
        != ButtonClicked::YES)
    {
      return;
    }
  }

  // Load the project only if a path is defined
  this->loadValuesFromJsonObject(Utils::LoadFromJsonFile(lFileToLoad));
  this->mLastUsedSavePath = lFileToLoad;
  this->parentWidget()->findChild<QAction*>(QString("action_save_project"))->setDisabled(false);

  this->setHasUserDoneSomething(false);
}

void PresetCreator::saveProject(const bool aIsSaveAsContext)
{
  auto lFilePath{QString(this->mLastUsedSavePath)};
  // Check the event source
  if (aIsSaveAsContext || (!aIsSaveAsContext && this->mLastUsedSavePath.isEmpty()))
  {
    // Open a file saver dialog
    const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "lastSavedProject", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), this->mSettings.eachButtonSavesItsLastUsedPath)};
    lFilePath = QFileDialog::getSaveFileName(this, "", lContextPath, "Preset Creator Project (*.pcp *.json)");
    Utils::UpdatePathAtKey(this->mLastPaths, "lastSavedProject", lFilePath);

    if (!lFilePath.isEmpty())
    {
      this->mLastUsedSavePath = lFilePath;
      this->parentWidget()->findChild<QAction*>(QString("action_save_project"))->setDisabled(false);
    }
  }

  if (this->mLastUsedSavePath.isEmpty())
  {
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  Utils::SaveAsJsonFile(this->saveValuesToJsonObject(), lFilePath, this, lIconFolder);
  this->setHasUserDoneSomething(false);
}

void PresetCreator::setHasUserDoneSomething(const bool aHasUserDoneSomething)
{
  this->mHasUserDoneSomething = aHasUserDoneSomething;

  // Update the state of the "save project" action menu
  if (!this->mLastUsedSavePath.isEmpty())
  {
    this->parentWidget()->findChild<QAction*>(QString("action_save_project"))->setDisabled(!this->mHasUserDoneSomething);
  }
}

bool PresetCreator::hasUserDoneSomething()
{
  return this->mHasUserDoneSomething;
}

void PresetCreator::updateSettings(Struct::Settings aSettings)
{
  this->mSettings = aSettings;
}

void PresetCreator::updateBodySlideSets()
{
  this->updateBodySlideFiltersList(Utils::LoadFiltersFromFile());
}

void PresetCreator::fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList)
{
  auto lBodiesHaveBeenSet{false};
  auto lFeetHaveBeenSet{false};
  auto lHandsHaveBeenSet{false};
  auto lSkeletonHasBeenSet{false};

  // Set the preset name
  this->findChild<QLineEdit*>(QString("names_osp_xml_input"))->setText(aPresetName);
  this->findChild<QLineEdit*>(QString("names_bodyslide_input"))->setText(aPresetName);

  for (const auto& lResult : aResultsList)
  {
    auto lRole{static_cast<AssistedConversionRole>(lResult.getRole())};
    switch (lRole)
    {
      case AssistedConversionRole::BODY:
        this->findChild<QLineEdit*>(QString("meshes_path_input_femalebody"))->setText(lResult.getPath());
        this->findChild<QLineEdit*>(QString("body_mesh_name_input"))->setText(lResult.getName());

        lBodiesHaveBeenSet = true;
        break;
      case AssistedConversionRole::FEET:
        this->findChild<QLineEdit*>(QString("meshes_path_input_femalefeet"))->setText(lResult.getPath());
        this->findChild<QLineEdit*>(QString("feet_mesh_name_input"))->setText(lResult.getName());

        lFeetHaveBeenSet = true;
        break;
      case AssistedConversionRole::HANDS:
        this->findChild<QLineEdit*>(QString("meshes_path_input_femalehands"))->setText(lResult.getPath());
        this->findChild<QLineEdit*>(QString("hands_mesh_name_input"))->setText(lResult.getName());

        lHandsHaveBeenSet = true;
        break;
      case AssistedConversionRole::SKELETON:
        auto lNeedCustomSkeleton{this->findChild<QCheckBox*>(QString("use_custom_skeleton"))};
        if (!lNeedCustomSkeleton->isChecked())
        {
          lNeedCustomSkeleton->setChecked(true);
        }

        this->findChild<QLineEdit*>(QString("skeleton_path_directory"))->setText(lResult.getPath());
        this->findChild<QLineEdit*>(QString("skeleton_name"))->setText(lResult.getName());

        lSkeletonHasBeenSet = true;
        break;
    }
  }

  // Check if at least one mesh has not been set previously
  if (!lBodiesHaveBeenSet || !lFeetHaveBeenSet || !lHandsHaveBeenSet)
  {
    // Compose the detailed string of errors
    auto lUnassignedEntries{QString()};
    auto lItemsNumber{0};

    if (!lBodiesHaveBeenSet)
    {
      lUnassignedEntries += tr("the body");
      lItemsNumber++;
    }

    if (!lFeetHaveBeenSet)
    {
      if (lItemsNumber == 1 && lHandsHaveBeenSet)
      {
        lUnassignedEntries += tr(" and the feet");
      }
      else if (lItemsNumber == 1)
      {
        lUnassignedEntries += tr(", the feet");
      }
      else
      {
        lUnassignedEntries += tr("the feet");
      }

      lItemsNumber++;
    }

    if (!lHandsHaveBeenSet)
    {
      if (lItemsNumber > 0)
      {
        lUnassignedEntries += tr(" and the hands");
      }
      else
      {
        lUnassignedEntries += tr("the hands");
      }

      lItemsNumber++;
    }

    auto lMessage{QString()};
    if (lItemsNumber > 1)
    {
      // Plural
      lMessage = tr("You did not select any file for %1 meshes. Would you like to clear all these unassigned entries or keep their currently set values?").arg(lUnassignedEntries);
    }
    else
    {
      // Singular
      lMessage = tr("You did not select any file for %1 mesh. Would you like to clear this unassigned entry or keep its currently set value?").arg(lUnassignedEntries);
    }

    if (Utils::DisplayQuestionMessage(this,
                                      tr("One mesh data was not assigned"),
                                      lMessage,
                                      Utils::GetIconRessourceFolder(this->mSettings.appTheme),
                                      "help-circle",
                                      tr("Clear all the unassigned entries"),
                                      tr("Keep the currently set values for the unassigned entries"),
                                      "",
                                      this->mSettings.warningColor,
                                      this->mSettings.warningColor,
                                      "",
                                      false)
        == ButtonClicked::YES)
    {
      // Clear the body path
      if (!lBodiesHaveBeenSet)
      {
        this->findChild<QLineEdit*>(QString("meshes_path_input_femalebody"))->setText("");
        this->findChild<QLineEdit*>(QString("body_mesh_name_input"))->setText("");
      }

      // Clear the feet path
      if (!lFeetHaveBeenSet)
      {
        this->findChild<QLineEdit*>(QString("meshes_path_input_femalefeet"))->setText("");
        this->findChild<QLineEdit*>(QString("feet_mesh_name_input"))->setText("");
      }

      // Clear the hands path
      if (!lHandsHaveBeenSet)
      {
        this->findChild<QLineEdit*>(QString("meshes_path_input_femalehands"))->setText("");
        this->findChild<QLineEdit*>(QString("hands_mesh_name_input"))->setText("");
      }
    }
  }

  if (!lSkeletonHasBeenSet)
  {
    this->findChild<QCheckBox*>(QString("use_custom_skeleton"))->setChecked(true);
    this->findChild<QLineEdit*>(QString("skeleton_path_directory"))->setText("");
    this->findChild<QLineEdit*>(QString("skeleton_name"))->setText("skeleton_female");
    this->findChild<QCheckBox*>(QString("use_custom_skeleton"))->setChecked(false);
  }
}

void PresetCreator::setupBodyMeshesGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Body meshes group box
  auto lMeshesGroupBox{new QGroupBox(tr("Original mod's body meshes").append("  "), this)};
  Utils::AddIconToGroupBox(lMeshesGroupBox, lIconFolder, "body", this->mSettings.font.size);
  this->connect(lMeshesGroupBox, &QGroupBox::toggled, this, &PresetCreator::groupBoxChecked);
  Utils::SetGroupBoxState(lMeshesGroupBox, false);
  aLayout.addWidget(lMeshesGroupBox, 0, 0);

  // Grid layout
  auto lMeshesGridLayout{new QGridLayout(lMeshesGroupBox)};
  lMeshesGridLayout->setSpacing(10);
  lMeshesGridLayout->setContentsMargins(15, 20, 15, 15);
  lMeshesGridLayout->setAlignment(Qt::AlignTop);
  lMeshesGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Body meshes names
  lMeshesGridLayout->addWidget(new QLabel(tr("Meshes paths and names:"), this), 1, 0, 3, 1);

  // femalebody
  auto lMeshesPathFemaleBodyLineEdit{new QLineEdit(this)};
  lMeshesPathFemaleBodyLineEdit->setObjectName(QString("meshes_path_input_femalebody"));
  lMeshesPathFemaleBodyLineEdit->setPlaceholderText("meshes/...");
  lMeshesGridLayout->addWidget(lMeshesPathFemaleBodyLineEdit, 1, 1);

  lMeshesGridLayout->addWidget(new QLabel("/", this), 1, 2);

  auto lBodyMeshNameInput{new QLineEdit(this)};
  lBodyMeshNameInput->setObjectName(QString("body_mesh_name_input"));
  lMeshesGridLayout->addWidget(lBodyMeshNameInput, 1, 3);
  lBodyMeshNameInput->setText("femalebody");
  lBodyMeshNameInput->setPlaceholderText("femalebody");

  lMeshesGridLayout->addWidget(new QLabel(tr("_0.nif/_1.nif"), this), 1, 4);

  // femalefeet
  auto lMeshesPathFemaleFeetLineEdit{new QLineEdit(this)};
  lMeshesPathFemaleFeetLineEdit->setObjectName(QString("meshes_path_input_femalefeet"));
  lMeshesPathFemaleFeetLineEdit->setPlaceholderText("meshes/...");
  lMeshesGridLayout->addWidget(lMeshesPathFemaleFeetLineEdit, 2, 1);

  lMeshesGridLayout->addWidget(new QLabel("/", this), 2, 2);

  auto lFeetMeshNameInput{new QLineEdit(this)};
  lFeetMeshNameInput->setObjectName(QString("feet_mesh_name_input"));
  lMeshesGridLayout->addWidget(lFeetMeshNameInput, 2, 3);
  lFeetMeshNameInput->setText("femalefeet");
  lFeetMeshNameInput->setPlaceholderText("femalefeet");

  lMeshesGridLayout->addWidget(new QLabel(tr("_0.nif/_1.nif"), this), 2, 4);

  // femalehands
  auto lMeshesPathFemaleHandsLineEdit{new QLineEdit(this)};
  lMeshesPathFemaleHandsLineEdit->setObjectName(QString("meshes_path_input_femalehands"));
  lMeshesPathFemaleHandsLineEdit->setPlaceholderText("meshes/...");
  lMeshesGridLayout->addWidget(lMeshesPathFemaleHandsLineEdit, 3, 1);

  lMeshesGridLayout->addWidget(new QLabel("/", this), 3, 2);

  auto lHandsMeshNameInput{new QLineEdit(this)};
  lHandsMeshNameInput->setObjectName(QString("hands_mesh_name_input"));
  lMeshesGridLayout->addWidget(lHandsMeshNameInput, 3, 3);
  lHandsMeshNameInput->setText("femalehands");
  lHandsMeshNameInput->setPlaceholderText("femalehands");

  lMeshesGridLayout->addWidget(new QLabel(tr("_0.nif/_1.nif"), this), 3, 4);

  // Preview
  lMeshesGridLayout->addWidget(new QLabel(tr("Preview:"), this), 4, 0);

  auto lMeshesPreview{new QLabel(this)};
  lMeshesPreview->setObjectName(QString("meshes_preview"));
  lMeshesPreview->setAutoFillBackground(true);
  lMeshesGridLayout->addWidget(lMeshesPreview, 4, 1, 1, 4);

  // Beast hands
  auto lLabelBeastHands{new QLabel(tr("Use beast hands?"), this)};
  lLabelBeastHands->setObjectName(QString("label_use_beast_hands"));
  lMeshesGridLayout->addWidget(lLabelBeastHands, 5, 0);

  auto lNeedBeastHands{new QCheckBox(tr("Check this box if the follower or NPC uses beast hands."), this)};
  lNeedBeastHands->setCursor(Qt::PointingHandCursor);
  lNeedBeastHands->setObjectName(QString("use_beast_hands"));
  lMeshesGridLayout->addWidget(lNeedBeastHands, 5, 1, 1, 4);

  // Event binding
  this->connect(lMeshesPathFemaleBodyLineEdit, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lMeshesPathFemaleFeetLineEdit, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lMeshesPathFemaleHandsLineEdit, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lBodyMeshNameInput, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lFeetMeshNameInput, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lHandsMeshNameInput, &QLineEdit::textChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lNeedBeastHands, &QCheckBox::stateChanged, this, qOverload<>(&PresetCreator::refreshAllPreviewFields));
}

void PresetCreator::setupSkeletonGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Custom skeleton group box
  auto lSkeletonGroupBox{new QGroupBox(tr("Skeleton").append("  "), this)};
  Utils::AddIconToGroupBox(lSkeletonGroupBox, lIconFolder, "skeleton", this->mSettings.font.size);
  this->connect(lSkeletonGroupBox, &QGroupBox::toggled, this, &PresetCreator::groupBoxChecked);
  Utils::SetGroupBoxState(lSkeletonGroupBox, false);
  aLayout.addWidget(lSkeletonGroupBox, 1, 0);

  auto lSkeletonGridLayout{new QGridLayout(lSkeletonGroupBox)};
  lSkeletonGridLayout->setSpacing(10);
  lSkeletonGridLayout->setContentsMargins(15, 20, 15, 15);
  lSkeletonGridLayout->setAlignment(Qt::AlignTop);
  lSkeletonGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Skeleton
  auto lLabelSkeleton{new QLabel(this)};
  auto lText{tr("Use a custom skeleton?")};
  auto lRichText{QString("<p style=\"text-align: left; padding: 0px; margin: 0px;\">"
                         "<img src=\":/%1/info-circle-smaller\" alt=\"~info icon~\" style=\"vertical-align: baseline;\">"
                         " %2"
                         "</p>")
                   .arg(lIconFolder)
                   .arg(lText)};
  lLabelSkeleton->setText(lRichText);
  lLabelSkeleton->setTextFormat(Qt::RichText);
  lLabelSkeleton->setToolTip(QString(tr("Not overriding a custom skeleton would cause breasts collision and physics to be inaccurate.")));
  lSkeletonGridLayout->addWidget(lLabelSkeleton, 0, 0);

  auto lNeedCustomSkeleton{new QCheckBox(tr("Check this box if the follower or NPC uses a custom skeleton."), this)};
  lNeedCustomSkeleton->setCursor(Qt::PointingHandCursor);
  lNeedCustomSkeleton->setObjectName(QString("use_custom_skeleton"));
  lSkeletonGridLayout->addWidget(lNeedCustomSkeleton, 0, 1);

  // Choose the skeleton file
  auto lLabelSkeletonChooser{new QLabel(tr("Skeleton file:"), this)};
  lLabelSkeletonChooser->setObjectName(QString("label_skeleton_chooser"));
  lSkeletonGridLayout->addWidget(lLabelSkeletonChooser, 1, 0);

  auto lSkeletonChooser{new QComboBox(this)};
  lSkeletonChooser->setItemDelegate(new QStyledItemDelegate());
  lSkeletonChooser->setCursor(Qt::PointingHandCursor);
  lSkeletonChooser->setObjectName(QString("skeleton_chooser"));
  lSkeletonGridLayout->addWidget(lSkeletonChooser, 1, 1);

  this->populateSkeletonChooser();

  auto lSkeletonRefresher{ComponentFactory::CreateButton(this, tr("Refresh"), "", "", lIconFolder, "skeleton_chooser_refresher")};
  lSkeletonGridLayout->addWidget(lSkeletonRefresher, 1, 2);

  // Skeleton path
  auto lLabelSkeletonPath{new QLabel(tr("Skeleton path:"), this)};
  lLabelSkeletonPath->setObjectName(QString("label_skeleton_path_directory"));
  lSkeletonGridLayout->addWidget(lLabelSkeletonPath, 2, 0);

  auto lSkeletonPathLineEdit{new QLineEdit(this)};
  lSkeletonPathLineEdit->setObjectName(QString("skeleton_path_directory"));
  lSkeletonPathLineEdit->setPlaceholderText("meshes/...");
  lSkeletonGridLayout->addWidget(lSkeletonPathLineEdit, 2, 1);

  // Skeleton name
  auto lSkeletonNameLabel{new QLabel(tr("Skeleton file name:"), this)};
  lSkeletonNameLabel->setObjectName(QString("label_skeleton_female"));
  lSkeletonGridLayout->addWidget(lSkeletonNameLabel, 3, 0);

  auto lSkeletonName{new QLineEdit(this)};
  lSkeletonName->setObjectName(QString("skeleton_name"));
  lSkeletonName->setPlaceholderText("skeleton_female");
  lSkeletonName->setText("skeleton_female");
  lSkeletonGridLayout->addWidget(lSkeletonName, 3, 1);

  auto lSkeletonNameExtension{new QLabel(tr(".nif"), this)};
  lSkeletonNameExtension->setObjectName(QString("skeleton_name_extension"));
  lSkeletonGridLayout->addWidget(lSkeletonNameExtension, 3, 2);

  // Skeleton path preview
  auto lSkeletontitlePreview{new QLabel(tr("Preview:"), this)};
  lSkeletontitlePreview->setObjectName(QString("label_skeleton_path_preview"));
  lSkeletonGridLayout->addWidget(lSkeletontitlePreview, 4, 0);

  auto lSkeletonPathsPreview{new QLabel(this)};
  lSkeletonPathsPreview->setObjectName(QString("skeleton_path_preview"));
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

void PresetCreator::setupBodySlideGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // BodySlide output settings group box
  auto lBodyslideGroupBox{new QGroupBox(tr("BodySlide output").append("  "), this)};
  Utils::AddIconToGroupBox(lBodyslideGroupBox, lIconFolder, "bodyslide-logo", this->mSettings.font.size);
  this->connect(lBodyslideGroupBox, &QGroupBox::toggled, this, &PresetCreator::groupBoxChecked);
  Utils::SetGroupBoxState(lBodyslideGroupBox, false);
  aLayout.addWidget(lBodyslideGroupBox, 2, 0);

  // Grid layout
  auto lBodyslideGridLayout{new QGridLayout(lBodyslideGroupBox)};
  lBodyslideGridLayout->setSpacing(10);
  lBodyslideGridLayout->setContentsMargins(15, 20, 15, 15);
  lBodyslideGridLayout->setAlignment(Qt::AlignTop);
  lBodyslideGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Targeted body and version
  auto lDefaultBodyVersionSettings{DataLists::GetSplittedNameVersionFromBodyVersion(mSettings.defaultMainWindowBody)};

  lBodyslideGridLayout->addWidget(new QLabel(tr("Targeted body and version:"), this), 0, 0);

  auto lBodyNameVersionWrapper{new QHBoxLayout(lBodyslideGroupBox)};
  lBodyNameVersionWrapper->setMargin(0);
  lBodyslideGridLayout->addLayout(lBodyNameVersionWrapper, 0, 1);

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

  // Second line
  lBodyslideGridLayout->addWidget(new QLabel(tr("BodySlide files names:"), this), 1, 0);

  auto lOSPXMLNamesLineEdit{new QLineEdit(this)};
  lOSPXMLNamesLineEdit->setObjectName(QString("names_osp_xml_input"));
  lBodyslideGridLayout->addWidget(lOSPXMLNamesLineEdit, 1, 1);

  // Third line
  lBodyslideGridLayout->addWidget(new QLabel(tr("Preview:"), this), 2, 0);

  auto lPathsNamesOspXmlNames{new QLabel("", this)};
  lPathsNamesOspXmlNames->setObjectName(QString("names_osp_xml_preview"));
  lPathsNamesOspXmlNames->setAutoFillBackground(true);
  lBodyslideGridLayout->addWidget(lPathsNamesOspXmlNames, 2, 1);

  // Fourth line
  auto lNamesInApp{new QLabel(this)};
  lNamesInApp->setTextFormat(Qt::RichText);
  lNamesInApp->setText(QString("<p style=\"text-align: left; padding: 0px; margin: 0px;\">"
                               "<img src=\":/%1/info-circle-smaller\" alt=\"~info icon~\" style=\"vertical-align: baseline;\"> %2</p>")
                         .arg(lIconFolder)
                         .arg(tr("Presets names:")));
  lNamesInApp->setToolTip(QString(tr("This field represents the names under which the presets will be listed in the BodySlide application.")));
  lBodyslideGridLayout->addWidget(lNamesInApp, 3, 0);

  auto lNamesInAppLineEdit{new QLineEdit(this)};
  lNamesInAppLineEdit->setObjectName(QString("names_bodyslide_input"));
  lBodyslideGridLayout->addWidget(lNamesInAppLineEdit, 3, 1);

  // Fifth line
  lBodyslideGridLayout->addWidget(new QLabel(tr("Preview:"), this), 4, 0);

  auto lResultNamesInApp{new QLabel("", this)};
  lResultNamesInApp->setObjectName(QString("names_bodyslide_preview"));
  lBodyslideGridLayout->addWidget(lResultNamesInApp, 4, 1);

  // Filters
  lBodyslideGridLayout->addWidget(new QLabel(tr("BodySlide filters:"), this), 5, 0);

  auto lFiltersWrapper{new QHBoxLayout(lBodyslideGroupBox)};
  lFiltersWrapper->setMargin(0);
  lBodyslideGridLayout->addLayout(lFiltersWrapper, 5, 1);

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

  auto lEditFilters{ComponentFactory::CreateButton(this, tr("Edit BodySlide filters sets"), "", "", lIconFolder, "edit_filters")};
  lFiltersWrapper->addWidget(lEditFilters);

  // Pre-bind initialization functions
  this->updateOSPXMLPreview(QString());
  this->updateBodyslideNamesPreview(QString());

  // Event binding
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &PresetCreator::updateAvailableBodyVersions);
  this->connect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lFeetSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&PresetCreator::refreshAllPreviewFields));
  this->connect(lOSPXMLNamesLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateOSPXMLPreview);
  this->connect(lNamesInAppLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateBodyslideNamesPreview);
  this->connect(lEditFilters, &QPushButton::clicked, this, &PresetCreator::openBodySlideFiltersEditor);

  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &PresetCreator::updateBodySlideFiltersListPreview);
  this->connect(lBodyVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &PresetCreator::updateBodySlideFiltersListPreview);
  this->connect(lFeetSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &PresetCreator::updateBodySlideFiltersListPreview);
  this->connect(lFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &PresetCreator::updateBodySlideFiltersListPreview);

  // Post-bind initialization functions
  this->initBodySlideFiltersList();
}

void PresetCreator::setupOutputGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Create the group box
  ComponentFactory::CreateOutputBox(this, aLayout, 3, 0, lIconFolder, mSettings.mainWindowOutputPath, this->mMinimumFirstColumnWidth, this->mSettings.font.size);
  auto lOutputGroupBox{this->findChild<QGroupBox*>(QString("output_group_box"))};
  this->connect(lOutputGroupBox, &QGroupBox::toggled, this, &PresetCreator::groupBoxChecked);

  // Event binding
  auto lOutputPathChooser{this->findChild<QPushButton*>(QString("output_path_chooser"))};
  this->connect(lOutputPathChooser, &QPushButton::clicked, this, &PresetCreator::chooseExportDirectory);

  auto lOutputSubpathLineEdit{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))};
  this->connect(lOutputSubpathLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateOutputPreview);

  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))};
  this->connect(lUseOnlySubdir, &QCheckBox::stateChanged, this, &PresetCreator::useOnlySubdirStateChanged);

  // Pre-filled data
  this->updateOutputPreview();
}

void PresetCreator::setupButtons(QHBoxLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Generate button
  auto lGenerateButton{ComponentFactory::CreateButton(this, tr("Generate the files on my computer"), "", "build", lIconFolder)};
  aLayout.addWidget(lGenerateButton);

  // Event binding
  this->connect(lGenerateButton, &QPushButton::clicked, this, &PresetCreator::generateDirectoryStructure);
}

void PresetCreator::loadValuesFromJsonObject(const QJsonObject& lFile)
{
  // Body Meshes
  this->findChild<QLineEdit*>(QString("meshes_path_input_femalebody"))->setText(lFile["meshes_path_input_femalebody"].toString());
  this->findChild<QLineEdit*>(QString("body_mesh_name_input"))->setText(lFile["body_mesh_name_input"].toString());

  this->findChild<QLineEdit*>(QString("meshes_path_input_femalefeet"))->setText(lFile["meshes_path_input_femalefeet"].toString());
  this->findChild<QLineEdit*>(QString("feet_mesh_name_input"))->setText(lFile["feet_mesh_name_input"].toString());

  this->findChild<QLineEdit*>(QString("meshes_path_input_femalehands"))->setText(lFile["meshes_path_input_femalehands"].toString());
  this->findChild<QLineEdit*>(QString("hands_mesh_name_input"))->setText(lFile["hands_mesh_name_input"].toString());

  this->findChild<QCheckBox*>(QString("use_beast_hands"))->setChecked(true); // Force to be true before putting the wanted value
  this->findChild<QCheckBox*>(QString("use_beast_hands"))->setChecked(lFile["use_beast_hands"].toBool());

  // Skeleton
  this->findChild<QCheckBox*>(QString("use_custom_skeleton"))->setChecked(true); // Force to be true before putting the wanted value
  this->findChild<QLineEdit*>(QString("skeleton_path_directory"))->setText(lFile["skeleton_path_directory"].toString());
  this->findChild<QLineEdit*>(QString("skeleton_name"))->setText(lFile["skeleton_name"].toString());
  this->findChild<QCheckBox*>(QString("use_custom_skeleton"))->setChecked(lFile["use_custom_skeleton"].toBool()); // Finish by checking or unchecking the box

  // BodySlide
  Utils::SelectComboBoxAt(this->findChild<QComboBox*>(QString("body_selector_name")), lFile["body_selector_name"].toInt());
  Utils::SelectComboBoxAt(this->findChild<QComboBox*>(QString("body_selector_version")), lFile["body_selector_version"].toInt());
  Utils::SelectComboBoxAt(this->findChild<QComboBox*>(QString("feet_mod_selector")), lFile["feet_mod_selector"].toInt());
  this->findChild<QLineEdit*>(QString("names_osp_xml_input"))->setText(lFile["names_osp_xml_input"].toString());
  this->findChild<QLineEdit*>(QString("names_bodyslide_input"))->setText(lFile["names_bodyslide_input"].toString());

  // Output
  auto lMainDirTextEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  auto lMainDirectory{lFile["output_path_directory"].toString()};
  lMainDirTextEdit->setText(lMainDirectory);
  lMainDirTextEdit->setDisabled(lMainDirectory.compare("") == 0);

  this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->setText(lFile["output_path_subdirectory"].toString());
}

QJsonObject PresetCreator::saveValuesToJsonObject()
{
  QJsonObject lProject;

  // Body
  auto lMeshesPathBody{this->findChild<QLineEdit*>(QString("meshes_path_input_femalebody"))->text().trimmed()};
  Utils::CleanPathString(lMeshesPathBody);
  lProject["meshes_path_input_femalebody"] = lMeshesPathBody;

  auto lBodyName{this->findChild<QLineEdit*>(QString("body_mesh_name_input"))->text().trimmed()};
  Utils::CleanBreaksString(lBodyName);
  lProject["body_mesh_name_input"] = lBodyName;

  // Feet
  auto lMeshesPathFeet{this->findChild<QLineEdit*>(QString("meshes_path_input_femalefeet"))->text().trimmed()};
  Utils::CleanPathString(lMeshesPathFeet);
  lProject["meshes_path_input_femalefeet"] = lMeshesPathFeet;

  auto lFeetName{this->findChild<QLineEdit*>(QString("feet_mesh_name_input"))->text().trimmed()};
  Utils::CleanBreaksString(lFeetName);
  lProject["feet_mesh_name_input"] = lFeetName;

  // Hands
  auto lMeshesPathHands{this->findChild<QLineEdit*>(QString("meshes_path_input_femalehands"))->text().trimmed()};
  Utils::CleanPathString(lMeshesPathHands);
  lProject["meshes_path_input_femalehands"] = lMeshesPathHands;

  auto lHandsName{this->findChild<QLineEdit*>(QString("hands_mesh_name_input"))->text().trimmed()};
  Utils::CleanBreaksString(lHandsName);
  lProject["hands_mesh_name_input"] = lHandsName;

  // Use beast hands
  auto lMustUseBeastHands{this->findChild<QCheckBox*>(QString("use_beast_hands"))->isChecked()};
  lProject["use_beast_hands"] = lMustUseBeastHands;

  // Use skeleton
  auto lMustCopySkeleton{this->findChild<QCheckBox*>(QString("use_custom_skeleton"))->isChecked()};
  lProject["use_custom_skeleton"] = lMustCopySkeleton;

  // Skeleton path
  auto lSkeletonPath{this->findChild<QLineEdit*>(QString("skeleton_path_directory"))->text().trimmed()};
  Utils::CleanPathString(lSkeletonPath);
  lProject["skeleton_path_directory"] = lSkeletonPath;

  // Skeleton name
  auto lSkeletonName{this->findChild<QLineEdit*>(QString("skeleton_name"))->text()};
  lProject["skeleton_name"] = lSkeletonName;

  // Body name
  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  lProject["body_selector_name"] = lBodyNameSelected;

  // Body version
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  lProject["body_selector_version"] = lBodyVersionSelected;

  // Feet mod
  auto lFeetModSelected{this->findChild<QComboBox*>(QString("feet_mod_selector"))->currentIndex()};
  lProject["feet_mod_selector"] = lFeetModSelected;

  // OSP and XML names
  auto lOSPXMLNames{this->findChild<QLineEdit*>(QString("names_osp_xml_input"))->text().trimmed()};
  lProject["names_osp_xml_input"] = lOSPXMLNames;

  // SliderSets names
  auto lBodyslideSlidersetsNames{this->findChild<QLineEdit*>(QString("names_bodyslide_input"))->text().trimmed()};
  lProject["names_bodyslide_input"] = lBodyslideSlidersetsNames;

  // Output
  auto lMainDirectory{this->findChild<QLineEdit*>(QString("output_path_directory"))->text().trimmed()};
  Utils::CleanPathString(lMainDirectory);
  lProject["output_path_directory"] = lMainDirectory;

  auto lSubDirectory{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->text().trimmed()};
  Utils::CleanPathString(lSubDirectory);
  lProject["output_path_subdirectory"] = lSubDirectory;

  // Version number string
  lProject["applicationVersion"] = Utils::GetApplicationVersion();

  // Return the generated object
  return lProject;
}

void PresetCreator::updateGUIOnBodyChange()
{
  // Get the GUI components
  auto lLabelBeastHands{this->findChild<QLabel*>(QString("label_use_beast_hands"))};
  auto lMustUseBeastHands{this->findChild<QCheckBox*>(QString("use_beast_hands"))};

  // The new "disabled" state to apply
  auto lDisableBeastHands{false};

  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  if (lBodyVersionSelected == -1)
  {
    return;
  }
  auto lBodySelected{DataLists::GetBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected)};

  if (!Utils::IsBodySupportingBeastHands(lBodySelected))
  {
    lDisableBeastHands = true;
    lMustUseBeastHands->setChecked(false);
  }

  lLabelBeastHands->setDisabled(lDisableBeastHands);
  lMustUseBeastHands->setDisabled(lDisableBeastHands);
}

void PresetCreator::updateAvailableBodyVersions()
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

bool PresetCreator::generateXMLFile(const QString& aEntryDirectory, const bool aGenerateFilesInExistingMainDirectory, const QString& aOSPXMLNames, const bool aMustUseBeastHands, const int aBodySelected, const int aFeetModIndex, const QString& aBodyslideSlidersetsNames)
{
  // Create the SliderGroups directory
  auto lSliderGroupsDirectory{aEntryDirectory + QDir::separator() + "CalienteTools" + QDir::separator() + "BodySlide" + QDir::separator() + "SliderGroups"};

  if (!QDir(lSliderGroupsDirectory).exists())
  {
    QDir().mkpath(lSliderGroupsDirectory);
  }
  else if (!aGenerateFilesInExistingMainDirectory)
  {
    Utils::DisplayWarningMessage(tr("Error while creating the meshes directory: \"%1\" already exists.").arg(lSliderGroupsDirectory));
    return false;
  }

  // Construct the file content
  auto lBodySelected{static_cast<BodyNameVersion>(aBodySelected)};
  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lUserFilters{Utils::GetFiltersForExport(this->mFiltersList, lFiltersListChooser->itemText(lFiltersListChooser->currentIndex()), lBodySelected, aFeetModIndex)};
  auto lXMLFileContent{SliderFileBuilder::BuildXMLFileContent(aBodyslideSlidersetsNames, lUserFilters, lBodySelected, aMustUseBeastHands, aFeetModIndex)};

  // Create the OSP file on disk
  auto lXMLPathName(lSliderGroupsDirectory + QDir::separator() + aOSPXMLNames + ".xml");

  QFile lXMLFile(lXMLPathName);
  if (lXMLFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
  {
    QTextStream lTextStream(&lXMLFile);
    lTextStream << lXMLFileContent;
    lTextStream.flush();

    lXMLFile.close();
  }
  else
  {
    Utils::DisplayWarningMessage(tr("Error while trying to create the XML file \"%1\". Aborting process.").arg(lXMLPathName));
    return false;
  }

  return true;
}

bool PresetCreator::generateOSPFile(const QString& aEntryDirectory, const bool aGenerateFilesInExistingMainDirectory, const QString& aOSPXMLNames, const bool aMustUseBeastHands, const int aBodySelected, const int aFeetModIndex, const QString& aBodyslideSlidersetsNames, QString aMeshesPathBody, QString aMeshesPathFeet, QString aMeshesPathHands, const QString& aBodyName, const QString& aFeetName, const QString& aHandsName)
{
  // Create the SliderSets directory
  auto lSliderSetsDirectory{aEntryDirectory + QDir::separator() + "CalienteTools" + QDir::separator() + "BodySlide" + QDir::separator() + "SliderSets"};

  if (!QDir(lSliderSetsDirectory).exists())
  {
    QDir().mkpath(lSliderSetsDirectory);
  }
  else if (!aGenerateFilesInExistingMainDirectory)
  {
    Utils::DisplayWarningMessage(tr("Error while creating the meshes directory: \"%1\" already exists.").arg(lSliderSetsDirectory));
    return false;
  }

  // Construct the file content
  auto lOSPFileContent{SliderFileBuilder::BuildOSPFileContent(aBodyslideSlidersetsNames, static_cast<BodyNameVersion>(aBodySelected), aMustUseBeastHands, aFeetModIndex)};

  // Fill the custom variables
  lOSPFileContent.replace(QString("{%%body_output_path%%}"), aMeshesPathBody.replace("/", "\\"));
  lOSPFileContent.replace(QString("{%%feet_output_path%%}"), aMeshesPathFeet.replace("/", "\\"));
  lOSPFileContent.replace(QString("{%%hands_output_path%%}"), aMeshesPathHands.replace("/", "\\"));
  lOSPFileContent.replace(QString("{%%body_output_file%%}"), !aBodyName.isEmpty() ? aBodyName : "femalebody");
  lOSPFileContent.replace(QString("{%%feet_output_file%%}"), !aFeetName.isEmpty() ? aFeetName : "femalefeet");
  lOSPFileContent.replace(QString("{%%hands_output_file%%}"), !aHandsName.isEmpty() ? aHandsName : "femalehands");

  // Create the OSP file on disk
  auto lOSPPathName(lSliderSetsDirectory + QDir::separator() + aOSPXMLNames + ".osp");

  QFile lOSPFile(lOSPPathName);
  if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
  {
    QTextStream lTextStream(&lOSPFile);
    lTextStream << lOSPFileContent;
    lTextStream.flush();

    lOSPFile.close();
  }
  else
  {
    Utils::DisplayWarningMessage(tr("Error while trying to create the OSP file \"%1\". Aborting process.").arg(lOSPPathName));
    return false;
  }

  return true;
}

bool PresetCreator::generateSkeletonFile(const QString& aEntryDirectory, const QString& aSkeletonPath)
{
  if (!aSkeletonPath.isEmpty())
  {
    auto lSkeletonDirectory{aEntryDirectory + QDir::separator() + aSkeletonPath};
    QDir().mkpath(lSkeletonDirectory);

    auto lSkeletonName{this->findChild<QLineEdit*>(QString("skeleton_name"))->text()};

    // Custom skeleton chooser
    auto lSkeletonChooser{this->findChild<QComboBox*>(QString("skeleton_chooser"))};
    auto lPath{QString("%1assets/skeletons/%2").arg(Utils::GetAppDataPathFolder()).arg(lSkeletonChooser->currentText())};

    auto lSkeletonWriteLocation{QString("%1%2%3.nif").arg(lSkeletonDirectory).arg(QDir::separator()).arg(lSkeletonName)};

    if (!QFile::copy(lPath, lSkeletonWriteLocation))
    {
      Utils::DisplayWarningMessage(tr("The custom skeleton file was not found or could not be copied. The application will take with the default XPMSSE (v4.72) skeleton instead..."));

      // Fallback option if the custom skeleton could not be copied
      if (!QFile::copy(":/ressources/skeleton_female", lSkeletonWriteLocation))
      {
        Utils::DisplayWarningMessage(tr("The skeleton file could not be created even using the default skeleton.\nBe sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions.\nBe sure that you used characters authorized by your OS in the given paths."));
        return false;
      }
    }
  }
  else
  {
    Utils::DisplayWarningMessage(tr("Error: no path given for the custom skeleton."));
    return false;
  }

  return true;
}

void PresetCreator::populateSkeletonChooser()
{
  auto lRootDir{Utils::GetAppDataPathFolder() + "assets/skeletons/"};
  Utils::CleanPathString(lRootDir);
  auto lAvailableSkeletons{QStringList()};

  // Search for all "*.nif" files
  QDirIterator it(lRootDir, QStringList() << QString("*.nif"), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
    lAvailableSkeletons.push_back(it.fileInfo().absoluteFilePath().remove(lRootDir, Qt::CaseInsensitive));
  }

  // Clear the combo box and add the found files to it
  auto lSkeletonChooser{this->findChild<QComboBox*>(QString("skeleton_chooser"))};
  lSkeletonChooser->clear();
  lSkeletonChooser->addItems(lAvailableSkeletons);
}

void PresetCreator::updateMeshesPreview()
{
  // Body meshes path and name
  auto lMeshesPathBody{this->findChild<QLineEdit*>(QString("meshes_path_input_femalebody"))->text().trimmed()};
  Utils::CleanPathString(lMeshesPathBody);
  auto lBodyName{this->findChild<QLineEdit*>(QString("body_mesh_name_input"))->text().trimmed()};
  Utils::CleanBreaksString(lBodyName);

  // Feet meshes path and name
  auto lMeshesPathFeet{this->findChild<QLineEdit*>(QString("meshes_path_input_femalefeet"))->text().trimmed()};
  Utils::CleanPathString(lMeshesPathFeet);
  auto lFeetName{this->findChild<QLineEdit*>(QString("feet_mesh_name_input"))->text().trimmed()};
  Utils::CleanBreaksString(lFeetName);

  // Hands meshes path and name
  auto lMeshesPathHands{this->findChild<QLineEdit*>(QString("meshes_path_input_femalehands"))->text().trimmed()};
  Utils::CleanPathString(lMeshesPathHands);
  auto lHandsName{this->findChild<QLineEdit*>(QString("hands_mesh_name_input"))->text().trimmed()};
  Utils::CleanBreaksString(lHandsName);

  // Get preview label
  auto lPreviewLabel{this->findChild<QLabel*>(QString("meshes_preview"))};
  auto lFullPreview{QString()};
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

  lFullPreview.append(QString("[...]/Skyrim Special Edition/Data/%1/%2_[0/1].nif\n").arg(lMeshesPathBody).arg(lBodyName));
  lFullPreview.append(QString("[...]/Skyrim Special Edition/Data/%1/%2_[0/1].nif\n").arg(lMeshesPathFeet).arg(lFeetName));
  lFullPreview.append(QString("[...]/Skyrim Special Edition/Data/%1/%2_[0/1].nif").arg(lMeshesPathHands).arg(lHandsName));

  auto lNewTextColor{this->mSettings.successColor};

  if (lIsValidPath)
  {
    if ((!lMeshesPathBody.startsWith("meshes/", Qt::CaseInsensitive) || (lMeshesPathBody.startsWith("meshes/", Qt::CaseInsensitive) && lMeshesPathBody.length() < 8))
        || (!lMeshesPathFeet.startsWith("meshes/", Qt::CaseInsensitive) || (lMeshesPathFeet.startsWith("meshes/", Qt::CaseInsensitive) && lMeshesPathFeet.length() < 8))
        || (!lMeshesPathHands.startsWith("meshes/", Qt::CaseInsensitive) || (lMeshesPathHands.startsWith("meshes/", Qt::CaseInsensitive) && lMeshesPathHands.length() < 8)))
    {
      lNewTextColor = this->mSettings.warningColor;
    }
  }
  else
  {
    lNewTextColor = this->mSettings.dangerColor;
  }

  lPreviewLabel->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lPreviewLabel->setText(lFullPreview);
}

void PresetCreator::useOnlySubdirStateChanged(int)
{
  this->updateOutputPreview();
}

void PresetCreator::updateOutputPreview()
{
  this->setHasUserDoneSomething(true);

  auto lMainDirTextEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  auto lSubDirectory{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->text().trimmed()};
  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))->isChecked()};
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("output_path_preview"))};

  Utils::UpdateOutputPreview(lMainDirTextEdit, lSubDirectory, lUseOnlySubdir, this->mSettings.successColor, this->mSettings.warningColor, this->mSettings.dangerColor, lOutputPathsPreview);
}

void PresetCreator::updateOSPXMLPreview(QString aText)
{
  this->setHasUserDoneSomething(true);

  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("names_osp_xml_preview"))};
  auto lIsValidPath{true};

  Utils::CleanPathString(aText);

  if (aText.trimmed().isEmpty())
  {
    aText = QString::fromStdString("*");
    lIsValidPath = false;
  }

  auto lConstructedPreviewText(
    QString(
      "[...]/Skyrim Special Edition/Data/CalienteTools/BodySlide/SliderGroups/%1.xml\n"
      "[...]/Skyrim Special Edition/Data/CalienteTools/BodySlide/SliderSets/%1.osp")
      .arg(aText));

  auto lNewTextColor{this->mSettings.successColor};

  if (!lIsValidPath)
  {
    lNewTextColor = this->mSettings.dangerColor;
  }

  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lConstructedPreviewText);
}

void PresetCreator::updateBodyslideNamesPreview(QString aText)
{
  this->setHasUserDoneSomething(true);

  auto lMustUseBeastHands{this->findChild<QCheckBox*>(QString("use_beast_hands"))->isChecked()};
  auto lIsValidPath{true};

  Utils::CleanPathString(aText);

  if (aText.trimmed().isEmpty())
  {
    aText = QString("*");
    lIsValidPath = false;
  }

  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  auto lBodySelected{DataLists::GetBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected)};
  auto lFeetModIndex{this->findChild<QComboBox*>(QString("feet_mod_selector"))->currentIndex()};

  auto lConstructedPreviewText{QString()};
  lConstructedPreviewText.append(Utils::GetBodySliderValue(lBodySelected));                      // Body
  lConstructedPreviewText.append(Utils::GetFeetSliderValue(lBodySelected, lFeetModIndex));       // Feet
  lConstructedPreviewText.append(Utils::GetHandsSliderValue(lBodySelected, lMustUseBeastHands)); // Hands
  lConstructedPreviewText = lConstructedPreviewText.arg(aText);

  auto lNewTextColor{this->mSettings.successColor};

  if (!lIsValidPath)
  {
    lNewTextColor = this->mSettings.dangerColor;
  }

  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("names_bodyslide_preview"))};
  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lConstructedPreviewText);
}

void PresetCreator::updateSkeletonPathState(int aState)
{
  // Chooser
  auto lLabelChooser{this->findChild<QLabel*>(QString("label_skeleton_chooser"))};
  auto lChooser{this->findChild<QComboBox*>(QString("skeleton_chooser"))};
  auto lChooserRefresher{this->findChild<QPushButton*>(QString("skeleton_chooser_refresher"))};

  // Directory
  auto lLabelPathDir{this->findChild<QLabel*>(QString("label_skeleton_path_directory"))};
  auto lPathDir{this->findChild<QLineEdit*>(QString("skeleton_path_directory"))};

  // Name
  auto lLabelSkeleton{this->findChild<QLabel*>(QString("label_skeleton_female"))};
  auto lSkeleton{this->findChild<QLineEdit*>(QString("skeleton_name"))};
  auto lSkeletonExtension{this->findChild<QLabel*>(QString("skeleton_name_extension"))};

  // Preview
  auto lLabelPreview{this->findChild<QLabel*>(QString("label_skeleton_path_preview"))};
  auto lPreview{this->findChild<QLabel*>(QString("skeleton_path_preview"))};

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
  this->setHasUserDoneSomething(true);

  auto lSkeletonPath{this->findChild<QLineEdit*>(QString("skeleton_path_directory"))->text()};
  Utils::CleanPathString(lSkeletonPath);
  auto lSkeletonName{this->findChild<QLineEdit*>(QString("skeleton_name"))->text()};
  Utils::CleanBreaksString(lSkeletonName);
  auto lIsValidPath{true};

  if (lSkeletonPath.trimmed().isEmpty())
  {
    lSkeletonPath = QString::fromStdString("*");
    lIsValidPath = false;
  }

  if (lSkeletonName.trimmed().isEmpty())
  {
    lSkeletonName = "skeleton_female";
  }

  auto lConstructedPath(QString("[...]/Skyrim Special Edition/Data/%1/%2.nif").arg(lSkeletonPath).arg(lSkeletonName));
  auto lOutputPathPreview{this->findChild<QLabel*>(QString("skeleton_path_preview"))};

  auto lNewTextColor{QString()};

  if (lIsValidPath)
  {
    auto lStart{lSkeletonPath.startsWith("meshes/", Qt::CaseInsensitive)};

    if (!lStart || (lStart && lSkeletonPath.length() < 8))
    {
      lNewTextColor = this->mSettings.warningColor;
    }
    else
    {
      lNewTextColor = this->mSettings.successColor;
    }
  }
  else if (this->findChild<QCheckBox*>(QString("use_custom_skeleton"))->isChecked())
  {
    lNewTextColor = this->mSettings.dangerColor;
  }

  lOutputPathPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathPreview->setText(lConstructedPath);
}

void PresetCreator::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "mainWindowOutput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "mainWindowOutput", lPath);
  this->updateOutputPreview();
}

void PresetCreator::generateDirectoryStructure()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Selected body
  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("body_selector_version"))->currentIndex()};
  auto lBodySelected{static_cast<int>(DataLists::GetBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected))};

  // Selected feet
  auto lFeetModIndex{this->findChild<QComboBox*>(QString("feet_mod_selector"))->currentIndex()};

  // Beast hands
  auto lCheckboxUseBeastHands{this->findChild<QCheckBox*>(QString("use_beast_hands"))};
  auto lMustUseBeastHands{lCheckboxUseBeastHands->isEnabled() && lCheckboxUseBeastHands->isChecked()};

  // Body meshes path and name
  auto lMeshesPathBody{this->findChild<QLineEdit*>(QString("meshes_path_input_femalebody"))->text().trimmed()};
  Utils::CleanPathString(lMeshesPathBody);
  auto lBodyName{this->findChild<QLineEdit*>(QString("body_mesh_name_input"))->text().trimmed()};
  Utils::CleanPathString(lBodyName);

  // Feet meshes path and name
  const auto lInputMeshesPathFeet{this->findChild<QLineEdit*>(QString("meshes_path_input_femalefeet"))};
  const auto lSkipFeetCheck{!lInputMeshesPathFeet->isEnabled()};
  auto lMeshesPathFeet{lInputMeshesPathFeet->text().trimmed()};
  Utils::CleanPathString(lMeshesPathFeet);
  auto lFeetName{this->findChild<QLineEdit*>(QString("feet_mesh_name_input"))->text().trimmed()};
  Utils::CleanPathString(lFeetName);

  // Hands meshes path and name
  auto lInputMeshesPathHands{this->findChild<QLineEdit*>(QString("meshes_path_input_femalehands"))};
  auto lSkipHandsCheck{!lInputMeshesPathHands->isEnabled()};
  auto lMeshesPathHands{lInputMeshesPathHands->text().trimmed()};
  Utils::CleanPathString(lMeshesPathHands);
  auto lHandsName{this->findChild<QLineEdit*>(QString("hands_mesh_name_input"))->text().trimmed()};
  Utils::CleanPathString(lHandsName);

  // BodySlide names
  auto lOSPXMLNames{this->findChild<QLineEdit*>(QString("names_osp_xml_input"))->text().trimmed()};
  Utils::CleanBreaksString(lOSPXMLNames);
  auto lBodyslideSlidersetsNames{this->findChild<QLineEdit*>(QString("names_bodyslide_input"))->text().trimmed()};
  Utils::CleanBreaksString(lBodyslideSlidersetsNames);

  // Output paths
  auto lMainDirectory{this->findChild<QLineEdit*>(QString("output_path_directory"))->text().trimmed()};
  auto lSubDirectory{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->text().trimmed()};
  Utils::CleanPathString(lSubDirectory);

  // Does the user want to define the path only through the secondary path?
  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))->isChecked()};

  // Full extract path
  auto lEntryDirectory{lSubDirectory};
  if (!lUseOnlySubdir)
  {
    lEntryDirectory = (lSubDirectory.isEmpty() ? lMainDirectory : (lMainDirectory + "/" + lSubDirectory));
  }

  // Check if the full extract path has been given by the user
  if (lEntryDirectory.isEmpty())
  {
    Utils::DisplayWarningMessage(tr("Error: no path given to export the files."));
    return;
  }

  // Check if the path could be valid
  if (lEntryDirectory.startsWith(QDir::separator()))
  {
    Utils::DisplayWarningMessage(tr("Error: the path given to export the files seems to be invalid."));
    return;
  }

  // Create main directory
  auto lGenerateFilesInExistingMainDirectory{false};
  if (!QDir(lEntryDirectory).exists())
  {
    // Wait to know the result of the mkdir()
    if (!QDir().mkpath(lEntryDirectory))
    {
      Utils::DisplayWarningMessage(tr("Error while creating the main directory: \"%1\" could not be created on your computer.\nBe sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions.\nBe sure that you used characters authorized by your OS in the given paths.").arg(lEntryDirectory));
      return;
    }
  }
  else
  {
    // Since the directory already exist, ask the user to generate another preset in it
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Already existing directory"),
                                      tr("The main directory \"%1\" already exists on your computer. Do you still want to continue the files generation in this directory?").arg(lEntryDirectory),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the files generation"),
                                      tr("Cancel the files generation"),
                                      "",
                                      this->mSettings.warningColor,
                                      this->mSettings.dangerColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      return;
    }

    lGenerateFilesInExistingMainDirectory = true;
  }

  // Export the meshes
  if (lMeshesPathBody.isEmpty())
  {
    Utils::DisplayWarningMessage(tr("Error: no path has been given for the body meshes."));

    // Remove the directory since the generation is incomplete
    if (!lGenerateFilesInExistingMainDirectory)
    {
      Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
    }

    return;
  }

  if (lMeshesPathFeet.isEmpty() && !lSkipFeetCheck)
  {
    Utils::DisplayWarningMessage(tr("Error: no path has been given for the feet meshes."));

    // Remove the directory since the generation is incomplete
    if (!lGenerateFilesInExistingMainDirectory)
    {
      Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
    }

    return;
  }

  if (lMeshesPathHands.isEmpty() && !lSkipHandsCheck)
  {
    Utils::DisplayWarningMessage(tr("Error: no path has been given for the hands meshes."));

    // Remove the directory since the generation is incomplete
    if (!lGenerateFilesInExistingMainDirectory)
    {
      Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
    }

    return;
  }

  // Check if a name has been given for the OSP and XML files
  if (lOSPXMLNames.isEmpty())
  {
    Utils::DisplayWarningMessage(tr("Error: no name given for the BodySlide files."));

    // Remove the directory since the generation is incomplete
    if (!lGenerateFilesInExistingMainDirectory)
    {
      Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
    }

    return;
  }

  // Check if a name has been given for the presets
  if (lBodyslideSlidersetsNames.isEmpty())
  {
    Utils::DisplayWarningMessage(tr("Error: no name given for the slider sets (names that appear in the BodySlide application)."));

    // Remove the directory since the generation is incomplete
    if (!lGenerateFilesInExistingMainDirectory)
    {
      Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
    }

    return;
  }

  // XML file
  if (!this->generateXMLFile(lEntryDirectory, lGenerateFilesInExistingMainDirectory, lOSPXMLNames, lMustUseBeastHands, lBodySelected, lFeetModIndex, lBodyslideSlidersetsNames))
  {
    // Remove the directory since the generation is incomplete
    if (!lGenerateFilesInExistingMainDirectory)
    {
      Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
    }

    return;
  }

  // OSP file
  if (!this->generateOSPFile(lEntryDirectory, lGenerateFilesInExistingMainDirectory, lOSPXMLNames, lMustUseBeastHands, lBodySelected, lFeetModIndex, lBodyslideSlidersetsNames, lMeshesPathBody, lMeshesPathFeet, lMeshesPathHands, lBodyName, lFeetName, lHandsName))
  {
    // Remove the directory since the generation is incomplete
    if (!lGenerateFilesInExistingMainDirectory)
    {
      Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
    }

    return;
  }

  // Skeleton
  auto lMustCopySkeleton{this->findChild<QCheckBox*>(QString("use_custom_skeleton"))->isChecked()};

  if (lMustCopySkeleton)
  {
    auto lSkeletonPath{this->findChild<QLineEdit*>(QString("skeleton_path_directory"))->text().trimmed()};
    Utils::CleanPathString(lSkeletonPath);

    if (!this->generateSkeletonFile(lEntryDirectory, lSkeletonPath))
    {
      // Remove the directory since the generation is incomplete
      if (!lGenerateFilesInExistingMainDirectory)
      {
        Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
      }

      return;
    }
  }

  // Update the color of the output directory preview
  this->updateOutputPreview();

  auto lTitle{tr("Generation successful")};
  auto lMessage{tr("Every file has been correctly generated. You can now exit the application or create another conversion!")};

  if (mSettings.mainWindowAutomaticallyOpenGeneratedDirectory)
  {
    QMessageBox lConfirmationBox(QMessageBox::Icon::Information, lTitle, lMessage, QMessageBox::StandardButton::NoButton, this);
    lConfirmationBox.setIconPixmap(QPixmap(":/icons/green-info-circle").scaledToHeight(48, Qt::SmoothTransformation));

    auto lOKButton{lConfirmationBox.addButton(tr("Open the generated directory"), QMessageBox::ButtonRole::AcceptRole)};
    lOKButton->setCursor(Qt::PointingHandCursor);
    lConfirmationBox.setDefaultButton(lOKButton);
    lConfirmationBox.exec();

    // Open the directory where the file structure has been created
    QDesktopServices::openUrl(QUrl::fromLocalFile(lEntryDirectory));
  }
  else
  {
    if (Utils::DisplayQuestionMessage(this,
                                      lTitle,
                                      lMessage,
                                      "icons",
                                      "green-info-circle",
                                      tr("Open the generated directory"),
                                      tr("OK"),
                                      "",
                                      "",
                                      "",
                                      "",
                                      false)
        == ButtonClicked::YES)
    {
      // Open the directory where the file structure has been created
      QDesktopServices::openUrl(QUrl::fromLocalFile(lEntryDirectory));
    }
  }
}

void PresetCreator::refreshAllPreviewFields(int)
{
  this->refreshAllPreviewFields();
}

void PresetCreator::refreshAllPreviewFields()
{
  this->setHasUserDoneSomething(true);

  // Update the GUI based on the available
  this->updateGUIOnBodyChange();

  // Refresh the preview of the body meshes parts
  this->updateMeshesPreview();

  // Refresh the names in the bodyslide application
  auto lBodyslideSlidersetsNames{this->findChild<QLineEdit*>(QString("names_bodyslide_input"))->text().trimmed()};
  this->updateBodyslideNamesPreview(lBodyslideSlidersetsNames);
}

void PresetCreator::openBodySlideFiltersEditor()
{
  auto lEditor{new BodySlideFiltersEditor(this, this->mSettings, this->mFiltersList)};
  this->connect(lEditor, &BodySlideFiltersEditor::listEdited, this, &PresetCreator::updateBodySlideFiltersList);
}

void PresetCreator::initBodySlideFiltersList()
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

void PresetCreator::updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList)
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

void PresetCreator::updateBodySlideFiltersListPreview()
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

  this->setHasUserDoneSomething(true);
}

void PresetCreator::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::SetGroupBoxState(lGroupBox, !aIsChecked);
}
