#include "MFBOPresetCreator.h"
#include "About.h"
#include "AssistedConversion.h"
#include "BatchConversion.h"
#include "PresetCreator.h"
#include "RetargetingTool.h"
#include "Settings.h"
#include "TexturesAssistant.h"
#include "Update.h"
#include "Utils.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDropEvent>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QScreen>

MFBOPresetCreator::MFBOPresetCreator(const Struct::Settings& aSettings, const QString& aInjectedFilePath, QWidget* aParent)
  : QMainWindow(aParent)
  , mSettings(aSettings)
  , mInjectedFilePath(aInjectedFilePath)
  , mLastPaths(Utils::loadLastPathsFromFile())
  , mNewStableVersionAvailable(false)
  , mNewBetaVersionAvailable(false)
{
  // Construct the GUI
  ui.setupUi(this);

#ifdef DEBUG
  qApp->setApplicationDisplayName(tr("[DEV] ") + qApp->applicationDisplayName());
#endif

  // Check for new versions
  if (mSettings.checkForUpdatesAtStartup)
  {
    Utils::printMessageStdOut("Checking for updates...");
    this->checkForUpdate();
  }
  else
  {
    Utils::printMessageStdOut("Skipped update checking");
    this->initializeGUI();
  }
}

void MFBOPresetCreator::closeEvent(QCloseEvent* aEvent)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  auto lMainContainer{qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))};
  if (!lMainContainer->hasUserDoneSomething())
  {
    aEvent->accept();
    return;
  }

  if (Utils::displayQuestionMessage(this,
                                    tr("Quitting"),
                                    tr("You will lose all the unsaved data. Do you still want to quit the application?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Quit the application"),
                                    tr("Go back to the application"),
                                    this->mSettings.dangerColor,
                                    this->mSettings.successColor,
                                    false)
      != ButtonClicked::Yes)
  {
    aEvent->ignore();
  }
  else
  {
    aEvent->accept();
  }
}

void MFBOPresetCreator::dragEnterEvent(QDragEnterEvent* aEvent)
{
  auto lAcceptEvent{false};

  for (const QUrl& lUrl : aEvent->mimeData()->urls())
  {
    auto lFileName{lUrl.toLocalFile()};
    QFileInfo lFileInfo(lFileName);
    if (lFileInfo.exists() && lFileInfo.isFile() && lFileInfo.completeSuffix().compare("pcp") == 0)
    {
      lAcceptEvent = true;
    }
  }

  if (lAcceptEvent)
  {
    aEvent->setDropAction(Qt::DropAction::MoveAction);
    aEvent->accept();
  }
  else
  {
    aEvent->ignore();
  }
}

void MFBOPresetCreator::dropEvent(QDropEvent* aEvent)
{
  for (const QUrl& lUrl : aEvent->mimeData()->urls())
  {
    auto lFileName{lUrl.toLocalFile()};
    QFileInfo lFileInfo(lFileName);
    if (lFileInfo.exists() && lFileInfo.isFile() && lFileInfo.completeSuffix().compare("pcp") == 0)
    {
      aEvent->acceptProposedAction();
      qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))->loadProject(lFileName, true);
    }
  }
}

void MFBOPresetCreator::initializeGUI()
{
  this->setupMenuBar();

  auto lMainContainer{new PresetCreator(this, this->mSettings, &this->mLastPaths)};
  lMainContainer->setObjectName("main_container");
  this->setCentralWidget(lMainContainer);

  this->refreshUI(mSettings, false);

  this->showWindow();

  this->setAcceptDrops(true);

  Utils::cleanPathString(this->mInjectedFilePath);
  lMainContainer->loadProject(this->mInjectedFilePath, true);
}

