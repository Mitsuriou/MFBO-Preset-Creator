#include "BatchConversionPicker.h"
#include "BCDropWidget.h"
#include "ComponentFactory.h"
#include "Utils.h"
#include <QCloseEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QSpinBox>
#include <QSplitter>

BatchConversionPicker::BatchConversionPicker(QWidget* aParent, const Struct::Settings& aSettings, const Struct::BatchConversionData& aData)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
  , mMinimumFirstColumnWidth(200)
  , mData(aData)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  this->displayLeftList();

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.batchConversionPickerDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void BatchConversionPicker::closeEvent(QCloseEvent* aEvent)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the batch conversion: results picker window"),
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
}

void BatchConversionPicker::reject()
{
  this->close();
}

void BatchConversionPicker::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Batch Conversion: Results picker"));
  this->setWindowIcon(QIcon(QPixmap(":/black/reorder")));
  this->setAcceptDrops(true); // Authorize drag&drop
}

void BatchConversionPicker::initializeGUI()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(this, false)};

  // 3 columns splitter
  auto lSplitter{new QSplitter(Qt::Orientation::Horizontal, this)};
  lMainLayout->setMargin(0);
  lMainLayout->addWidget(lSplitter, 0, 0);

  /*===========*/
  /* Left list */
  /*===========*/
  // Layout
  auto lLeftLayout{new QVBoxLayout(this)};
  lLeftLayout->setAlignment(Qt::AlignTop);

  // Wrapper widget
  auto lLeftWrapper{new QWidget(this)};
  lLeftWrapper->setLayout(lLeftLayout);
  lSplitter->addWidget(lLeftWrapper);

  // Title
  auto lLeftTitle{new QLabel(tr("Origin mod(s)' directory(ies) (click to display the data)"))};
  lLeftTitle->setAlignment(Qt::AlignCenter);
  lLeftLayout->addWidget(lLeftTitle);

  // Paths list
  auto lLeftList{new QListWidget(this)};
  lLeftList->setObjectName(QString("left_list"));
  lLeftLayout->addWidget(lLeftList);

  /*=============*/
  /* Middle list */
  /*=============*/
  // Layout
  auto lMiddleLayout{new QVBoxLayout(this)};
  lMiddleLayout->setAlignment(Qt::AlignTop);

  // Wrapper widget
  auto lMiddleWrapper{new QWidget(this)};
  lMiddleWrapper->setLayout(lMiddleLayout);
  lSplitter->addWidget(lMiddleWrapper);

  // Title
  auto lMiddleTitle{new QLabel(tr("Available data (drag these entries)"))};
  lMiddleTitle->setAlignment(Qt::AlignCenter);
  lMiddleLayout->addWidget(lMiddleTitle);

  // Label for the "no data available" case
  auto lNoDataLabel{new QLabel(tr("No data available for the selected origin directory"), this)};
  lNoDataLabel->setObjectName(QString("no_data_label"));
  lNoDataLabel->hide();
  lMiddleLayout->addWidget(lNoDataLabel);

  // Create the middle list scroll area + list which will contain all the available data entries
  ComponentFactory::CreateScrollAreaWindowLayout(this, true, false, lMiddleLayout, "middle_list_scrollable_zone", QMargins(0, 0, 0, 0), "middle_list");

  // Quick preset creation button
  auto lQuickPresetCreationButton{ComponentFactory::CreateButton(this, tr("Quick preset(s) creation"), "", "bolt", lIconFolder, "quick_preset_creation", true, true)};
  lMiddleLayout->addWidget(lQuickPresetCreationButton);

  /*============*/
  /* Right list */
  /*============*/
  // Nested layouts
  auto lRightDataLayout{new QVBoxLayout(this)};
  lRightDataLayout->setAlignment(Qt::AlignTop);

  auto lRightNavigationLayout{new QGridLayout(this)};
  lRightNavigationLayout->setColumnStretch(0, 1);
  lRightNavigationLayout->setColumnStretch(1, 0);
  lRightNavigationLayout->setColumnStretch(2, 1);
  lRightNavigationLayout->setColumnStretch(3, 1);
  lRightNavigationLayout->setColumnStretch(4, 1);

  // Common layout
  auto lCommonRightLayout{new QVBoxLayout(this)};
  lCommonRightLayout->setStretch(0, 1);
  lCommonRightLayout->setStretch(1, 0);
  lCommonRightLayout->addLayout(lRightDataLayout);
  lCommonRightLayout->addLayout(lRightNavigationLayout);

  // Wrapper widget
  auto lRightWrapper{new QWidget(this)};
  lRightWrapper->setLayout(lCommonRightLayout);
  lSplitter->addWidget(lRightWrapper);

  // Title
  auto lRightTitle{new QLabel(tr("Presets to generate (drop the entries here)"))};
  lRightTitle->setAlignment(Qt::AlignCenter);
  lRightDataLayout->addWidget(lRightTitle);

  // Label for the "no preset" case
  auto lNoPresetLabel{new QLabel(tr("No preset created at the moment."), this)};
  lNoPresetLabel->setObjectName(QString("no_preset_label"));
  lRightDataLayout->addWidget(lNoPresetLabel);

  // Body drop widget
  auto lDropSectionBody{new BCGroupWidget(this, this->mSettings, tr("Body"), "body", BCGroupWidgetCallContext::BODY)};
  lDropSectionBody->setObjectName(QString("drop_section_body"));
  lRightDataLayout->addWidget(lDropSectionBody);

  // Feet drop widget
  auto lDropSectionFeet{new BCGroupWidget(this, this->mSettings, tr("Feet"), "foot", BCGroupWidgetCallContext::FEET)};
  lDropSectionFeet->setObjectName(QString("drop_section_feet"));
  lRightDataLayout->addWidget(lDropSectionFeet);

  // Hands drop widget
  auto lDropSectionHands{new BCGroupWidget(this, this->mSettings, tr("Hands"), "hand", BCGroupWidgetCallContext::HANDS)};
  lDropSectionHands->setObjectName(QString("drop_section_hands"));
  lRightDataLayout->addWidget(lDropSectionHands);

  // Hands drop widget
  auto lDropSectionSkeleton{new BCGroupWidget(this, this->mSettings, tr("Skeleton"), "skeleton", BCGroupWidgetCallContext::SKELETON)};
  lDropSectionSkeleton->setObjectName(QString("drop_section_skeleton"));
  lRightDataLayout->addWidget(lDropSectionSkeleton);

  // BodySlide output settings group box
  auto lBodyslideGroupBox{new QGroupBox(tr("BodySlide output").append("  "), this)};
  lBodyslideGroupBox->setObjectName(QString("bodyslide_groupbox"));
  Utils::AddIconToGroupBox(lBodyslideGroupBox, lIconFolder, "bodyslide-logo", this->mSettings.font.size);
  this->connect(lBodyslideGroupBox, &QGroupBox::toggled, this, &BatchConversionPicker::groupBoxChecked);
  Utils::SetGroupBoxState(lBodyslideGroupBox, false);
  lRightDataLayout->addWidget(lBodyslideGroupBox);

  // Grid layout
  auto lBodyslideGridLayout{new QGridLayout(lBodyslideGroupBox)};
  lBodyslideGridLayout->setColumnStretch(0, 0);
  lBodyslideGridLayout->setColumnStretch(1, 1);
  lBodyslideGridLayout->setSpacing(10);
  lBodyslideGridLayout->setContentsMargins(15, 20, 15, 15);
  lBodyslideGridLayout->setAlignment(Qt::AlignTop);
  lBodyslideGridLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // First line
  lBodyslideGridLayout->addWidget(new QLabel(tr("BodySlide files names:"), this), 1, 0);

  auto lOSPXMLNamesLineEdit{new QLineEdit(this)};
  lOSPXMLNamesLineEdit->setObjectName(QString("names_osp_xml_input"));
  lBodyslideGridLayout->addWidget(lOSPXMLNamesLineEdit, 1, 1, 1, 4);

  // Second line
  lBodyslideGridLayout->addWidget(new QLabel(tr("Preview:"), this), 2, 0);

  auto lPathsNamesOspXmlNames{new QLabel("", this)};
  lPathsNamesOspXmlNames->setObjectName(QString("names_osp_xml_preview"));
  lPathsNamesOspXmlNames->setAutoFillBackground(true);
  lBodyslideGridLayout->addWidget(lPathsNamesOspXmlNames, 2, 1, 1, 4);

  // Third line
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
  lBodyslideGridLayout->addWidget(lNamesInAppLineEdit, 3, 1, 1, 4);

  // Fourth line
  lBodyslideGridLayout->addWidget(new QLabel(tr("Preview:"), this), 4, 0);

  auto lResultNamesInApp{new QLabel("", this)};
  lResultNamesInApp->setObjectName(QString("names_bodyslide_preview"));
  lBodyslideGridLayout->addWidget(lResultNamesInApp, 4, 1, 1, 4);

  /*==============================*/
  /* Right list: Presets controls */
  /*==============================*/
  // Previous preset
  auto lPreviousPreset{ComponentFactory::CreateButton(this, tr("Previous preset"), "", "arrow-left", lIconFolder, "previous_preset", false, true)};
  lRightNavigationLayout->addWidget(lPreviousPreset, 0, 0);

  // Active preset number
  auto lActivePresetNumber{new QSpinBox(this)};
  lActivePresetNumber->setObjectName(QString("active_preset_number"));
  lRightNavigationLayout->addWidget(lActivePresetNumber, 0, 1);

  // Next preset
  auto lNextPreset{ComponentFactory::CreateButton(this, tr("Next preset"), "", "arrow-right", lIconFolder, "next_preset", false, true)};
  lRightNavigationLayout->addWidget(lNextPreset, 0, 2);

  // Remove current preset
  auto lRemoveActivePreset{ComponentFactory::CreateButton(this, tr("Remove current preset"), "", "minus", lIconFolder, "remove_current_preset", false, true)};
  lRightNavigationLayout->addWidget(lRemoveActivePreset, 0, 3);

  // Add new empty preset
  auto lAddEmptyPreset{ComponentFactory::CreateButton(this, tr("Add new preset"), "", "plus", lIconFolder, "add_empty_preset", false, true)};
  lRightNavigationLayout->addWidget(lAddEmptyPreset, 0, 4);

  /*========================================*/
  /* Validate selection and generate button */
  /*========================================*/
  auto lButtonLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};

  auto lGenerateButton{ComponentFactory::CreateButton(this, tr("Batch generate the files on my computer"), "", "build", lIconFolder, "generate")};
  lGenerateButton->setAutoDefault(true);
  lGenerateButton->setDefault(true);
  lButtonLayout->addWidget(lGenerateButton);

  // Pre-bind initialization functions
  this->updateOSPXMLPreview(QString());
  this->updateBodyslideNamesPreview(QString());
  this->updatePresetInterfaceState(0);

  // Event binding
  this->connectGroupWidgetEvents(lDropSectionBody);
  this->connectGroupWidgetEvents(lDropSectionFeet);
  this->connectGroupWidgetEvents(lDropSectionHands);
  this->connect(lDropSectionHands, &BCGroupWidget::checkBoxStateChangedTriggered, this, &BatchConversionPicker::handsCheckBoxStateChanged);
  this->connectGroupWidgetEvents(lDropSectionSkeleton);
  this->connect(lDropSectionSkeleton, &BCGroupWidget::checkBoxStateChangedTriggered, this, &BatchConversionPicker::skeletonCheckBoxStateChanged);

  this->connect(lOSPXMLNamesLineEdit, &QLineEdit::textChanged, this, &BatchConversionPicker::updateOSPXMLPreview);
  this->connect(lNamesInAppLineEdit, &QLineEdit::textChanged, this, &BatchConversionPicker::updateBodyslideNamesPreview);
  this->connect(lGenerateButton, &QPushButton::clicked, this, &BatchConversionPicker::validateSelection);
  this->connect(lLeftList, &QListWidget::itemSelectionChanged, this, &BatchConversionPicker::refreshMiddleList);
  this->connect(lQuickPresetCreationButton, &QPushButton::clicked, this, &BatchConversionPicker::quickCreatePreset);

  this->connect(lPreviousPreset, &QPushButton::clicked, this, &BatchConversionPicker::goToPreviousPreset);
  this->connect(lNextPreset, &QPushButton::clicked, this, &BatchConversionPicker::goToNextPreset);
  this->connect(lActivePresetNumber, QOverload<int>::of(&QSpinBox::valueChanged), this, &BatchConversionPicker::updatePresetInterfaceState);
  this->connect(lRemoveActivePreset, &QPushButton::clicked, this, &BatchConversionPicker::removeActivePreset);
  this->connect(lAddEmptyPreset, &QPushButton::clicked, this, &BatchConversionPicker::addNewEmptyPreset);

  // Post-bind initialization functions
  lLeftList->setCurrentRow(0);
  mPreventPresetSave = false;
}

