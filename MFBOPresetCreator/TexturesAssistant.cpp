#include "TexturesAssistant.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "FileIDPicker.h"
#include "Utils.h"
#include <QAbstractSlider>
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkReply>
#include <QProgressBar>
#include <QProgressDialog>
#include <QPushButton>
#include <QScrollArea>
#include <QStyledItemDelegate>

TexturesAssistant::TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mFileWatcher(new QFileSystemWatcher())
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mHasUserDoneSomething(false)
  , mMinimumFirstColumnWidth(300)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  aSettings.display.texturesAssistantDialogOpeningMode == DialogOpeningMode::WINDOWED ? this->show() : this->showMaximized();
}

TexturesAssistant::~TexturesAssistant()
{
  this->mFileWatcher->removePaths(this->mFileWatcher->files());
  delete this->mFileWatcher;
}

void TexturesAssistant::closeEvent(QCloseEvent* aEvent)
{
  if (!this->mHasUserDoneSomething)
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
                                    tr("Go back to the textures assistant window"),
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
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Main layout
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setRowStretch(2, 1); // Make the hint zone as high as possible
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  // Tab widget
  auto lTabWidget{new QTabWidget(this)};
  lTabWidget->setObjectName(QString("tab_widget"));
  lTabWidget->setAutoFillBackground(true);
  lTabWidget->tabBar()->setCursor(Qt::CursorShape::PointingHandCursor);
  lMainLayout->addWidget(lTabWidget, 0, 0, Qt::AlignTop);

  this->setupFromLocalFolderTab(*lTabWidget);
  this->setupFromURLTab(*lTabWidget);

  // Launch search button
  auto lLaunchSearchButton{ComponentFactory::CreateButton(this, tr("Launch the scan of the mod"), "", "search", lIconFolder, "launch_search_button", true, true)};
  lMainLayout->addWidget(lLaunchSearchButton, 1, 0, Qt::AlignTop);

  // Setup all the different GUI components
  this->displayHintZone();
  this->setupTexturesSetGUI(*lMainLayout);
  this->setupOutputBox(*lMainLayout);
  this->setupButtons(*lMainLayout);

  // Event binding
  QObject::connect(lTabWidget, &QTabWidget::currentChanged, this, QOverload<int>::of(&TexturesAssistant::updateLaunchSearchButtonState));
  QObject::connect(lLaunchSearchButton, &QPushButton::clicked, this, &TexturesAssistant::launchSearchProcess);

  QObject::connect(this->mFileWatcher, &QFileSystemWatcher::directoryChanged, this, &TexturesAssistant::updateOutputPreview);
}

void TexturesAssistant::setupFromLocalFolderTab(QTabWidget& aTabWidget)
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

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/folder").arg(lIconFolder))), tr("From local directory"));

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
  QObject::connect(lInputPathChooser, &QPushButton::clicked, this, &TexturesAssistant::chooseInputDirectory);
}

void TexturesAssistant::setupFromURLTab(QTabWidget& aTabWidget)
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
  lTabLayout->addWidget(new QLabel(tr("Mod's URL or ID:"), this), 0, 0, Qt::AlignTop);

  // Input mod's URL/ID value
  auto lModURLOrIDLineEdit{new QLineEdit(this)};
  lModURLOrIDLineEdit->setObjectName(QString("mod_url_or_id"));
  lModURLOrIDLineEdit->setPlaceholderText(tr("https://www.nexusmods.com/skyrimspecialedition/mods/XXXXX"));
  lTabLayout->addWidget(lModURLOrIDLineEdit, 0, 1, 1, 3, Qt::AlignTop);

  // API Key label
  lTabLayout->addWidget(new QLabel(tr("API Key:"), this), 1, 0, Qt::AlignTop);

  // API Key value
  auto lAPIKeyLineEdit{new QLineEdit(this)};
  lAPIKeyLineEdit->setObjectName(QString("api_key"));
  lAPIKeyLineEdit->setPlaceholderText(tr("Enter your NexusMods API key here"));
  lAPIKeyLineEdit->setText(Utils::ReadAPIKeyFromFile());
  lTabLayout->addWidget(lAPIKeyLineEdit, 1, 1, Qt::AlignTop);

  // Save the API key
  auto lSaveAPIKey{ComponentFactory::CreateButton(this, tr("Save this API key"), "", "save", lIconFolder, "save_api_key", false, true)};
  lTabLayout->addWidget(lSaveAPIKey, 1, 2, Qt::AlignTop);

  // Open the NexusMods API keys' management page
  auto lOpenAPIKeysManagementPage{ComponentFactory::CreateButton(this,
                                                                 tr("View my keys (nexusmods.com)"),
                                                                 "",
                                                                 "external",
                                                                 lIconFolder,
                                                                 "open_api_keys_management_page",
                                                                 false,
                                                                 true)};
  lTabLayout->addWidget(lOpenAPIKeysManagementPage, 1, 3, Qt::AlignTop);

  // Event binding
  QObject::connect(lModURLOrIDLineEdit, &QLineEdit::textChanged, this, QOverload<const QString&>::of(&TexturesAssistant::updateLaunchSearchButtonState));
  QObject::connect(lAPIKeyLineEdit, &QLineEdit::textChanged, this, QOverload<const QString&>::of(&TexturesAssistant::updateLaunchSearchButtonState));
  QObject::connect(lAPIKeyLineEdit, &QLineEdit::textChanged, this, QOverload<const QString&>::of(&TexturesAssistant::updateSaveAPIKeyButtonState));
  QObject::connect(lSaveAPIKey, &QPushButton::clicked, this, &TexturesAssistant::saveAPIKey);
  QObject::connect(lOpenAPIKeysManagementPage, &QPushButton::clicked, this, &TexturesAssistant::openAPIKeysManagementPage);

  // Post-bind initialization functions
  this->updateSaveAPIKeyButtonState(true);
}

