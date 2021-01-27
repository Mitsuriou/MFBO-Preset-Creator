#include "MFBOPresetCreator.h"

MFBOPresetCreator::MFBOPresetCreator(Struct::Settings aSettings, QWidget* parent)
  : QMainWindow(parent)
  , mSettings(aSettings)
  , mLastPaths(Utils::loadLastPathsFromFile())
  , mNewVersionAvailable(false)
{
  // Construct the GUI
  ui.setupUi(this);

#ifdef DEBUG
  qApp->setApplicationDisplayName(qApp->applicationDisplayName() + tr(" (DEV)"));
#endif

  // Check for new versions
  if (mSettings.checkForUpdatesAtStartup)
  {
    this->checkForUpdate();
  }
  else
  {
    this->initializeGUI();
  }

  this->downloadLatestUpdate();
}

void MFBOPresetCreator::closeEvent(QCloseEvent* aEvent)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  QMessageBox lConfirmationBox(QMessageBox::Icon::Question, tr("Quitting"), tr("Do you want to quit the application?"), QMessageBox::StandardButton::NoButton, this);
  lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/help-circle").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

  auto lCloseButton{lConfirmationBox.addButton(tr("Quit the application"), QMessageBox::ButtonRole::YesRole)};
  lCloseButton->setCursor(Qt::PointingHandCursor);
  lCloseButton->setStyleSheet(QString("color: %1;").arg(this->mSettings.dangerColor));

  auto lStayButton{lConfirmationBox.addButton(tr("Go back to the application"), QMessageBox::ButtonRole::NoRole)};
  lStayButton->setCursor(Qt::PointingHandCursor);
  lStayButton->setStyleSheet(QString("color: %1;").arg(this->mSettings.successColor));

  lConfirmationBox.setDefaultButton(lStayButton);
  lConfirmationBox.exec();

  if (lConfirmationBox.clickedButton() != lCloseButton)
  {
    aEvent->ignore();
  }
  else
  {
    aEvent->accept();
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
}

void MFBOPresetCreator::setupMenuBar()
{
  // Keep a reference to the user theme
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // Menu bar
  auto lMenuBar{new QMenuBar(this)};
  lMenuBar->setCursor(Qt::PointingHandCursor);
  this->setMenuBar(lMenuBar);

  // Menu: File
  auto lFile{new QMenu(tr("File"), this)};
  lFile->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lFile);

  // Action: Relaunch the app
  auto lRelaunchApp{Utils::buildQAction(this, tr("Quick relaunch"), QKeySequence(Qt::CTRL + Qt::Key_F5), "refresh", lIconFolder)};
  lRelaunchApp->setObjectName("action_quick_relaunch");
  lFile->addAction(lRelaunchApp);

  // Action: Exit
  auto lExitApp{Utils::buildQAction(this, tr("Exit"), QKeySequence(Qt::ALT + Qt::Key_F4), "cross", lIconFolder)};
  lFile->addAction(lExitApp);

  // Menu: Tools
  auto lTools{new QMenu(tr("Tools"), this)};
  lTools->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lTools);

  // Action: Assisted conversion
  auto lOpenAssiConv{Utils::buildQAction(this, tr("Assisted Conversion"), QKeySequence(Qt::CTRL + Qt::Key_T), "pencil", lIconFolder)};
  lTools->addAction(lOpenAssiConv);

  // Action: BodySlide Presets' Retargeting
  auto lOpenRetaTools{Utils::buildQAction(this, tr("BodySlide Presets' Retargeting"), QKeySequence(Qt::CTRL + Qt::Key_Y), "arrow-up", lIconFolder)};
  lTools->addAction(lOpenRetaTools);

  // Action: Settings
  auto lOpenSettings{Utils::buildQAction(this, tr("Settings"), QKeySequence(Qt::CTRL + Qt::Key_O), "cog", lIconFolder)};
  lTools->addAction(lOpenSettings);

  // Menu: Help
  const auto& lUpdateAvailableText{this->mNewVersionAvailable ? tr(" (update available)") : QString("")};

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
  this->connect(lRelaunchApp, &QAction::triggered, this, &MFBOPresetCreator::quickRelaunch);
  this->connect(lExitApp, &QAction::triggered, this, &MFBOPresetCreator::close);
  this->connect(lOpenAssiConv, &QAction::triggered, this, &MFBOPresetCreator::launchAssistedConversion);
  this->connect(lOpenRetaTools, &QAction::triggered, this, &MFBOPresetCreator::launchPresetsRetargeting);
  this->connect(lOpenSettings, &QAction::triggered, this, &MFBOPresetCreator::launchSettingsDialog);
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
  QRect lScreenGeom{lDesktopScreen->availableGeometry()};

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
  auto lQSSFileName{QString("")};

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
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

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