void BatchConversionPicker::connectGroupWidgetEvents(BCGroupWidget* lGroupWidget)
{
  this->connect(lGroupWidget, &BCGroupWidget::removePressed, this, &BatchConversionPicker::addDataToActiveMiddleList);
  this->connect(lGroupWidget, &BCGroupWidget::duplicatePressed, this, &BatchConversionPicker::addDataToActiveMiddleList);
  this->connect(lGroupWidget, &BCGroupWidget::dropEventTriggered, this, &BatchConversionPicker::removeDataFromActiveMiddleList);
}

void BatchConversionPicker::displayLeftList()
{
  auto lPathsList{this->findChild<QListWidget*>(QString("left_list"))};
  for (const auto& lEntry : this->mData.scannedData)
  {
    lPathsList->addItem(lEntry.first);
  }

  // Focus the first line of the list
  lPathsList->setAlternatingRowColors(true);
  lPathsList->setCurrentRow(0);
  lPathsList->setFocus();
}

void BatchConversionPicker::refreshMiddleList()
{
  // Delete all children of the middle list
  auto lOptionsList{this->findChild<QGridLayout*>(QString("middle_list"))};

  const auto lButtonsListSize{static_cast<int>(this->mMiddleListButtons.size())};
  for (int i = 0; i < lButtonsListSize; i++)
  {
    delete this->mMiddleListButtons.at(0);
    this->mMiddleListButtons.erase(this->mMiddleListButtons.begin() + 0);
  }

  // Add the entries in the options list, based on the paths list' selected item
  auto lPathsList{this->findChild<QListWidget*>(QString("left_list"))};
  auto lSelectedEntry{lPathsList->currentItem()};
  if (lSelectedEntry != nullptr)
  {
    auto lQuickPresetCreationButton{this->findChild<QPushButton*>(QString("quick_preset_creation"))};
    auto lNoDataLabel{this->findChild<QLabel*>(QString("no_data_label"))};
    auto lPosition{this->mData.scannedData.find(lSelectedEntry->text())};

    if (lPosition != this->mData.scannedData.end())
    {
      // Check how many entries are still available for the selected origin folder
      if (lPosition->second.size() == 0)
      {
        lNoDataLabel->show();
        lQuickPresetCreationButton->setDisabled(true);
      }
      else
      {
        lNoDataLabel->hide();

        // Create the BCDragWidget
        for (const auto& lValue : lPosition->second)
        {
          auto lDraggableWidget{new BCDragWidget(this, this->mSettings, lPosition->first, lValue)};
          this->mMiddleListButtons.push_back(lDraggableWidget);
          lOptionsList->addWidget(lDraggableWidget);
        }

        lQuickPresetCreationButton->setDisabled(!Utils::ContainsBodyOrHandsOrFeetMesh<std::vector<QString>>(lPosition->second));
      }
    }
  }

  // TODO: change the color of the left list if there is not any data available in the middle list anymore
  // TODO: make the current left list index disabled if there is not any data available in the middle list anymore
}

