#include "MFBOPresetCreator.h"
#include "About.h"
#include "AssistedConversion.h"
#include "BatchConversion.h"
#include "Enum.h"
#include "PresetCreator.h"
#include "ReleaseNotesViewer.h"
#include "RetargetingTool.h"
#include "Settings.h"
#include "TexturesAssistant.h"
#include "Update.h"
#include "Utils.h"
#include "WelcomeScreen.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDropEvent>
#include <QFileInfo>
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
  , mLastPaths(Utils::LoadLastPathsFromFile())
  , mNewStableVersionAvailable(false)
  , mNewBetaVersionAvailable(false)
{
  // Construct the GUI
  ui.setupUi(this);

#ifdef DEBUG
  qApp->setApplicationDisplayName(tr("[DEV] ") + qApp->applicationDisplayName());
#endif

  // Check for new versions
  if (this->mSettings.startupAction == StartupAction::OPEN_WELCOME_SCREEN || this->mSettings.startupAction == StartupAction::CHECK_FOR_UPDATES)
  {
    this->checkForUpdate();
  }
  else
  {
    Utils::PrintMessageStdOut("Skipped update checking");
    this->initializeGUI();
  }
}

void MFBOPresetCreator::closeEvent(QCloseEvent* aEvent)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  auto lMainContainer{qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))};
  if (!lMainContainer->hasUserDoneSomething())
  {
    aEvent->accept();
    return;
  }

  auto lUserAnswer{Utils::DisplayQuestionMessage(this,
                                                 tr("Quitting"),
                                                 tr("You will lose all the unsaved data. Do you still want to quit the application?"),
                                                 lIconFolder,
                                                 "help-circle",
                                                 tr("Quit the application"),
                                                 tr("Go back to the application"),
                                                 tr("Save the project and quit the application"),
                                                 this->mSettings.dangerColor,
                                                 this->mSettings.successColor,
                                                 this->mSettings.successColor,
                                                 false)};

  if (lUserAnswer == ButtonClicked::YES)
  {
    aEvent->accept();
  }
  else if (lUserAnswer == ButtonClicked::OTHER)
  {
    this->saveProject(true);
    aEvent->accept();
  }
  else
  {
    aEvent->ignore();
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
      qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))->loadProject(lFileName, true);
    }
  }
}

void MFBOPresetCreator::initializeGUI()
{
  this->setupMenuBar();

  auto lMainContainer{new PresetCreator(this, this->mSettings, &this->mLastPaths)};
  lMainContainer->setObjectName(QString("main_container"));
  this->setCentralWidget(lMainContainer);

  this->refreshUI(mSettings, false);

  this->setAcceptDrops(true);

  Utils::CleanPathString(this->mInjectedFilePath);
  lMainContainer->loadProject(this->mInjectedFilePath, true);

  if (this->mSettings.startupAction == StartupAction::OPEN_WELCOME_SCREEN)
  {
    this->launchWelcomeScreen();
  }

  this->showWindow();
}