void MFBOPresetCreator::refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings)
{
  if (aMustUpdateSettings)
  {
    this->mSettings = aSettings;

    // Update the settings
    auto lMainContainer{qobject_cast<PresetCreator*>(this->findChild<QWidget*>("main_container"))};
    lMainContainer->updateSettings(aSettings);
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

void MFBOPresetCreator::checkForUpdate()
{
  QString lGitHubURL{"https://api.github.com/repos/Mitsuriou/MFBO-Preset-Creator/releases/latest"};
  HTTPRequesterGet* lHTTPDownloader{new HTTPRequesterGet(lGitHubURL, this)};
  this->connect(lHTTPDownloader, &HTTPRequesterGet::resultReady, this, &MFBOPresetCreator::pageFetched);
  this->connect(lHTTPDownloader, &HTTPRequesterGet::finished, lHTTPDownloader, &QObject::deleteLater);
  lHTTPDownloader->start();
}

void MFBOPresetCreator::downloadLatestUpdate()
{
  QString lGitHubURL{"https://github.com/Mitsuriou/MFBO-Preset-Creator/releases/latest/download/mfbopc-install-wizard.exe"};
  HTTPRequesterFile* lHTTPDownloader{new HTTPRequesterFile(lGitHubURL, this)};
  this->connect(lHTTPDownloader, &HTTPRequesterFile::resultReady, this, &MFBOPresetCreator::fileFetched);
  this->connect(lHTTPDownloader, &HTTPRequesterFile::finished, lHTTPDownloader, &QObject::deleteLater);
  lHTTPDownloader->start();
}

void MFBOPresetCreator::pageFetched(const QString& aResult)
{
  // Display a message based on new available versions
  auto lTitle{QString("")};
  auto lMessage{QString("")};

  if (aResult == "fetch_error")
  {
    lTitle = "Error while searching for a new update";
    lMessage = tr("An error has occurred while searching for a new version... Make sure your internet connection is operational and try again.");
  }
  else
  {
    // Create a JSON from the fetched string and parse the "tag_name" data
    QJsonDocument doc{QJsonDocument::fromJson(aResult.toUtf8())};
    QJsonObject obj{doc.object()};
    auto lTagName{obj["tag_name"].toString()};
    Utils::cleanBreaksString(lTagName);
    auto lCurrentVersion{Utils::getApplicationVersion()};

#ifndef DEBUG
    if (lCurrentVersion != lTagName)
    {
      this->mNewVersionAvailable = true;
      lTitle = tr("Application update available");
      lMessage = tr("You are currently running the version \"%1\".\nThe new version \"%2\" is available on GitHub.").arg(lCurrentVersion).arg(lTagName);
    }
#endif
  }

  this->initializeGUI();

  if (aResult == "fetch_error" || this->mNewVersionAvailable)
  {
    // User theme accent
    const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

    QMessageBox lConfirmationBox(QMessageBox::Icon::Information, lTitle, lMessage, QMessageBox::StandardButton::NoButton, this);
    lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/info-circle").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

    auto lOKButton{lConfirmationBox.addButton(tr("OK"), QMessageBox::ButtonRole::AcceptRole)};
    lOKButton->setCursor(Qt::PointingHandCursor);
    lConfirmationBox.setDefaultButton(lOKButton);
    lConfirmationBox.exec();
  }
}

void MFBOPresetCreator::fileFetched(const bool& aResult)
{
  auto stop = true;
}

void MFBOPresetCreator::quickRelaunch()
{
  qApp->exit(Utils::EXIT_CODE_REBOOT);
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

void MFBOPresetCreator::launchPresetsRetargeting()
{
  new RetargetingTool(this, this->mSettings, &this->mLastPaths);
}

void MFBOPresetCreator::launchSettingsDialog()
{
  auto lSettings{new Settings(this, this->mSettings)};
  this->connect(lSettings, &Settings::refreshMainUI, this, &MFBOPresetCreator::refreshUI);
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
