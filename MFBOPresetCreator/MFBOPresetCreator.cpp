#include "MFBOPresetCreator.h"

MFBOPresetCreator::MFBOPresetCreator(Struct::Settings aSettings, QWidget* parent)
  : QMainWindow(parent)
  , mSettings(aSettings)
  , mLastPaths(Utils::loadLastPathsFromFile())
  , mNewVersionAvailable(false)
{
  // Construct the GUI
  ui.setupUi(this);

  // Check for new versions
  if (mSettings.checkForUpdatesAtStartup)
  {
    this->checkForUpdate();
  }
  else
  {
    this->initializeGUI();
  }
}

void MFBOPresetCreator::closeEvent(QCloseEvent* aEvent)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  QMessageBox lConfirmationBox(QMessageBox::Icon::Question, tr("Quitting"), tr("Do you want to quit the application?"), QMessageBox::StandardButton::NoButton, this);
  lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/help-circle").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

  auto lCloseButton{lConfirmationBox.addButton(tr("Quit the application"), QMessageBox::ButtonRole::YesRole)};
  lCloseButton->setCursor(Qt::PointingHandCursor);
  lCloseButton->setStyleSheet("color: hsl(4, 90%, 58%);");

  auto lStayButton{lConfirmationBox.addButton(tr("Go back to the application"), QMessageBox::ButtonRole::NoRole)};
  lStayButton->setCursor(Qt::PointingHandCursor);
  lStayButton->setStyleSheet("color: hsl(141, 53%, 53%)");

  lConfirmationBox.setDefaultButton(lStayButton);
  lConfirmationBox.exec();

  if (lConfirmationBox.clickedButton() != lCloseButton)
  {
    aEvent->ignore();
  }
  else
  {
    Utils::saveLastPathsToFile(this->mLastPaths);
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

  // Construct the menu bar
  auto lMenuBar{new QMenuBar(this)};
  lMenuBar->setCursor(Qt::PointingHandCursor);
  this->setMenuBar(lMenuBar);

  // File
  auto lFile{new QMenu(tr("File"), this)};
  lFile->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lFile);

  // Action: Relaunch the app
  auto lRelaunchApp{new QAction(this)};
  lRelaunchApp->setObjectName("action_quick_relaunch");
  lRelaunchApp->setText(tr("Quick relaunch"));
  lRelaunchApp->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
  lRelaunchApp->setIcon(QIcon(QPixmap(QString(":/%1/refresh").arg(lIconFolder))));
  lFile->addAction(lRelaunchApp);

  // Action: Exit
  auto lExitApp{new QAction(this)};
  lExitApp->setText(tr("Exit"));
  lExitApp->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
  lExitApp->setIcon(QIcon(QPixmap(QString(":/%1/cross").arg(lIconFolder))));
  lFile->addAction(lExitApp);

  // Tools
  auto lTools{new QMenu(tr("Tools"), this)};
  lTools->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lTools);

  // Action: Assisted conversion
  auto lOpenAssiConv{new QAction(this)};
  lOpenAssiConv->setText(tr("Assisted Conversion"));
  lOpenAssiConv->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
  lOpenAssiConv->setIcon(QIcon(QPixmap(QString(":/%1/pencil").arg(lIconFolder))));
  lTools->addAction(lOpenAssiConv);

  // Submenu: BodySlide Presets' Retargeting
  auto lRetaToolsSubmenu{new QAction(this)};
  lRetaToolsSubmenu->setText(tr("BodySlide Presets' Retargeting"));
  lRetaToolsSubmenu->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
  lRetaToolsSubmenu->setIcon(QIcon(QPixmap(QString(":/%1/arrow-up").arg(lIconFolder))));
  lTools->addAction(lRetaToolsSubmenu);

  // Action: Settings
  auto lOpenSettings{new QAction(this)};
  lOpenSettings->setText(tr("Settings"));
  lOpenSettings->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
  lOpenSettings->setIcon(QIcon(QPixmap(QString(":/%1/cog").arg(lIconFolder))));
  lTools->addAction(lOpenSettings);

  // Help
  const auto& lUpdateAvailableText{this->mNewVersionAvailable ? tr(" (update available)") : QString("")};

  auto lHelp{new QMenu(tr("Help") + lUpdateAvailableText, this)};
  lHelp->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lHelp);

  // Action: Check for updates
  auto lOpenUpdate{new QAction(this)};
  lOpenUpdate->setText(tr("Check for updates") + lUpdateAvailableText);
  lOpenUpdate->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
  lOpenUpdate->setIcon(QIcon(QPixmap(QString(":/%1/cloud-search").arg(lIconFolder))));
  lHelp->addAction(lOpenUpdate);

  // Action: About
  auto lOpenAbout{new QAction(this)};
  lOpenAbout->setText(tr("About"));
  lOpenAbout->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  lOpenAbout->setIcon(QIcon(QPixmap(QString(":/%1/info-circle").arg(lIconFolder))));
  lHelp->addAction(lOpenAbout);

  // Event binding
  this->connect(lRelaunchApp, &QAction::triggered, this, &MFBOPresetCreator::quickRelaunch);
  this->connect(lExitApp, &QAction::triggered, this, &MFBOPresetCreator::close);
  this->connect(lOpenAssiConv, &QAction::triggered, this, &MFBOPresetCreator::launchAssistedConversion);
  this->connect(lRetaToolsSubmenu, &QAction::triggered, this, &MFBOPresetCreator::launchPresetsRetargeting);
  this->connect(lOpenSettings, &QAction::triggered, this, &MFBOPresetCreator::launchSettingsDialog);
  this->connect(lOpenUpdate, &QAction::triggered, this, &MFBOPresetCreator::launchUpdateDialog);
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

void MFBOPresetCreator::launchAboutDialog()
{
  new About(this, this->mSettings);
}
