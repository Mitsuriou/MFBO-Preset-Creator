#include "TexturesAssistant.h"
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
#include <QProgressBar>
#include <QProgressDialog>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>

TexturesAssistant::TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mHasUserDoneSomething(false)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void TexturesAssistant::closeEvent(QCloseEvent* aEvent)
{
  if (!this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  if (Utils::displayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the textures assistant window"),
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

void TexturesAssistant::reject()
{
  this->close();
}

void TexturesAssistant::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Textures Assistant"));
  this->setWindowIcon(QIcon(QPixmap(":/black/textures")));
}

void TexturesAssistant::initializeGUI()
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
  lInputPathLineEdit->setObjectName("input_path_directory");
  lInputPathLineEdit->setDisabled(true);
  lMainGrid->addWidget(lInputPathLineEdit, 0, 1);

  // Input chooser
  auto lInputPathChooser{ComponentFactory::createButton(this, tr("Choose a directory..."), "", "folder", lIconFolder, "", false, true)};
  lMainGrid->addWidget(lInputPathChooser, 0, 2);

  // Launch search button
  auto lLaunchSearchButton{ComponentFactory::createButton(this, tr("Launch the scan of the mod"), "", "search", lIconFolder, "launch_search_button", true, true)};
  lMainGrid->addWidget(lLaunchSearchButton, 1, 0, 1, 3, Qt::AlignTop);

  // Hint zone
  this->displayHintZone();

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &TexturesAssistant::chooseInputDirectory);
  this->connect(lLaunchSearchButton, &QPushButton::clicked, this, &TexturesAssistant::launchSearchProcess);
}

void TexturesAssistant::displayHintZone()
{
  this->deleteAlreadyExistingWindowBottom();

  // Get the window's layout
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};
  auto lHintZone{new QLabel(tr("Awaiting the launch of a scan..."), this)};
  lHintZone->setMinimumHeight(300);
  lHintZone->setObjectName("hint_zone");
  lHintZone->setAlignment(Qt::AlignCenter);
  lMainLayout->addWidget(lHintZone, 2, 0, 1, 3);
}

void TexturesAssistant::deleteAlreadyExistingWindowBottom() const
{
  auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
  if (lHintZone)
  {
    delete lHintZone;
    lHintZone = nullptr;
  }

  auto lOldScrollArea{this->findChild<QScrollArea*>(QString("scrollable_zone"))};
  if (lOldScrollArea)
  {
    delete lOldScrollArea;
    lOldScrollArea = nullptr;
  }
}

TexturesAssistant::ScannedData TexturesAssistant::scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const
{
  // Progress bar
  auto lProgressbar{new QProgressBar(this->parentWidget())};
  lProgressbar->setFormat("");
  lProgressbar->setMinimum(0);
  lProgressbar->setMaximum(0);
  lProgressbar->setValue(0);
  lProgressbar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Scanning the directory. Please wait..."), tr("Cancel treatment"), 0, 0, this->parentWidget());
  lProgressDialog.setBar(lProgressbar);
  lProgressDialog.setWindowFlags(lProgressDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
  lProgressDialog.setModal(true);
  lProgressDialog.show();

  qApp->processEvents();

  TexturesAssistant::ScannedData lScannedFiles;

  auto lRelativeDirPath{QString()};
  auto lFileName{QString()};
  auto lKey{std::string()};

  auto lTexturesFilesToFind{
    QStringList({"femalebody_1.dds",
                 "femalebody_1_msn.dds",
                 "femalebody_1_s.dds",
                 "femalebody_1_sk.dds",
                 "femalehands_1.dds",
                 "femalehands_1_msn.dds",
                 "femalehands_1_s.dds",
                 "femalehands_1_sk.dds",
                 "femalehead.dds",
                 "femalehead_msn.dds",
                 "femalehead_s.dds",
                 "femalehead_sk.dds"})};

  // Suffix the root directory to earch only in the "textures" subfolder
  auto lRootDir{QString("%1/textures").arg(aRootDir)};

  QDirIterator it(lRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::displayWarningMessage(tr("Process aborted by the user."));
      return {};
    }

    it.next();

    // Get the current directory
    lRelativeDirPath = it.fileInfo().absolutePath().remove(aRootDir + "/", Qt::CaseInsensitive);

    lFileName = it.fileInfo().fileName();

    // Construct the key of the map
    lKey = QString("%1/textures/%2").arg(lRelativeDirPath).arg(lFileName).toStdString();

    // check if the file is relative to a body, hands or head textures
    if (lTexturesFilesToFind.contains(lFileName))
    {
      lScannedFiles.groupedTextures.insert(std::make_pair(lKey, std::make_pair(lRelativeDirPath, lFileName)));
    }
    else
    {
      lScannedFiles.otherTextures.insert(std::make_pair(lKey, std::make_pair(lRelativeDirPath, lFileName)));
    }
  }

  return lScannedFiles;
}