void BatchConversionPicker::updateOSPXMLPreview(QString aText)
{
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

  // Save the data in the preset
  if (!this->mPreventPresetSave)
  {
    this->saveBodySlideDataToPreset();
  }
}

void BatchConversionPicker::updateBodyslideNamesPreview(QString aText)
{
  //auto lMustUseBeastHands{this->findChild<QCheckBox*>(QString("use_beast_hands"))->isChecked()};
  auto lMustUseBeastHands{false}; // TODO: Check if it is necessary to get this parameter from a dynamic analyze
  auto lIsValidPath{true};

  Utils::CleanPathString(aText);

  if (aText.trimmed().isEmpty())
  {
    aText = QString("*");
    lIsValidPath = false;
  }

  auto lConstructedPreviewText{QString()};
  lConstructedPreviewText.append(Utils::GetBodySliderValue(this->mData.bodyMod));                           // Body
  lConstructedPreviewText.append(Utils::GetFeetSliderValue(this->mData.bodyMod, this->mData.feetModIndex)); // Feet
  lConstructedPreviewText.append(Utils::GetHandsSliderValue(this->mData.bodyMod, lMustUseBeastHands));      // Hands
  lConstructedPreviewText = lConstructedPreviewText.arg(aText);

  auto lNewTextColor{this->mSettings.successColor};

  if (!lIsValidPath)
  {
    lNewTextColor = this->mSettings.dangerColor;
  }

  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("names_bodyslide_preview"))};
  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lConstructedPreviewText);

  // Save the data in the preset
  if (!this->mPreventPresetSave)
  {
    this->saveBodySlideDataToPreset();
  }
}

