#include "AssistedConversion.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Utils.h"
#include <QAbstractItemView>
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDirIterator>
#include <QDomDocument>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QListView>
#include <QNetworkReply>
#include <QProgressBar>
#include <QProgressDialog>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

AssistedConversion::AssistedConversion(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
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
  aSettings.display.assistedConversionDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

void AssistedConversion::closeEvent(QCloseEvent* aEvent)
{
  // Catch the sender of the event
  auto lEventSource{qobject_cast<QPushButton*>(this->sender())};
  auto lValidationBtn{this->findChild<QPushButton*>(QString("validate_selection"))};

  if ((lEventSource == lValidationBtn) || !this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the assisted conversion tool window"),
                                    "",
                                    this->mSettings.display.dangerColor,
                                    this->mSettings.display.successColor,
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

void AssistedConversion::reject()
{
  this->close();
}

void AssistedConversion::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Assisted Conversion"));
  this->setWindowIcon(QIcon(QPixmap(":/black/pencil")));
}

void AssistedConversion::initializeGUI()
{
  // Main window layout
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setRowStretch(3, 1); // Make the hint zone as high as possible
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Tab widget
  auto lTabWidget{new QTabWidget(this)};
  lTabWidget->setObjectName(QString("tab_widget"));
  lTabWidget->setAutoFillBackground(true);
  lTabWidget->tabBar()->setCursor(Qt::CursorShape::PointingHandCursor);
  lMainLayout->addWidget(lTabWidget, 0, 0, Qt::AlignTop);

  this->setupFromLocalFolderTab(*lTabWidget);
  this->setupFromURLTab(*lTabWidget);

  // Scan only "meshes" directory
  auto lScanMeshesSubdirsOnly{ComponentFactory::CreateCheckBox(this, tr("Only scan the \"meshes\" subdirectory of the mod"), "", "only_scan_meshes_dir", true)};
  lMainLayout->addWidget(lScanMeshesSubdirsOnly, 1, 0, Qt::AlignTop);

  // Launch search button
  auto lLaunchSearchButton{ComponentFactory::CreateButton(this, tr("Launch the scan of the mod"), "", "search", lIconFolder, "launch_search_button", true, true)};
  lMainLayout->addWidget(lLaunchSearchButton, 2, 0, Qt::AlignTop);

  // Hint zone
  this->displayHintZone();

  // Event binding
  this->connect(lTabWidget, &QTabWidget::currentChanged, this, QOverload<int>::of(&AssistedConversion::updateLaunchSearchButtonState));
  this->connect(lLaunchSearchButton, &QPushButton::clicked, this, &AssistedConversion::launchSearchProcess);
}

void AssistedConversion::setupFromLocalFolderTab(QTabWidget& aTabWidget)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QHBoxLayout(lTabContent)};
  lTabLayout->setStretch(0, 0);
  lTabLayout->setStretch(1, 1);
  lTabLayout->setStretch(2, 0);
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/folder").arg(lIconFolder))), tr("From local folder"));

  // Input path label
  lTabLayout->addWidget(new QLabel(tr("Input path:"), this));

  // Input path value
  auto lInputPathLineEdit{new QLineEdit(this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setObjectName(QString("input_path_directory"));
  lInputPathLineEdit->setDisabled(true);
  lTabLayout->addWidget(lInputPathLineEdit);

  // Input chooser
  auto lInputPathChooser{ComponentFactory::CreateButton(this, tr("Choose a directory..."), "", "folder", lIconFolder, "", false, true)};
  lTabLayout->addWidget(lInputPathChooser);

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &AssistedConversion::chooseInputDirectory);
}

void AssistedConversion::setupFromURLTab(QTabWidget& aTabWidget)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setColumnStretch(0, 0);
  lTabLayout->setColumnStretch(1, 1);
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/nexus-logo").arg(lIconFolder))), tr("From NexusMods URL"));

  // Input mod's URL/ID label
  lTabLayout->addWidget(new QLabel(tr("Input mod's URL or ID:"), this), 0, 0, Qt::AlignTop);

  // Input mod's URL/ID value
  auto lModURLOrIDLineEdit{new QLineEdit(this)};
  lModURLOrIDLineEdit->setObjectName(QString("mod_url_or_id"));
  lModURLOrIDLineEdit->setPlaceholderText(tr("https://www.nexusmods.com/skyrimspecialedition/mods/XXXXX"));
  lTabLayout->addWidget(lModURLOrIDLineEdit, 0, 1, 1, 2, Qt::AlignTop);

  // API Key label
  lTabLayout->addWidget(new QLabel(tr("API Key:"), this), 1, 0, Qt::AlignTop);

  // API Key value
  auto lAPIKeyLineEdit{new QLineEdit(this)};
  lAPIKeyLineEdit->setObjectName(QString("api_key"));
  lAPIKeyLineEdit->setPlaceholderText(tr("Enter your NexusMods API key here"));
  lAPIKeyLineEdit->setText(Utils::ReadAPIKeyFromFile());
  lTabLayout->addWidget(lAPIKeyLineEdit, 1, 1, Qt::AlignTop);

  // Input chooser
  auto lSaveAPIKey{ComponentFactory::CreateButton(this, tr("Save API key"), "", "save", lIconFolder, "save_api_key", false, true)};
  lTabLayout->addWidget(lSaveAPIKey, 1, 2, Qt::AlignTop);

  // Event binding
  this->connect(lModURLOrIDLineEdit, &QLineEdit::textChanged, this, QOverload<const QString&>::of(&AssistedConversion::updateLaunchSearchButtonState));
  this->connect(lAPIKeyLineEdit, &QLineEdit::textChanged, this, QOverload<const QString&>::of(&AssistedConversion::updateLaunchSearchButtonState));
  this->connect(lAPIKeyLineEdit, &QLineEdit::textChanged, this, QOverload<const QString&>::of(&AssistedConversion::updateSaveAPIKeyButtonState));
  this->connect(lSaveAPIKey, &QPushButton::clicked, this, &AssistedConversion::saveAPIKey);

  // Post-bind initialization functions
  this->updateSaveAPIKeyButtonState(true);
}

