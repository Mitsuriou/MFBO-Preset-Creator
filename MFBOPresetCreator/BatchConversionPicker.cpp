#include "BatchConversionPicker.h"
#include "BCGroupWidget.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Utils.h"
#include <QAbstractSlider>
#include <QApplication>
#include <QCloseEvent>
#include <QDirIterator>
#include <QFileDialog>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QProgressBar>
#include <QProgressDialog>
#include <QScrollArea>
#include <QSpinBox>
#include <QSplitter>
#include <QStyledItemDelegate>

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
  this->show();
}

void BatchConversionPicker::closeEvent(QCloseEvent* aEvent)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  if (Utils::displayQuestionMessage(this,
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
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::createScrollAreaWindowLayout(this, false)};

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
  auto lLeftTitle{new QLabel(tr("Found mods (click to display the data)"))};
  lLeftTitle->setAlignment(Qt::AlignCenter);
  lLeftLayout->addWidget(lLeftTitle);

  // Paths list
  auto lLeftList{new QListWidget(this)};
  lLeftList->setObjectName("left_list");
  lLeftList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  lLeftList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  lLeftLayout->addWidget(lLeftList);

  /*=============*/
  /* Middle list */
  /*=============*/
  // Layout (the BCDragWidget components will be directly in this layout)
  auto lMiddleList{new QVBoxLayout(this)};
  lMiddleList->setAlignment(Qt::AlignTop);
  lMiddleList->setObjectName("middle_list");

  // Wrapper widget
  auto lMiddleWrapper{new QWidget(this)};
  lMiddleWrapper->setLayout(lMiddleList);
  lSplitter->addWidget(lMiddleWrapper);

  // Title
  auto lMiddleTitle{new QLabel(tr("Available data (drag these entries)"))};
  lMiddleTitle->setAlignment(Qt::AlignCenter);
  lMiddleList->addWidget(lMiddleTitle);

  // Label for the "no data available" case
  auto lNoDataLabel{new QLabel(tr("No data available for the selected mod"), this)};
  lNoDataLabel->setObjectName("no_data_label");
  lNoDataLabel->hide();
  lMiddleList->addWidget(lNoDataLabel);

  /*============*/
  /* Right list */
  /*============*/
  // Nested layouts
  auto lRightDataLayout{new QVBoxLayout(this)};
  lRightDataLayout->setAlignment(Qt::AlignTop);

  auto lRightNavigationLayout{new QGridLayout(this)};
  lRightNavigationLayout->setColumnStretch(0, 1);
  lRightNavigationLayout->setColumnStretch(1, 0);
  lRightNavigationLayout->setColumnStretch(2, 0);
  lRightNavigationLayout->setColumnStretch(3, 1);
  lRightNavigationLayout->setColumnStretch(4, 1);
  lRightNavigationLayout->setColumnStretch(5, 1);

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
  lNoPresetLabel->setObjectName("no_preset_label");
  lRightDataLayout->addWidget(lNoPresetLabel);

  // Body drop widget
  auto lDropSectionBody{new BCGroupWidget(this, this->mSettings, tr("Body"), "body", BCGroupWidgetCallContext::BODY)};
  lDropSectionBody->setObjectName("drop_section_body");
  lRightDataLayout->addWidget(lDropSectionBody);

  // Feet drop widget
  auto lDropSectionFeet{new BCGroupWidget(this, this->mSettings, tr("Feet"), "foot", BCGroupWidgetCallContext::FEET)};
  lDropSectionFeet->setObjectName("drop_section_feet");
  lRightDataLayout->addWidget(lDropSectionFeet);

  // Hands drop widget
  auto lDropSectionHands{new BCGroupWidget(this, this->mSettings, tr("Hands"), "hand", BCGroupWidgetCallContext::HANDS)};
  lDropSectionHands->setObjectName("drop_section_hands");
  lRightDataLayout->addWidget(lDropSectionHands);

  // BodySlide output settings group box
  auto lBodyslideGroupBox{new QGroupBox(tr("BodySlide output").append("  "), this)};
  lBodyslideGroupBox->setObjectName("bodyslide_groupbox");
  Utils::addIconToGroupBox(lBodyslideGroupBox, lIconFolder, "bodyslide-logo", this->mSettings.font.size);
  this->connect(lBodyslideGroupBox, &QGroupBox::toggled, this, &BatchConversionPicker::groupBoxChecked);
  Utils::setGroupBoxState(lBodyslideGroupBox, false);
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
  lOSPXMLNamesLineEdit->setObjectName("names_osp_xml_input");
  lBodyslideGridLayout->addWidget(lOSPXMLNamesLineEdit, 1, 1, 1, 4);

  // Second line
  lBodyslideGridLayout->addWidget(new QLabel(tr("Preview:"), this), 2, 0);

  auto lPathsNamesOspXmlNames{new QLabel("", this)};
  lPathsNamesOspXmlNames->setObjectName("names_osp_xml_preview");
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
  lNamesInAppLineEdit->setObjectName("names_bodyslide_input");
  lBodyslideGridLayout->addWidget(lNamesInAppLineEdit, 3, 1, 1, 4);

  // Fourth line
  lBodyslideGridLayout->addWidget(new QLabel(tr("Preview:"), this), 4, 0);

  auto lResultNamesInApp{new QLabel("", this)};
  lResultNamesInApp->setObjectName("names_bodyslide_preview");
  lBodyslideGridLayout->addWidget(lResultNamesInApp, 4, 1, 1, 4);

  /*==============================*/
  /* Right list: Presets controls */
  /*==============================*/
  // Previous preset
  auto lPreviousPreset{ComponentFactory::createButton(this, tr("Previous preset"), "", "arrow-left", lIconFolder, "previous_preset", false, true)};
  lRightNavigationLayout->addWidget(lPreviousPreset, 0, 0);

  // Active preset number
  auto lActivePresetNumber{new QSpinBox(this)}; // TODO: Add styles for QSpinBox in Mitsuriou's stylesheets
  lActivePresetNumber->setObjectName("active_preset_number");
  lRightNavigationLayout->addWidget(lActivePresetNumber, 0, 1);

  // Active preset out of number of presets
  auto lNumberOfPresets{new QLabel("/ 0", this)};
  lNumberOfPresets->setObjectName("number_of_presets");
  lRightNavigationLayout->addWidget(lNumberOfPresets, 0, 2);

  // Next preset
  auto lNextPreset{ComponentFactory::createButton(this, tr("Next preset"), "", "arrow-right", lIconFolder, "next_preset", false, true)};
  lRightNavigationLayout->addWidget(lNextPreset, 0, 3);

  // Remove current preset
  auto lRemoveActivePreset{ComponentFactory::createButton(this, tr("Remove current preset"), "", "minus", lIconFolder, "remove_current_preset", false, true)};
  lRightNavigationLayout->addWidget(lRemoveActivePreset, 0, 4);

  // Add new empty preset
  auto lAddEmptyPreset{ComponentFactory::createButton(this, tr("Add new preset"), "", "plus", lIconFolder, "add_empty_preset", false, true)};
  lRightNavigationLayout->addWidget(lAddEmptyPreset, 0, 5);

  /*========================================*/
  /* Validate selection and generate button */
  /*========================================*/
  auto lButtonLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};

  auto lGenerateButton{ComponentFactory::createButton(this, tr("Batch generate the files on my computer"), "", "build", lIconFolder, "generate")};
  lGenerateButton->setAutoDefault(true);
  lGenerateButton->setDefault(true);
  lButtonLayout->addWidget(lGenerateButton);

  // Pre-bind initialization functions
  this->updateOSPXMLPreview(QString());
  this->updateBodyslideNamesPreview(QString());
  this->updatePresetInterfaceState(0);

  // Event binding
  this->connect(lDropSectionBody, &BCGroupWidget::removePressed, this, &BatchConversionPicker::addDataToActiveMiddleList);
  this->connect(lDropSectionBody, &BCGroupWidget::dropEventTriggered, this, &BatchConversionPicker::removeDataFromActiveMiddleList);
  this->connect(lDropSectionFeet, &BCGroupWidget::removePressed, this, &BatchConversionPicker::addDataToActiveMiddleList);
  this->connect(lDropSectionFeet, &BCGroupWidget::dropEventTriggered, this, &BatchConversionPicker::removeDataFromActiveMiddleList);
  this->connect(lDropSectionHands, &BCGroupWidget::removePressed, this, &BatchConversionPicker::addDataToActiveMiddleList);
  this->connect(lDropSectionHands, &BCGroupWidget::dropEventTriggered, this, &BatchConversionPicker::removeDataFromActiveMiddleList);

  this->connect(lOSPXMLNamesLineEdit, &QLineEdit::textChanged, this, &BatchConversionPicker::updateOSPXMLPreview);
  this->connect(lNamesInAppLineEdit, &QLineEdit::textChanged, this, &BatchConversionPicker::updateBodyslideNamesPreview);
  this->connect(lGenerateButton, &QPushButton::clicked, this, &BatchConversionPicker::validateSelection);
  this->connect(lLeftList, &QListWidget::itemSelectionChanged, this, &BatchConversionPicker::refreshMiddleList);

  this->connect(lPreviousPreset, &QPushButton::clicked, this, &BatchConversionPicker::goToPreviousPreset);
  this->connect(lNextPreset, &QPushButton::clicked, this, &BatchConversionPicker::goToNextPreset);
  this->connect(lActivePresetNumber, QOverload<int>::of(&QSpinBox::valueChanged), this, &BatchConversionPicker::updatePresetInterfaceState);
  this->connect(lRemoveActivePreset, &QPushButton::clicked, this, &BatchConversionPicker::removeActivePreset);
  this->connect(lAddEmptyPreset, &QPushButton::clicked, this, &BatchConversionPicker::addNewEmptyPreset);

  // Post-bind initialization functions
  lLeftList->setCurrentRow(0);
  mPreventPresetSave = false;
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
  auto lOptionsList{this->findChild<QVBoxLayout*>(QString("middle_list"))};

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
    auto lNoDataLabel{this->findChild<QLabel*>(QString("no_data_label"))};
    auto lPosition{this->mData.scannedData.find(lSelectedEntry->text())};

    if (lPosition != this->mData.scannedData.end())
    {
      // Check how many entries are still available for the selected mod
      if (lPosition->second.size() == 0)
      {
        lNoDataLabel->show();
      }
      else
      {
        for (const auto& lValue : lPosition->second)
        {
          auto lButton{new BCDragWidget(this, this->mSettings, lPosition->first, lValue)};
          this->mMiddleListButtons.push_back(lButton);
          lOptionsList->addWidget(lButton);
        }

        lNoDataLabel->hide();
      }
    }
  }
}

