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
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QStandardPaths>
#include <QStyledItemDelegate>

#include "TargetMeshesPicker.h"

PresetCreator::PresetCreator(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QWidget(aParent)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mMinimumFirstColumnWidth(275)
  , mTargetBodyMesh(aSettings.presetCreator.defaultBodyFeet.bodyMesh)
  , mTargetFeetMesh(aSettings.presetCreator.defaultBodyFeet.feetMesh)
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
    const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "lastLoadedProject", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
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
                                      Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme),
                                      "help-circle",
                                      tr("Open the project without saving"),
                                      tr("Cancel the file opening"),
                                      "",
                                      this->mSettings.display.dangerColor,
                                      this->mSettings.display.successColor,
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
    const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "lastSavedProject", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
    lFilePath = QFileDialog::getSaveFileName(this, "", lContextPath, "Preset Creator Project (*.pcp *.json)");

    // If the user canceled
    if (lFilePath.isEmpty())
    {
      return;
    }

    Utils::UpdatePathAtKey(this->mLastPaths, "lastSavedProject", lFilePath);
    this->mLastUsedSavePath = lFilePath;
    this->parentWidget()->findChild<QAction*>(QString("action_save_project"))->setDisabled(false);
  }

  if (this->mLastUsedSavePath.isEmpty())
  {
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

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
      {
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
      case AssistedConversionRole::IGNORED:
        // It should not be possible to reach this statement
        break;
      default:
        // It should not be possible to reach this statement
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
                                      Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme),
                                      "help-circle",
                                      tr("Clear all the unassigned entries"),
                                      tr("Keep the currently set values for the unassigned entries"),
                                      "",
                                      this->mSettings.display.warningColor,
                                      this->mSettings.display.warningColor,
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
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Body meshes group box
  auto lMeshesGroupBox{ComponentFactory::CreateGroupBox(this, tr("Original mod's body meshes"), "body", lIconFolder, this->mSettings.display.font.size)};
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

  auto lNeedBeastHands{ComponentFactory::CreateCheckBox(this, tr("Check this box if the follower or NPC uses beast hands"), "", "use_beast_hands")};
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
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Custom skeleton group box
  auto lSkeletonGroupBox{ComponentFactory::CreateGroupBox(this, tr("Skeleton"), "skeleton", lIconFolder, this->mSettings.display.font.size)};
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
                         "<img src=\":/%1/info-circle-smaller\" alt=\"~info icon~\" style=\"vertical-align: baseline;\"> %2</p>")
                   .arg(lIconFolder, lText)};
  lLabelSkeleton->setText(lRichText);
  lLabelSkeleton->setTextFormat(Qt::RichText);
  lLabelSkeleton->setToolTip(QString(tr("Not overriding a custom skeleton would cause breasts collision and physics to be inaccurate.")));
  lSkeletonGridLayout->addWidget(lLabelSkeleton, 0, 0);

  auto lNeedCustomSkeleton{ComponentFactory::CreateCheckBox(this, tr("Check this box if the follower or NPC uses a custom skeleton"), "", "use_custom_skeleton")};
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

  // Refresh button
  auto lSkeletonRefresher{ComponentFactory::CreateButton(this, tr("Refresh"), "", "", lIconFolder, "skeleton_chooser_refresher")};
  lSkeletonGridLayout->addWidget(lSkeletonRefresher, 1, 2);

  // Open assets directory
  auto lOpenAssetsDirectory{ComponentFactory::CreateButton(this, "View in explorer", "", "folder-search", lIconFolder, "open_skeletons_assets_directory")};
  lSkeletonGridLayout->addWidget(lOpenAssetsDirectory, 1, 3);

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
  lSkeletonGridLayout->addWidget(lSkeletonPathsPreview, 4, 1, 1, 3);

  // Initialization functions
  this->updateSkeletonPreview();

  // Event binding
  this->connect(lNeedCustomSkeleton, &QCheckBox::stateChanged, this, &PresetCreator::updateSkeletonPathState);
  lNeedCustomSkeleton->setChecked(true);
  lNeedCustomSkeleton->setChecked(false);

  this->connect(lSkeletonPathLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateSkeletonPreview);
  this->connect(lSkeletonRefresher, &QPushButton::clicked, this, &PresetCreator::populateSkeletonChooser);
  this->connect(lOpenAssetsDirectory, &QPushButton::clicked, this, &PresetCreator::openSkeletonsAssetsDirectory);
  this->connect(lSkeletonName, &QLineEdit::textChanged, this, &PresetCreator::updateSkeletonPreview);
}