void TexturesAssistant::displayHintZone()
{
  this->deleteAlreadyExistingWindowBottom();

  // Get the window's layout
  auto lHintZone{new QLabel(tr("Awaiting the launch of a scan..."), this)};
  lHintZone->setObjectName(QString("hint_zone"));
  lHintZone->setAlignment(Qt::AlignCenter);
  qobject_cast<QGridLayout*>(this->layout())->addWidget(lHintZone, 2, 0);

  // Disable the groupboxes
  this->toggleGroupBoxesState(true);
}

void TexturesAssistant::setupTexturesSetGUI(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Group box
  auto lGroupBox{ComponentFactory::CreateGroupBox(this, tr("Textures set"), "textures", lIconFolder, this->mSettings.display.font.pointSize, "textures_set_groupbox", true)};
  aLayout.addWidget(lGroupBox, 3, 0);

  auto lLayout{new QGridLayout(lGroupBox)};
  lLayout->setColumnStretch(0, 0);
  lLayout->setColumnStretch(1, 1);
  lLayout->setColumnStretch(2, 0);
  lLayout->setSpacing(10);
  lLayout->setContentsMargins(15, 20, 15, 15);
  lLayout->setAlignment(Qt::AlignTop);
  lLayout->setColumnMinimumWidth(0, this->mMinimumFirstColumnWidth);

  // Human skeleton file
  lLayout->addWidget(new QLabel(tr("Textures set:"), this), 0, 0);

  auto lTexturesSetChooser{new QComboBox(this)};
  lTexturesSetChooser->setItemDelegate(new QStyledItemDelegate());
  lTexturesSetChooser->setCursor(Qt::PointingHandCursor);
  lTexturesSetChooser->setObjectName(QString("textures_set_chooser"));
  lLayout->addWidget(lTexturesSetChooser, 0, 1);

  // Refresh button
  auto lTexturesSetRefresher{ComponentFactory::CreateButton(this, tr("Refresh"), "", "refresh", lIconFolder)};
  lLayout->addWidget(lTexturesSetRefresher, 0, 2);

  // Open assets directory
  auto lOpenAssetsDirectory{ComponentFactory::CreateButton(this, tr("View in explorer"), "", "folder-search", lIconFolder)};
  lLayout->addWidget(lOpenAssetsDirectory, 0, 3);

  this->populateTexturesSetChooser();

  // Event binding
  QObject::connect(lTexturesSetRefresher, &QPushButton::clicked, this, &TexturesAssistant::populateTexturesSetChooser);
  QObject::connect(lOpenAssetsDirectory, &QPushButton::clicked, this, &TexturesAssistant::openTexturesSetsAssetsDirectory);
}