void MFBOPresetCreator::setupMenuBar()
{
  // Keep a reference to the user theme
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  // Menu bar
  auto lMenuBar{new QMenuBar(this)};
  lMenuBar->setCursor(Qt::PointingHandCursor);
  this->setMenuBar(lMenuBar);

  // Menu: File
  auto lFile{new QMenu(tr("File"), this)};
  lFile->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lFile);

  // Action: Launch the welcome screen
  auto lLaunchWelcomeScreen{Utils::BuildQAction(this, tr("Welcome screen"), QKeySequence(Qt::SHIFT + Qt::Key::Key_Escape), "home", lIconFolder)};
  lFile->addAction(lLaunchWelcomeScreen);

  lFile->addSeparator();

  // Action: Open project file
  auto lOpenProjectFile{Utils::BuildQAction(this, tr("Open project..."), QKeySequence(Qt::CTRL + Qt::Key_O), "file", lIconFolder)};
  lOpenProjectFile->setObjectName(QString("action_open_project"));
  lFile->addAction(lOpenProjectFile);

  // Action: Save current project
  auto lSaveProject{Utils::BuildQAction(this, tr("Save"), QKeySequence(Qt::CTRL + Qt::Key_S), "save", lIconFolder)};
  lSaveProject->setObjectName(QString("action_save_project"));
  lSaveProject->setDisabled(true);
  lFile->addAction(lSaveProject);

  // Action: Save current project as
  auto lSaveProjectAs{Utils::BuildQAction(this, tr("Save as"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), "save", lIconFolder)};
  lSaveProjectAs->setObjectName(QString("action_save_project_as"));
  lFile->addAction(lSaveProjectAs);

  lFile->addSeparator();

  // Action: Settings
  auto lOpenSettings{Utils::BuildQAction(this, tr("Settings"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O), "cog", lIconFolder)};
  lFile->addAction(lOpenSettings);

  lFile->addSeparator();

  // Action: Relaunch the app
  auto lRelaunchApp{Utils::BuildQAction(this, tr("Quick restart"), QKeySequence(Qt::CTRL + Qt::Key_F5), "refresh", lIconFolder)};
  lFile->addAction(lRelaunchApp);

  // Action: Exit
  auto lExitApp{Utils::BuildQAction(this, tr("Exit"), QKeySequence(Qt::ALT + Qt::Key_F4), "cross", lIconFolder)};
  lFile->addAction(lExitApp);

  // Menu: Tools
  auto lTools{new QMenu(tr("Tools"), this)};
  lTools->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lTools);

  // Action: Batch conversion
  auto lOpenBatchConv{Utils::BuildQAction(this, tr("Batch Conversion"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R), "reorder", lIconFolder)};
  lTools->addAction(lOpenBatchConv);

  // Action: Textures Assistant
  auto lOpenTextAssist{Utils::BuildQAction(this, tr("Textures Assistant"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T), "textures", lIconFolder)};
  lTools->addAction(lOpenTextAssist);

  // Action: Assisted Conversion
  auto lOpenAssiConv{Utils::BuildQAction(this, tr("Assisted Conversion"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Y), "pencil", lIconFolder)};
  lTools->addAction(lOpenAssiConv);

  // Action: BodySlide Presets' Retargeting
  auto lOpenRetaTools{Utils::BuildQAction(this, tr("BodySlide Presets' Retargeting"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_H), "arrow-up", lIconFolder)};
  lTools->addAction(lOpenRetaTools);

  // Menu: Help
  auto lUpdateAvailableText{QString()};
  if (this->mNewStableVersionAvailable)
    lUpdateAvailableText = tr(" (stable update available)");
  else if (this->mNewBetaVersionAvailable)
    lUpdateAvailableText = tr(" (BETA update available)");

  auto lHelp{new QMenu(tr("Help") + lUpdateAvailableText, this)};
  lHelp->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lHelp);

  // Action: Check for updates
  auto lOpenUpdate{Utils::BuildQAction(this, tr("Check for updates") + lUpdateAvailableText, QKeySequence(Qt::CTRL + Qt::Key_U), "cloud-search", lIconFolder)};
  lHelp->addAction(lOpenUpdate);

  // Action: Open current version's release notes
  auto lOpenCurrentVersionReleaseNotes{Utils::BuildQAction(this, tr("Current version's release notes"), QKeySequence(), "text-snippet", lIconFolder)};
  lHelp->addAction(lOpenCurrentVersionReleaseNotes);

  // Submenu: Report a bug
  auto lReportBugSubmenu{new QMenu(tr("Report a bug..."), this)};
  lReportBugSubmenu->setIcon(QIcon(QPixmap(QString(":/%1/bug").arg(lIconFolder))));
  lReportBugSubmenu->setCursor(Qt::PointingHandCursor);
  lHelp->addMenu(lReportBugSubmenu);

  // Action: Report a bug from Nexus Mods
  auto lReportBugNexusMods{Utils::BuildQAction(this, tr("Nexus Mods page (nexusmods.com)"), QKeySequence(), "nexus-logo", lIconFolder)};
  lReportBugSubmenu->addAction(lReportBugNexusMods);

  // Action: Report a bug from GitHub
  auto lReportBugGitHub{Utils::BuildQAction(this, tr("GitHub (github.com)"), QKeySequence(), "github", lIconFolder)};
  lReportBugSubmenu->addAction(lReportBugGitHub);

  // Action: Report a bug from GitLab
  auto lReportBugGitLab{Utils::BuildQAction(this, tr("GitLab (gitlab.com)"), QKeySequence(), "gitlab", lIconFolder)};
  lReportBugSubmenu->addAction(lReportBugGitLab);

  // Submenu: Links
  auto lLinksSubmenu{new QMenu(tr("Useful links..."), this)};
  lLinksSubmenu->setIcon(QIcon(QPixmap(QString(":/%1/link").arg(lIconFolder))));
  lLinksSubmenu->setCursor(Qt::PointingHandCursor);
  lHelp->addMenu(lLinksSubmenu);

  // Action: Nexus page link
  auto lOpenNexus{Utils::BuildQAction(this, tr("Nexus Mods page (nexusmods.com)"), QKeySequence(Qt::CTRL + Qt::Key_N), "nexus-logo", lIconFolder)};
  lLinksSubmenu->addAction(lOpenNexus);

  // Action: open URL to Doogle Docs: Guide and tutorials
  auto lOpenGuide{Utils::BuildQAction(this, tr("User guide and tutorials (docs.google.com)"), QKeySequence(Qt::CTRL + Qt::Key_G), "text-file", lIconFolder)};
  lLinksSubmenu->addAction(lOpenGuide);

  // Action: Source code on GitHub
  auto lOpenSourceCodeGitHub{Utils::BuildQAction(this, tr("View the source code (github.com)"), QKeySequence(), "github", lIconFolder)};
  lLinksSubmenu->addAction(lOpenSourceCodeGitHub);

  // Action: Source code on GitLab
  auto lOpenSourceCodeGitLab{Utils::BuildQAction(this, tr("View the source code (gitlab.com)"), QKeySequence(), "gitlab", lIconFolder)};
  lLinksSubmenu->addAction(lOpenSourceCodeGitLab);

  // Action: About
  auto lOpenAbout{Utils::BuildQAction(this, tr("About"), QKeySequence(Qt::CTRL + Qt::Key_I), "info-circle", lIconFolder)};
  lHelp->addAction(lOpenAbout);

  // Event binding
  this->connect(lLaunchWelcomeScreen, &QAction::triggered, this, &MFBOPresetCreator::launchWelcomeScreen);
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
  this->connect(lOpenCurrentVersionReleaseNotes, &QAction::triggered, this, &MFBOPresetCreator::launchCurrentVersionReleaseNotes);
  this->connect(lReportBugNexusMods, &QAction::triggered, this, &MFBOPresetCreator::reportBugNexusMods);
  this->connect(lReportBugGitHub, &QAction::triggered, this, &MFBOPresetCreator::reportBugGitHub);
  this->connect(lReportBugGitLab, &QAction::triggered, this, &MFBOPresetCreator::reportBugGitLab);
  this->connect(lOpenNexus, &QAction::triggered, this, &MFBOPresetCreator::openNexusModsPage);
  this->connect(lOpenSourceCodeGitHub, &QAction::triggered, this, &MFBOPresetCreator::openGitHubSourceCodePage);
  this->connect(lOpenSourceCodeGitLab, &QAction::triggered, this, &MFBOPresetCreator::openGitLabSourceCodePage);
  this->connect(lOpenGuide, &QAction::triggered, this, &MFBOPresetCreator::openGoogleDriveGuide);
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
  if (mSettings.mainWindowOpeningMode == WindowOpeningMode::MINIMIZED)
  {
    this->showMinimized();

    // Make the taskbar icon blink (Windows only)
    QApplication::alert(this, 4000);
  }
  else if (mSettings.mainWindowOpeningMode == WindowOpeningMode::WINDOWED)
  {
    this->show();
  }

  auto lPosX{(lScreenGeom.width() - this->frameGeometry().width()) / 2 + 1};
  auto lPosY{(lScreenGeom.height() - this->frameGeometry().height()) / 2 + 31};
  this->setGeometry(QRect(lPosX, lPosY, this->geometry().width(), this->geometry().height()));

  if (mSettings.mainWindowOpeningMode == WindowOpeningMode::MAXIMIZED)
  {
    this->showMaximized();
  }
}