void PresetCreator::setupBodySlideGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // BodySlide output settings group box
  auto lBodyslideGroupBox{ComponentFactory::CreateGroupBox(this, tr("BodySlide output"), "bodyslide-logo", lIconFolder, this->mSettings.display.font.size)};
  aLayout.addWidget(lBodyslideGroupBox, 2, 0);

  // Grid layout
  auto lBodyslideGridLayout{new QGridLayout(lBodyslideGroupBox)};
  lBodyslideGridLayout->setSpacing(10);
  lBodyslideGridLayout->setContentsMargins(15, 20, 15, 15);
  lBodyslideGridLayout->setAlignment(Qt::AlignTop);
  lBodyslideGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Targeted body and version
  auto lTargetMeshesPicker{ComponentFactory::CreateTargetMeshesPickerLine(this,
                                                                          *lBodyslideGridLayout,
                                                                          true,
                                                                          0,
                                                                          lIconFolder,
                                                                          QString("target_meshes_picker_button"),
                                                                          QString("currently_targeted_body_feet"))};

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
                         .arg(lIconFolder, tr("Presets names:")));
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
  lFiltersListChooser->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
  lFiltersWrapper->addWidget(lFiltersListChooser);

  auto lFiltersList{new QLabel("", this)};
  lFiltersList->setObjectName(QString("bodyslide_filters"));
  lFiltersList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  lFiltersList->setWordWrap(true);
  lFiltersWrapper->addWidget(lFiltersList);

  auto lEditFilters{ComponentFactory::CreateButton(this, tr("Edit BodySlide filters sets"), "", "filter", lIconFolder, "edit_filters")};
  lFiltersWrapper->addWidget(lEditFilters);

  // Pre-bind initialization functions
  this->targetMeshesChanged(this->mTargetBodyMesh, this->mTargetFeetMesh);
  this->updateOSPXMLPreview(QString());
  this->updateBodyslideNamesPreview(QString());

  // Event binding
  this->connect(lTargetMeshesPicker, &QPushButton::clicked, this, &PresetCreator::openTargetMeshesPicker);
  this->connect(lOSPXMLNamesLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateOSPXMLPreview);
  this->connect(lNamesInAppLineEdit, &QLineEdit::textChanged, this, &PresetCreator::updateBodyslideNamesPreview);
  this->connect(lEditFilters, &QPushButton::clicked, this, &PresetCreator::openBodySlideFiltersEditor);
  this->connect(lFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &PresetCreator::updateBodySlideFiltersListPreview);

  // Post-bind initialization functions
  this->initBodySlideFiltersList();
}

void PresetCreator::setupOutputGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Create the group box
  ComponentFactory::CreateOutputBox(this, aLayout, 3, 0, lIconFolder, this->mMinimumFirstColumnWidth, this->mSettings.display.font.size);

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
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

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

  // Body and feet meshes: compatiblity
  if (Utils::CompareVersionNumbers(lFile["applicationVersion"].toString(), "3.5.0.0") == ApplicationVersionRelative::OLDER)
  {
    const auto lCompatibilityData{DataLists::ReadBodyFeetModsCompatibility(lFile["body_selector_name"].toInt(),
                                                                           lFile["body_selector_version"].toInt(),
                                                                           lFile["feet_mod_selector"].toInt())};
    this->targetMeshesChanged(lCompatibilityData.first, lCompatibilityData.second);
  }
  // Body and feet meshes: last format version
  else
  {
    this->targetMeshesChanged(static_cast<BodyNameVersion>(lFile["body_mesh"].toInt()),
                              static_cast<FeetNameVersion>(lFile["feet_mesh"].toInt()));
  }

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

  // Targeted body mesh
  lProject["body_mesh"] = static_cast<int>(this->mTargetBodyMesh);

  // Targeted feet mesh
  lProject["feet_mesh"] = static_cast<int>(this->mTargetFeetMesh);

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

void PresetCreator::updateBeastHandsCheckboxState()
{
  // Get the GUI components
  auto lLabelBeastHands{this->findChild<QLabel*>(QString("label_use_beast_hands"))};
  auto lMustUseBeastHands{this->findChild<QCheckBox*>(QString("use_beast_hands"))};

  // The new "disabled" state to apply
  auto lDisableBeastHands{false};

  if (!Utils::IsBodySupportingBeastHands(this->mTargetBodyMesh))
  {
    lDisableBeastHands = true;
    lMustUseBeastHands->setChecked(false);
  }

  lLabelBeastHands->setDisabled(lDisableBeastHands);
  lMustUseBeastHands->setDisabled(lDisableBeastHands);
}

