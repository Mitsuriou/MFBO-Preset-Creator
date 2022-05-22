#include "BatchConversionPicker.h"
#include "BCDropWidget.h"
#include "ComponentFactory.h"
#include "Utils.h"
#include <QCloseEvent>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QSpinBox>
#include <QSplitter>

BatchConversionPicker::BatchConversionPicker(QWidget* aParent,
                                             const Struct::Settings& aSettings,
                                             std::map<QString, QString>* aLastPaths,
                                             const Struct::BatchConversionData& aData)
  : TitleDialog(aParent, tr("Batch Conversion: Results picker"), "reorder", aSettings, aLastPaths, 700)
  , mData(aData)
{
  // Authorize drag&drop
  this->setAcceptDrops(true);

  // Build the window's interface
  this->initializeGUI();

  this->displayLeftList();

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.display.batchConversionPickerDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void BatchConversionPicker::closeEvent(QCloseEvent* aEvent)
{
  if (this->mData.presets.size() == 0
      || Utils::DisplayQuestionMessage(this,
                                       tr("Closing"),
                                       tr("Do you want to close the window?"),
                                       this->getThemedResourcePath(),
                                       "help-circle",
                                       tr("Close the window"),
                                       tr("Go back to the batch conversion: results picker window"),
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
}

void BatchConversionPicker::initializeGUI()
{
  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(this->getCentralWidget(), false)};

  // 3 columns splitter
  auto lSplitter{new QSplitter(Qt::Orientation::Horizontal, this)};
  lMainLayout->setContentsMargins(0, 0, 0, 0);
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
  lLeftTitle->setWordWrap(true);
  lLeftLayout->addWidget(lLeftTitle);

  // Paths list
  auto lLeftList{new QListWidget(this)};
  lLeftList->setObjectName(QString("left_list"));
  lLeftLayout->addWidget(lLeftList);

  // Full quick preset creation button
  auto lFullQuickPresetCreationButton{ComponentFactory::CreateButton(this,
                                                                     tr("Quick preset(s) creation (all mods)"),
                                                                     "",
                                                                     "bolt",
                                                                     this->getThemedResourcePath(),
                                                                     "full_quick_preset_creation",
                                                                     false,
                                                                     true)};
  lLeftLayout->addWidget(lFullQuickPresetCreationButton);

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
  lMiddleTitle->setWordWrap(true);
  lMiddleLayout->addWidget(lMiddleTitle);

  // Label for the "no data available" case
  auto lNoDataLabel{new QLabel(tr("No data available for the selected origin directory"), this)};
  lNoDataLabel->setObjectName(QString("no_data_label"));
  auto lItalicFont{lNoDataLabel->font()};
  lItalicFont.setItalic(true);
  lNoDataLabel->setFont(lItalicFont);
  lNoDataLabel->setAlignment(Qt::AlignCenter);
  lNoDataLabel->setWordWrap(true);
  lNoDataLabel->hide();
  lMiddleLayout->addWidget(lNoDataLabel);

  // Create the middle list scroll area + list which will contain all the available data entries
  ComponentFactory::CreateScrollAreaWindowLayout(this->getCentralWidget(),
                                                 true,
                                                 false,
                                                 lMiddleLayout,
                                                 "middle_list_scrollable_zone",
                                                 QMargins(0, 0, 0, 0),
                                                 "middle_list");

  // Quick preset creation button
  auto lSimpleQuickPresetCreationButton{ComponentFactory::CreateButton(this,
                                                                       tr("Quick preset(s) creation (active mod)"),
                                                                       "",
                                                                       "bolt",
                                                                       this->getThemedResourcePath(),
                                                                       "simple_quick_preset_creation",
                                                                       true,
                                                                       true)};
  lMiddleLayout->addWidget(lSimpleQuickPresetCreationButton);

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
  auto lDropSectionBody{new BCGroupWidget(this, this->settings(), tr("Body"), "body", BCGroupWidgetCallContext::BODY)};
  lDropSectionBody->setObjectName(QString("drop_section_body"));
  lRightDataLayout->addWidget(lDropSectionBody);

  // Feet drop widget
  auto lDropSectionFeet{new BCGroupWidget(this, this->settings(), tr("Feet"), "foot", BCGroupWidgetCallContext::FEET)};
  lDropSectionFeet->setObjectName(QString("drop_section_feet"));
  lRightDataLayout->addWidget(lDropSectionFeet);

  // Hands drop widget
  auto lDropSectionHands{new BCGroupWidget(this, this->settings(), tr("Hands"), "hand", BCGroupWidgetCallContext::HANDS)};
  lDropSectionHands->setObjectName(QString("drop_section_hands"));
  lRightDataLayout->addWidget(lDropSectionHands);

  // Hands drop widget
  auto lDropSectionSkeleton{new BCGroupWidget(this, this->settings(), tr("Skeleton"), "skeleton", BCGroupWidgetCallContext::SKELETON)};
  lDropSectionSkeleton->setObjectName(QString("drop_section_skeleton"));
  lRightDataLayout->addWidget(lDropSectionSkeleton);

  // BodySlide output settings group box
  auto lBodyslideGroupBox{ComponentFactory::CreateGroupBox(this,
                                                           tr("BodySlide output"),
                                                           "bodyslide-logo",
                                                           this->getThemedResourcePath(),
                                                           this->settings().display.font.pointSize,
                                                           "bodyslide_groupbox")};
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
                         .arg(this->getThemedResourcePath(), tr("Presets names:")));
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
  auto lPreviousPreset{ComponentFactory::CreateButton(this,
                                                      tr("Previous preset"),
                                                      "",
                                                      "arrow-left",
                                                      this->getThemedResourcePath(),
                                                      "previous_preset",
                                                      false,
                                                      true)};
  lRightNavigationLayout->addWidget(lPreviousPreset, 0, 0);

  // Active preset number
  auto lActivePresetNumber{new QSpinBox(this)};
  lActivePresetNumber->setObjectName(QString("active_preset_number"));
  lRightNavigationLayout->addWidget(lActivePresetNumber, 0, 1);

  // Next preset
  auto lNextPreset{ComponentFactory::CreateButton(this,
                                                  tr("Next preset"),
                                                  "",
                                                  "arrow-right",
                                                  this->getThemedResourcePath(),
                                                  "next_preset",
                                                  false,
                                                  true)};
  lRightNavigationLayout->addWidget(lNextPreset, 0, 2);

  // Remove current preset
  auto lRemoveActivePreset{ComponentFactory::CreateButton(this,
                                                          tr("Remove current preset"),
                                                          "",
                                                          "minus",
                                                          this->getThemedResourcePath(),
                                                          "remove_current_preset",
                                                          false,
                                                          true)};
  lRightNavigationLayout->addWidget(lRemoveActivePreset, 0, 3);

  // Add new empty preset
  auto lAddEmptyPreset{ComponentFactory::CreateButton(this,
                                                      tr("Add new preset"),
                                                      "",
                                                      "plus",
                                                      this->getThemedResourcePath(),
                                                      "add_empty_preset",
                                                      false,
                                                      true)};
  lRightNavigationLayout->addWidget(lAddEmptyPreset, 0, 4);

  /*========================================*/
  /* Validate selection and generate button */
  /*========================================*/
  const auto lButtonsLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};

  auto lGenerateButton{ComponentFactory::CreateButton(this,
                                                      tr("Batch generate the files on my computer"),
                                                      "",
                                                      "build",
                                                      this->getThemedResourcePath(),
                                                      "generate")};
  lGenerateButton->setAutoDefault(true);
  lGenerateButton->setDefault(true);
  lButtonsLayout->addWidget(lGenerateButton);

  // Pre-bind initialization functions
  this->updateOSPXMLPreview(QString());
  this->updateBodyslideNamesPreview(QString());
  this->updatePresetInterfaceState(0);

  // Event binding
  this->connectGroupWidgetEvents(lDropSectionBody);
  this->connectGroupWidgetEvents(lDropSectionFeet);
  this->connectGroupWidgetEvents(lDropSectionHands);
  QObject::connect(lDropSectionHands, &BCGroupWidget::checkBoxStateChangedTriggered, this, &BatchConversionPicker::handsCheckBoxStateChanged);
  this->connectGroupWidgetEvents(lDropSectionSkeleton);
  QObject::connect(lDropSectionSkeleton, &BCGroupWidget::checkBoxStateChangedTriggered, this, &BatchConversionPicker::skeletonCheckBoxStateChanged);

  QObject::connect(lOSPXMLNamesLineEdit, &QLineEdit::textChanged, this, &BatchConversionPicker::updateOSPXMLPreview);
  QObject::connect(lNamesInAppLineEdit, &QLineEdit::textChanged, this, &BatchConversionPicker::updateBodyslideNamesPreview);
  QObject::connect(lGenerateButton, &QPushButton::clicked, this, &BatchConversionPicker::validateSelection);
  QObject::connect(lLeftList, &QListWidget::itemSelectionChanged, this, &BatchConversionPicker::refreshMiddleList);
  QObject::connect(lFullQuickPresetCreationButton, &QPushButton::clicked, this, &BatchConversionPicker::fullQuickCreatePreset);
  QObject::connect(lSimpleQuickPresetCreationButton, &QPushButton::clicked, this, &BatchConversionPicker::simpleQuickCreatePreset);

  QObject::connect(lPreviousPreset, &QPushButton::clicked, this, &BatchConversionPicker::goToPreviousPreset);
  QObject::connect(lNextPreset, &QPushButton::clicked, this, &BatchConversionPicker::goToNextPreset);
  QObject::connect(lActivePresetNumber, QOverload<int>::of(&QSpinBox::valueChanged), this, &BatchConversionPicker::updatePresetInterfaceState);
  QObject::connect(lRemoveActivePreset, &QPushButton::clicked, this, &BatchConversionPicker::removeActivePreset);
  QObject::connect(lAddEmptyPreset, &QPushButton::clicked, this, &BatchConversionPicker::addNewEmptyPreset);

  // Post-bind initialization functions
  lLeftList->setCurrentRow(0);
  mPreventPresetSave = false;
}