void MFBOPresetCreator::setupMenuBar()
{
  // Keep a reference to the user theme
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Menu bar
  auto lMenuBar{new QMenuBar(this)};
  lMenuBar->setCursor(Qt::PointingHandCursor);
  this->setMenuBar(lMenuBar);

  // Menu: File
  auto lFile{new QMenu(tr("File"), this)};
  lFile->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lFile);

  // Action: Open project file
  auto lOpenProjectFile{Utils::buildQAction(this, tr("Open project..."), QKeySequence(Qt::CTRL + Qt::Key_O), "file", lIconFolder)};
  lOpenProjectFile->setObjectName("action_open_project");
  lFile->addAction(lOpenProjectFile);

  // Action: Save current project
  auto lSaveProject{Utils::buildQAction(this, tr("Save"), QKeySequence(Qt::CTRL + Qt::Key_S), "save", lIconFolder)};
  lSaveProject->setObjectName("action_save_project");
  lSaveProject->setDisabled(true);
  lFile->addAction(lSaveProject);

  // Action: Save current project as
  auto lSaveProjectAs{Utils::buildQAction(this, tr("Save as"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), "save", lIconFolder)};
  lSaveProjectAs->setObjectName("action_save_project_as");
  lFile->addAction(lSaveProjectAs);

  lFile->addSeparator();

  // Action: Settings
  auto lOpenSettings{Utils::buildQAction(this, tr("Settings"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O), "cog", lIconFolder)};
  lFile->addAction(lOpenSettings);

  lFile->addSeparator();

  // Action: Relaunch the app
  auto lRelaunchApp{Utils::buildQAction(this, tr("Quick restart"), QKeySequence(Qt::CTRL + Qt::Key_F5), "refresh", lIconFolder)};
  lFile->addAction(lRelaunchApp);

  // Action: Exit
  auto lExitApp{Utils::buildQAction(this, tr("Exit"), QKeySequence(Qt::ALT + Qt::Key_F4), "cross", lIconFolder)};
  lFile->addAction(lExitApp);

  // Menu: Tools
  auto lTools{new QMenu(tr("Tools"), this)};
  lTools->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lTools);

  // Action: Batch conversion
  auto lOpenBatchConv{Utils::buildQAction(this, tr("Batch Conversion") + " (BETA)", QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R), "reorder", lIconFolder)};
  lTools->addAction(lOpenBatchConv);

  // Action: Textures Assistant
  auto lOpenTextAssist{Utils::buildQAction(this, tr("Textures Assistant"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T), "textures", lIconFolder)};
  lTools->addAction(lOpenTextAssist);

  // Action: Assisted Conversion
  auto lOpenAssiConv{Utils::buildQAction(this, tr("Assisted Conversion"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Y), "pencil", lIconFolder)};
  lTools->addAction(lOpenAssiConv);

  // Action: BodySlide Presets' Retargeting
  auto lOpenRetaTools{Utils::buildQAction(this, tr("BodySlide Presets' Retargeting"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_H), "arrow-up", lIconFolder)};
  lTools->addAction(lOpenRetaTools);

  // Menu: Help
  auto lUpdateAvailableText{QString()};
  if (this->mNewStableVersionAvailable)
    lUpdateAvailableText = tr(" (stable update available)");
  else if (this->mNewBetaVersionAvailable)
    lUpdateAvailableText = tr(" (beta update available)");

  auto lHelp{new QMenu(tr("Help") + lUpdateAvailableText, this)};
  lHelp->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lHelp);

  // Action: Check for updates
  auto lOpenUpdate{Utils::buildQAction(this, tr("Check for updates") + lUpdateAvailableText, QKeySequence(Qt::CTRL + Qt::Key_U), "cloud-search", lIconFolder)};
  lHelp->addAction(lOpenUpdate);

  // Submenu: Links
  auto lLinksSubmenu{new QMenu(tr("Useful links"), this)};
  lLinksSubmenu->setIcon(QIcon(QPixmap(QString(":/%1/link").arg(lIconFolder))));
  lLinksSubmenu->setCursor(Qt::PointingHandCursor);
  lHelp->addMenu(lLinksSubmenu);

  // Action: Nexus page link
  auto lOpenNexus{Utils::buildQAction(this, tr("Nexus Mods page"), QKeySequence(Qt::CTRL + Qt::Key_N), "nexus-logo", lIconFolder)};
  lLinksSubmenu->addAction(lOpenNexus);

  // Action: open URL to Doogle Docs: Guide and tutorials
  auto lOpenGuide{Utils::buildQAction(this, tr("User guide and tutorials (Google Docs)"), QKeySequence(Qt::CTRL + Qt::Key_G), "text-file", lIconFolder)};
  lLinksSubmenu->addAction(lOpenGuide);

  // Action: Source code on GitHub
  auto lOpenSourceCodeGitHub{Utils::buildQAction(this, tr("View the source code (GitHub.com)"), NULL, "code", lIconFolder)};
  lLinksSubmenu->addAction(lOpenSourceCodeGitHub);

  // Action: Source code on GitLab
  auto lOpenSourceCodeGitLab{Utils::buildQAction(this, tr("View the source code (GitLab.com)"), NULL, "code", lIconFolder)};
  lLinksSubmenu->addAction(lOpenSourceCodeGitLab);

  // Action: About
  auto lOpenAbout{Utils::buildQAction(this, tr("About"), QKeySequence(Qt::CTRL + Qt::Key_I), "info-circle", lIconFolder)};
  lHelp->addAction(lOpenAbout);

  // Event binding
  this->connect(lOpenProjectFile, &QAction::triggered, this, &MFBOPresetCreator::loadProject);
  this->connect(lSaveProject, &QAction::triggered, this, &MFBOPresetCreator::saveProject);
  this->connect(lSaveProjectAs, &QAction::triggered, this, &MFBOPresetCreator::saveProject);
  this->connect(lOpenSettings, &QAction::triggered, this, &MFBOPresetCreator::launchSettingsDialog);
  this->connect(lRelaunchApp, &QAction::triggered, this, &MFBOPresetCreator::quickRelaunch);
  this->connect(lExitApp, &QAction::triggered, this, &MFBOPresetCreator::close);
  this->connect(lOpenBatchConv, &QAction::triggered, this, &MFBOPresetCreator::launchBatchConversion);
  this->connect(lOpenTextAssist, &QAction::triggered, this, &MFBOPresetCreator::launchTexturesAssistant);
  this->connect(lOpenAssiConv, &QAction::triggered, this, &MFBOPresetCreator::launchAssistedConversion);
  this->connect(lOpenRetaTools, &QAction::triggered, this, &MFBOPresetCreator::launchPresetsRetargeting);
  this->connect(lOpenUpdate, &QAction::triggered, this, &MFBOPresetCreator::launchUpdateDialog);
  this->connect(lOpenNexus, &QAction::triggered, this, &MFBOPresetCreator::openNexusPageInDefaultBrowser);
  this->connect(lOpenSourceCodeGitHub, &QAction::triggered, this, &MFBOPresetCreator::openGitHubSourceCodePageInDefaultBrowser);
  this->connect(lOpenSourceCodeGitLab, &QAction::triggered, this, &MFBOPresetCreator::openGitLabSourceCodePageInDefaultBrowser);
  this->connect(lOpenGuide, &QAction::triggered, this, &MFBOPresetCreator::openGuideInDefaultBrowser);
  this->connect(lOpenAbout, &QAction::triggered, this, &MFBOPresetCreator::launchAboutDialog);
}