void BatchConversionPicker::updateOSPXMLPreview(QString aText)
{
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("names_osp_xml_preview"))};
  auto lIsValidPath{true};

  Utils::cleanPathString(aText);

  if (aText.trimmed().length() == 0)
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

  Utils::cleanPathString(aText);

  if (aText.trimmed().length() == 0)
  {
    aText = QString("*");
    lIsValidPath = false;
  }

  auto lConstructedPreviewText{QString()};
  lConstructedPreviewText.append(Utils::getBodySliderValue(this->mData.bodyMod));                           // Body
  lConstructedPreviewText.append(Utils::getFeetSliderValue(this->mData.bodyMod, this->mData.feetModIndex)); // Feet
  lConstructedPreviewText.append(Utils::getHandsSliderValue(this->mData.bodyMod, lMustUseBeastHands));      // Hands
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

void BatchConversionPicker::removeDataFromActiveMiddleList(const QString& aOriginFolder, const QString& aRessourcePath)
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

  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  auto& lPreset{this->mData.presets.at(lActivePresetNumber->value() - 1)};

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
    lPreset.setHandsData(aOriginFolder, aRessourcePath, false); // TODO: Pass the data for the alternative model
  }
}

void BatchConversionPicker::addDataToActiveMiddleList(const QString& aOriginFolder, const QString& aRessourcePath)
{
  // Search to remove the used entry from the available data to make presets
  auto lPosition{this->mData.scannedData.find(aOriginFolder)};
  if (lPosition != this->mData.scannedData.end())
  {
    // Re-insert the wanted entry
    lPosition->second.insert(aRessourcePath);
  }

  // Finally, refresh the middle list with the entry added
  refreshMiddleList();

  // Track the changes in the output data
  if (!this->mPreventPresetSave)
  {
    auto lDropSectionBody{this->findChild<BCGroupWidget*>(QString("drop_section_body"))};
    auto lDropSectionFeet{this->findChild<BCGroupWidget*>(QString("drop_section_feet"))};
    auto lDropSectionHands{this->findChild<BCGroupWidget*>(QString("drop_section_hands"))};

    auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
    auto& lPreset{this->mData.presets.at(lActivePresetNumber->value() - 1)};

    if (this->sender() == lDropSectionBody)
    {
      lPreset.setBodyData("", "");
    }
    else if (this->sender() == lDropSectionFeet)
    {
      lPreset.setFeetData("", "");
    }
    else if (this->sender() == lDropSectionHands)
    {
      lPreset.setHandsData("", "", false);
    }
  }
}