void BatchConversionPicker::connectGroupWidgetEvents(BCGroupWidget* lGroupWidget)
{
  QObject::connect(lGroupWidget, &BCGroupWidget::removePressed, this, &BatchConversionPicker::addDataToActiveMiddleList);
  QObject::connect(lGroupWidget, &BCGroupWidget::duplicatePressed, this, &BatchConversionPicker::addDataToActiveMiddleList);
  QObject::connect(lGroupWidget, &BCGroupWidget::dropEventTriggered, this, &BatchConversionPicker::removeDataFromActiveMiddleList);
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

void BatchConversionPicker::refreshLeftListFont()
{
  const auto lPathsList{this->findChild<QListWidget*>(QString("left_list"))};

  for (int i = 0; i < lPathsList->count(); i++)
  {
    const auto lListItem{lPathsList->item(i)};
    const auto lPosition{this->mData.scannedData.find(lListItem->text())};
    if (lPosition != this->mData.scannedData.end())
    {
      // Make the item written in italic if no data is available anymore
      auto lFont{lListItem->font()};
      lFont.setItalic(lPosition->second.size() == 0);
      lListItem->setFont(lFont);
    }
  }
}

void BatchConversionPicker::refreshMiddleList()
{
  // Delete all children of the middle list
  const auto lOptionsList{this->findChild<QGridLayout*>(QString("middle_list"))};

  const auto lButtonsListSize{static_cast<int>(this->mMiddleListButtons.size())};
  for (int i = 0; i < lButtonsListSize; i++)
  {
    delete this->mMiddleListButtons.at(0);
    this->mMiddleListButtons.erase(this->mMiddleListButtons.begin() + 0);
  }

  // Add the entries in the options list, based on the paths list' selected item
  const auto lPathsList{this->findChild<QListWidget*>(QString("left_list"))};
  const auto lSelectedEntry{lPathsList->currentItem()};
  if (lSelectedEntry != nullptr)
  {
    const auto lQuickPresetCreationButton{this->findChild<QPushButton*>(QString("simple_quick_preset_creation"))};
    const auto lNoDataLabel{this->findChild<QLabel*>(QString("no_data_label"))};
    const auto lPosition{this->mData.scannedData.find(lSelectedEntry->text())};

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
          const auto lDraggableWidget{new BCDragWidget(this, this->settings(), lPosition->first, lValue)};
          this->mMiddleListButtons.push_back(lDraggableWidget);
          lOptionsList->addWidget(lDraggableWidget);
        }

        lQuickPresetCreationButton->setDisabled(!Utils::ContainsBodyOrHandsOrFeetMesh<std::vector<QString>>(lPosition->second));
      }
    }
  }

  this->refreshLeftListFont();
}