void MFBOPresetCreator::showWindow()
{
  this->adjustSize();

  // Set the size of the window
  auto lDesktopScreen{QGuiApplication::primaryScreen()};
  auto lScreenGeom{lDesktopScreen->availableGeometry()};

  // If the window size is correct for the user's screen
  if (mSettings.mainWindowWidth < lScreenGeom.width() && mSettings.mainWindowHeight < lScreenGeom.height())
  {
    auto lWidth{mSettings.mainWindowWidth};
    auto lHeight{mSettings.mainWindowHeight};

    // The window should not be too small
    if (lWidth < 900)
    {
      lWidth = 900;
    }

    if (lHeight < 600)
    {
      lHeight = 600;
    }

    // Resize the window
    this->resize(lWidth - 2, lHeight - 62);
  }

  // Select the main window opening mode
  if (mSettings.mainWindowOpeningMode == WindowOpeningMode::Minimized)
  {
    this->showMinimized();

    // Make the icon in the taskbar blink
    FLASHWINFO* finfo{new FLASHWINFO()};
    finfo->cbSize = sizeof(FLASHWINFO);
    finfo->hwnd = (HWND)this->winId();
    finfo->uCount = 5;
    finfo->dwTimeout = 500;
    finfo->dwFlags = FLASHW_ALL;
    FlashWindowEx(finfo);

    delete finfo;
    finfo = nullptr;
  }
  else if (mSettings.mainWindowOpeningMode == WindowOpeningMode::Windowed)
  {
    this->show();
  }

  auto lPosX{(lScreenGeom.width() - this->frameGeometry().width()) / 2 + 1};
  auto lPosY{(lScreenGeom.height() - this->frameGeometry().height()) / 2 + 31};
  this->setGeometry(QRect(lPosX, lPosY, this->geometry().width(), this->geometry().height()));

  if (mSettings.mainWindowOpeningMode == WindowOpeningMode::Maximized)
  {
    this->showMaximized();
  }
}