void AssistedConversion::displayHintZone()
{
  this->deleteAlreadyExistingWindowBottom();

  // Get the window's layout
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};
  auto lHintZone{new QLabel(tr("Awaiting the launch of a scan..."), this)};
  lHintZone->setMinimumHeight(300);
  lHintZone->setObjectName(QString("hint_zone"));
  lHintZone->setAlignment(Qt::AlignCenter);
  lMainLayout->addWidget(lHintZone, 3, 0);
}

void AssistedConversion::deleteAlreadyExistingWindowBottom() const
{
  auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
  if (lHintZone)
  {
    delete lHintZone;
    lHintZone = nullptr;
  }

  auto lOldValidationButton{this->findChild<QPushButton*>(QString("validate_selection"))};
  if (lOldValidationButton)
  {
    delete lOldValidationButton;
    lOldValidationButton = nullptr;
  }

  auto lOldScrollArea{this->findChild<QScrollArea*>(QString("scrollable_zone"))};
  if (lOldScrollArea)
  {
    delete lOldScrollArea;
    lOldScrollArea = nullptr;
  }
}

void AssistedConversion::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};

  // Open a directory chooser dialog
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "assistedConversionInput", lLineEdit->text(), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "assistedConversionInput", lPath);

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

void AssistedConversion::saveAPIKey()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  const auto lAPIKeyLineEdit{this->findChild<QLineEdit*>("api_key")};
  const auto lSucceed{Utils::SaveAPIKeyToFile(lAPIKeyLineEdit->text(), this, lIconFolder)};

  this->updateSaveAPIKeyButtonState(lSucceed);
}

void AssistedConversion::updateSaveAPIKeyButtonState(const QString&)
{
  this->updateSaveAPIKeyButtonState(false);
}

void AssistedConversion::updateSaveAPIKeyButtonState(const bool aMustBeDisabled)
{
  const auto lSaveAPIKey{this->findChild<QPushButton*>(QString("save_api_key"))};
  lSaveAPIKey->setDisabled(aMustBeDisabled);
}