void TexturesAssistant::setupOutputBox(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Create the group box
  ComponentFactory::CreateOutputBox(this, aLayout, 4, 0, lIconFolder, this->mMinimumFirstColumnWidth, this->mSettings.display.font.pointSize);
  auto lOutputGroupBox{this->findChild<GroupBox*>(QString("output_group_box"))};
  lOutputGroupBox->setDisabled(true);

  // Event binding
  auto lOutputPathChooser{this->findChild<QPushButton*>(QString("output_path_chooser"))};
  QObject::connect(lOutputPathChooser, &QPushButton::clicked, this, &TexturesAssistant::chooseExportDirectory);

  auto lOutputSubpathLineEdit{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))};
  QObject::connect(lOutputSubpathLineEdit, &QLineEdit::textChanged, this, &TexturesAssistant::updateOutputPreview);

  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))};
  QObject::connect(lUseOnlySubdir, &QCheckBox::stateChanged, this, &TexturesAssistant::useOnlySubdirStateChanged);

  // Pre-filled data
  this->updateOutputPreview();
}

void TexturesAssistant::setupButtons(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Generate button
  auto lGenerateButton{ComponentFactory::CreateButton(this, tr("Create the files structure on my computer"), "", "build", lIconFolder, "generate_set")};
  lGenerateButton->setDisabled(true);
  aLayout.addWidget(lGenerateButton, 5, 0);

  // Event binding
  QObject::connect(lGenerateButton, &QPushButton::clicked, this, &TexturesAssistant::generateTexturesStructure);
}

void TexturesAssistant::deleteAlreadyExistingWindowBottom() const
{
  auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
  if (lHintZone != nullptr)
  {
    delete lHintZone;
    lHintZone = nullptr;
  }

  auto lScrollZone{this->findChild<QScrollArea*>(QString("scrollable_zone"))};
  if (lScrollZone != nullptr)
  {
    delete lScrollZone;
    lScrollZone = nullptr;
  }
}

void TexturesAssistant::chooseInputDirectory()
{
  // Fetch GUI components
  auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  auto lLineEdit{this->findChild<QLineEdit*>(QString("input_path_directory"))};

  // Open a directory chooser dialog
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "texturesAssistantInput", lLineEdit->text(), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "texturesAssistantInput", lPath);

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

void TexturesAssistant::saveAPIKey()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  const auto lAPIKeyLineEdit{this->findChild<QLineEdit*>("api_key")};
  const auto lSucceed{Utils::SaveAPIKeyToFile(lAPIKeyLineEdit->text(), this, lIconFolder)};

  this->updateSaveAPIKeyButtonState(lSucceed);
}

void TexturesAssistant::updateSaveAPIKeyButtonState(const QString&)
{
  this->updateSaveAPIKeyButtonState(false);
}

void TexturesAssistant::updateSaveAPIKeyButtonState(const bool aMustBeDisabled)
{
  const auto lSaveAPIKey{this->findChild<QPushButton*>(QString("save_api_key"))};
  lSaveAPIKey->setDisabled(aMustBeDisabled);
}

void TexturesAssistant::toggleGroupBoxesState(const bool aMustBeDisabled) const
{
  auto lTexturesSetGroupBox{this->findChild<GroupBox*>(QString("textures_set_groupbox"))};
  if (lTexturesSetGroupBox != nullptr)
    lTexturesSetGroupBox->setDisabled(aMustBeDisabled);

  auto lOutputGroupBox{this->findChild<GroupBox*>(QString("output_group_box"))};
  if (lOutputGroupBox != nullptr)
    lOutputGroupBox->setDisabled(aMustBeDisabled);

  auto lGenerateButton{this->findChild<QPushButton*>(QString("generate_set"))};
  if (lGenerateButton != nullptr)
    lGenerateButton->setDisabled(aMustBeDisabled);
}

void TexturesAssistant::updateLaunchSearchButtonState(const QString&) const
{
  const auto lTabWidget{this->findChild<QTabWidget*>("tab_widget")};
  this->updateLaunchSearchButtonState(lTabWidget->currentIndex());
}

void TexturesAssistant::updateLaunchSearchButtonState(int aCurrentTabIndex) const
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

      lLaunchSearchButton->setDisabled(lModURLOrIDLineEdit->text().isEmpty() || lAPIKeyLineEdit->text().isEmpty() || !isModIDValid());

      break;
    }
  }
}

void TexturesAssistant::launchSearchProcess()
{
  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  lLaunchSearchButton->setDisabled(true);

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

void TexturesAssistant::launchSearchFromLocalFolder()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  const auto lInputPath{this->findChild<QLineEdit*>(QString("input_path_directory"))->text()};

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

  // The root directory should at least contain a "textures" directory to be scanned.
  if (!QDir(QString("%1/textures").arg(lInputPath)).exists())
  {
    Utils::DisplayWarningMessage(tr("No \"textures\" directory has been found in the scanned directory."));
  }

  // Fetch all the "*dds" files
  this->scanForTexturesFiles(lInputPath, "*.dds");

  // Display the data
  this->displayObtainedData();

  // Re-enable the groupboxes
  this->toggleGroupBoxesState(false);
}