void BatchConversionPicker::removeDataFromActiveMiddleList(const QString& aOriginFolder, const QString& aRessourcePath, const bool isCheckBoxChecked)
{
  // Search to remove the used entry from the available data to make presets
  auto lPosition{this->mData.scannedData.find(aOriginFolder)};
  if (lPosition != this->mData.scannedData.end())
  {
    // Search the entry
    auto lIterator{lPosition->second.begin()};
    for (const auto& lValue : lPosition->second)
    {
      // Delete the entry
      if (lValue == aRessourcePath)
      {
        lPosition->second.erase(lIterator);
        break;
      }

      ++lIterator;
    }
  }

  // Finally, refresh the middle list with the entry removed
  refreshMiddleList();

  // Track the changes in the output data
  auto lDropSectionBody{this->findChild<BCGroupWidget*>(QString("drop_section_body"))};
  auto lDropSectionFeet{this->findChild<BCGroupWidget*>(QString("drop_section_feet"))};
  auto lDropSectionHands{this->findChild<BCGroupWidget*>(QString("drop_section_hands"))};
  auto lDropSectionSkeleton{this->findChild<BCGroupWidget*>(QString("drop_section_skeleton"))};

  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  auto& lPreset{this->mData.presets.at(static_cast<size_t>(lActivePresetNumber->value() - 1))};

  if (this->sender() == lDropSectionBody)
  {
    lPreset.setBodyData(aOriginFolder, aRessourcePath);
  }
  else if (this->sender() == lDropSectionFeet)
  {
    lPreset.setFeetData(aOriginFolder, aRessourcePath);
  }
  else if (this->sender() == lDropSectionHands)
  {
    lPreset.setHandsData(aOriginFolder, aRessourcePath, isCheckBoxChecked);
  }
  else if (this->sender() == lDropSectionSkeleton)
  {
    lPreset.setSkeletonData(aOriginFolder, aRessourcePath, isCheckBoxChecked);
  }
}