void TexturesAssistant::displayFoundTextures(QGridLayout* aLayout, const TexturesAssistant::ScannedData& aScannedData)
{
  // Parse the grouped textures to split them in multiple storages
  TexturesAssistant::GroupedData lGroupedPaths;

  for (const auto& lNifFile : aScannedData.groupedTextures)
  {
    const auto& lFilePath{lNifFile.second.first.toStdString()};
    const auto& lFileName{lNifFile.second.second};
    std::map<std::string, std::vector<QString>>* lMap = nullptr;

    if (lFileName.contains("head"))
    {
      lMap = &lGroupedPaths.headTextures;
    }
    else if (lNifFile.second.second.contains("hands"))
    {
      lMap = &lGroupedPaths.handsTextures;
    }
    else if (lNifFile.second.second.contains("body"))
    {
      lMap = &lGroupedPaths.bodyTextures;
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

  // Parse the other textures to change their storage type to be compatible with the display function
  auto lOtherPaths{std::map<std::string, std::vector<QString>>()};

  for (const auto& lNifFile : aScannedData.otherTextures)
  {
    const auto& lFilePath{lNifFile.second.first.toStdString()};
    const auto& lFileName{lNifFile.second.second};

    auto lPosition{lOtherPaths.find(lFilePath)};
    if (lPosition == lOtherPaths.end())
    {
      lOtherPaths.insert(std::make_pair(lFilePath, std::vector<QString>({lFileName})));
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
  Utils::addIconToGroupBox(lHeadGroup, lIconFolder, "woman-head", this->mSettings.font.size);
  this->connect(lHeadGroup, &QGroupBox::toggled, this, &TexturesAssistant::groupBoxChecked);
  Utils::setGroupBoxState(lHeadGroup, false);

  auto lHeadGroupContainer{new QGridLayout(this)};
  lHeadGroupContainer->setSpacing(16);
  lHeadGroup->setLayout(lHeadGroupContainer);
  this->createRessourceBlock(lGroupedPaths.headTextures, lHeadGroupContainer);
  aLayout->addWidget(lHeadGroup, lRowIndex++, 0);

  // Hands ressources blocks
  auto lHandsGroup{new QGroupBox(tr("Hands textures").append("  "), this)};
  Utils::addIconToGroupBox(lHandsGroup, lIconFolder, "hand", this->mSettings.font.size);
  this->connect(lHandsGroup, &QGroupBox::toggled, this, &TexturesAssistant::groupBoxChecked);
  Utils::setGroupBoxState(lHandsGroup, false);

  auto lHandsGroupContainer{new QGridLayout(this)};
  lHandsGroupContainer->setSpacing(16);
  lHandsGroup->setLayout(lHandsGroupContainer);
  this->createRessourceBlock(lGroupedPaths.handsTextures, lHandsGroupContainer);
  aLayout->addWidget(lHandsGroup, lRowIndex++, 0);

  // Body ressources blocks
  auto lBodyGroup{new QGroupBox(tr("Body textures").append("  "), this)};
  Utils::addIconToGroupBox(lBodyGroup, lIconFolder, "body", this->mSettings.font.size);
  this->connect(lBodyGroup, &QGroupBox::toggled, this, &TexturesAssistant::groupBoxChecked);
  Utils::setGroupBoxState(lBodyGroup, false);

  auto lBodyGroupContainer{new QGridLayout(this)};
  lBodyGroupContainer->setSpacing(16);
  lBodyGroup->setLayout(lBodyGroupContainer);
  this->createRessourceBlock(lGroupedPaths.bodyTextures, lBodyGroupContainer);
  aLayout->addWidget(lBodyGroup, lRowIndex++, 0);

  // Other textures files
  auto lOtherGroup{new QGroupBox(tr("Other .DDS textures").append("  "), this)};
  Utils::addIconToGroupBox(lOtherGroup, lIconFolder, "textures", this->mSettings.font.size);
  this->connect(lOtherGroup, &QGroupBox::toggled, this, &TexturesAssistant::groupBoxChecked);
  Utils::setGroupBoxState(lOtherGroup, false);

  auto lOtherGroupContainer{new QGridLayout(this)};
  lOtherGroupContainer->setSpacing(16);
  lOtherGroup->setLayout(lOtherGroupContainer);
  this->createRessourceBlock(lOtherPaths, lOtherGroupContainer);
  aLayout->addWidget(lOtherGroup, lRowIndex++, 0);
}

void TexturesAssistant::createRessourceBlock(const std::map<std::string, std::vector<QString>>& aMap, QGridLayout* aLayout)
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

void TexturesAssistant::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};

  // Open a directory chooser dialog
  const auto& lContextPath{Utils::getPathFromKey(this->mLastPaths, "texturesAssistantInput", lLineEdit->text(), this->mSettings.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::updatePathAtKey(this->mLastPaths, "texturesAssistantInput", lPath);

  if (!this->mHasUserDoneSomething && lPath.compare("") != 0)
  {
    this->mHasUserDoneSomething = true;
  }

  // Enable or disable path viewer label and launch button
  auto lMustDisableButton{lPath.compare("", Qt::CaseInsensitive) == 0};
  lLineEdit->setDisabled(lMustDisableButton);
  lLaunchSearchButton->setDisabled(lMustDisableButton);

  this->displayHintZone();
}

void TexturesAssistant::launchSearchProcess()
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  const auto& lInputPath{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};

  // Warn the user if the scan found a BSA file
  if (Utils::getNumberFilesByExtensions(lInputPath, QStringList("*.bsa")) > 0)
  {
    if (Utils::displayQuestionMessage(this,
                                      tr("BSA file found"),
                                      tr("At least one BSA file was found in the scanned directory. Please note that the application cannot read the data contained in the BSA files, so it is advisable to decompress the BSA file before continuing the scan. Do you still want to continue the scan?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the scan"),
                                      tr("Cancel the scan"),
                                      "",
                                      this->mSettings.successColor,
                                      this->mSettings.dangerColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      return;
    }
  }

  // The root directory should at least contain a DDS file to be scanned.
  if (!QDir(QString("%1/textures").arg(lInputPath)).exists())
  {
    Utils::displayWarningMessage(tr("No \"textures\" directory has been found in the scanned directory."));
  }

  // Fetch all the "*dds" files
  const auto& lFoundDdsFiles{this->scanForFilesByExtension(lInputPath, "*.dds")};
  this->mScannedDirName = QDir(lInputPath).dirName();

  // No file found
  if (lFoundDdsFiles.groupedTextures.size() == 0 && lFoundDdsFiles.otherTextures.size() == 0)
  {
    this->displayHintZone();
    auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
    if (lHintZone)
    {
      lHintZone->setText(tr("No DDS file was found in the scanned directory."));
    }
    return;
  }

  // Delete already existing the hint label or the layout and the validation button
  this->deleteAlreadyExistingWindowBottom();

  // Create the scroll area chooser
  auto lDataContainer{ComponentFactory::createScrollAreaComponentLayout(this)};

  this->displayFoundTextures(lDataContainer, lFoundDdsFiles);
}

void TexturesAssistant::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);
}