void MFBOPresetCreator::launchWelcomeScreen()
{
  auto lWelcomeScreen{new WelcomeScreen(this, this->mSettings)};
  this->connect(lWelcomeScreen, &WelcomeScreen::refreshMainUI, this, &MFBOPresetCreator::refreshUI);
}

void MFBOPresetCreator::applyGlobalStyleSheet()
{
  auto lQSSFileName{QString()};

  switch (this->mSettings.appTheme)
  {
    case GUITheme::WINDOWS_VISTA:
      break;
    case GUITheme::MITSURIOU_BLACK_THEME:
      lQSSFileName = "Mitsuriou's Black Theme";
      break;
    case GUITheme::MITSURIOU_DARK_THEME:
      lQSSFileName = "Mitsuriou's Dark Theme";
      break;
    case GUITheme::MITSURIOU_LIGHT_THEME:
      lQSSFileName = "Mitsuriou's Light Theme";
      break;
    case GUITheme::ALEXHUSZAGH_BREEZE_DARK:
      lQSSFileName = "Alexhuszagh's Breeze Dark";
      break;
    case GUITheme::ALEXHUSZAGH_BREEZE_LIGHT:
      lQSSFileName = "Alexhuszagh's Breeze Light";
      break;
    case GUITheme::PAPER_DARK:
      lQSSFileName = "Paper Dark by 6788";
      break;
    case GUITheme::PAPER_LIGHT:
      lQSSFileName = "Paper Light by 6788";
      break;
    case GUITheme::PAPER_BLACK_MONO:
      lQSSFileName = "Paper Black Mono";
      break;
    case GUITheme::PAPER_WHITE_MONO:
      lQSSFileName = "Paper White Mono";
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
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  auto lEditFiltersButton{this->findChild<QPushButton*>(QString("edit_filters"))};
  lEditFiltersButton->setIcon(QIcon(QPixmap(QString(":/%1/filter").arg(lIconFolder))));

  auto lSkeletonRefresherButton{this->findChild<QPushButton*>(QString("skeleton_chooser_refresher"))};
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

  auto lMeshesPathBody{this->findChild<QLineEdit*>(QString("meshes_path_input_femalebody"))};
  if (lMeshesPathBody->isEnabled())
  {
    lMeshesPathBody->setDisabled(true);
    lList.push_back(lMeshesPathBody);
  }

  auto lMeshesPathFeet{this->findChild<QLineEdit*>(QString("meshes_path_input_femalefeet"))};
  if (lMeshesPathFeet->isEnabled())
  {
    lMeshesPathFeet->setDisabled(true);
    lList.push_back(lMeshesPathFeet);
  }

  auto lMeshesPathHands{this->findChild<QLineEdit*>(QString("meshes_path_input_femalehands"))};
  if (lMeshesPathHands->isEnabled())
  {
    lMeshesPathHands->setDisabled(true);
    lList.push_back(lMeshesPathHands);
  }

  auto lBodyMeshNameInput{this->findChild<QLineEdit*>(QString("body_mesh_name_input"))};
  if (lBodyMeshNameInput->isEnabled())
  {
    lBodyMeshNameInput->setDisabled(true);
    lList.push_back(lBodyMeshNameInput);
  }

  auto lFeetMeshNameInput{this->findChild<QLineEdit*>(QString("feet_mesh_name_input"))};
  if (lFeetMeshNameInput->isEnabled())
  {
    lFeetMeshNameInput->setDisabled(true);
    lList.push_back(lFeetMeshNameInput);
  }

  auto lHandsMeshNameInput{this->findChild<QLineEdit*>(QString("hands_mesh_name_input"))};
  if (lHandsMeshNameInput->isEnabled())
  {
    lHandsMeshNameInput->setDisabled(true);
    lList.push_back(lHandsMeshNameInput);
  }

  auto lSkeletonPathLineEdit{this->findChild<QLineEdit*>(QString("skeleton_path_directory"))};
  if (lSkeletonPathLineEdit->isEnabled())
  {
    lSkeletonPathLineEdit->setDisabled(true);
    lList.push_back(lSkeletonPathLineEdit);
  }

  auto lSkeletonName{this->findChild<QLineEdit*>(QString("skeleton_name"))};
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
  Utils::PrintMessageStdOut("Checking for updates...");

  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases"};

  QNetworkReply* lReply{this->mManager.get(QNetworkRequest(QUrl(lGitHubURL)))};
  connect(lReply, &QNetworkReply::finished, this, &MFBOPresetCreator::updateCheckFinished);
}

void MFBOPresetCreator::displayUpdateMessage(const QString& aResult)
{
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
    const auto lCurrentVersion{Utils::GetApplicationVersion()};
    const auto lVersionsInformation{Utils::ParseGitHubReleasesRequestResult(aResult)};

    if (lVersionsInformation.sizeStableVersionsList() > 0 && lVersionsInformation.sizeBetaVersionsList() > 0)
    {
      // A new BETA version is available and no newer stable available
      if (Utils::CompareVersionNumbers(lVersionsInformation.getLatestBetaVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::NEWER
          && Utils::CompareVersionNumbers(lVersionsInformation.getLatestBetaVersionNumber(), lVersionsInformation.getLatestStableVersionNumber()) == ApplicationVersionRelative::NEWER)
      {
        this->mNewBetaVersionAvailable = true;
        lTitle = tr("BETA update available");
        lMessage = tr("You are currently running the version \"%1\".\nThe new BETA version \"%2\" is available on GitHub.\n\nDo you want to download it now?")
                     .arg(lCurrentVersion)
                     .arg(lVersionsInformation.getLatestBetaVersionNumber());
      }
      // A new stable version is available
      else if (Utils::CompareVersionNumbers(lVersionsInformation.getLatestStableVersionNumber(), lCurrentVersion) == ApplicationVersionRelative::NEWER)
      {
        this->mNewStableVersionAvailable = true;
        lTitle = tr("Stable update available");
        lMessage = tr("You are currently running the version \"%1\".\nThe new stable version \"%2\" is available on GitHub.\n\nDo you want to download it now?")
                     .arg(lCurrentVersion)
                     .arg(lVersionsInformation.getLatestStableVersionNumber());
      }
    }
  }

  this->initializeGUI();

  // TODO: Correct the behavior of the popup, when the main window launches as minimized
  if (this->mSettings.startupAction == StartupAction::CHECK_FOR_UPDATES && !lTitle.isEmpty() && !lMessage.isEmpty())
  {
    // User theme accent
    const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

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
      if (Utils::DisplayQuestionMessage(this,
                                        lTitle,
                                        lMessage,
                                        lIconFolder,
                                        "info-circle",
                                        tr("Download and install the update now"),
                                        tr("Download later"),
                                        "",
                                        this->mSettings.successColor,
                                        this->mSettings.dangerColor,
                                        "",
                                        true)
          == ButtonClicked::YES)
      {
        this->launchUpdateDialog();
      }
    }
  }
}