void BatchConversionPicker::addDataToActiveMiddleList(const QString& aOriginFolder, const QString& aRessourcePath)
{
  // Search to remove the used entry from the available data to make presets
  auto lPosition{this->mData.scannedData.find(aOriginFolder)};
  if (lPosition != this->mData.scannedData.end())
  {
    // Re-insert the wanted entry
    lPosition->second.push_back(aRessourcePath);
  }

  // Finally, refresh the middle list with the entry added
  refreshMiddleList();

  // Track the changes in the output data
  if (!this->mPreventPresetSave)
  {
    auto lDropSectionBody{this->findChild<BCGroupWidget*>(QString("drop_section_body"))};
    auto lDropSectionFeet{this->findChild<BCGroupWidget*>(QString("drop_section_feet"))};
    auto lDropSectionHands{this->findChild<BCGroupWidget*>(QString("drop_section_hands"))};
    auto lDropSectionSkeleton{this->findChild<BCGroupWidget*>(QString("drop_section_skeleton"))};

    auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
    auto& lPreset{this->mData.presets.at(static_cast<size_t>(lActivePresetNumber->value() - 1))};

    if (this->sender() == lDropSectionBody)
    {
      lPreset.resetBodyData();
    }
    else if (this->sender() == lDropSectionFeet)
    {
      lPreset.resetFeetData();
    }
    else if (this->sender() == lDropSectionHands)
    {
      lPreset.resetHandsData();
    }
    else if (this->sender() == lDropSectionSkeleton)
    {
      lPreset.resetSkeletonData();
    }
  }
}