void AssistedConversion::updateLaunchSearchButtonState(const QString&)
{
  const auto lTabWidget{this->findChild<QTabWidget*>("tab_widget")};
  this->updateLaunchSearchButtonState(lTabWidget->currentIndex());
}

void AssistedConversion::updateLaunchSearchButtonState(int aCurrentTabIndex)
{
  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};

  switch (aCurrentTabIndex)
  {
    case 0:
    {
      // Local folder
      const auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};
      lLaunchSearchButton->setDisabled(lLineEdit->text().isEmpty());
      break;
    }
    case 1:
    {
      // NexusMods URL/ID
      const auto lModURLOrIDLineEdit{this->findChild<QLineEdit*>(QString("mod_url_or_id"))};
      const auto lAPIKeyLineEdit{this->findChild<QLineEdit*>("api_key")};

      lLaunchSearchButton->setDisabled(lModURLOrIDLineEdit->text().isEmpty() || lAPIKeyLineEdit->text().isEmpty());

      break;
    }
  }
}

bool AssistedConversion::isFileNameRecognized(const QString& aFileName)
{
  return (aFileName.compare("skeleton_female", Qt::CaseInsensitive) == 0
          || aFileName.compare("skeletonbeast_female", Qt::CaseInsensitive) == 0
          || aFileName.compare("femalehands", Qt::CaseInsensitive) == 0
          || aFileName.compare("femalefeet", Qt::CaseInsensitive) == 0
          || aFileName.compare("femalebody", Qt::CaseInsensitive) == 0);
}

void AssistedConversion::launchSearchProcess()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  if (this->hasUserSelectedAnything())
  {
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Relaunch the scan"),
                                      tr("You will lose all the unsaved data. Do you still want to relaunch the scan?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Relaunch the scan"),
                                      tr("Cancel the relaunch"),
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

  const auto lTabWidget{this->findChild<QTabWidget*>("tab_widget")};
  switch (lTabWidget->currentIndex())
  {
    case 0:
      // Local folder
      this->launchSearchFromLocalFolder();
      break;
    case 1:
      // NexusMods URL/ID
      this->launchSearchNexusModsURL();
      break;
  }
}

void AssistedConversion::launchSearchFromLocalFolder()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  const auto& lInputPath{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};

  // The root directory should at least contain an ESP, ESL or ESM file to be scanned.
  if (Utils::GetNumberFilesByExtensions(lInputPath, QStringList({"*.esl", "*.esm", "*.esp"})) == 0)
  {
    // If it does not contain one of the wanted files, ask the user to start or cancel the scan
    if (Utils::DisplayQuestionMessage(this,
                                      tr("No root file has been found"),
                                      tr("No ESL, ESM or ESP files were found in the scanned directory. Do you still want to continue the scan?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the scan"),
                                      tr("Cancel the scan"),
                                      "",
                                      this->mSettings.display.warningColor,
                                      this->mSettings.display.successColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      return;
    }
  }

  // Warn the user if the scan found a BSA file
  if (Utils::GetNumberFilesByExtensions(lInputPath, QStringList("*.bsa")) > 0)
  {
    if (Utils::DisplayQuestionMessage(this,
                                      tr("BSA file found"),
                                      tr("At least one BSA file was found in the scanned directory. Please note that the application cannot read the data contained in the BSA files, so it is advisable to decompress the BSA file before continuing the scan. Do you still want to continue the scan?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the scan"),
                                      tr("Cancel the scan"),
                                      "",
                                      this->mSettings.display.warningColor,
                                      this->mSettings.display.successColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      return;
    }
  }

  // Fetch all the "*.nif" files
  const auto lFoundNifFiles{this->scanForFilesByExtension(lInputPath, "*.nif")};
  this->mScannedDirName = QDir(lInputPath).dirName();

  this->displayObtainedData(lFoundNifFiles);
}

std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> AssistedConversion::scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const
{
  // Progress bar
  auto lProgressBar{new QProgressBar(this->parentWidget())};
  lProgressBar->setFormat("");
  lProgressBar->setMinimum(0);
  lProgressBar->setMaximum(0);
  lProgressBar->setValue(0);
  lProgressBar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Scanning the directory. Please wait..."), tr("Cancel treatment"), 0, 0, this->parentWidget());
  lProgressDialog.setBar(lProgressBar);
  lProgressDialog.setModal(true);
  lProgressDialog.show();

  QCoreApplication::processEvents();

  // The map is storing <path+fileName, <path, fileName>>
  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> lScannedValues;

  QString lRelativeDirPath;
  QString lFileName;
  std::string lKey;

  auto lRootDir{aRootDir};
  if (this->findChild<QCheckBox*>(QString("only_scan_meshes_dir"))->isChecked())
  {
    lRootDir.append("/meshes");
  }

  QDirIterator it(lRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::DisplayWarningMessage(tr("Process aborted by the user."));
      return {};
    }

    it.next();

    // Get the current directory
    lRelativeDirPath = it.fileInfo().absolutePath().remove(aRootDir + "/", Qt::CaseInsensitive);

    lFileName = it.fileInfo().fileName();

    // Clean the file name from any artifact
    lFileName.remove("_0.nif", Qt::CaseInsensitive);
    lFileName.remove("_1.nif", Qt::CaseInsensitive);
    lFileName.remove(".nif", Qt::CaseInsensitive);

    // Construct the key of the map
    lKey = QString("%1/%2").arg(lRelativeDirPath, lFileName).toStdString();

    // Insert the key-value into the map
    lScannedValues.insert(std::make_pair(lKey, std::make_pair(lRelativeDirPath, lFileName)));
  }

  return lScannedValues;
}