void BatchConversionPicker::saveBodySlideDataToPreset()
{
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  if (lActivePresetNumber->value() == 0)
    return;

  // Read the data in the GUI
  auto lOSPXMLNamesValue{this->findChild<QLineEdit*>("names_osp_xml_input")->text()};
  auto lNamesInAppValue{this->findChild<QLineEdit*>("names_bodyslide_input")->text()};

  // Update the current preset
  this->mData.presets.at(lActivePresetNumber->value() - 1).setNames(lOSPXMLNamesValue, lNamesInAppValue);
}

void BatchConversionPicker::goToPreviousPreset() const
{
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  lActivePresetNumber->setValue(lActivePresetNumber->value() - 1);
}

void BatchConversionPicker::goToNextPreset() const
{
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  lActivePresetNumber->setValue(lActivePresetNumber->value() + 1);
}

void BatchConversionPicker::removeActivePreset()
{
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  auto lCurrentIndex{lActivePresetNumber->value()};

  this->mPreventPresetSave = true;

  // Re-put the data in the scannedData list
  auto lDropSectionBody{this->findChild<BCGroupWidget*>(QString("drop_section_body"))};
  lDropSectionBody->removeData();
  auto lDropSectionFeet{this->findChild<BCGroupWidget*>(QString("drop_section_feet"))};
  lDropSectionFeet->removeData();
  auto lDropSectionHands{this->findChild<BCGroupWidget*>(QString("drop_section_hands"))};
  lDropSectionHands->removeData();

  this->mPreventPresetSave = false;

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

  auto lNumberOfPresets{static_cast<int>(this->mData.presets.size())};

  // Update the preset interface block
  this->updatePresetInterfaceState(lNumberOfPresets);

  // Display the last preset
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  lActivePresetNumber->setValue(lNumberOfPresets);
}