void BatchConversionPicker::handsCheckBoxStateChanged(const bool aIsActive)
{
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  if (lActivePresetNumber->value() == 0)
    return;

  this->mData.presets.at(static_cast<size_t>(lActivePresetNumber->value() - 1)).setHandsUseAlternativeModel(aIsActive);
}

void BatchConversionPicker::skeletonCheckBoxStateChanged(const bool aIsActive)
{
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  if (lActivePresetNumber->value() == 0)
    return;

  this->mData.presets.at(static_cast<size_t>(lActivePresetNumber->value() - 1)).setSkeletonUseAlternativeModel(aIsActive);
}

void BatchConversionPicker::saveBodySlideDataToPreset()
{
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  if (lActivePresetNumber->value() == 0)
    return;

  // Read the data in the GUI
  auto lOSPXMLNamesValue{this->findChild<QLineEdit*>(QString("names_osp_xml_input"))->text()};
  auto lNamesInAppValue{this->findChild<QLineEdit*>(QString("names_bodyslide_input"))->text()};

  // Update the current preset
  this->mData.presets.at(static_cast<size_t>(lActivePresetNumber->value() - 1)).setNames(lOSPXMLNamesValue, lNamesInAppValue);
}

void BatchConversionPicker::goToPreviousPreset() const
{
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  lActivePresetNumber->setValue(static_cast<size_t>(lActivePresetNumber->value() - 1));
}

void BatchConversionPicker::goToNextPreset() const
{
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  lActivePresetNumber->setValue(lActivePresetNumber->value() + 1);
}

void BatchConversionPicker::removeActivePreset()
{
  // Re-put the data in the scannedData list
  this->mPreventPresetSave = true;
  this->findChild<BCGroupWidget*>(QString("drop_section_body"))->removeData();
  this->findChild<BCGroupWidget*>(QString("drop_section_feet"))->removeData();
  this->findChild<BCGroupWidget*>(QString("drop_section_hands"))->removeData();
  this->findChild<BCGroupWidget*>(QString("drop_section_skeleton"))->removeData();
  this->mPreventPresetSave = false;

  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  auto lCurrentIndex{lActivePresetNumber->value()};

  // Remove the preset entry
  this->mData.presets.erase(this->mData.presets.begin() + lCurrentIndex - 1);

  // Update the preset interface block
  auto lNumberOfPresets{static_cast<int>(this->mData.presets.size())};
  this->updatePresetInterfaceState(lNumberOfPresets);

  // Display the new focused preset
  auto lNextIndex{lCurrentIndex > lNumberOfPresets ? lNumberOfPresets : lCurrentIndex};
  lActivePresetNumber->setValue(lNextIndex);
}

void BatchConversionPicker::addNewEmptyPreset()
{
  // Push a default object
  this->mData.presets.push_back(Struct::BatchConversionPresetData());

  // Update the spin box data
  this->updateActivePresetNumberSpinBox();

  // Display the last preset
  auto lNumberOfPresets{static_cast<int>(this->mData.presets.size())};

  // Display the new created preset
  this->findChild<QSpinBox*>(QString("active_preset_number"))->setValue(lNumberOfPresets);
}