void MFBOPresetCreator::applyGlobalStyleSheet()
{
  auto lQSSFileName{QString()};

  switch (this->mSettings.appTheme)
  {
    case GUITheme::WindowsVista:
      break;
    case GUITheme::PaperLight:
      lQSSFileName = "Paper Light by 6788";
      break;
    case GUITheme::PaperDark:
      lQSSFileName = "Paper Dark by 6788";
      break;
    case GUITheme::PaperWhiteMono:
      lQSSFileName = "Paper White Mono";
      break;
    case GUITheme::PaperBlackMono:
      lQSSFileName = "Paper Black Mono";
      break;
    case GUITheme::AlexhuszaghBreezeLight:
      lQSSFileName = "Alexhuszagh's Breeze Light";
      break;
    case GUITheme::AlexhuszaghBreezeDark:
      lQSSFileName = "Alexhuszagh's Breeze Dark";
      break;
    case GUITheme::QuasarAppDarkStyle:
      lQSSFileName = "QuasarApp's Dark Style";
      break;
    case GUITheme::QuasarAppVisualStudioDark:
      lQSSFileName = "QuasarApp's Visual Studio Dark";
      break;
    case GUITheme::MitsuriouLightTheme:
      lQSSFileName = "Mitsuriou's Light Theme";
      break;
    case GUITheme::MitsuriouDarkTheme:
      lQSSFileName = "Mitsuriou's Dark Theme";
      break;
  }

  auto lLineEditsToReactivate{this->disableLineEditPlaceholders()};

  qApp->setStyleSheet("");

  if (lQSSFileName != "")
  {
    QFile lQSSFile(":qss/" + lQSSFileName + ".qss");
    if (lQSSFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QTextStream lStream(&lQSSFile);
      qApp->setStyleSheet(lStream.readAll());
      lQSSFile.close();
    }
  }

  this->enableLineEditPlaceholders(lLineEditsToReactivate);

  // Reset icons color
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  auto lEditFiltersButton{this->findChild<QPushButton*>("edit_filters")};
  lEditFiltersButton->setIcon(QIcon(QPixmap(QString(":/%1/filter").arg(lIconFolder))));

  auto lSkeletonRefresherButton{this->findChild<QPushButton*>("skeleton_chooser_refresher")};
  lSkeletonRefresherButton->setIcon(QIcon(QPixmap(QString(":/%1/refresh").arg(lIconFolder))));
}

void MFBOPresetCreator::applyFont(QString aFamily, QString aStyleName, int aSize, int aWeight, bool aItalic, bool aUnderline, bool aStrikeOut)
{
  // Set the font properties
  QFont lFont(aFamily, aSize, aWeight, aItalic);
  lFont.setStyleName(aStyleName);
  lFont.setUnderline(aUnderline);
  lFont.setStrikeOut(aStrikeOut);
  lFont.setStyleStrategy(QFont::PreferAntialias);
  qApp->setFont(lFont);
}