void TexturesAssistant::scanForTexturesFiles(const QString& aRootDir, const QString& aFileExtension)
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

  this->mScannedFiles.groupedTextures.clear();
  this->mScannedFiles.otherTextures.clear();

  const QStringList lTexturesFilesToFind{DataLists::GetKnownTexturesFilesNames()};

  // Suffix the root directory to earch only in the "textures" subfolder
  const auto lRootDir{QString("%1/textures").arg(aRootDir)};

  QDirIterator it(lRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    // Cancel the treatment if the user canceled it
    if (lProgressDialog.wasCanceled())
    {
      Utils::DisplayWarningMessage(tr("Process aborted by the user."));
      return;
    }

    it.next();

    // Get the current directory
    const auto lRelativeDirPath{it.fileInfo().absolutePath().remove(aRootDir + "/", Qt::CaseInsensitive)};

    auto lFileName = it.fileInfo().fileName();

    // Clean the file name from any artifact
    lFileName.remove(".dds", Qt::CaseInsensitive);

    // Check if the file is relative to a texture for a known mesh type
    if (lTexturesFilesToFind.contains(lFileName))
    {
      this->mScannedFiles.groupedTextures.push_back(std::make_pair(lRelativeDirPath, lFileName));
    }
    else
    {
      this->mScannedFiles.otherTextures.push_back(std::make_pair(lRelativeDirPath, lFileName));
    }
  }
}
void TexturesAssistant::launchSearchNexusModsURL()
{
  auto lModID{this->getModIDFromUserInput()};
  this->requestModInformation(lModID);
}

int TexturesAssistant::getModIDFromUserInput() const
{
  const auto lModURLOrID{this->findChild<QLineEdit*>(QString("mod_url_or_id"))->text()};
  const auto lURLPattern{QString("nexusmods.com/skyrimspecialedition/mods/")};

  // ID
  bool lIsInteger;
  lModURLOrID.toInt(&lIsInteger);
  if (lIsInteger)
  {
    return lModURLOrID.toInt();
  }
  // URL
  else if (lModURLOrID.contains(lURLPattern))
  {
    auto lModID{0};
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

    return lModID;
  }

  // Invalid format
  return -1;
}

bool TexturesAssistant::isModIDValid() const
{
  return this->getModIDFromUserInput() != -1;
}

void TexturesAssistant::requestModInformation(const int aModID)
{
  const auto lURL{QString("https://api.nexusmods.com/v1/games/skyrimspecialedition/mods/%1/files.json").arg(QString::number(aModID))};

  QNetworkRequest lRequest{QUrl(lURL)};
  lRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  lRequest.setRawHeader("apikey", this->findChild<QLineEdit*>("api_key")->text().toUtf8());
  QNetworkReply* lReply{this->mManager.get(lRequest)};
  connect(lReply, &QNetworkReply::finished, this, &TexturesAssistant::requestModInformationFinished);
}

void TexturesAssistant::requestModInformationFinished()
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

std::vector<Struct::NexusModsFileInformation> TexturesAssistant::parseFilesListFromModInformation(const bool aSucceeded, const QByteArray& aResult)
{
  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};

  if (!aSucceeded)
  {
    lLaunchSearchButton->setDisabled(false);
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
                                                       lFile["version"].toString(),
                                                       lFile["content_preview_link"].toString(),
                                                       lFile["category_name"].isNull() ? "-" : lFile["category_name"].toString()));
    }
  }
  else
  {
    lLaunchSearchButton->setDisabled(false);
    Utils::DisplayErrorMessage(tr("Parse error: could not find the \"files\" element."));
    return std::vector<Struct::NexusModsFileInformation>();
  }

  return lList;
}

void TexturesAssistant::displayFileIDPicker(const std::vector<Struct::NexusModsFileInformation>& aFilesInformation)
{
  if (aFilesInformation.empty())
  {
    const auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
    lLaunchSearchButton->setDisabled(false);
    return;
  }

  auto lFilePicker{new FileIDPicker(this, this->mSettings, aFilesInformation)};
  QObject::connect(lFilePicker, QOverload<QString>::of(&FileIDPicker::fileContentPreviewURLChosen), this, &TexturesAssistant::requestModFileContent);
}