void BatchConversionPicker::updatePresetInterfaceState(const int aNextIndex)
{
  auto lNumberOfPresets{static_cast<int>(this->mData.presets.size())};

  // Previous preset
  auto lPreviousPreset{this->findChild<QPushButton*>(QString("previous_preset"))};
  auto lWasPreviousPresetFocused{lPreviousPreset->hasFocus()};
  lPreviousPreset->setDisabled(lNumberOfPresets == 0 || aNextIndex <= 1);

  // Update active preset number spinbox
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  if (qobject_cast<QSpinBox*>(this->sender()) != lActivePresetNumber)
  {
    this->updateActivePresetNumberSpinBox();
  }

  // Next preset
  auto lNextPreset{this->findChild<QPushButton*>(QString("next_preset"))};
  auto lWasNextPresetFocused{lNextPreset->hasFocus()};
  lNextPreset->setDisabled(lNumberOfPresets == 0 || aNextIndex >= lNumberOfPresets);

  // Remove current preset
  this->findChild<QPushButton*>(QString("remove_current_preset"))->setDisabled(lNumberOfPresets == 0);

  // Swap the focus between the previous and next buttons
  if (lWasPreviousPresetFocused && !lPreviousPreset->isEnabled())
  {
    lNextPreset->setFocus();
  }
  else if (lWasNextPresetFocused && !lNextPreset->isEnabled())
  {
    lPreviousPreset->setFocus();
  }

  // Drop widgets: Body, Feet, Hands
  auto lNoPresetLabel{this->findChild<QLabel*>(QString("no_preset_label"))};
  lNoPresetLabel->setHidden(lNumberOfPresets > 0);

  auto lDropSectionBody{this->findChild<BCGroupWidget*>(QString("drop_section_body"))};
  lDropSectionBody->setHidden(lNumberOfPresets == 0);
  auto lDropSectionFeet{this->findChild<BCGroupWidget*>(QString("drop_section_feet"))};
  lDropSectionFeet->setHidden(lNumberOfPresets == 0);
  auto lDropSectionHands{this->findChild<BCGroupWidget*>(QString("drop_section_hands"))};
  lDropSectionHands->setHidden(lNumberOfPresets == 0);
  auto lDropSectionSkeleton{this->findChild<BCGroupWidget*>(QString("drop_section_skeleton"))};
  lDropSectionSkeleton->setHidden(lNumberOfPresets == 0);

  if (lNumberOfPresets > 0)
  {
    const auto& lDataToSet = this->mData.presets.at(static_cast<size_t>(aNextIndex - 1));
    lDropSectionBody->setData(lDataToSet);
    lDropSectionFeet->setData(lDataToSet);
    lDropSectionHands->setData(lDataToSet);
    lDropSectionSkeleton->setData(lDataToSet);
  }

  // BodySlide data
  auto lBodyslideGroupBox{this->findChild<QGroupBox*>(QString("bodyslide_groupbox"))};
  lBodyslideGroupBox->setHidden(lNumberOfPresets == 0);

  if (lNumberOfPresets > 0)
  {
    auto lOSPXMLNamesLineEdit{this->findChild<QLineEdit*>(QString("names_osp_xml_input"))};
    auto lNamesInAppLineEdit{this->findChild<QLineEdit*>(QString("names_bodyslide_input"))};

    mPreventPresetSave = true;
    const auto& lPathsPair = this->mData.presets.at(static_cast<size_t>(aNextIndex - 1)).getNames();
    lOSPXMLNamesLineEdit->setText(lPathsPair.first);
    lNamesInAppLineEdit->setText(lPathsPair.second);
    mPreventPresetSave = false;
  }
}

void BatchConversionPicker::updateActivePresetNumberSpinBox()
{
  auto lNumberOfPresets{static_cast<int>(this->mData.presets.size())};

  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  lActivePresetNumber->setDisabled(lNumberOfPresets == 0);
  lActivePresetNumber->setMinimum(lNumberOfPresets == 0 ? 0 : 1);
  lActivePresetNumber->setMaximum(lNumberOfPresets);
  lActivePresetNumber->setSuffix(QString(" / %1").arg(lNumberOfPresets));
}