void AssistedConversion::launchSearchNexusModsURL()
{
  const auto lModURLOrID{this->findChild<QLineEdit*>(QString("mod_url_or_id"))->text()};
  const auto lURLPattern{QString("nexusmods.com/skyrimspecialedition/mods/")};
  auto lModID{0};

  // ID
  bool lIsInteger;
  lModURLOrID.toInt(&lIsInteger);
  if (lIsInteger)
  {
    lModID = lModURLOrID.toInt();
  }
  // URL
  else if (lModURLOrID.contains(lURLPattern))
  {
    const auto lStart{lModURLOrID.indexOf(lURLPattern)};
    for (const auto& lChar : lModURLOrID.mid(lStart + lURLPattern.length()))
    {
      if (lChar.isDigit())
      {
        lModID = lModID * 10 + lChar.digitValue();
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    // Invalid format
    return;
  }

  this->requestModInformation(lModID);
}

void AssistedConversion::requestModInformation(const int aModID)
{
  const auto lURL{QString("https://api.nexusmods.com/v1/games/skyrimspecialedition/mods/%1/files.json").arg(QString::number(aModID))};

  QNetworkRequest lRequest{QUrl(lURL)};
  lRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  lRequest.setRawHeader("apikey", this->findChild<QLineEdit*>("api_key")->text().toUtf8());
  QNetworkReply* lReply{this->mManager.get(lRequest)};
  connect(lReply, &QNetworkReply::finished, this, &AssistedConversion::requestModInformationFinished);
}

void AssistedConversion::requestModInformationFinished()
{
  auto lReply{qobject_cast<QNetworkReply*>(this->sender())};

  std::vector<Struct::NexusModsFileInformation> lParsedFilesInformation;
  if (lReply->error() == QNetworkReply::NoError)
  {
    lParsedFilesInformation = this->parseFilesListFromModInformation(true, lReply->readAll());
  }
  else
  {
    lParsedFilesInformation = this->parseFilesListFromModInformation(false, "");
  }

  lReply->deleteLater();

  this->displayFileIDPicker(lParsedFilesInformation);
}

std::vector<Struct::NexusModsFileInformation> AssistedConversion::parseFilesListFromModInformation(const bool aSucceeded, const QByteArray& aResult)
{
  if (!aSucceeded)
  {
    Utils::DisplayErrorMessage(tr("An error has occurred... Make sure your internet connection is operational and try again."));
    return std::vector<Struct::NexusModsFileInformation>();
  }

  std::vector<Struct::NexusModsFileInformation> lList;

  const auto lJson{QJsonDocument::fromJson(aResult)};
  const auto lRootElement{lJson.object()};
  if (lRootElement.contains("files") && lRootElement["files"].isArray())
  {
    const auto lFiles{lRootElement["files"].toArray()};
    for (const auto& lFile : lFiles)
    {
      lList.push_back(Struct::NexusModsFileInformation(lFile["file_id"].toInt(),
                                                       lFile["name"].toString(),
                                                       lFile["uploaded_timestamp"].toInt(),
                                                       lFile["version"].toString()));
    }
  }
  else
  {
    Utils::DisplayErrorMessage(tr("Parse error: could not find the \"files\" element."));
    return std::vector<Struct::NexusModsFileInformation>();
  }

  return lList;
}

void AssistedConversion::displayFileIDPicker(const std::vector<Struct::NexusModsFileInformation>& aFilesInformation)
{
  if (aFilesInformation.empty())
    return;

  // TODO: Create the new UI
  // TODO: Display the brand new UI

  this->requestModFileContent(""); // TODO: conect this function to a callback of the new UI
}

void AssistedConversion::requestModFileContent(const QString& aContentPreviewLink)
{
  QNetworkRequest lRequest{QUrl(aContentPreviewLink)};
  lRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  lRequest.setRawHeader("apikey", this->findChild<QLineEdit*>("api_key")->text().toUtf8());
  QNetworkReply* lReply{this->mManager.get(lRequest)};
  connect(lReply, &QNetworkReply::finished, this, &AssistedConversion::requestModFileContentFinished);
}

void AssistedConversion::requestModFileContentFinished()
{
  // TODO: parse the data to have the type "std::map<std::string, std::pair<QString, QString>, std::greater<std::string>>"
  this->displayObtainedData(std::map<std::string, std::pair<QString, QString>, std::greater<std::string>>());
}

void AssistedConversion::displayObtainedData(const std::map<std::string, std::pair<QString, QString>, std::greater<std::string>>& aFoundNifFiles)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // No file found
  if (aFoundNifFiles.empty())
  {
    this->displayHintZone();
    auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
    if (lHintZone)
    {
      lHintZone->setText(tr("No NIF file was found in the scanned directory."));
    }
    return;
  }

  // Delete already existing the hint label or the layout and the validation button
  this->deleteAlreadyExistingWindowBottom();

  // Create the scroll area chooser
  auto lDataContainer{ComponentFactory::CreateScrollAreaComponentLayout(this, 3, 0, 1, 3)};
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};

  // Columns header
  lDataContainer->addWidget(new QLabel(tr("File path"), this), 0, 0, Qt::AlignCenter);
  lDataContainer->addWidget(new QLabel(tr("*.nif file name"), this), 0, 1, Qt::AlignCenter);
  lDataContainer->addWidget(new QLabel(tr("Action"), this), 0, 2, Qt::AlignCenter);

  auto lNextRow{1};
  this->mBoxSelectedIndexes.clear();

  std::vector<std::pair<QString, QString>> lNifFilesInformation;
  for (const auto& lNifFile : aFoundNifFiles)
  {
    lNifFilesInformation.push_back(lNifFile.second);
  }

  std::sort(lNifFilesInformation.begin(), lNifFilesInformation.end(), [=](const std::pair<QString, QString>& lhs, const std::pair<QString, QString>& rhs) {
    return isFileNameRecognized(lhs.second) && !isFileNameRecognized(rhs.second);
  });

  for (const auto& lNifFileInformation : lNifFilesInformation)
  {
    this->createSelectionBlock(*lDataContainer, lNifFileInformation.second, lNifFileInformation.first, lNextRow++);
  }

  // Create the validation button
  auto lValidateSelection{ComponentFactory::CreateButton(this, tr("Validate the selection(s) above and go back to the main window"), "", "playlist-check", lIconFolder, "validate_selection")};
  lMainLayout->addWidget(lValidateSelection, 4, 0);

  this->connect(lValidateSelection, &QPushButton::clicked, this, &AssistedConversion::validateSelection);
}