std::vector<QLineEdit*> MFBOPresetCreator::disableLineEditPlaceholders()
{
  // Workaround function for style glitch with QSS and QLineEdit
  std::vector<QLineEdit*> lList;

  auto lMeshesPathBody{this->findChild<QLineEdit*>("meshes_path_input_femalebody")};
  if (lMeshesPathBody->isEnabled())
  {
    lMeshesPathBody->setDisabled(true);
    lList.push_back(lMeshesPathBody);
  }

  auto lMeshesPathFeet{this->findChild<QLineEdit*>("meshes_path_input_femalefeet")};
  if (lMeshesPathFeet->isEnabled())
  {
    lMeshesPathFeet->setDisabled(true);
    lList.push_back(lMeshesPathFeet);
  }

  auto lMeshesPathHands{this->findChild<QLineEdit*>("meshes_path_input_femalehands")};
  if (lMeshesPathHands->isEnabled())
  {
    lMeshesPathHands->setDisabled(true);
    lList.push_back(lMeshesPathHands);
  }

  auto lBodyMeshNameInput{this->findChild<QLineEdit*>("body_mesh_name_input")};
  if (lBodyMeshNameInput->isEnabled())
  {
    lBodyMeshNameInput->setDisabled(true);
    lList.push_back(lBodyMeshNameInput);
  }

  auto lFeetMeshNameInput{this->findChild<QLineEdit*>("feet_mesh_name_input")};
  if (lFeetMeshNameInput->isEnabled())
  {
    lFeetMeshNameInput->setDisabled(true);
    lList.push_back(lFeetMeshNameInput);
  }

  auto lHandsMeshNameInput{this->findChild<QLineEdit*>("hands_mesh_name_input")};
  if (lHandsMeshNameInput->isEnabled())
  {
    lHandsMeshNameInput->setDisabled(true);
    lList.push_back(lHandsMeshNameInput);
  }

  auto lSkeletonPathLineEdit{this->findChild<QLineEdit*>("skeleton_path_directory")};
  if (lSkeletonPathLineEdit->isEnabled())
  {
    lSkeletonPathLineEdit->setDisabled(true);
    lList.push_back(lSkeletonPathLineEdit);
  }

  auto lSkeletonName{this->findChild<QLineEdit*>("skeleton_name")};
  if (lSkeletonName->isEnabled())
  {
    lSkeletonName->setDisabled(true);
    lList.push_back(lSkeletonName);
  }

  return lList;
}

void MFBOPresetCreator::enableLineEditPlaceholders(std::vector<QLineEdit*> aLineEditsToReactivate)
{
  // Workaround for style glitch with QSS and QLineEdit
  for (auto& lLineEdit : aLineEditsToReactivate)
  {
    lLineEdit->setDisabled(false);
  }
}

void MFBOPresetCreator::checkForUpdate()
{
  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases"};

  QNetworkReply* lReply{this->mManager.get(QNetworkRequest(QUrl(lGitHubURL)))};
  connect(lReply, &QNetworkReply::finished, this, &MFBOPresetCreator::updateCheckFinished);
}