void TexturesAssistant::requestModFileContent(const QString& aContentPreviewLink)
{
  if (aContentPreviewLink.isEmpty())
  {
    const auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
    lLaunchSearchButton->setDisabled(false);
    Utils::DisplayErrorMessage(tr("An error has occurred while trying to read the file content preview. Make sure your internet connection is operational and try again. If the error occurs again, please check NexusMods servers status."));
    return;
  }

  QNetworkRequest lRequest{QUrl(aContentPreviewLink)};
  lRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  lRequest.setRawHeader("apikey", this->findChild<QLineEdit*>("api_key")->text().toUtf8());
  QNetworkReply* lReply{this->mManager.get(lRequest)};
  connect(lReply, &QNetworkReply::finished, this, &TexturesAssistant::requestModFileContentFinished);
}

void TexturesAssistant::requestModFileContentFinished()
{
  auto lReply{qobject_cast<QNetworkReply*>(this->sender())};

  bool lHasFoundBSAFile{false};

  if (lReply->error() == QNetworkReply::NoError)
  {
    this->parseFileContent(true, lReply->readAll(), lHasFoundBSAFile);
  }
  else
  {
    this->parseFileContent(false, "", lHasFoundBSAFile);
  }

  lReply->deleteLater();

  // Warn the user if the scan found a BSA file
  if (lHasFoundBSAFile)
  {
    Utils::DisplayWarningMessage(tr("At least one BSA file was found in the scanned mod. Please note that the application cannot read the data contained in the BSA files, so it is advisable to download and install the mod, to then decompress the BSA file, before continuing the scan."));
  }

  this->displayObtainedData();

  // Re-enable the groupboxes
  this->toggleGroupBoxesState(false);
}

void TexturesAssistant::parseFileContent(const bool aSucceeded, const QByteArray& aResult, bool& aHasFoundBSAFile)
{
  if (!aSucceeded)
  {
    Utils::DisplayErrorMessage(tr("An error has occurred... Make sure your internet connection is operational and try again."));
    return;
  }

  this->mScannedFiles.groupedTextures.clear();
  this->mScannedFiles.otherTextures.clear();

  std::map<QString, std::vector<QString>> lFoundNifFiles;

  const auto lJson{QJsonDocument::fromJson(aResult)};
  const auto lRootElement{lJson.object()};

  if (lRootElement.contains("children") && lRootElement["children"].isArray())
  {
    const auto lChildren{lRootElement["children"].toArray()};
    if (lChildren.size() == 1 && lChildren.first().toObject().contains("name"))
    {
      this->parseNode(lChildren, lFoundNifFiles, lChildren.first().toObject()["name"].toString(), aHasFoundBSAFile);
    }
  }
}

void TexturesAssistant::parseNode(const QJsonArray& aArray,
                                  std::map<QString, std::vector<QString>>& aNifFilesList,
                                  const QString& aRootNodeName,
                                  bool& aHasFoundBSAFile)
{
  for (const auto& lEntry : aArray)
  {
    if (lEntry.isObject())
    {
      // The node contains some children
      if (lEntry.toObject().contains("children") && lEntry["children"].isArray())
      {
        // Parse the content of the array
        const auto lChildren{lEntry["children"].toArray()};
        this->parseNode(lChildren, aNifFilesList, aRootNodeName, aHasFoundBSAFile);
      }
      else
      {
        // Parse the file node
        this->parseNode(lEntry.toObject(), aRootNodeName, aHasFoundBSAFile);
      }
    }
  }
}

