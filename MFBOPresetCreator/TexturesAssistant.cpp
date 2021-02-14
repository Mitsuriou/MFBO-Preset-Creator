#include "TexturesAssistant.h"

TexturesAssistant::TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent)
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
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  if (Utils::displayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the textures assistant window"),
                                    this->mSettings.dangerColor,
                                    this->mSettings.successColor,
                                    false)
      == ButtonClicked::Yes)
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
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Textures Assistant"));
  this->setWindowIcon(QIcon(QPixmap(":/black/textures")));
}

void TexturesAssistant::initializeGUI()
{
  // Main window layout
  auto lMainGrid{new QGridLayout(this)};
  lMainGrid->setAlignment(Qt::AlignTop);
  this->setLayout(lMainGrid);

  this->setupInterface();
}

void TexturesAssistant::setupInterface()
{
  // Get the window's layout
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // First line
  auto lInputPathLabel{new QLabel(tr("Input path:"), this)};
  lMainLayout->addWidget(lInputPathLabel, 0, 0);

  // Input label
  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  lInputPathLineEdit->setDisabled(true);
  lMainLayout->addWidget(lInputPathLineEdit, 0, 1);

  // Input chooser
  auto lInputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lInputPathChooser->setCursor(Qt::PointingHandCursor);
  lInputPathChooser->setIcon(QIcon(QPixmap(QString(":/%1/folder").arg(lIconFolder))));
  lInputPathChooser->setAutoDefault(false);
  lInputPathChooser->setDefault(false);
  lMainLayout->addWidget(lInputPathChooser, 0, 2);

  // Launch search button
  auto lLaunchSearchButton{new QPushButton(tr("Launch the scan of the mod"), this)};
  lLaunchSearchButton->setCursor(Qt::PointingHandCursor);
  lLaunchSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/search").arg(lIconFolder))));
  lLaunchSearchButton->setObjectName("launch_search_button");
  lLaunchSearchButton->setAutoDefault(false);
  lLaunchSearchButton->setDefault(false);
  lLaunchSearchButton->setDisabled(true);
  lMainLayout->addWidget(lLaunchSearchButton, 1, 0, 1, 3, Qt::AlignTop);

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

std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> TexturesAssistant::scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const
{
  // The map is storing <path+fileName, <path, fileName>>
  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> lScannedValues;

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
    it.next();

    // Get the current directory
    lRelativeDirPath = it.fileInfo().absolutePath().remove(aRootDir + "/", Qt::CaseInsensitive);

    lFileName = it.fileInfo().fileName();

    // If the file is not a body, hands or head textures file, continue to iterate
    if (lTexturesFilesToFind.contains(lFileName))
    {
      // Construct the key of the map
      lKey = QString("%1/textures/%2").arg(lRelativeDirPath).arg(lFileName).toStdString();

      // Insert the key-value into the map
      lScannedValues.insert(std::make_pair(lKey, std::make_pair(lRelativeDirPath, lFileName)));
    }
    else
    {
      // TODO: Add others DDS files in an complete other map
    }
  }

  return lScannedValues;
}