void BatchConversionPicker::updatePresetInterfaceState(const int aNextIndex)
{
  auto lNumberOfPresets{static_cast<int>(this->mData.presets.size())};

  // Previous preset
  auto lPreviousPreset{this->findChild<QPushButton*>(QString("previous_preset"))};
  auto lWasPreviousPresetFocused{lPreviousPreset->hasFocus()};
  lPreviousPreset->setDisabled(lNumberOfPresets == 0 || aNextIndex <= 1);

  // Active preset number
  auto lActivePresetNumber{this->findChild<QSpinBox*>(QString("active_preset_number"))};
  lActivePresetNumber->setDisabled(lNumberOfPresets == 0);
  lActivePresetNumber->setMinimum(lNumberOfPresets == 0 ? 0 : 1);
  lActivePresetNumber->setMaximum(lNumberOfPresets);

  // Active preset out of number of presets
  this->findChild<QLabel*>(QString("number_of_presets"))->setText(QString("/ %1").arg(lNumberOfPresets));

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

  if (lNumberOfPresets > 0)
  {
    lDropSectionBody->setData(this->mData.presets.at(aNextIndex - 1));
    lDropSectionFeet->setData(this->mData.presets.at(aNextIndex - 1));
    lDropSectionHands->setData(this->mData.presets.at(aNextIndex - 1));
  }

  // BodySlide data
  auto lBodyslideGroupBox{this->findChild<QGroupBox*>(QString("bodyslide_groupbox"))};
  lBodyslideGroupBox->setHidden(lNumberOfPresets == 0);

  if (lNumberOfPresets > 0)
  {
    auto lOSPXMLNamesLineEdit{this->findChild<QLineEdit*>("names_osp_xml_input")};
    auto lNamesInAppLineEdit{this->findChild<QLineEdit*>("names_bodyslide_input")};

    mPreventPresetSave = true;
    lOSPXMLNamesLineEdit->setText(this->mData.presets.at(aNextIndex - 1).getNames().first);
    lNamesInAppLineEdit->setText(this->mData.presets.at(aNextIndex - 1).getNames().second);
    mPreventPresetSave = false;
  }
}

void BatchConversionPicker::validateSelection()
{
  this->mData.scannedData.clear(); // Clear the data to send a skinier object

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
}

void BatchConversionPicker::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);
}