void PresetCreator::populateSkeletonChooser()
{
  auto lRootDir{Utils::GetAppDataPathFolder() + "assets/skeletons/"};
  Utils::CleanPathString(lRootDir);
  QStringList lAvailableSkeletons;

  // Search for all "*.nif" files
  QDirIterator it(lRootDir, QStringList() << QString("*.nif"), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
    lAvailableSkeletons.push_back(it.fileInfo().absoluteFilePath().remove(lRootDir, Qt::CaseInsensitive));
  }

  // Get the combobox
  const auto lSkeletonChooser{this->findChild<QComboBox*>(QString("skeleton_chooser"))};

  // Save the selected skeleton file name
  const auto lPreviousIndex{lSkeletonChooser->currentIndex()};
  const auto lPreviousSkeleton{lSkeletonChooser->itemText(lPreviousIndex)};

  // Clear the combo box and add the found files to it
  lSkeletonChooser->clear();
  lSkeletonChooser->addItems(lAvailableSkeletons);

  // Reselect the previously selected skeleton, if it still exists
  if (lPreviousIndex != -1)
    lSkeletonChooser->setCurrentIndex(std::max(lAvailableSkeletons.indexOf(lPreviousSkeleton), 0));
  else if (lSkeletonChooser->count() > 0)
    lSkeletonChooser->setCurrentIndex(0);
}

void PresetCreator::openSkeletonsAssetsDirectory()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(QString("%1assets/skeletons").arg(Utils::GetAppDataPathFolder())));
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

  lFullPreview.append(QString("[...]/Skyrim Special Edition/Data/%1/%2_[0/1].nif\n").arg(lMeshesPathBody, lBodyName));
  lFullPreview.append(QString("[...]/Skyrim Special Edition/Data/%1/%2_[0/1].nif\n").arg(lMeshesPathFeet, lFeetName));
  lFullPreview.append(QString("[...]/Skyrim Special Edition/Data/%1/%2_[0/1].nif").arg(lMeshesPathHands, lHandsName));

  auto lNewTextColor{this->mSettings.display.successColor};

  if (lIsValidPath)
  {
    if ((!lMeshesPathBody.startsWith("meshes/", Qt::CaseInsensitive) || (lMeshesPathBody.startsWith("meshes/", Qt::CaseInsensitive) && lMeshesPathBody.length() < 8))
        || (!lMeshesPathFeet.startsWith("meshes/", Qt::CaseInsensitive) || (lMeshesPathFeet.startsWith("meshes/", Qt::CaseInsensitive) && lMeshesPathFeet.length() < 8))
        || (!lMeshesPathHands.startsWith("meshes/", Qt::CaseInsensitive) || (lMeshesPathHands.startsWith("meshes/", Qt::CaseInsensitive) && lMeshesPathHands.length() < 8)))
    {
      lNewTextColor = this->mSettings.display.warningColor;
    }
  }
  else
  {
    lNewTextColor = this->mSettings.display.dangerColor;
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

  Utils::UpdateOutputPreview(lMainDirTextEdit, lSubDirectory, lUseOnlySubdir, this->mSettings.display.successColor, this->mSettings.display.warningColor, this->mSettings.display.dangerColor, lOutputPathsPreview);
}