void TexturesAssistant::parseNode(const QJsonObject& aNode, const QString& aRootNodeName, bool& aHasFoundBSAFile)
{
  if (aNode.contains("path") && aNode["path"].isString()
      && aNode.contains("name") && aNode["name"].isString()
      && aNode.contains("type") && aNode["type"].isString()
      && aNode.contains("size"))
  {
    const QStringList lTexturesFilesToFind{DataLists::GetKnownTexturesFilesNames()};

    const auto lName{aNode["name"].toString()};

    // BSA file
    if (lName.endsWith(".bsa", Qt::CaseSensitivity::CaseInsensitive))
    {
      aHasFoundBSAFile = true;
    }

    // Not a file or not a DDS file
    if (aNode["type"].toString().compare("file") != 0
        || !lName.endsWith(".dds", Qt::CaseSensitivity::CaseInsensitive))
    {
      return;
    }

    auto lFileName{aNode["path"].toString()};

    // Remove the root node name from the file path
    if (lFileName.startsWith(aRootNodeName + "/"))
    {
      lFileName.remove(aRootNodeName + "/");
    }

    // The root directory should at least contain a "textures" directory to be scanned.
    if (!lFileName.startsWith("textures/", Qt::CaseSensitivity::CaseInsensitive))
    {
      return;
    }

    // Clean the file name from any artifact
    lFileName.remove(".dds", Qt::CaseInsensitive);

    const auto lLastSlashPosition{lFileName.lastIndexOf('/')};

    auto lSplittedPath = std::make_pair(lFileName.left(lLastSlashPosition), lFileName.mid(lLastSlashPosition + 1));
    if (lSplittedPath.first.isEmpty() || lSplittedPath.second.isEmpty())
    {
      return;
    }

    if (lTexturesFilesToFind.contains(lSplittedPath.second))
    {
      this->mScannedFiles.groupedTextures.push_back(std::make_pair(lSplittedPath.first, lSplittedPath.second));
    }
    else
    {
      this->mScannedFiles.otherTextures.push_back(std::make_pair(lSplittedPath.first, lSplittedPath.second));
    }
  }
}