void MFBOPresetCreator::displayUpdateMessage(const QString& aResult)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Display a message based on new available versions
  auto lTitle{QString()};
  auto lMessage{QString()};

  if (aResult == "fetch_error")
  {
    lTitle = tr("Error while searching for a new update");
    lMessage = tr("An error has occurred while searching for a new version...\nMake sure your internet connection is operational and try again.");
  }
  else
  {
    // Declare and initialize local variables
    auto lStableVersions{QStringList()};
    auto lBetaVersions{QStringList()};
    auto lUserRunningBetaVersion{false};
    auto lTagName{QString()};
    auto lCurrentVersion{Utils::getApplicationVersion()};

    // Create a JSON from the fetched string and parse the "tag_name" data
    QJsonDocument lJsonDocument{QJsonDocument::fromJson(aResult.toUtf8())};
    QJsonArray lTagsArray{lJsonDocument.array()};

    // Iterate in the versions array
    for (int i = 0; i < lTagsArray.size(); i++)
    {
      // Parse the tag_name
      lTagName = lTagsArray.at(i)["tag_name"].toString();
      Utils::cleanBreaksString(lTagName);

      // Check if it is a stable or a BETA version
      if (lTagsArray.at(i)["prerelease"].toBool())
      {
        // Add this version name to the beta versions list
        lBetaVersions.push_back(lTagName);
      }
      else
      {
        // Add this version name to the stable versions list
        lStableVersions.push_back(lTagName);
      }
    }

#ifndef DEBUG
    auto lIsUserRunningBeta{lBetaVersions.size() > 0 && lBetaVersions.contains(lCurrentVersion)};
    if (lIsUserRunningBeta && lBetaVersions.at(0) != lCurrentVersion && lStableVersions.size() > 0)
    {
      this->mNewBetaVersionAvailable = true;
      lTitle = tr("Application update available (beta)");
      lMessage = tr("You are currently running the beta version \"%1\".\nThe latest stable version is tagged \"%2\".\nThe new beta version \"%3\" is available on GitHub.\nDo you want to download it now?").arg(lCurrentVersion).arg(lStableVersions.at(0)).arg(lBetaVersions.at(0));
    }
    else if (!lIsUserRunningBeta && lStableVersions.size() > 0 && lStableVersions.at(0) != lCurrentVersion)
    {
      this->mNewStableVersionAvailable = true;
      lTitle = tr("Application update available (stable)");
      lMessage = tr("You are currently running the stable version \"%1\".\nThe new stable version \"%2\" is available on GitHub.\nDo you want to download it now?").arg(lCurrentVersion).arg(lStableVersions.at(0));
    }
#endif
  }

  this->initializeGUI();

  if (aResult == "fetch_error")
  {
    QMessageBox lConfirmationBox(QMessageBox::Icon::Information, lTitle, lMessage, QMessageBox::StandardButton::NoButton, this);
    lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/alert-circle").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

    auto lOKButton{lConfirmationBox.addButton(tr("OK"), QMessageBox::ButtonRole::AcceptRole)};
    lOKButton->setCursor(Qt::PointingHandCursor);
    lConfirmationBox.setDefaultButton(lOKButton);
    lConfirmationBox.exec();
  }
  else if (this->mNewStableVersionAvailable || this->mNewBetaVersionAvailable)
  {
    if (Utils::displayQuestionMessage(this,
                                      lTitle,
                                      lMessage,
                                      lIconFolder,
                                      "info-circle",
                                      tr("Download and install the update now"),
                                      tr("Download later"),
                                      this->mSettings.successColor,
                                      this->mSettings.dangerColor,
                                      true)
        == ButtonClicked::Yes)
    {
      this->launchUpdateDialog();
    }
  }
}

void MFBOPresetCreator::refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings)
{
  if (aMustUpdateSettings)
  {
    this->mSettings = aSettings;

    // Update the settings
    auto lMainHandler{qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))};
    lMainHandler->updateSettings(aSettings);
  }
  else
  {
    // Apply the QSS theme
    this->applyGlobalStyleSheet();

    // Set the font properties
    applyFont(aSettings.font.family,
              aSettings.font.styleName,
              aSettings.font.size,
              aSettings.font.weight,
              aSettings.font.italic,
              aSettings.font.underline,
              aSettings.font.strikeOut);
  }
}

void MFBOPresetCreator::loadProject()
{
  qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))->loadProject();
}

void MFBOPresetCreator::saveProject()
{
  auto lEventSource{qobject_cast<QAction*>(sender())};
  if (lEventSource->objectName().compare("action_save_project") == 0)
  {
    qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))->saveProject(false);
  }
  else if (lEventSource->objectName().compare("action_save_project_as") == 0)
  {
    qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))->saveProject(true);
  }
}