void MFBOPresetCreator::refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings)
{
  if (aMustUpdateSettings)
  {
    this->mSettings = aSettings;

    // Update the settings
    auto lMainHandler{qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))};
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
  qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))->loadProject();
}

void MFBOPresetCreator::saveProject(const bool aIsQuittingContext)
{
  auto lEventSource{qobject_cast<QAction*>(this->sender())};
  if (aIsQuittingContext || lEventSource->objectName().compare("action_save_project") == 0)
  {
    qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))->saveProject(false);
  }
  else if (lEventSource->objectName().compare("action_save_project_as") == 0)
  {
    qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))->saveProject(true);
  }
}

void MFBOPresetCreator::quickRelaunch()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.appTheme)};

  auto lMainContainer{qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))};
  if (!lMainContainer->hasUserDoneSomething())
  {
    qApp->exit(Utils::EXIT_CODE_REBOOT);
    return;
  }

  auto lUserAnswer{Utils::DisplayQuestionMessage(this,
                                                 tr("Quick restart"),
                                                 tr("You will lose all the unsaved data. Do you still want to quickly restart the application?"),
                                                 lIconFolder,
                                                 "help-circle",
                                                 tr("Quickly restart the application"),
                                                 tr("Go back to the application"),
                                                 tr("Save the project and quit the application"),
                                                 this->mSettings.dangerColor,
                                                 this->mSettings.successColor,
                                                 this->mSettings.successColor,
                                                 false)};

  if (lUserAnswer == ButtonClicked::YES)
  {
    qApp->exit(Utils::EXIT_CODE_REBOOT);
  }
  else if (lUserAnswer == ButtonClicked::OTHER)
  {
    this->saveProject(true);
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
  auto lMainContainer{qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))};
  lMainContainer->fillUIByAssistedConversionValues(aPresetName, aResultsList);
}