void PresetCreator::updateOSPXMLPreview(QString aText)
{
  this->setHasUserDoneSomething(true);

  aText = Utils::CleanBreaksString(aText.trimmed());
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("names_osp_xml_preview"))};
  auto lIsValidPath{true};

  if (aText.isEmpty() || aText.contains('\\') || aText.contains('/'))
  {
    aText = QString::fromStdString("*");
    lIsValidPath = false;
  }

  auto lConstructedPreviewText{QString(
                                 "[...]/Skyrim Special Edition/Data/CalienteTools/BodySlide/SliderGroups/%1.xml\n"
                                 "[...]/Skyrim Special Edition/Data/CalienteTools/BodySlide/SliderSets/%1.osp")
                                 .arg(aText)};

  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lIsValidPath ? this->mSettings.display.successColor : this->mSettings.display.dangerColor));
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

  auto lConstructedPreviewText{QString()};
  lConstructedPreviewText.append(Utils::GetBodySliderValue(this->mTargetBodyMesh));                      // Body
  lConstructedPreviewText.append(Utils::GetFeetSliderValue(this->mTargetFeetMesh));                      // Feet
  lConstructedPreviewText.append(Utils::GetHandsSliderValue(this->mTargetBodyMesh, lMustUseBeastHands)); // Hands
  lConstructedPreviewText = lConstructedPreviewText.arg(aText);

  auto lNewTextColor{this->mSettings.display.successColor};

  if (!lIsValidPath)
  {
    lNewTextColor = this->mSettings.display.dangerColor;
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
  auto lOpenAssetsDirectory{this->findChild<QPushButton*>(QString("open_skeletons_assets_directory"))};

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
      lOpenAssetsDirectory->setDisabled(true);
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
      lOpenAssetsDirectory->setDisabled(false);
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

  auto lConstructedPath(QString("[...]/Skyrim Special Edition/Data/%1/%2.nif").arg(lSkeletonPath, lSkeletonName));
  auto lOutputPathPreview{this->findChild<QLabel*>(QString("skeleton_path_preview"))};

  auto lNewTextColor{QString()};

  if (lIsValidPath)
  {
    auto lStart{lSkeletonPath.startsWith("meshes/", Qt::CaseInsensitive)};

    if (!lStart || (lStart && lSkeletonPath.length() < 8))
    {
      lNewTextColor = this->mSettings.display.warningColor;
    }
    else
    {
      lNewTextColor = this->mSettings.display.successColor;
    }
  }
  else if (this->findChild<QCheckBox*>(QString("use_custom_skeleton"))->isChecked())
  {
    lNewTextColor = this->mSettings.display.dangerColor;
  }

  lOutputPathPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathPreview->setText(lConstructedPath);
}

void PresetCreator::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "mainWindowOutput", lLineEdit->text(), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
  const auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "mainWindowOutput", lPath);
  this->updateOutputPreview();
}

void PresetCreator::generateDirectoryStructure()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

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
  auto lMeshesPathFeet{lInputMeshesPathFeet->text().trimmed()};
  Utils::CleanPathString(lMeshesPathFeet);
  auto lFeetName{this->findChild<QLineEdit*>(QString("feet_mesh_name_input"))->text().trimmed()};
  Utils::CleanPathString(lFeetName);

  // Hands meshes path and name
  auto lInputMeshesPathHands{this->findChild<QLineEdit*>(QString("meshes_path_input_femalehands"))};
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
  QString lEntryDirectory;
  if (lUseOnlySubdir)
    lEntryDirectory = lSubDirectory;
  else if (!lMainDirectory.isEmpty())
    lEntryDirectory = (lSubDirectory.isEmpty() ? lMainDirectory : (lMainDirectory + "/" + lSubDirectory));

  // Check if the full extract path has been given by the user
  if (lEntryDirectory.isEmpty())
  {
    Utils::DisplayWarningMessage(tr("Error: no path given to export the files."));
    return;
  }

  // Check if the path could be valid
  if (lEntryDirectory.startsWith('/') || lEntryDirectory.startsWith('\\'))
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
                                      tr("The directory \"%1\" already exists on your computer. Do you still want to generate the files in this directory?").arg(lEntryDirectory),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the files generation"),
                                      tr("Cancel the files generation"),
                                      "",
                                      this->mSettings.display.dangerColor,
                                      this->mSettings.display.successColor,
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

  if (lMeshesPathFeet.isEmpty())
  {
    Utils::DisplayWarningMessage(tr("Error: no path has been given for the feet meshes."));

    // Remove the directory since the generation is incomplete
    if (!lGenerateFilesInExistingMainDirectory)
    {
      Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
    }

    return;
  }

  if (lMeshesPathHands.isEmpty())
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
  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lUserFilters{Utils::GetFiltersForExport(this->mFiltersList, lFiltersListChooser->itemText(lFiltersListChooser->currentIndex()), this->mTargetBodyMesh, this->mTargetFeetMesh)};

  if (!Utils::CreateXMLFile(lEntryDirectory, lGenerateFilesInExistingMainDirectory, lOSPXMLNames, lMustUseBeastHands, this->mTargetBodyMesh, this->mTargetFeetMesh, lBodyslideSlidersetsNames, lUserFilters, false))
  {
    // Remove the directory since the generation is incomplete
    if (!lGenerateFilesInExistingMainDirectory)
    {
      Utils::RemoveDirectoryAndSubDirs(lEntryDirectory);
    }

    return;
  }

  // OSP file
  if (!Utils::CreateOSPFile(lEntryDirectory, lGenerateFilesInExistingMainDirectory, lOSPXMLNames, lMustUseBeastHands, this->mTargetBodyMesh, this->mTargetFeetMesh, lBodyslideSlidersetsNames, lMeshesPathBody, lMeshesPathFeet, lMeshesPathHands, lBodyName, lFeetName, lHandsName, false))
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
    // Read location
    auto lSourceSkeletonFileName{this->findChild<QComboBox*>(QString("skeleton_chooser"))->currentText()};
    auto lSourceSkeletonReadPath{QString("%1assets/skeletons/%2").arg(Utils::GetAppDataPathFolder(), lSourceSkeletonFileName)};

    // Write location
    auto lDestinationSkeletonRelativePath{this->findChild<QLineEdit*>(QString("skeleton_path_directory"))->text().trimmed()};
    Utils::CleanPathString(lDestinationSkeletonRelativePath);
    auto lDestinationSkeletonFileName{this->findChild<QLineEdit*>(QString("skeleton_name"))->text()};

    if (!Utils::CreateSkeletonFile(lSourceSkeletonReadPath, lEntryDirectory, lDestinationSkeletonRelativePath, lDestinationSkeletonFileName))
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

  // Open the directory where the file structure has been created
  if (this->mSettings.presetCreator.automaticallyOpenFinalDirectory)
  {
    Utils::DisplayInfoMessage(this, lTitle, lMessage, "icons", "green-info", tr("Open the generated directory"));
    QDesktopServices::openUrl(QUrl::fromLocalFile(lEntryDirectory));
  }
  else if (Utils::DisplayQuestionMessage(this, lTitle, lMessage, "icons", "green-info", tr("Open the generated directory"), tr("OK"), "", "", "", "", false) == ButtonClicked::YES)
  {
    QDesktopServices::openUrl(QUrl::fromLocalFile(lEntryDirectory));
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
  this->updateBeastHandsCheckboxState();

  // Refresh the preview of the body meshes parts
  this->updateMeshesPreview();

  // Refresh the names in the bodyslide application
  auto lBodyslideSlidersetsNames{this->findChild<QLineEdit*>(QString("names_bodyslide_input"))->text().trimmed()};
  this->updateBodyslideNamesPreview(lBodyslideSlidersetsNames);
}