void AssistedConversion::createSelectionBlock(QGridLayout& aLayout, const QString& aFileName, const QString& aPath, const int aRowIndex)
{
  const auto lMustUseBoldFont{isFileNameRecognized(aFileName)};
  const auto lFontSize{qApp->font().pointSize()};

  // Path
  auto lPathLabel{new QLabel(aPath, this)};
  lPathLabel->setTextFormat(Qt::TextFormat::RichText);

  const auto lFirstTagStart{QString("<span style=\"font-weight: 900; font-size: %1pt;\">").arg(lFontSize + 1)};
  const auto lSecondTagStart{QString("<span style=\"font-weight: 100; font-style: italic; font-size: %1pt;\">").arg(lFontSize - 1)};
  const auto lTagEnd{QString("</span>")};

  if (lMustUseBoldFont)
    lPathLabel->setText(lFirstTagStart + aPath + lTagEnd);
  else
    lPathLabel->setText(lSecondTagStart + aPath + lTagEnd);

  aLayout.addWidget(lPathLabel, aRowIndex, 0, Qt::AlignLeft);

  // File name
  auto lFileNameLabel{new QLabel(aFileName, this)};
  lFileNameLabel->setTextFormat(Qt::TextFormat::RichText);

  if (lMustUseBoldFont)
    lFileNameLabel->setText(lFirstTagStart + aFileName + lTagEnd);
  else
    lFileNameLabel->setText(lSecondTagStart + aFileName + lTagEnd);

  aLayout.addWidget(lFileNameLabel, aRowIndex, 1, Qt::AlignLeft);

  // Choice combobox
  auto lChoiceCombo{new QComboBox(this)};
  lChoiceCombo->addItems(DataLists::GetAssistedConversionActions());
  lChoiceCombo->setCursor(Qt::PointingHandCursor);
  lChoiceCombo->setMouseTracking(true);
  lChoiceCombo->view()->setMouseTracking(true);
  lChoiceCombo->setItemDelegate(new QStyledItemDelegate());
  aLayout.addWidget(lChoiceCombo, aRowIndex, 2, Qt::AlignRight);

  // Add a new index store to the list
  this->mBoxSelectedIndexes.push_back(0);

  // Event binding
  this->connect(lChoiceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AssistedConversion::modifyComboBoxLockState);
}

