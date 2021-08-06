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
  lMainLayout->addWidget(lSplitter, 0, 0);

  /*===========*/
  /* Left list */
  /*===========*/
  // Layout
  auto lLeftLayout{new QVBoxLayout(this)};
  lLeftLayout->setMargin(0);
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

  /*============*/
  /* Right list */
  /*============*/
  // Layout
  auto lRightLayout{new QVBoxLayout(this)};
  lRightLayout->setAlignment(Qt::AlignTop);

  // Wrapper widget
  auto lRightWrapper{new QWidget(this)};
  lRightWrapper->setLayout(lRightLayout);
  lSplitter->addWidget(lRightWrapper);

  // Title
  auto lRightTitle{new QLabel(tr("Presets to generate (drop the entries here)"))};
  lRightTitle->setAlignment(Qt::AlignCenter);
  lRightLayout->addWidget(lRightTitle);

  // Body drop widget
  auto lDropSectionBody{new BCGroupWidget(this, this->mSettings, tr("Body"))};
  lRightLayout->addWidget(lDropSectionBody);

  // Feet drop widget
  auto lDropSectionFeet{new BCGroupWidget(this, this->mSettings, tr("Feet"))};
  lRightLayout->addWidget(lDropSectionFeet);

  // Hands drop widget
  auto lDropSectionHands{new BCGroupWidget(this, this->mSettings, tr("Hands"))};
  lRightLayout->addWidget(lDropSectionHands);

  // BodySlide output settings group box
  auto lBodyslideGroupBox{new QGroupBox(tr("BodySlide output").append("  "), this)};
  Utils::addIconToGroupBox(lBodyslideGroupBox, lIconFolder, "bodyslide-logo", this->mSettings.font.size);
  this->connect(lBodyslideGroupBox, &QGroupBox::toggled, this, &BatchConversionPicker::groupBoxChecked);
  Utils::setGroupBoxState(lBodyslideGroupBox, false);
  lRightLayout->addWidget(lBodyslideGroupBox);

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

  // Validate selection and generate button
  auto lButtonLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};

  auto lGenerateButton{ComponentFactory::createButton(this, tr("Batch generate the files on my computer"), "", "build", lIconFolder)};
  lButtonLayout->addWidget(lGenerateButton);

  // Pre-bind initialization functions
  this->updateOSPXMLPreview(QString());
  this->updateBodyslideNamesPreview(QString());

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

  // Post-bind initialization functions
  lLeftList->setCurrentRow(0);
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
    auto lPosition{this->mData.scannedData.find(lSelectedEntry->text())};

    if (lPosition != this->mData.scannedData.end())
    {
      for (const auto& lValue : lPosition->second)
      {
        auto lButton{new BCDragWidget(this, this->mSettings, lPosition->first, lValue)};
        this->mMiddleListButtons.push_back(lButton);
        lOptionsList->addWidget(lButton);
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

  // TODO: Track the changes in the output data
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

  // TODO: Track the changes in the output data
}

void BatchConversionPicker::validateSelection()
{
  // TODO:
}

void BatchConversionPicker::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);
}
