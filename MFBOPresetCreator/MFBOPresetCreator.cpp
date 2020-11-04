#include "MFBOPresetCreator.h"

MFBOPresetCreator::MFBOPresetCreator(QWidget* parent)
  : QMainWindow(parent)
  , mSettings(Utils::loadSettingsFromFile())
  , mNewVersionAvailable(false)
{
  // Construct the GUI
  ui.setupUi(this);

  // Check for new versions
  this->checkForUpdate();
}

void MFBOPresetCreator::closeEvent(QCloseEvent* aEvent)
{
  auto lResult{QMessageBox::question(this, tr("Quitting"), tr("Are you sure you want to quit the software?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)};

  if (lResult != QMessageBox::Yes)
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
  // Main window container
  auto lMainVertical{new QVBoxLayout(this->ui.mainContainer)};
  lMainVertical->setContentsMargins(0, 0, 0, 0);

  // Create the tabs
  auto lTabsContainer{new QTabWidget()};
  lTabsContainer->setMovable(true);

  auto lCBBETab{new TabCBBESE(this, mSettings)};
  this->mTabs.push_back(lCBBETab);
  lTabsContainer->addTab(lCBBETab, QString("CBBE SE"));
  lMainVertical->addWidget(lTabsContainer);
}

void MFBOPresetCreator::setupMenuBar()
{
  // Keep a reference to the user theme
  auto lIconFolder{Utils::isThemeDark(mSettings.appTheme) ? QString("white") : QString("black")};

  // Construct the menu bar
  auto lMenuBar{new QMenuBar(this)};
  this->setMenuBar(lMenuBar);

  // File
  auto lFileMenu{new QMenu(tr("File"), this)};
  lMenuBar->addMenu(lFileMenu);

  // Submenu: relaunch the app
  auto lQuickRelaunch{new QAction(this)};
  lQuickRelaunch->setText(tr("Quick relaunch"));
  lQuickRelaunch->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
  lQuickRelaunch->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/reload")));
  lFileMenu->addAction(lQuickRelaunch);

  // Submenu: Exit
  auto lExitAction{new QAction(this)};
  lExitAction->setText(tr("Exit"));
  lExitAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
  lExitAction->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/exit")));
  lFileMenu->addAction(lExitAction);

  // Tools
  auto lToolsMenu{new QMenu(tr("Tools"), this)};
  lMenuBar->addMenu(lToolsMenu);

  // Submenu: Upgrader
  auto lRetargetingToolAction{new QAction(this)};
  lRetargetingToolAction->setText(tr("CBBE 3BBB Version Retargeting Tool"));
  lRetargetingToolAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
  lRetargetingToolAction->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/arrow_up")));
  lToolsMenu->addAction(lRetargetingToolAction);

  // Submenu: Settings
  auto lSettingsAction{new QAction(this)};
  lSettingsAction->setText(tr("Settings"));
  lSettingsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
  lSettingsAction->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/cog")));
  lToolsMenu->addAction(lSettingsAction);

  // Help
  auto lUpdateAvailableText{QString("")};

  if (this->mNewVersionAvailable)
  {
    lUpdateAvailableText = tr(" (update available)");
  }

  auto lHelpMenu{new QMenu(tr("Help") + lUpdateAvailableText, this)};
  lMenuBar->addMenu(lHelpMenu);

  // Submenu: Check for updates
  auto lCheckUpdateAction{new QAction(this)};
  lCheckUpdateAction->setText(tr("Check for updates") + lUpdateAvailableText);
  lCheckUpdateAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
  lCheckUpdateAction->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/download")));
  lHelpMenu->addAction(lCheckUpdateAction);

  // Submenu: About
  auto lAboutAction{new QAction(this)};
  lAboutAction->setText(tr("About"));
  lAboutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  lAboutAction->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/information")));
  lHelpMenu->addAction(lAboutAction);

  // Event binding
  this->connect(lQuickRelaunch, &QAction::triggered, this, &MFBOPresetCreator::quickRelaunch);
  this->connect(lExitAction, &QAction::triggered, this, &MFBOPresetCreator::close);
  this->connect(lRetargetingToolAction, &QAction::triggered, this, &MFBOPresetCreator::launchRetargetingTool);
  this->connect(lSettingsAction, &QAction::triggered, this, &MFBOPresetCreator::showSettingsDialog);
  this->connect(lCheckUpdateAction, &QAction::triggered, this, &MFBOPresetCreator::showUpdateDialog);
  this->connect(lAboutAction, &QAction::triggered, this, &MFBOPresetCreator::showAboutDialog);
}

void MFBOPresetCreator::showWindow()
{
  // Set the size of the window
  auto lDesktopScreen{QGuiApplication::primaryScreen()};
  QRect lScreenGeom{lDesktopScreen->availableGeometry()};

  // If the window size is correct for the user's screen
  if (mSettings.mainWindowWidth < lScreenGeom.width() && mSettings.mainWindowHeight < lScreenGeom.height())
  {
    // resize the window
    this->resize(mSettings.mainWindowWidth - 2, mSettings.mainWindowHeight - 62);
  }

  // Select the main window opening mode
  if (mSettings.mainWindowOpeningMode == WindowOpeningMode::Minimized)
  {
    this->showMinimized();

    // Make the icon in the taskbar blink
    FLASHWINFO finfo;
    finfo.cbSize = sizeof(FLASHWINFO);
    finfo.hwnd = (HWND)this->winId();
    finfo.uCount = 5;
    finfo.dwTimeout = 500;
    finfo.dwFlags = FLASHW_ALL;
    FlashWindowEx(&finfo);
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

  switch (mSettings.appTheme)
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
    case GUITheme::QuasarAppMaterialStyle:
      lQSSFileName = "QuasarApp's Material Style";
      break;
    case GUITheme::QuasarAppVisualStudioDark:
      lQSSFileName = "QuasarApp's Visual Studio Dark";
      break;
  }

  auto lLineEditsToReactivate{this->disableLineEditPlaceholders()};

  if (lQSSFileName == "")
  {
    qApp->setStyleSheet("");
  }
  else
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
  auto lIconFolder{Utils::isThemeDark(mSettings.appTheme) ? QString("white") : QString("black")};

  auto lEditFiltersButton{this->findChild<QPushButton*>("edit_filters")};
  lEditFiltersButton->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/pencil")));

  auto lSkeletonRefresherButton{this->findChild<QPushButton*>("skeleton_chooser_refresher")};
  lSkeletonRefresherButton->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/reload")));
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
  std::vector<QLineEdit*> lList;

  // Workaround for style glitch with QSS and QLineEdit
  auto lMeshesPathLineEdit{this->findChild<QLineEdit*>("meshes_path_input")};
  if (lMeshesPathLineEdit->isEnabled())
  {
    lMeshesPathLineEdit->setDisabled(true);
    lList.push_back(lMeshesPathLineEdit);
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
  for (auto lLineEdit : aLineEditsToReactivate)
  {
    lLineEdit->setDisabled(false);
  }
}

void MFBOPresetCreator::refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings)
{
  if (aMustUpdateSettings)
  {
    // Store old values to check if GUI modifications are needed
    auto lOldAppTheme{mSettings.appTheme};
    auto lOldFont{mSettings.font};

    auto lNewAppTheme{aSettings.appTheme};
    auto lNewFont{aSettings.font};

    mSettings = aSettings;

    // Update the settings of each tab
    for (auto lTab : this->mTabs)
    {
      lTab->updateSettings(aSettings);
    }

    if (lOldAppTheme != lNewAppTheme)
    {
      // Refresh the status bar to swap dark and white icons if needed
      this->setupMenuBar();

      // Apply the chosen QSS theme
      this->applyGlobalStyleSheet();
    }

    if (lOldFont != lNewFont)
    {
      applyFont(lNewFont.family,
                lNewFont.styleName,
                lNewFont.size,
                lNewFont.weight,
                lNewFont.italic,
                lNewFont.underline,
                lNewFont.strikeOut);
    }
  }
  else
  {
    this->setupMenuBar();

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

  HTTPDownloader* lHTTPDownloader{new HTTPDownloader(lGitHubURL, this)};
  this->connect(lHTTPDownloader, &HTTPDownloader::resultReady, this, &MFBOPresetCreator::pageFetched);
  this->connect(lHTTPDownloader, &HTTPDownloader::finished, lHTTPDownloader, &QObject::deleteLater);
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
    auto lTagName = obj["tag_name"].toString();
    auto lCurrentVersion{Utils::getSoftwareVersion()};

    if (lCurrentVersion != lTagName)
    {

#ifdef DEBUG

      lTitle = "Running a developper version";
      lMessage = tr("[DEV]\nYou are currently running the unreleased version \"%1\".\nThe last available version on GitHub is tagged \"%2\".").arg(lCurrentVersion).arg(lTagName);

#else

      this->mNewVersionAvailable = true;
      lTitle = tr("New update available");
      lMessage = tr("You are currently running the version \"%1\".\nThe new version \"%2\" is available on GitHub.").arg(lCurrentVersion).arg(lTagName);

#endif
    }
  }

  this->initializeGUI();
  this->refreshUI(mSettings, false);

  this->showWindow();

  if (aResult == "fetch_error" || this->mNewVersionAvailable)
  {
    QMessageBox::information(this, lTitle, lMessage, QMessageBox::StandardButton::Ok);
  }
}

void MFBOPresetCreator::quickRelaunch()
{
  qApp->exit(Settings::EXIT_CODE_REBOOT);
}

void MFBOPresetCreator::launchRetargetingTool()
{
  new RetargetingTool(this, this->mSettings);
}

void MFBOPresetCreator::showSettingsDialog()
{
  auto lSettings{new Settings(this, this->mSettings)};
  this->connect(lSettings, &Settings::refreshMainUI, this, &MFBOPresetCreator::refreshUI);
}

void MFBOPresetCreator::showUpdateDialog()
{
  new Update(this, this->mSettings);
}

void MFBOPresetCreator::showAboutDialog()
{
  new About(this, this->mSettings);
}