void AssistedConversion::modifyComboBoxLockState(int aIndex)
{
  // Catch the sender of the event
  auto lEventSource{qobject_cast<QComboBox*>(this->sender())};

  // Fetch the grid layout
  auto lDataContainer{this->findChild<QGridLayout*>(QString("data_container"))};
  auto lLinesToTreat{lDataContainer->rowCount()};
  QComboBox* lComboBox{nullptr};
  auto lComboBoxIndex{0};

  // For each row (skip the row 0 because it is a "header")
  for (int i = 1; i < lLinesToTreat; i++)
  {
    // Get the combobox
    lComboBox = qobject_cast<QComboBox*>(lDataContainer->itemAtPosition(i, 2)->widget());

    if (lComboBox == lEventSource)
    {
      lComboBoxIndex = i - 1;
      break;
    }
  }

  // Check if an option can be set before actually setting it.
  // This will prevent the mouse scrolling bug that permits the user
  // to select a hidden option from the combobox.
  if (aIndex != 0)
  {
    auto lStep{1};
    if (aIndex < this->mBoxSelectedIndexes.at(lComboBoxIndex))
    {
      // If the user took an option that is placed first
      lStep = -1;
    }

    auto lSourceView{qobject_cast<QListView*>(lEventSource->view())};

    auto lTestIndex{aIndex};
    while (lSourceView->isRowHidden(lTestIndex) && lTestIndex <= lEventSource->count())
    {
      lTestIndex += lStep;
    }

    this->disconnect(lEventSource, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AssistedConversion::modifyComboBoxLockState);

    if (lTestIndex == lEventSource->count())
    {
      lEventSource->setCurrentIndex(this->mBoxSelectedIndexes.at(lComboBoxIndex));
      this->connect(lEventSource, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AssistedConversion::modifyComboBoxLockState);
      return;
    }

    lEventSource->setCurrentIndex(lTestIndex);
    aIndex = lTestIndex;
    this->connect(lEventSource, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AssistedConversion::modifyComboBoxLockState);
  }

  // Keep the old index and store the new selected index
  auto lOldIndex{this->mBoxSelectedIndexes.at(lComboBoxIndex)};
  this->mBoxSelectedIndexes.at(lComboBoxIndex) = aIndex;

  // For each row (skip the row 0 because it is a "header")
  for (int i = 1; i < lLinesToTreat; i++)
  {
    // Get the combobox
    lComboBox = qobject_cast<QComboBox*>(lDataContainer->itemAtPosition(i, 2)->widget());

    if (lComboBox == lEventSource)
    {
      continue;
    }

    auto lView{qobject_cast<QListView*>(lComboBox->view())};

    // Disable the new selected option
    if (aIndex != 0)
    {
      lView->setRowHidden(aIndex, true);
    }

    // Enable the old option
    auto lModel{qobject_cast<QStandardItemModel*>(lComboBox->model())};
    auto lItem{lModel->item(lOldIndex)};
    lView->setRowHidden(lOldIndex, false);
    lItem->setFlags(lItem->flags() | Qt::ItemIsEnabled);
  }
}