void PresetCreator::openTargetMeshesPicker()
{
  auto lDialog{new TargetMeshesPicker(this, this->mSettings, this->mTargetBodyMesh, this->mTargetFeetMesh)};
  this->connect(lDialog, &TargetMeshesPicker::valuesChosen, this, &PresetCreator::targetMeshesChanged);
}

void PresetCreator::targetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet)
{
  // Update the class members
  this->mTargetBodyMesh = aBody;
  this->mTargetFeetMesh = aFeet;

  // Update the "targeted body mesh" text content
  const auto lBodyText{
    QString("%1 [v.%2]").arg(DataLists::GetBodyVariantsList(DataLists::GetName(aBody), DataLists::GetVersionIndex(aBody)).at(DataLists::GetVariantIndex(aBody)), DataLists::GetVersionString(aBody))};

  // Update the "targeted feet mesh" text content
  const auto lFeetText{
    QString("%1 [v.%2]").arg(DataLists::GetFeetVariantsList(DataLists::GetName(aFeet)).at(DataLists::GetVariantIndex(aFeet)), DataLists::GetVersionString(aBody, aFeet))};

  auto lCurrentlyTargetedBody{this->findChild<QLabel*>("currently_targeted_body_feet")};
  lCurrentlyTargetedBody->setText(tr("Targeted body: %1\nTargeted feet: %2").arg(lBodyText, lFeetText));

  // Force the refresh of preview fields
  this->refreshAllPreviewFields();

  // Force the refresh of the filters list preview
  this->updateBodySlideFiltersListPreview();
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
  // Get the GUI widgets
  auto lFiltersListChooser{this->findChild<QComboBox*>(QString("bodyslide_filters_chooser"))};
  auto lFiltersList{this->findChild<QLabel*>(QString("bodyslide_filters"))};

  // Get any eventual additional filters
  auto lAdditionalFilter{Utils::GetAdditionalFeetFilter(this->mTargetBodyMesh, this->mTargetFeetMesh)};
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