void TexturesAssistant::displayObtainedData()
{
  const auto lLaunchSearchButton{this->findChild<QPushButton*>(QString("launch_search_button"))};
  lLaunchSearchButton->setDisabled(false);

  // No file found
  if (this->mScannedFiles.groupedTextures.empty() && this->mScannedFiles.otherTextures.empty())
  {
    this->displayHintZone();

    auto lHintZone{this->findChild<QLabel*>(QString("hint_zone"))};
    if (lHintZone != nullptr)
    {
      lHintZone->setText(tr("No DDS file was found in the scanned directory."));
    }
    return;
  }

  // Delete already existing the hint label or the layout and the validation button
  this->deleteAlreadyExistingWindowBottom();

  // Create the scroll area wrapper
  auto lDataContainer{ComponentFactory::CreateScrollAreaComponentLayout(this, 2, 0)};

  // Parse the grouped textures to split them in multiple storages
  TexturesAssistant::GroupedData lGroupedPaths;

  for (const auto& lNifFile : this->mScannedFiles.groupedTextures)
  {
    std::map<QString, std::vector<QString>>* lTargetMapToFill{nullptr};

    if (lNifFile.second.contains("head"))
    {
      lTargetMapToFill = &lGroupedPaths.headTextures;
    }
    else if (lNifFile.second.contains("hands"))
    {
      lTargetMapToFill = &lGroupedPaths.handsTextures;
    }
    else if (lNifFile.second.contains("body_etc"))
    {
      lTargetMapToFill = &lGroupedPaths.extraBodyTextures;
    }
    else if (lNifFile.second.contains("body"))
    {
      lTargetMapToFill = &lGroupedPaths.bodyTextures;
    }
    else if (lNifFile.second.contains("mouth"))
    {
      lTargetMapToFill = &lGroupedPaths.mouthTextures;
    }

    if (lTargetMapToFill != nullptr)
    {
      (*lTargetMapToFill)[lNifFile.first].push_back(lNifFile.second);
    }
  }

  // Parse the other textures to change their storage type to be compatible with the display function
  auto lOtherPaths{std::map<QString, std::vector<QString>>()};

  for (const auto& lNifFile : this->mScannedFiles.otherTextures)
  {
    lOtherPaths[lNifFile.first].push_back(lNifFile.second);
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  auto lRowIndex{0};

  // Head resources blocks
  auto lHeadGroup{ComponentFactory::CreateGroupBox(this, tr("Head textures"), "woman-head", lIconFolder, this->mSettings.display.font.pointSize)};

  auto lHeadGroupContainer{new QGridLayout(this)};
  lHeadGroupContainer->setSpacing(16);
  lHeadGroup->setLayout(lHeadGroupContainer);
  this->createResourceBlock(lGroupedPaths.headTextures, lHeadGroupContainer);
  lDataContainer->addWidget(lHeadGroup, lRowIndex++, 0);

  // Mouth resources blocks
  auto lMouthGroup{ComponentFactory::CreateGroupBox(this, tr("Mouth textures"), "mouth", lIconFolder, this->mSettings.display.font.pointSize)};

  auto lMouthGroupContainer{new QGridLayout(this)};
  lMouthGroupContainer->setSpacing(16);
  lMouthGroup->setLayout(lMouthGroupContainer);
  this->createResourceBlock(lGroupedPaths.mouthTextures, lMouthGroupContainer);
  lDataContainer->addWidget(lMouthGroup, lRowIndex++, 0);

  // Body resources blocks
  auto lBodyGroup{ComponentFactory::CreateGroupBox(this, tr("Body textures"), "body", lIconFolder, this->mSettings.display.font.pointSize)};

  auto lBodyGroupContainer{new QGridLayout(this)};
  lBodyGroupContainer->setSpacing(16);
  lBodyGroup->setLayout(lBodyGroupContainer);
  this->createResourceBlock(lGroupedPaths.bodyTextures, lBodyGroupContainer);
  lDataContainer->addWidget(lBodyGroup, lRowIndex++, 0);

  // Extra body resources blocks
  auto lBodyExtraGroup{ComponentFactory::CreateGroupBox(this, tr("Extra body textures"), "more", lIconFolder, this->mSettings.display.font.pointSize)};

  auto lBodyExtraGroupContainer{new QGridLayout(this)};
  lBodyExtraGroupContainer->setSpacing(16);
  lBodyExtraGroup->setLayout(lBodyExtraGroupContainer);
  this->createResourceBlock(lGroupedPaths.extraBodyTextures, lBodyExtraGroupContainer);
  lDataContainer->addWidget(lBodyExtraGroup, lRowIndex++, 0);

  // Hands resources blocks
  auto lHandsGroup{ComponentFactory::CreateGroupBox(this, tr("Hands textures"), "hand", lIconFolder, this->mSettings.display.font.pointSize)};

  auto lHandsGroupContainer{new QGridLayout(this)};
  lHandsGroupContainer->setSpacing(16);
  lHandsGroup->setLayout(lHandsGroupContainer);
  this->createResourceBlock(lGroupedPaths.handsTextures, lHandsGroupContainer);
  lDataContainer->addWidget(lHandsGroup, lRowIndex++, 0);

  // Other texture files
  auto lOtherGroup{ComponentFactory::CreateGroupBox(this, tr("Other .DDS textures"), "textures", lIconFolder, this->mSettings.display.font.pointSize)};

  auto lOtherGroupContainer{new QGridLayout(this)};
  lOtherGroupContainer->setSpacing(16);
  lOtherGroup->setLayout(lOtherGroupContainer);
  this->createResourceBlock(lOtherPaths, lOtherGroupContainer);
  lDataContainer->addWidget(lOtherGroup, lRowIndex++, 0);
}

void TexturesAssistant::createResourceBlock(const std::map<QString, std::vector<QString>>& aMap, QGridLayout* aLayout)
{
  auto lRowIndex{0};
  for (const auto& lRootPath : aMap)
  {
    QString lConcatenatedFileNames;

    for (const auto& lFileName : lRootPath.second)
    {
      lConcatenatedFileNames.append(QString("%1\n").arg(lFileName));
    }
    lConcatenatedFileNames = lConcatenatedFileNames.left(lConcatenatedFileNames.length() - 1);

    aLayout->addWidget(new QLabel(lRootPath.first, this), lRowIndex, 0, Qt::AlignLeft);
    aLayout->addWidget(new QLabel(lConcatenatedFileNames, this), lRowIndex++, 1, Qt::AlignLeft);
  }
}

void TexturesAssistant::generateTexturesStructure()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Textures set
  auto lTexturesSetChooser{this->findChild<QComboBox*>(QString("textures_set_chooser"))};
  if (lTexturesSetChooser->currentIndex() == -1 || lTexturesSetChooser->count() == 0)
  {
    Utils::DisplayWarningMessage(tr("Error: no textures set chosen."));
    return;
  }

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
  if (!QDir(lEntryDirectory).exists() || (QDir(lEntryDirectory).exists() && QDir(lEntryDirectory).isEmpty()))
  {
    // Wait to know the result of the mkdir()
    if (!QDir().mkpath(lEntryDirectory))
    {
      Utils::DisplayWarningMessage(tr("Error while creating the main directory: \"%1\" could not be created on your computer.\nBe sure to not create the files in a OneDrive/DropBox space and that you executed the application with sufficient permissions.\nBe sure that you used characters authorized by your OS in the given paths.").arg(lEntryDirectory));
      return;
    }
  }
  else
  {
    // Since the directory already exist, ask the user to put other files in it
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Already existing directory"),
                                      tr("The directory \"%1\" already exists on your computer. Do you still want to create the files in this directory?").arg(lEntryDirectory),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Continue the files creation"),
                                      tr("Cancel the files creation"),
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

  // Read location
  auto lSourceTexturesFolderName{lTexturesSetChooser->currentText()};
  auto lSourceTexturesReadPath{QString("%1assets/textures/%2").arg(Utils::GetAppDataPathFolder(), lSourceTexturesFolderName)};

  // Iterate through the texture files lists
  for (const auto& lFoundTextureFile : this->mScannedFiles.groupedTextures)
  {
    Utils::CreateTextureFile(lSourceTexturesReadPath, lEntryDirectory, lFoundTextureFile.first, lFoundTextureFile.second);
  }

  // Update the color of the output directory preview
  this->updateOutputPreview();

  auto lTitle{tr("Files creation successful")};
  auto lMessage{tr("The texture files have been correctly created.")};

  // Open the directory where the file structure has been created
  if (Utils::DisplayQuestionMessage(this, lTitle, lMessage, "icons", "green-info", tr("Open the created directory"), tr("OK"), "", "", "", "", false) == ButtonClicked::YES)
  {
    QDesktopServices::openUrl(QUrl::fromLocalFile(lEntryDirectory));
  }
}

void TexturesAssistant::openAPIKeysManagementPage()
{
  QDesktopServices::openUrl(QUrl("https://www.nexusmods.com/users/myaccount?tab=api%20access"));
}

void TexturesAssistant::updateOutputPreview()
{
  auto lMainDirTextEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  auto lSubDirectory{this->findChild<QLineEdit*>(QString("output_path_subdirectory"))->text().trimmed()};
  auto lUseOnlySubdir{this->findChild<QCheckBox*>(QString("only_use_subdirectory"))->isChecked()};
  auto lOutputPathsPreview{this->findChild<QLabel*>(QString("output_path_preview"))};

  Utils::UpdateOutputPreview(this->mFileWatcher, lMainDirTextEdit, lSubDirectory, lUseOnlySubdir, this->mSettings.display.successColor, this->mSettings.display.warningColor, this->mSettings.display.dangerColor, lOutputPathsPreview);
}

void TexturesAssistant::populateTexturesSetChooser()
{
  auto lRootDir{Utils::GetAppDataPathFolder() + "assets/textures/"};
  Utils::CleanPathString(lRootDir);
  QStringList lAvailableTexturesSets;

  // Search for all "*.nif" files
  QDirIterator it(lRootDir, QStringList(), QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);
  while (it.hasNext())
  {
    it.next();

    const auto lCurrentDirectory{QDir(it.filePath())};
    const auto lFilesList{lCurrentDirectory.entryList(QStringList() << "*.dds", QDir::Filter::Files)};
    if (lFilesList.count() > 0)
    {
      lAvailableTexturesSets.push_back(it.fileInfo().absoluteFilePath().remove(lRootDir, Qt::CaseInsensitive));
    }
  }

  // Get the combobox
  const auto lTexturesSetChooser{this->findChild<QComboBox*>(QString("textures_set_chooser"))};

  // Save the selected textures set file name
  const auto lPreviousIndex{lTexturesSetChooser->currentIndex()};
  const auto lPreviousTexturesSet{lTexturesSetChooser->itemText(lPreviousIndex)};

  // Clear the combo box and add the found files to it
  lTexturesSetChooser->clear();
  lTexturesSetChooser->addItems(lAvailableTexturesSets);

  // Reselect the previously selected textures set, if it still exists
  if (lPreviousIndex != -1)
    lTexturesSetChooser->setCurrentIndex(std::max(lAvailableTexturesSets.indexOf(lPreviousTexturesSet), 0));
  else if (lTexturesSetChooser->count() > 0)
    lTexturesSetChooser->setCurrentIndex(0);
}

void TexturesAssistant::openTexturesSetsAssetsDirectory()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(QString("%1assets/textures").arg(Utils::GetAppDataPathFolder())));
}

void TexturesAssistant::useOnlySubdirStateChanged(int)
{
  this->updateOutputPreview();
}

void TexturesAssistant::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>(QString("output_path_directory"))};
  const auto& lContextPath{Utils::GetPathFromKey(this->mLastPaths, "texturesAssistantOutput", lLineEdit->text(), this->mSettings.general.eachButtonSavesItsLastUsedPath)};
  const auto lPath{QFileDialog::getExistingDirectory(this, "", lContextPath)};
  lLineEdit->setText(lPath);
  Utils::UpdatePathAtKey(this->mLastPaths, "texturesAssistantOutput", lPath);
  this->updateOutputPreview();
}
