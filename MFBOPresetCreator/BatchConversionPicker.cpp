#include "BatchConversionPicker.h"
#include "ComponentFactory.h"
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
#include <QProgressBar>
#include <QProgressDialog>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>

BatchConversionPicker::BatchConversionPicker(QWidget* aParent, const Struct::Settings& aSettings, const std::map<std::string, std::pair<QString, QString>, std::greater<std::string>>& aScannedData)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  this->displayFoundFiles(qobject_cast<QGridLayout*>(this->layout()), aScannedData);

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void BatchConversionPicker::closeEvent(QCloseEvent* aEvent)
{
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
  this->setWindowTitle(tr("Textures Assistant"));
  this->setWindowIcon(QIcon(QPixmap(":/black/textures")));
}

void BatchConversionPicker::initializeGUI()
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Main window layout
  auto lMainGrid{new QGridLayout(this)};
  lMainGrid->setSpacing(10);
  lMainGrid->setContentsMargins(10, 10, 10, 10);
  lMainGrid->setAlignment(Qt::AlignTop);
  this->setLayout(lMainGrid);

  // First line
  lMainGrid->addWidget(new QLabel(tr("Input path:"), this), 0, 0);

  // Input label
  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  lInputPathLineEdit->setDisabled(true);
  lMainGrid->addWidget(lInputPathLineEdit, 0, 1);
}

void BatchConversionPicker::deleteAlreadyExistingWindowBottom() const
{
  auto lHintZone{this->findChild<QLabel*>("hint_zone")};
  if (lHintZone)
  {
    delete lHintZone;
    lHintZone = nullptr;
  }

  auto lOldScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  if (lOldScrollArea)
  {
    delete lOldScrollArea;
    lOldScrollArea = nullptr;
  }
}

void BatchConversionPicker::displayFoundFiles(QGridLayout* aLayout, const std::map<std::string, std::pair<QString, QString>, std::greater<std::string>>& aScannedData)
{
  // Parse the grouped textures to split them in multiple storages
  BatchConversionPicker::GroupedData lGroupedPaths;

  for (const auto& lNifFile : aScannedData)
  {
    const auto& lFilePath{lNifFile.second.first.toStdString()};
    const auto& lFileName{lNifFile.second.second};
    std::map<std::string, std::vector<QString>>* lMap = nullptr;

    if (lNifFile.second.second.contains("hands"))
    {
      lMap = &lGroupedPaths.hands;
    }
    else if (lNifFile.second.second.contains("feet"))
    {
      lMap = &lGroupedPaths.feet;
    }
    else if (lNifFile.second.second.contains("body"))
    {
      lMap = &lGroupedPaths.body;
    }

    auto lPosition{lMap->find(lFilePath)};
    if (lPosition == lMap->end())
    {
      lMap->insert(std::make_pair(lFilePath, std::vector<QString>({lFileName})));
    }
    else
    {
      lPosition->second.push_back(lFileName);
    }
  }

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};
  auto lRowIndex{0};

  // Head ressources blocks
  auto lHeadGroup{new QGroupBox(tr("Head textures").append("  "), this)};
  Utils::addIconToGroupBox(lHeadGroup, lIconFolder, "woman-head");
  this->connect(lHeadGroup, &QGroupBox::toggled, this, &BatchConversionPicker::groupBoxChecked);
  Utils::setGroupBoxState(lHeadGroup, false);

  auto lHeadGroupContainer{new QGridLayout(this)};
  lHeadGroupContainer->setSpacing(16);
  lHeadGroup->setLayout(lHeadGroupContainer);
  this->createRessourceBlock(lGroupedPaths.feet, lHeadGroupContainer);
  aLayout->addWidget(lHeadGroup, lRowIndex++, 0);

  // Hands ressources blocks
  auto lHandsGroup{new QGroupBox(tr("Hands textures").append("  "), this)};
  Utils::addIconToGroupBox(lHandsGroup, lIconFolder, "hand");
  this->connect(lHandsGroup, &QGroupBox::toggled, this, &BatchConversionPicker::groupBoxChecked);
  Utils::setGroupBoxState(lHandsGroup, false);

  auto lHandsGroupContainer{new QGridLayout(this)};
  lHandsGroupContainer->setSpacing(16);
  lHandsGroup->setLayout(lHandsGroupContainer);
  this->createRessourceBlock(lGroupedPaths.hands, lHandsGroupContainer);
  aLayout->addWidget(lHandsGroup, lRowIndex++, 0);

  // Body ressources blocks
  auto lBodyGroup{new QGroupBox(tr("Body textures").append("  "), this)};
  Utils::addIconToGroupBox(lBodyGroup, lIconFolder, "body");
  this->connect(lBodyGroup, &QGroupBox::toggled, this, &BatchConversionPicker::groupBoxChecked);
  Utils::setGroupBoxState(lBodyGroup, false);

  auto lBodyGroupContainer{new QGridLayout(this)};
  lBodyGroupContainer->setSpacing(16);
  lBodyGroup->setLayout(lBodyGroupContainer);
  this->createRessourceBlock(lGroupedPaths.body, lBodyGroupContainer);
  aLayout->addWidget(lBodyGroup, lRowIndex++, 0);
}

void BatchConversionPicker::createRessourceBlock(const std::map<std::string, std::vector<QString>>& aMap, QGridLayout* aLayout)
{
  auto lRowIndex{0};
  for (const auto& lRootPath : aMap)
  {
    auto lConcatenatedFileNames{QString()};

    for (const auto& lFileName : lRootPath.second)
    {
      lConcatenatedFileNames.append(QString("%1\n").arg(lFileName));
    }
    lConcatenatedFileNames = lConcatenatedFileNames.left(lConcatenatedFileNames.length() - 1);

    aLayout->addWidget(new QLabel(QString::fromStdString(lRootPath.first), this), lRowIndex, 0, Qt::AlignLeft);
    aLayout->addWidget(new QLabel(lConcatenatedFileNames, this), lRowIndex++, 1, Qt::AlignLeft);
  }
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