void TexturesAssistant::createFoundRessourcesBlocks(QGridLayout* aLayout, const TexturesAssistant::GroupedData& aGroupedPaths)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};
  auto lRowIndex{0};

  // General title
  aLayout->addWidget(new QLabel(tr("Textures related to a female body found during the scan:"), this), lRowIndex++, 0, 1, 3, Qt::AlignLeft);

  // Head icon and title
  auto lHeadIconLabel{new QLabel(this)};
  lHeadIconLabel->setPixmap(QPixmap(QString(":/%1/woman-head").arg(lIconFolder)).scaledToHeight(16, Qt::SmoothTransformation));
  aLayout->addWidget(lHeadIconLabel, lRowIndex, 0, 1, 1, Qt::AlignLeft);
  aLayout->addWidget(new QLabel(tr("Head textures:"), this), lRowIndex++, 1, 1, 2, Qt::AlignLeft);

  // Head ressources blocks
  for (const auto& lRootPath : aGroupedPaths.headTextures)
  {
    auto lConcatenatedFileNames{QString()};

    for (const auto& lFileName : lRootPath.second)
    {
      lConcatenatedFileNames.append(QString("%1\n").arg(lFileName));
    }
    lConcatenatedFileNames = lConcatenatedFileNames.left(lConcatenatedFileNames.length() - 1);

    aLayout->addWidget(new QLabel(QString::fromStdString(lRootPath.first), this), lRowIndex, 1, 1, 1, Qt::AlignLeft);
    aLayout->addWidget(new QLabel(lConcatenatedFileNames, this), lRowIndex++, 2, 1, 2, Qt::AlignLeft);
  }

  // Hands icon and title
  auto lHandsIconLabel{new QLabel(this)};
  lHandsIconLabel->setPixmap(QPixmap(QString(":/%1/hand").arg(lIconFolder)).scaledToHeight(16, Qt::SmoothTransformation));
  aLayout->addWidget(lHandsIconLabel, lRowIndex, 0, 1, 1, Qt::AlignLeft);
  aLayout->addWidget(new QLabel(tr("Hands textures:"), this), lRowIndex++, 1, 1, 2, Qt::AlignLeft);

  // Hands ressources blocks
  for (const auto& lRootPath : aGroupedPaths.handsTextures)
  {
    auto lConcatenatedFileNames{QString()};

    for (const auto& lFileName : lRootPath.second)
    {
      lConcatenatedFileNames.append(QString("%1\n").arg(lFileName));
    }
    lConcatenatedFileNames = lConcatenatedFileNames.left(lConcatenatedFileNames.length() - 1);

    aLayout->addWidget(new QLabel(QString::fromStdString(lRootPath.first), this), lRowIndex, 1, 1, 1, Qt::AlignLeft);
    aLayout->addWidget(new QLabel(lConcatenatedFileNames, this), lRowIndex++, 2, 1, 2, Qt::AlignLeft);
  }

  // Body icon and title
  auto lBodyIconLabel{new QLabel(this)};
  lBodyIconLabel->setPixmap(QPixmap(QString(":/%1/body").arg(lIconFolder)).scaledToHeight(16, Qt::SmoothTransformation));
  aLayout->addWidget(lBodyIconLabel, lRowIndex, 0, 1, 1, Qt::AlignLeft);
  aLayout->addWidget(new QLabel(tr("Body textures:"), this), lRowIndex++, 1, 1, 2, Qt::AlignLeft);

  // Body ressources blocks
  for (const auto& lRootPath : aGroupedPaths.bodyTextures)
  {
    auto lConcatenatedFileNames{QString()};

    for (const auto& lFileName : lRootPath.second)
    {
      lConcatenatedFileNames.append(QString("%1\n").arg(lFileName));
    }
    lConcatenatedFileNames = lConcatenatedFileNames.left(lConcatenatedFileNames.length() - 1);

    aLayout->addWidget(new QLabel(QString::fromStdString(lRootPath.first), this), lRowIndex, 1, 1, 1, Qt::AlignLeft);
    aLayout->addWidget(new QLabel(lConcatenatedFileNames, this), lRowIndex++, 2, 1, 2, Qt::AlignLeft);
  }

  // Other textures files title
  aLayout->addWidget(new QLabel(tr("Other textures files found during the scan:"), this), lRowIndex++, 0, 1, 3, Qt::AlignLeft);
}

void TexturesAssistant::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>("launch_search_button")};
  auto lLineEdit{this->findChild<QLineEdit*>("input_path_directory")};

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
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  const auto& lInputPath{this->findChild<QLineEdit*>("input_path_directory")->text()};

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
                                      this->mSettings.successColor,
                                      this->mSettings.dangerColor,
                                      true)
        != ButtonClicked::Yes)
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
  const auto& lFoundNifFiles{this->scanForFilesByExtension(lInputPath, "*.dds")};
  this->mScannedDirName = QDir(lInputPath).dirName();

  // No file found
  if (lFoundNifFiles.size() == 0)
  {
    this->displayHintZone();
    auto lHintZone{this->findChild<QLabel*>("hint_zone")};
    if (lHintZone)
    {
      lHintZone->setText(tr("No DDS files were found in the scanned directory."));
    }
    return;
  }

  // Delete already existing the hint label or the layout and the validation button
  this->deleteAlreadyExistingWindowBottom();

  // Create the scroll area chooser
  auto lScrollArea{new QScrollArea(this)};
  lScrollArea->setObjectName("scrollable_zone");
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->setWidgetResizable(true);

  auto lMainWidget{new QFrame(this)};
  lScrollArea->setWidget(lMainWidget);

  auto lDataContainer{new QGridLayout(this)};
  lDataContainer->setObjectName("data_container");
  lDataContainer->setAlignment(Qt::AlignTop);
  lDataContainer->setContentsMargins(0, 0, 0, 0);
  lDataContainer->setColumnStretch(0, 0);
  lDataContainer->setColumnStretch(1, 0);
  lDataContainer->setColumnStretch(2, 1);

  lMainWidget->setLayout(lDataContainer);

  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};
  lMainLayout->addWidget(lScrollArea, 2, 0, 1, 3);

  TexturesAssistant::GroupedData lGroupedPaths;

  for (const auto& lNifFile : lFoundNifFiles)
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

  this->createFoundRessourcesBlocks(lDataContainer, lGroupedPaths);

  // Cursor change for the scroll bar
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderPressed, this, &TexturesAssistant::scrollbarPressed);
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderReleased, this, &TexturesAssistant::scrollbarReleased);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderPressed, this, &TexturesAssistant::scrollbarPressed);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderReleased, this, &TexturesAssistant::scrollbarReleased);
}

void TexturesAssistant::scrollbarPressed()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::ClosedHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::ClosedHandCursor);
}

void TexturesAssistant::scrollbarReleased()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
}