void MFBOPresetCreator::launchBatchConversion()
{
  new BatchConversion(this, this->mSettings, &this->mLastPaths);

  // Update the BodySlide sets in case they were modified through the RetargetingTool's window
  auto lMainHandler{qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))};
  lMainHandler->updateBodySlideSets();
}

void MFBOPresetCreator::launchPresetsRetargeting()
{
  new RetargetingTool(this, this->mSettings, &this->mLastPaths);

  // Update the BodySlide sets in case they were modified through the RetargetingTool's window
  auto lMainHandler{qobject_cast<PresetCreator*>(this->findChild<QWidget*>(QString("main_container")))};
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
  new Update(this, this->mSettings, false, false);
}

void MFBOPresetCreator::launchCurrentVersionReleaseNotes()
{
  new ReleaseNotesViewer(this, this->mSettings);
}

void MFBOPresetCreator::reportBugNexusMods()
{
  QDesktopServices::openUrl(QUrl("https://www.nexusmods.com/skyrimspecialedition/mods/44706?tab=bugs"));
}

void MFBOPresetCreator::reportBugGitHub()
{
  QDesktopServices::openUrl(QUrl("https://github.com/Mitsuriou/MFBO-Preset-Creator/issues"));
}

void MFBOPresetCreator::reportBugGitLab()
{
  QDesktopServices::openUrl(QUrl("https://gitlab.com/Mitsuriou/MFBO-Preset-Creator/-/issues"));
}

void MFBOPresetCreator::openNexusModsPage()
{
  QDesktopServices::openUrl(QUrl("https://www.nexusmods.com/skyrimspecialedition/mods/44706"));
}

void MFBOPresetCreator::openGitHubSourceCodePage()
{
  QDesktopServices::openUrl(QUrl("https://github.com/Mitsuriou/MFBO-Preset-Creator"));
}

void MFBOPresetCreator::openGitLabSourceCodePage()
{
  QDesktopServices::openUrl(QUrl("https://gitlab.com/Mitsuriou/MFBO-Preset-Creator"));
}

void MFBOPresetCreator::openGoogleDriveGuide()
{
  QDesktopServices::openUrl(QUrl("https://docs.google.com/document/d/1WpDKMk_WoPRrj0Lkst6TptUGEFAC2xYGd3HUBYxPQ-A/edit?usp=sharing"));
}

void MFBOPresetCreator::launchAboutDialog()
{
  new About(this, this->mSettings);
}

void MFBOPresetCreator::updateCheckFinished()
{
  auto lReply{qobject_cast<QNetworkReply*>(this->sender())};

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