void BatchConversionPicker::updateOSPXMLPreview(QString aText)
{
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

  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lIsValidPath ? this->settings().display.successColor : this->settings().display.dangerColor));
  lOutputPathsPreview->setText(lConstructedPreviewText);

  // Save the data in the preset
  if (!this->mPreventPresetSave)
  {
    this->saveBodySlideDataToPreset();
  }
}

void BatchConversionPicker::updateBodyslideNamesPreview(QString aText)
{
  auto lIsValidPath{true};

  Utils::CleanPathString(aText);

  if (aText.trimmed().isEmpty())
  {
    aText = QString("*");
    lIsValidPath = false;
  }

  // Read the beast hands state
  auto lMustUseBeastHands{false};

  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  const auto lCurrentIndex{lActivePresetNumber->value()};
  if (lCurrentIndex > 0
      && lCurrentIndex <= static_cast<int>(this->mData.presets.size())
      && this->mData.presets.at(static_cast<size_t>(lCurrentIndex - 1)).mustHandsUseAlternativeModel())
  {
    lMustUseBeastHands = true;
  }

  QString lConstructedPreviewText;
  lConstructedPreviewText.append(Utils::GetBodySliderValue(this->mData.getBodyMod()));                      // Body
  lConstructedPreviewText.append(Utils::GetFeetSliderValue(this->mData.getFeetModIndex()));                 // Feet
  lConstructedPreviewText.append(Utils::GetHandsSliderValue(this->mData.getBodyMod(), lMustUseBeastHands)); // Hands
  lConstructedPreviewText = lConstructedPreviewText.arg(aText);

  auto lNewTextColor{this->settings().display.successColor};

  if (!lIsValidPath)
  {
    lNewTextColor = this->settings().display.dangerColor;
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

void BatchConversionPicker::removeDataFromActiveMiddleList(const QString& aOriginFolder, const QString& aResourcePath, const bool isCheckBoxChecked)
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
      if (lValue == aResourcePath)
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
    lPreset.setBodyData(aOriginFolder, aResourcePath);
  }
  else if (this->sender() == lDropSectionFeet)
  {
    lPreset.setFeetData(aOriginFolder, aResourcePath);
  }
  else if (this->sender() == lDropSectionHands)
  {
    lPreset.setHandsData(aOriginFolder, aResourcePath, isCheckBoxChecked);
  }
  else if (this->sender() == lDropSectionSkeleton)
  {
    lPreset.setSkeletonData(aOriginFolder, aResourcePath, isCheckBoxChecked);
  }
}

