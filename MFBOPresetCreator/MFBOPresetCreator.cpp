#include "MFBOPresetCreator.h"

MFBOPresetCreator::MFBOPresetCreator(QWidget* parent)
  : QMainWindow(parent)
  , mSettings(Utils::loadSettingsFromFile())
{
  // Construct the GUI
  ui.setupUi(this);
  this->initializeGUI();
  this->refreshUI(mSettings, false);
  this->showWindow();
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
  lQuickRelaunch->setIcon(QIcon(":/" + lIconFolder + "/reload"));
  lFileMenu->addAction(lQuickRelaunch);

  // Submenu: Exit
  auto lExitAction{new QAction(this)};
  lExitAction->setText(tr("Exit"));
  lExitAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
  lExitAction->setIcon(QIcon(":/" + lIconFolder + "/exit"));
  lFileMenu->addAction(lExitAction);

  // Tools
  auto lToolsMenu{new QMenu(tr("Tools"), this)};
  lMenuBar->addMenu(lToolsMenu);

  // Submenu: Upgrader
  auto lRetargetingToolAction{new QAction(this)};
  lRetargetingToolAction->setText(tr("CBBE 3BBB Version Retargeting Tool"));
  lRetargetingToolAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
  lRetargetingToolAction->setIcon(QIcon(":/" + lIconFolder + "/arrow_up"));
  lToolsMenu->addAction(lRetargetingToolAction);

  // Submenu: Settings
  auto lSettingsAction{new QAction(this)};
  lSettingsAction->setText(tr("Settings"));
  lSettingsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
  lSettingsAction->setIcon(QIcon(":/" + lIconFolder + "/cog"));
  lToolsMenu->addAction(lSettingsAction);

  // Help
  auto lHelpMenu{new QMenu(tr("Help"), this)};
  lMenuBar->addMenu(lHelpMenu);

  // Submenu: About
  auto lAboutAction{new QAction(this)};
  lAboutAction->setText(tr("About"));
  lAboutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  lAboutAction->setIcon(QIcon(":/" + lIconFolder + "/information"));
  lHelpMenu->addAction(lAboutAction);

  // Event binding
  connect(lQuickRelaunch, SIGNAL(triggered()), this, SLOT(quickRelaunch()));
  connect(lExitAction, SIGNAL(triggered()), this, SLOT(close()));
  connect(lRetargetingToolAction, SIGNAL(triggered()), this, SLOT(launchRetargetingTool()));
  connect(lSettingsAction, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
  connect(lAboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
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

void MFBOPresetCreator::applyStyleSheet()
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
    default:
      break;
  }

  if (lQSSFileName == "")
  {
    qApp->setStyleSheet("");
  }
  else
  {
    QFile lQSSFile(":qss/" + lQSSFileName + ".qss");

    lQSSFile.open(QFile::ReadOnly);
    QString lStyleSheet{QLatin1String(lQSSFile.readAll())};

    qApp->setStyleSheet(lStyleSheet);
  }
}

void MFBOPresetCreator::refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings)
{
  if (aMustUpdateSettings)
  {
    // Store old values to check if GUI modifications are needed
    auto lOldAppTheme = mSettings.appTheme;
    auto lOldFontFamily = mSettings.fontFamily;
    auto lOldFontSize = mSettings.fontSize;

    auto lNewAppTheme = aSettings.appTheme;
    auto lNewFontFamily = aSettings.fontFamily;
    auto lNewFontSize = aSettings.fontSize;

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

      // Apply the chosen theme
      this->applyStyleSheet();
    }

    if (lOldFontFamily != lNewFontFamily || lOldFontSize != lNewFontSize)
    {
      // Set the font properties
      QFont lFont(aSettings.fontFamily, aSettings.fontSize, -1, false);
      this->setFont(lFont);
      this->setStyleSheet("font-family: \"" + aSettings.fontFamily + "\"; font-size: " + QString::number(aSettings.fontSize) + "px;");
    }
  }
  else
  {
    this->setupMenuBar();

    // Apply the chosen theme
    this->applyStyleSheet();

    // Set the font properties
    QFont lFont(aSettings.fontFamily, aSettings.fontSize, -1, false);
    this->setFont(lFont);
    this->setStyleSheet("font-family: \"" + aSettings.fontFamily + "\"; font-size: " + QString::number(aSettings.fontSize) + "px;");
  }
}

void MFBOPresetCreator::quickRelaunch()
{
  qApp->exit(Settings::EXIT_CODE_REBOOT);
}

void MFBOPresetCreator::launchRetargetingTool()
{
  new RetargetingTool(this);
}

void MFBOPresetCreator::showSettingsDialog()
{
  auto lSettings{new Settings(this)};

  connect(lSettings, SIGNAL(refreshMainUI(Struct::Settings, bool)), this, SLOT(refreshUI(Struct::Settings, bool)));
}

void MFBOPresetCreator::showAboutDialog()
{
  // Build the description
  auto lDescription(
    QStringLiteral(
      "<h1 style=\"text-align: center; padding: 0; margin: 0; margin-right: 20px;\">About this software</h1><br />"
      "<p style=\"font-size: 12px; padding: 0; margin: 0; margin-right: 20px;\">"
      "Mitsuriou's Follower Bodies Overhaul Preset Creator (MFBOPC) is a software "
      "created by Dylan Jacquemin (also known under the nickname <a href=\"https://www.nexusmods.com/users/37026145\">Mitsuriou</a>).<br />"
      "This software has been developed to be provided for free to any user that wants to use the software.<br />"
      "The totality of the source code is available on "
      "<a href=\"https://github.com/Mitsuriou/MFBO-Preset-Creator\">GitHub.com</a>."
      "<br /><br />"
      "Ressources used to make this software:<br />"
      "&bull; <a href='https://www.qt.io'>Qt</a> (free version) is used for the Graphical User Iterface (GUI).<br />"
      "&bull; <a href='http://buildnumber.sourceforge.net/'>BuildNumber</a> is used for version number auto-incrementat.<br />"
      "&bull; All the icons were taken from <a href=\"https://materialdesignicons.com\">MaterialDesignIcons.com</a>.<br />"
      "&bull; Some GUI themes were taken from <a href=\"https://github.com/6788-00\">6788-00's GitHub repository</a>.<br />"
      "&bull; Some GUI themes were taken from <a href=\"https://github.com/Alexhuszagh/BreezeStyleSheets\">Alexhuszagh/BreezeStyleSheets GitHub page</a>.<br />"
      "&bull; Some GUI themes were taken from <a href=\"https://github.com/QuasarApp/QStyleSheet\">QuasarApp/QStyleSheet GitHub page</a>.<br />"
      "<br />"
      "Ressources bundled in this software:<br />"
      "&bull; The BodySlide (OSP and XML) files that are generated with MFBOPC were taken from the "
      "<a href=\"https://www.nexusmods.com/skyrimspecialedition/mods/30174\">CBBE 3BBB</a> mod on NexusMod "
      "and modified by Dylan Jacquemin.<br />"
      "&bull; The \"female_skeleton.nif\" file has been taken from the "
      "<a href=\"https://www.nexusmods.com/skyrimspecialedition/mods/1988\">XP32 Maximum Skeleton Special Extended - XPMSSE</a> "
      "mod on NexusMod. The file has not been modified."
      "</p>"));

  // Construct the message box
  QMessageBox lDialog(QMessageBox::Icon::NoIcon, tr("About"), lDescription, QMessageBox::StandardButton::Close);
  lDialog.setTextFormat(Qt::RichText);
  lDialog.adjustSize();

  // Display the message box
  lDialog.exec();
}