void BatchConversionPicker::quickCreatePreset()
{
  // Analyze the left data to split between everything
  std::map<QString, std::set<QString>> lDetectedPresets;

  auto lPathsList{this->findChild<QListWidget*>(QString("left_list"))};
  auto lSelectedEntry{lPathsList->currentItem()};
  if (lSelectedEntry != nullptr)
  {
    auto lPosition{this->mData.scannedData.find(lSelectedEntry->text())};

    if (lPosition != this->mData.scannedData.end())
    {
      // Create the BCDragWidget
      for (const auto& lValue : lPosition->second)
      {
        auto lLastSlashPosition{lValue.lastIndexOf('/')};
        auto lKey{lValue.left(lLastSlashPosition)};
        auto lMeshName{lValue.mid(lLastSlashPosition + 1)};

        // The key does not exist
        if (lDetectedPresets.find(lKey) == lDetectedPresets.end())
        {
          lDetectedPresets.insert({lKey, {lMeshName}});
        }
        else
        {
          lDetectedPresets.at(lKey).insert(lMeshName);
        }
      }
    }
  }

  // Clear the presets that do not have any useful entries
  Utils::ClearUselessEntries(lDetectedPresets);

  // Warn the user that no preset could be made from left data
  if (lDetectedPresets.size() == 0)
  {
    Utils::DisplayWarningMessage(tr("Error: No preset could be made from available data."));
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Ask the user if they want to create the detected possible presets
  if (Utils::DisplayQuestionMessage(this,
                                    tr("Create %1 presets?").arg(lDetectedPresets.size()),
                                    tr("Do you want to create %1 new preset(s)?").arg(lDetectedPresets.size()),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Create the presets"),
                                    tr("Cancel the creation"),
                                    "",
                                    this->mSettings.warningColor,
                                    this->mSettings.successColor,
                                    "",
                                    false)
      == ButtonClicked::YES)
  {
    // TODO: Check if any already existing preset can be completed with left data

    // Iterate the list of new presets that need to be created
    for (const auto& lPreset : lDetectedPresets)
    {
      // Create a new empty preset for each newly created preset
      this->addNewEmptyPreset();

      // Iterate the meshes names
      for (const auto& lMeshPart : lPreset.second)
      {
        // Simulate the drag&drop of the ressources
        auto lRessourcePath{lPreset.first + "/" + lMeshPart};
        auto lRessourceType{Utils::GetMeshTypeFromFileName(lRessourcePath)};
        BCDropWidget* lTarget{nullptr};

        switch (lRessourceType)
        {
          case BCGroupWidgetCallContext::BODY:
            lTarget = this->findChild<BCGroupWidget*>(QString("drop_section_body"))->findChild<BCDropWidget*>(QString("drop_widget"));
            break;
          case BCGroupWidgetCallContext::FEET:
            lTarget = this->findChild<BCGroupWidget*>(QString("drop_section_feet"))->findChild<BCDropWidget*>(QString("drop_widget"));
            break;
          case BCGroupWidgetCallContext::HANDS:
            lTarget = this->findChild<BCGroupWidget*>(QString("drop_section_hands"))->findChild<BCDropWidget*>(QString("drop_widget"));
            break;
          case BCGroupWidgetCallContext::SKELETON:
            lTarget = this->findChild<BCGroupWidget*>(QString("drop_section_skeleton"))->findChild<BCDropWidget*>(QString("drop_widget"));
            break;
          default:
            continue;
        }

        if (lTarget != nullptr)
        {
          lTarget->simulateDropEvent(lSelectedEntry->text(), lRessourcePath);
        }
      }
    }
  }
}

void BatchConversionPicker::validateSelection()
{
  // TODO: Check to restore this line, after the TODO below
  //this->mData.scannedData.clear(); // Clear the data to send a skinier object

  auto lRemovedPresetsCount{0};
  for (int i = 0; i < this->mData.presets.size(); i++)
  {
    if (!this->mData.presets.at(i).isValid())
    {
      this->mData.presets.erase(this->mData.presets.begin() + i);
      // TODO: Avoid removing the preset directly. Ask the user if they want to continue this way (removed) or they want to fix the issues
      lRemovedPresetsCount++;
      i--;
    }
  }

  // TODO: Tell the user that some presets were not valid

  // TODO: Emit the signal with the mData object as unique parameter
  emit presetsCreationValidated(this->mData);

  this->accept();
}

void BatchConversionPicker::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::SetGroupBoxState(lGroupBox, !aIsChecked);
}
