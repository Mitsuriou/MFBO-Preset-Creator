#include "MFBOPresetCreator.h"

MFBOPresetCreator::MFBOPresetCreator(Struct::Settings aSettings, QWidget* parent)
  : QMainWindow(parent)
  , mSettings(aSettings)
  , mNewVersionAvailable(false)
{
  // Construct the GUI
  ui.setupUi(this);

  // Check for new versions
  this->checkForUpdate();
}

void MFBOPresetCreator::closeEvent(QCloseEvent* aEvent)
{
  QMessageBox lConfirmationBox(QMessageBox::Icon::Question, tr("Quitting"), tr("Do you want to quit the application?"), QMessageBox::StandardButton::NoButton, this);

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
    aEvent->accept();
  }
}

void MFBOPresetCreator::setupMenuBar()
{
  // Keep a reference to the user theme
  const auto& lIconFolder{Utils::getIconFolder(mSettings.appTheme)};

  // Construct the menu bar
  auto lMenuBar{new QMenuBar(this)};
  lMenuBar->setCursor(Qt::PointingHandCursor);
  this->setMenuBar(lMenuBar);

  // File
  auto lFileMenu{new QMenu(tr("File"), this)};
  lFileMenu->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lFileMenu);

  // Action: Relaunch the app
  auto lQuickRelaunch{new QAction(this)};
  lQuickRelaunch->setObjectName("action_quick_relaunch");
  lQuickRelaunch->setText(tr("Quick relaunch"));
  lQuickRelaunch->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
  lQuickRelaunch->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/reload")));
  lFileMenu->addAction(lQuickRelaunch);

  // Action: Exit
  auto lExitAction{new QAction(this)};
  lExitAction->setText(tr("Exit"));
  lExitAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
  lExitAction->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/exit")));
  lFileMenu->addAction(lExitAction);

  // Tools
  auto lToolsMenu{new QMenu(tr("Tools"), this)};
  lToolsMenu->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lToolsMenu);

  // Action: Assisted conversion
  auto lAssistedConversionAction{new QAction(this)};
  lAssistedConversionAction->setText(tr("Assisted Conversion"));
  lAssistedConversionAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
  lAssistedConversionAction->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/pencil")));
  lToolsMenu->addAction(lAssistedConversionAction);

  // Submenu: Upgrader
  auto lRetargetingToolsMenu{new QMenu(tr("BodySlide Presets' Version Retargeting"), this)};
  lRetargetingToolsMenu->setCursor(Qt::PointingHandCursor);
  lRetargetingToolsMenu->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/arrow_up")));
  lToolsMenu->addMenu(lRetargetingToolsMenu);

  auto lRTCBBE3BBB3BAAction{new QAction(this)};
  lRTCBBE3BBB3BAAction->setText("[TO BE REWORKED] CBBE 3BBB 3BA");
  lRTCBBE3BBB3BAAction->setDisabled(true); // TODO: repair the retargeting tool
  lRetargetingToolsMenu->addAction(lRTCBBE3BBB3BAAction);

  // Action: Settings
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
  lHelpMenu->setCursor(Qt::PointingHandCursor);
  lMenuBar->addMenu(lHelpMenu);

  // Action: Check for updates
  auto lCheckUpdateAction{new QAction(this)};
  lCheckUpdateAction->setText(tr("Check for updates") + lUpdateAvailableText);
  lCheckUpdateAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
  lCheckUpdateAction->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/download")));
  lHelpMenu->addAction(lCheckUpdateAction);

  // Action: About
  auto lAboutAction{new QAction(this)};
  lAboutAction->setText(tr("About"));
  lAboutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  lAboutAction->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/information")));
  lHelpMenu->addAction(lAboutAction);

  // Event binding
  this->connect(lQuickRelaunch, &QAction::triggered, this, &MFBOPresetCreator::quickRelaunch);
  this->connect(lExitAction, &QAction::triggered, this, &MFBOPresetCreator::close);
  this->connect(lAssistedConversionAction, &QAction::triggered, this, &MFBOPresetCreator::launchAssistedConversion);
  this->connect(lRTCBBE3BBB3BAAction, &QAction::triggered, this, &MFBOPresetCreator::launchRetargetingToolCBBE3BBB3BA);
  this->connect(lSettingsAction, &QAction::triggered, this, &MFBOPresetCreator::launchSettingsDialog);
  this->connect(lCheckUpdateAction, &QAction::triggered, this, &MFBOPresetCreator::launchUpdateDialog);
  this->connect(lAboutAction, &QAction::triggered, this, &MFBOPresetCreator::launchAboutDialog);
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
    // resize the window
    this->resize(mSettings.mainWindowWidth - 2, mSettings.mainWindowHeight - 62);
  }

  // Select the main window opening mode
  if (mSettings.mainWindowOpeningMode == WindowOpeningMode::Minimized)
  {
    this->showMinimized();

    // Make the icon in the taskbar blink
    FLASHWINFO* finfo = new FLASHWINFO();
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
  const auto& lIconFolder{Utils::getIconFolder(mSettings.appTheme)};

  auto lEditFiltersButton{this->findChild<QPushButton*>("edit_filters")};
  lEditFiltersButton->setIcon(QIcon(QPixmap(":/" + lIconFolder + "/filter")));

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
    mSettings = aSettings;

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
    auto lTagName = obj["tag_name"].toString();
    auto lCurrentVersion{Utils::getApplicationVersion()};

    if (lCurrentVersion != lTagName)
    {

#ifdef DEBUG

      lTitle = "Running a developper version";
      lMessage = tr("[DEV]\nYou are currently running the version \"%1\".\nThe last available version on GitHub is tagged \"%2\".").arg(lCurrentVersion).arg(lTagName);

#else

      this->mNewVersionAvailable = true;
      lTitle = tr("New update available");
      lMessage = tr("You are currently running the version \"%1\".\nThe new version \"%2\" is available on GitHub.").arg(lCurrentVersion).arg(lTagName);

#endif
    }
  }

  this->setupMenuBar();

  auto lMainContainer{new PresetCreator(this, mSettings)};
  lMainContainer->setObjectName("main_container");
  this->setCentralWidget(lMainContainer);

  this->refreshUI(mSettings, false);

  this->showWindow();

  if (aResult == "fetch_error" || this->mNewVersionAvailable)
  {
    QMessageBox lConfirmationBox(QMessageBox::Icon::Information, lTitle, lMessage, QMessageBox::StandardButton::NoButton, this);

    auto lOKButton{lConfirmationBox.addButton(tr("OK"), QMessageBox::ButtonRole::AcceptRole)};
    lOKButton->setCursor(Qt::PointingHandCursor);
    lConfirmationBox.setDefaultButton(lOKButton);
    lConfirmationBox.exec();
  }
}

void MFBOPresetCreator::quickRelaunch()
{
  qApp->exit(Settings::EXIT_CODE_REBOOT);
}

void MFBOPresetCreator::launchAssistedConversion()
{
  auto lDialog{new AssistedConversion(this, this->mSettings)};
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

void MFBOPresetCreator::launchRetargetingToolCBBE3BBB3BA()
{
  new RetargetingTool(this, this->mSettings);
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