void MFBOPresetCreator::quickRelaunch()
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  auto lMainContainer{qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))};
  if (!lMainContainer->hasUserDoneSomething())
  {
    qApp->exit(Utils::EXIT_CODE_REBOOT);
    return;
  }

  if (Utils::displayQuestionMessage(this,
                                    tr("Quick restart"),
                                    tr("You will lose all the unsaved data. Do you still want to quickly restart the application?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Quickly restart the application"),
                                    tr("Go back to the application"),
                                    this->mSettings.dangerColor,
                                    this->mSettings.successColor,
                                    false)
      == ButtonClicked::Yes)
  {
    qApp->exit(Utils::EXIT_CODE_REBOOT);
  }
}

void MFBOPresetCreator::launchAssistedConversion()
{
  auto lDialog{new AssistedConversion(this, this->mSettings, &this->mLastPaths)};
  this->connect(lDialog, &AssistedConversion::valuesChosen, this, &MFBOPresetCreator::fillUIByAssistedConversionValues);
}

void MFBOPresetCreator::fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList)
{
  if (aResultsList.size() == 0)
  {
    return;
  }

  // Proxy the event to the current tab
  auto lMainContainer{qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))};
  lMainContainer->fillUIByAssistedConversionValues(aPresetName, aResultsList);
}

void MFBOPresetCreator::launchBatchConversion()
{
  new BatchConversion(this, this->mSettings, &this->mLastPaths);

  // Update the BodySlide sets in case they were modified through the RetargetingTool's window
  auto lMainHandler{qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))};
  lMainHandler->updateBodySlideSets();
}

void MFBOPresetCreator::launchPresetsRetargeting()
{
  new RetargetingTool(this, this->mSettings, &this->mLastPaths);

  // Update the BodySlide sets in case they were modified through the RetargetingTool's window
  auto lMainHandler{qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))};
  lMainHandler->updateBodySlideSets();
}

void MFBOPresetCreator::launchTexturesAssistant()
{
  new TexturesAssistant(this, this->mSettings, &this->mLastPaths);
}

void MFBOPresetCreator::launchSettingsDialog()
{
  auto lSettings{new Settings(this, this->mSettings, this->mLastPaths)};
  this->connect(lSettings, &Settings::refreshMainUI, this, &MFBOPresetCreator::refreshUI);
  this->connect(lSettings, &Settings::refreshLastPaths, this, [=](const std::map<QString, QString>& aLastPaths) { this->mLastPaths = aLastPaths; });
}

void MFBOPresetCreator::launchUpdateDialog()
{
  new Update(this, this->mSettings);
}

void MFBOPresetCreator::openNexusPageInDefaultBrowser()
{
  QDesktopServices::openUrl(QUrl("https://www.nexusmods.com/skyrimspecialedition/mods/44706"));
}

void MFBOPresetCreator::openGitHubSourceCodePageInDefaultBrowser()
{
  QDesktopServices::openUrl(QUrl("https://github.com/Mitsuriou/MFBO-Preset-Creator"));
}

void MFBOPresetCreator::openGitLabSourceCodePageInDefaultBrowser()
{
  QDesktopServices::openUrl(QUrl("https://gitlab.com/Mitsuriou/MFBO-Preset-Creator"));
}

void MFBOPresetCreator::openGuideInDefaultBrowser()
{
  QDesktopServices::openUrl(QUrl("https://docs.google.com/document/d/1WpDKMk_WoPRrj0Lkst6TptUGEFAC2xYGd3HUBYxPQ-A/edit?usp=sharing"));
}

void MFBOPresetCreator::launchAboutDialog()
{
  new About(this, this->mSettings);
}

void MFBOPresetCreator::updateCheckFinished()
{
  auto lReply{qobject_cast<QNetworkReply*>(sender())};

  if (lReply->error() == QNetworkReply::NoError)
  {
    this->displayUpdateMessage(QString::fromLocal8Bit(lReply->readAll()));
  }
  else
  {
    this->displayUpdateMessage("fetch_error");
  }

  lReply->deleteLater();
}