bool AssistedConversion::hasUserSelectedAnything() const
{
  for (const auto& lIndex : this->mBoxSelectedIndexes)
  {
    // The first index that is different than 0 means that a value has been selected
    if (lIndex != 0)
    {
      return true;
    }
  }

  // The user did not select any value
  return false;
}

std::vector<Struct::AssistedConversionResult> AssistedConversion::getChosenValuesFromInterface() const
{
  // Fetch the grid layout
  auto lDataContainer{this->findChild<QGridLayout*>(QString("data_container"))};

  // Iterate in the layout
  auto lLinesToTreat{lDataContainer->rowCount()};

  if (lLinesToTreat <= 1)
  {
    return std::vector<Struct::AssistedConversionResult>();
  }

  std::vector<Struct::AssistedConversionResult> lResults;
  QComboBox* lComboBox{nullptr};
  QString lFilePath;
  QString lFileName;

  // For each row (skip the row 0 because it is a "header")
  for (int i = 1; i < lLinesToTreat; i++)
  {
    lComboBox = qobject_cast<QComboBox*>(lDataContainer->itemAtPosition(i, 2)->widget());

    // Third column is the chosen action on the line
    if (lComboBox->currentIndex() == 0)
    {
      continue;
    }

    // First column is the file path
    lFilePath = qobject_cast<QLabel*>(lDataContainer->itemAtPosition(i, 0)->widget())->text();
    if (lFilePath.startsWith("<span"))
    {
      // Parse the inner text of the XML span tag
      QDomDocument lDoc;
      lDoc.setContent(lFilePath);
      const auto lElementToParse{lDoc.documentElement()};

      lFilePath = lElementToParse.text();
    }

    // Second column is the file name
    lFileName = qobject_cast<QLabel*>(lDataContainer->itemAtPosition(i, 1)->widget())->text();
    if (lFileName.startsWith("<span"))
    {
      // Parse the inner text of the XML span tag
      QDomDocument lDoc;
      lDoc.setContent(lFileName);
      const auto lElementToParse{lDoc.documentElement()};

      lFileName = lElementToParse.text();
    }

    // Save the gotten values
    lResults.push_back(Struct::AssistedConversionResult(lFilePath, lFileName, lComboBox->currentIndex()));
  }

  return lResults;
}

void AssistedConversion::validateSelection()
{
  auto lValues{this->getChosenValuesFromInterface()};

  if (lValues.empty())
  {
    // If not any file as been tagged, ask the user to continue or reselect values
    if (Utils::DisplayQuestionMessage(this,
                                      tr("No entry selected"),
                                      tr("You did not select any entry. Do you still want to validate this selection as is?"),
                                      Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme),
                                      "help-circle",
                                      tr("Validate as is"),
                                      tr("Cancel, I wanted to select values"),
                                      "",
                                      this->mSettings.display.dangerColor,
                                      this->mSettings.display.successColor,
                                      "",
                                      true)
        != ButtonClicked::YES)
    {
      return;
    }
  }

  emit valuesChosen(this->mScannedDirName, lValues);
  this->close();
}
