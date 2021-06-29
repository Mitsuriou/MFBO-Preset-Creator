#include "BatchConversionPicker.h"
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
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
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

  if (Utils::displayQuestionMessage(this, tr("Closing"), tr("Do you want to close the window?"), lIconFolder, "help-circle", tr("Close the window"), tr("Go back to the batch conversion: results picker window"), this->mSettings.dangerColor, this->mSettings.successColor, false)
      == ButtonClicked::Yes)
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
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Batch Conversion: Results picker"));
  this->setWindowIcon(QIcon(QPixmap(":/black/reorder")));
}

void BatchConversionPicker::initializeGUI()
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::createScrollAreaWindowLayout(this, false)};

  // Cursor change for the scroll bar
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderPressed, this, &BatchConversionPicker::scrollbarPressed);
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderReleased, this, &BatchConversionPicker::scrollbarReleased);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderPressed, this, &BatchConversionPicker::scrollbarPressed);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderReleased, this, &BatchConversionPicker::scrollbarReleased);

  // 3 columns splitter
  auto lSplitter{new QSplitter(Qt::Orientation::Horizontal, this)};
  lMainLayout->addWidget(lSplitter, 0, 0);

  // Paths list
  auto lPathsList{new QListWidget(this)};
  lPathsList->setObjectName("paths_list");
  lPathsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  lPathsList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  lSplitter->addWidget(lPathsList);

  // Available options list
  auto lOptionsList{new QVBoxLayout(this)};
  lOptionsList->setObjectName("options_list");

  // Middle wrapper
  auto lMiddleWrapper{new QWidget(this)};
  lMiddleWrapper->setObjectName("middle_wrapper");
  lMiddleWrapper->setLayout(lOptionsList);
  lSplitter->addWidget(lMiddleWrapper);

  // Preset maker
  auto lDataMaker{new QGridLayout(this)};
  lDataMaker->setObjectName("data_maker");

  // Right wrapper
  auto lRightWrapper{new QWidget(this)};
  lRightWrapper->setLayout(lDataMaker);
  lSplitter->addWidget(lRightWrapper);

  // Validate selection and generate button
  auto lButtonLayout{this->findChild<QHBoxLayout*>("window_buttons_layout")};

  auto lGenerateButton{ComponentFactory::createButton(this, tr("Batch generate the files on my computer"), "", "build", lIconFolder)};
  lButtonLayout->addWidget(lGenerateButton);

  // Event binding
  this->connect(lGenerateButton, &QPushButton::clicked, this, &BatchConversionPicker::validateSelection);
  this->connect(lPathsList, &QListWidget::itemSelectionChanged, this, &BatchConversionPicker::leftListIndexChanged);

  // Post-bind initialization functions
  lPathsList->setCurrentRow(0);
}

void BatchConversionPicker::displayLeftList()
{
  auto lPathsList{this->findChild<QListWidget*>("paths_list")};
  for (const auto& lEntry : this->mData.scannedData)
  {
    lPathsList->addItem(lEntry.first);
  }

  // Focus the first line of the list
  lPathsList->setAlternatingRowColors(true);
  lPathsList->setCurrentRow(0);
  lPathsList->setFocus();
}

void BatchConversionPicker::leftListIndexChanged()
{
  // Delete all children of the middlel list
  auto lOptionsList{this->findChild<QVBoxLayout*>("options_list")};
  // TODO: Delete the children

  // Add the entries in the options list, based on the paths list' selected item
  auto lPathsList{this->findChild<QListWidget*>("paths_list")};
  auto lSelectedEntry{lPathsList->currentItem()};
  if (lSelectedEntry != nullptr)
  {
    auto lPosition{this->mData.scannedData.find(lSelectedEntry->text())};

    if (lPosition != this->mData.scannedData.end())
    {
      for (const auto& lValue : lPosition->second)
      {
        auto lButton{new QPushButton(lValue, this)};
        lOptionsList->addWidget(lButton);
      }
    }
  }
}

void BatchConversionPicker::updateOSPXMLPreview(QString aText)
{
  auto lOutputPathsPreview{this->findChild<QLabel*>("names_osp_xml_preview")};
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
  //auto lMustUseBeastHands{this->findChild<QCheckBox*>("use_beast_hands")->isChecked()};
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

  auto lOutputPathsPreview{this->findChild<QLabel*>("names_bodyslide_preview")};
  lOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  lOutputPathsPreview->setText(lConstructedPreviewText);
}

void BatchConversionPicker::validateSelection()
{
  // TODO:
  auto debug_stop = true;
}

void BatchConversionPicker::scrollbarPressed()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::ClosedHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::ClosedHandCursor);
}

void BatchConversionPicker::scrollbarReleased()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
}

void BatchConversionPicker::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);
}