void BatchConversionPicker::addDataToActiveMiddleList(const QString& aOriginFolder, const QString& aResourcePath)
{
  // Search to remove the used entry from the available data to make presets
  auto lPosition{this->mData.scannedData.find(aOriginFolder)};
  if (lPosition != this->mData.scannedData.end())
  {
    // Re-insert the wanted entry
    lPosition->second.push_back(aResourcePath);
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

  // Refresh the preview since the alternative model can impact the BodySlide presets names
  auto lNamesInAppValue{this->findChild<QLineEdit*>(QString("names_bodyslide_input"))->text()};
  this->updateBodyslideNamesPreview(lNamesInAppValue);
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
  auto lOSPXMLNames{this->findChild<QLineEdit*>(QString("names_osp_xml_input"))->text().trimmed()};
  Utils::CleanBreaksString(lOSPXMLNames);
  auto lBodyslideSlidersetsNames{this->findChild<QLineEdit*>(QString("names_bodyslide_input"))->text().trimmed()};
  Utils::CleanBreaksString(lBodyslideSlidersetsNames);

  // Update the current preset
  this->mData.presets.at(static_cast<size_t>(lActivePresetNumber->value() - 1)).setNames(lOSPXMLNames, lBodyslideSlidersetsNames);
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
  const auto lCurrentIndex{lActivePresetNumber->value()};

  // Remove the preset entry
  this->mData.presets.erase(this->mData.presets.begin() + lCurrentIndex - 1);

  // Update the preset interface block
  auto lNumberOfPresets{static_cast<int>(this->mData.presets.size())};
  auto lNextIndex{lCurrentIndex > lNumberOfPresets ? lNumberOfPresets : lCurrentIndex};
  this->updatePresetInterfaceState(lNextIndex);
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
  auto lBodyslideGroupBox{this->findChild<GroupBox*>(QString("bodyslide_groupbox"))};
  lBodyslideGroupBox->setHidden(lNumberOfPresets == 0);

  if (lNumberOfPresets > 0)
  {
    auto lOSPXMLNamesLineEdit{this->findChild<QLineEdit*>(QString("names_osp_xml_input"))};
    auto lNamesInAppLineEdit{this->findChild<QLineEdit*>(QString("names_bodyslide_input"))};

    mPreventPresetSave = true;
    const auto& lPathsPair = this->mData.presets.at(static_cast<size_t>(aNextIndex - 1)).getNames();
    lOSPXMLNamesLineEdit->setText(lPathsPair.first);
    lNamesInAppLineEdit->setText(lPathsPair.second);

    // Force refresh (sometimes it will be done twice)
    this->updateOSPXMLPreview(lPathsPair.first);
    this->updateBodyslideNamesPreview(lPathsPair.second);

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

void BatchConversionPicker::fullQuickCreatePreset()
{
  std::multimap<QString, std::map<QString, std::set<QString>>> lPresets;

  auto lPathsList{this->findChild<QListWidget*>(QString("left_list"))};
  for (int i = 0; i < lPathsList->count(); i++)
  {
    auto lEntry{lPathsList->item(i)};
    if (lEntry != nullptr)
    {
      lPresets.insert({lEntry->text(), this->findNewPresets(lEntry->text())});
      // + add a message if the key is already defined
    }
  }

  this->generateNewPresets(lPresets);
}

void BatchConversionPicker::simpleQuickCreatePreset()
{
  auto lPathsList{this->findChild<QListWidget*>(QString("left_list"))};
  auto lSelectedEntry{lPathsList->currentItem()};
  if (lSelectedEntry != nullptr)
  {
    std::multimap<QString, std::map<QString, std::set<QString>>> lPresets;
    lPresets.insert({lSelectedEntry->text(), this->findNewPresets(lSelectedEntry->text())});
    this->generateNewPresets(lPresets);
  }
}

std::map<QString, std::set<QString>> BatchConversionPicker::findNewPresets(const QString& aOriginFolder)
{
  // Analyze the left data to split between everything
  std::map<QString, std::set<QString>> lPresets;

  auto lPosition{this->mData.scannedData.find(aOriginFolder)};
  if (lPosition != this->mData.scannedData.end())
  {
    // Create the BCDragWidget
    for (const auto& lValue : lPosition->second)
    {
      // Parse the data
      const auto lLastSlashPosition{lValue.lastIndexOf('/')};
      const auto lKey{lValue.left(lLastSlashPosition)};
      const auto lMeshName{lValue.mid(lLastSlashPosition + 1)};

      // Insert the data
      lPresets[lKey].insert(lMeshName);
    }
  }

  // Clear the presets that do not have any useful entries
  Utils::ClearUselessEntries(lPresets);

  return lPresets;
}

void BatchConversionPicker::generateNewPresets(const std::multimap<QString, std::map<QString, std::set<QString>>>& aPresets)
{
  // Count the number of presets to create
  auto lNumberOfNewPresets{0};
  for (const auto& lEntry : aPresets)
  {
    lNumberOfNewPresets += static_cast<int>(lEntry.second.size());
  }

  // Warn the user that no preset could be made from left data
  if (lNumberOfNewPresets == 0)
  {
    Utils::DisplayWarningMessage(tr("Error: No preset could be made from available data."));
    return;
  }

  // Ask the user if they want to create the detected possible presets
  if (Utils::DisplayQuestionMessage(this,
                                    tr("Create new preset(s)?"),
                                    tr("Do you want to create %1 new preset(s)?").arg(lNumberOfNewPresets),
                                    this->getThemedResourcePath(),
                                    "help-circle",
                                    tr("Create the %1 new preset(s)").arg(lNumberOfNewPresets),
                                    tr("Do not create the %1 new preset(s)").arg(lNumberOfNewPresets),
                                    "",
                                    this->settings().display.successColor,
                                    "",
                                    "",
                                    true)
      == ButtonClicked::YES)
  {
    // TODO: Check if any already existing preset can be completed with left data

    // Iterate the list of new presets that need to be created
    for (const auto& lEntry : aPresets)
    {
      for (const auto& lPreset : lEntry.second)
      {
        // Create a new empty preset for each newly created preset
        this->addNewEmptyPreset();

        // Iterate the meshes names
        for (const auto& lMeshPart : lPreset.second)
        {
          // Simulate the drag&drop of the resources
          auto lResourcePath{lPreset.first + "/" + lMeshPart};
          auto lResourceType{Utils::GetMeshTypeFromFileName(lResourcePath)};
          BCDropWidget* lTarget{nullptr};

          switch (lResourceType)
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
            case BCGroupWidgetCallContext::UNDEFINED:
            case BCGroupWidgetCallContext::HEAD:
            case BCGroupWidgetCallContext::EYES:
              continue;
          }

          if (lTarget != nullptr)
          {
            lTarget->simulateDropEvent(lEntry.first, lResourcePath);
          }
        }
      }
    }
  }

  this->refreshLeftListFont();
}

void BatchConversionPicker::validateSelection()
{
  // Inform the user that the window will be closed since no preset were made
  if (this->mData.presets.size() == 0)
  {
    Utils::DisplayInfoMessage(this,
                              tr("No preset created"),
                              tr("You have not created any preset. Batch conversion: results picker window is going to be closed."),
                              this->getThemedResourcePath(),
                              "alert-circle",
                              tr("OK"));
    this->accept();
    return;
  }

  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};

  for (size_t i = 0; i < this->mData.presets.size(); i++)
  {
    const auto lNaturalIndex{static_cast<int>(i + 1)};

    // If the current preset is not valid
    if (!this->mData.presets.at(i).isValid())
    {
      // Display the invalid preset in the interface
      lActivePresetNumber->setValue(lNaturalIndex);

      // TODO: In the message below, detail to the user why the preset is not valid precisely
      // Ask the user what to do with it
      if (Utils::DisplayQuestionMessage(this,
                                        tr("Invalid preset detected"),
                                        tr("The preset number %1 is not valid.\n\nA preset is considered as valid when at least one mesh part has been defined and the BodySlide files names and BodySlide preset name have been given.\n\nWould you like to delete this invalid preset and continue, or cancel the generation and fix this preset?").arg(lNaturalIndex),
                                        this->getThemedResourcePath(),
                                        "help-circle",
                                        tr("Delete the preset number %1 and continue the generation").arg(lNaturalIndex),
                                        tr("Go back to the batch conversion: results picker window"),
                                        "",
                                        this->settings().display.dangerColor,
                                        this->settings().display.successColor,
                                        "",
                                        false)
          == ButtonClicked::YES)
      {
        this->removeActivePreset();
        this->updateActivePresetNumberSpinBox(); // Force the refresh of the spin box since the preset since it will be ignored then
        this->updatePresetInterfaceState(static_cast<int>(i));
        i--;
      }
      else
      {
        // Cancel the whole check and go back to the Batch Conversion Picker window
        return;
      }
    }

    // Check if two presets have the same BodySlide files names,
    // because it will not be possible to generate both of them under the same name
    for (size_t j = 0; j < this->mData.presets.size(); j++)
    {
      if (i != j && mData.presets.at(i).getNames().first.compare(mData.presets.at(j).getNames().first) == 0)
      {
        Utils::DisplayErrorMessage(tr("The presets number %1 and number %2 have the same BodySlide files names \"%3\" defined.\n\nPlease make sure the files names are different before trying to batch generate the files again.")
                                     .arg(lNaturalIndex)
                                     .arg(j + 1)
                                     .arg(mData.presets.at(i).getNames().first));

        // Cancel the whole check and go back to the Batch Conversion Picker window
        return;
      }
    }
  }

  // Tell the user that there is not any valid preset
  if (this->mData.presets.size() == 0)
  {
    if (Utils::DisplayQuestionMessage(this,
                                      tr("No valid preset"),
                                      tr("No valid preset has been found.\n\nWould you like to close the batch conversion: results picker window now?"),
                                      this->getThemedResourcePath(),
                                      "help-circle",
                                      tr("Close the batch conversion: results picker window"),
                                      tr("Go back to the batch conversion: results picker window"),
                                      "",
                                      this->settings().display.dangerColor,
                                      this->settings().display.successColor,
                                      "",
                                      false)
        == ButtonClicked::YES)
    {
      this->accept();
    }
    else
    {
      // Cancel the whole check and go back to the Batch Conversion Picker window
      return;
    }
  }

  // Ask the user if they want to start the generation now
  if (Utils::DisplayQuestionMessage(this,
                                    tr("Start batch generation now?"),
                                    tr("%1 valid preset(s) has (have) been found.\n\nWould you like to generate it (them) now?").arg(this->mData.presets.size()),
                                    this->getThemedResourcePath(),
                                    "help-circle",
                                    tr("Generate the preset(s) files on my disk"),
                                    tr("Go back to the batch conversion: results picker window"),
                                    "",
                                    this->settings().display.warningColor,
                                    this->settings().display.successColor,
                                    "",
                                    false)
      == ButtonClicked::YES)
  {
    // Clear the scanned data to send a thinier object
    this->mData.scannedData.clear();

    // Emit the event to start generating the presets files on the user's disk
    emit presetsCreationValidated(this->mData);

    this->accept();
  }
}
