#include "Settings.h"

Settings::Settings(QWidget* parent)
  : QDialog(parent)
  , mSettings(Utils::loadSettingsFromFile())
  , mMustRebootMainApp{false}
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  this->refreshUI();

  // Show the window when it's completely built
  this->show();
}

void Settings::closeEvent(QCloseEvent* aEvent)
{
  QObject* lEventSourceObject{sender()};
  QPushButton* lEventButton{qobject_cast<QPushButton*>(lEventSourceObject)};

  auto lSaveButton{this->findChild<QPushButton*>("save_close")};

  if (lEventButton == lSaveButton)
  {
    aEvent->accept();
    return;
  }

  auto lResult{QMessageBox::question(this, tr("Closing"), tr("Are you sure you want to close the Settings window without saving?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)};

  if (lResult != QMessageBox::Yes)
  {
    aEvent->ignore();
  }
  else
  {
    aEvent->accept();
  }
}

void Settings::reject()
{
  this->close();
}

void Settings::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setMinimumWidth(650);
  this->setWindowTitle(tr("Settings"));
}

void Settings::initializeGUI()
{
  // Main window containers
  auto lMainVertical{new QGridLayout()};
  this->setupInterface(*lMainVertical);

  auto lButtonsContainer{new QGridLayout()};
  this->setupButtons(*lButtonsContainer);

  auto lContainer{new QVBoxLayout(this)};
  lContainer->addLayout(lMainVertical);
  lContainer->addLayout(lButtonsContainer);

  // Load the settings in the interface
  this->loadSettings();
}

void Settings::setupInterface(QGridLayout& aLayout)
{
  // LANGUAGE
  auto lLanguageLabel{new QLabel(tr("Language:"), this)};
  aLayout.addWidget(lLanguageLabel, 0, 0);

  QStringList lSupportedLanguages;
  lSupportedLanguages.append(tr("English"));
  lSupportedLanguages.append(tr("Français"));

  auto lLanguageSelector{new QComboBox(this)};
  lLanguageSelector->addItems(lSupportedLanguages);
  lLanguageSelector->setObjectName(QString("language"));
  aLayout.addWidget(lLanguageSelector, 0, 1);

  // FONT FAMILY
  auto lFontFamilyLabel{new QLabel(tr("Font family:"), this)};
  aLayout.addWidget(lFontFamilyLabel, 1, 0);

  auto lFontFamilySelector{new QComboBox(this)};
  QFontDatabase lFontDB;
  lFontFamilySelector->addItems(lFontDB.families(QFontDatabase::WritingSystem::Any));
  lFontFamilySelector->setObjectName(QString("font_family"));
  aLayout.addWidget(lFontFamilySelector, 1, 1);

  // FONT SIZE
  auto lFontSizeLabel{new QLabel(tr("Font size:"), this)};
  aLayout.addWidget(lFontSizeLabel, 2, 0);

  auto lFontSizeInput{new QLineEdit("", this)};
  lFontSizeInput->setObjectName(QString("font_size"));
  lFontSizeInput->setValidator(new QIntValidator(1, 99, this));
  aLayout.addWidget(lFontSizeInput, 2, 1);

  // GUI THEME
  auto lGUIThemeLabel{new QLabel(tr("Application Theme:"), this)};
  aLayout.addWidget(lGUIThemeLabel, 3, 0);

  auto lGUIThemeSelector{new QComboBox(this)};
  lGUIThemeSelector->addItems(Utils::getAppThemes());
  lGUIThemeSelector->setObjectName(QString("app_theme"));
  aLayout.addWidget(lGUIThemeSelector, 3, 1);

  // MAIN WINDOW OPENING MODE
  auto lWindowOpeningModeLabel{new QLabel(tr("Window opening mode:"), this)};
  aLayout.addWidget(lWindowOpeningModeLabel, 4, 0);

  QStringList lSupportedWindowOpeningMode;
  lSupportedWindowOpeningMode.append(tr("English"));
  lSupportedWindowOpeningMode.append(tr("Français"));

  auto lWindowOpeningModeSelector{new QComboBox(this)};
  lWindowOpeningModeSelector->addItems(Utils::getWindowOpeningModes());
  lWindowOpeningModeSelector->setObjectName(QString("window_opening_mode"));
  aLayout.addWidget(lWindowOpeningModeSelector, 4, 1);

  // WINDOW WIDTH
  auto lWinWidthLabel{new QLabel(tr("Default main window width:"), this)};
  aLayout.addWidget(lWinWidthLabel, 5, 0);

  auto lWinWidthInput{new QLineEdit("", this)};
  lWinWidthInput->setObjectName(QString("window_width"));
  lWinWidthInput->setValidator(new QIntValidator(0, 9999, this));
  aLayout.addWidget(lWinWidthInput, 5, 1);

  // WINDOW HEIGHT
  auto lWinHeightLabel{new QLabel(tr("Default main window height:"), this)};
  aLayout.addWidget(lWinHeightLabel, 6, 0);

  auto lWinHeightInput{new QLineEdit("", this)};
  lWinHeightInput->setObjectName(QString("window_height"));
  lWinHeightInput->setValidator(new QIntValidator(0, 9999, this));
  aLayout.addWidget(lWinHeightInput, 6, 1);

  // DEFAULT SELECTED CBBE 3BBB VERSION (MAIN)
  auto ldefaultCbbe3BBBVersionLabel{new QLabel(tr("Default selected CBBE 3BBB version (main window):"), this)};
  aLayout.addWidget(ldefaultCbbe3BBBVersionLabel, 7, 0);

  auto ldefaultCbbe3BBBVersionSelector{new QComboBox(this)};
  ldefaultCbbe3BBBVersionSelector->addItems(Utils::getCBBE3BBBVersions());
  ldefaultCbbe3BBBVersionSelector->setObjectName(QString("default_cbbe_3bbb_version"));
  aLayout.addWidget(ldefaultCbbe3BBBVersionSelector, 7, 1);

  // DEFAULT SELECTED CBBE 3BBB VERSION (RETARGETING TOOL)
  auto lupgradeCbbe3BBBVersionLabel{new QLabel(tr("Default selected CBBE 3BBB version (Retargeting tool):"), this)};
  aLayout.addWidget(lupgradeCbbe3BBBVersionLabel, 8, 0);

  auto lupgradeCbbe3BBBVersionSelector{new QComboBox(this)};
  lupgradeCbbe3BBBVersionSelector->addItems(Utils::getCBBE3BBBVersions());
  lupgradeCbbe3BBBVersionSelector->setObjectName(QString("upgrade_cbbe_3bbb_version"));
  aLayout.addWidget(lupgradeCbbe3BBBVersionSelector, 8, 1);
}

void Settings::setupButtons(QGridLayout& aLayout)
{
  // Create the buttons
  auto lRestoreDefaultButton{new QPushButton(tr("Restore default without saving"), this)};
  lRestoreDefaultButton->setAutoDefault(false);
  lRestoreDefaultButton->setDefault(false);
  aLayout.addWidget(lRestoreDefaultButton, 0, 0);

  auto lSaveButton{new QPushButton(tr("Save and close"), this)};
  lSaveButton->setObjectName("save_close");
  lSaveButton->setAutoDefault(false);
  lSaveButton->setDefault(false);
  aLayout.addWidget(lSaveButton, 0, 1);

  auto lCloseButton{new QPushButton(tr("Close without saving"), this)};
  lCloseButton->setAutoDefault(false);
  lCloseButton->setDefault(false);
  aLayout.addWidget(lCloseButton, 0, 2);

  // Event binding
  connect(lRestoreDefaultButton, SIGNAL(clicked()), this, SLOT(restoreDefaultSettings()));
  connect(lSaveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
  connect(lCloseButton, SIGNAL(clicked()), this, SLOT(close()));
}

void Settings::loadSettings()
{
  auto lLang{this->findChild<QComboBox*>("language")};
  lLang->setCurrentIndex(static_cast<int>(mSettings.language));

  auto lFontFamily{this->findChild<QComboBox*>("font_family")};
  QFontDatabase lFontDB;
  auto lFamilyFontIndex{lFontDB.families(QFontDatabase::WritingSystem::Any).indexOf(QRegularExpression(mSettings.fontFamily))};
  lFontFamily->setCurrentIndex(lFamilyFontIndex);

  auto lFontSize{this->findChild<QLineEdit*>("font_size")};
  lFontSize->setText(QString::number(mSettings.fontSize));

  auto lAppTheme{this->findChild<QComboBox*>("app_theme")};
  lAppTheme->setCurrentIndex(static_cast<int>(mSettings.appTheme));

  auto lWindowWidth{this->findChild<QLineEdit*>("window_width")};
  lWindowWidth->setText(QString::number(mSettings.mainWindowWidth));

  auto lWindowHeight{this->findChild<QLineEdit*>("window_height")};
  lWindowHeight->setText(QString::number(mSettings.mainWindowHeight));

  auto lWindowOpeningMode{this->findChild<QComboBox*>("window_opening_mode")};
  lWindowOpeningMode->setCurrentIndex(static_cast<int>(mSettings.mainWindowOpeningMode));

  auto lDefaultCBBE3BBBVersion{this->findChild<QComboBox*>("default_cbbe_3bbb_version")};
  lDefaultCBBE3BBBVersion->setCurrentIndex(static_cast<int>(mSettings.defaultMainWindowCBBE3BBBVersion));

  auto lDefaultUpgradeCBBE3BBBVersion{this->findChild<QComboBox*>("upgrade_cbbe_3bbb_version")};
  lDefaultUpgradeCBBE3BBBVersion->setCurrentIndex(static_cast<int>(mSettings.defaultRetargetingToolCBBE3BBBVersion));
}

void Settings::refreshUI()
{
  // Set the font properties
  QFont lFont(mSettings.fontFamily, mSettings.fontSize, -1, false);
  this->setFont(lFont);
  this->setStyleSheet("font-family: \"" + mSettings.fontFamily + "\"; font-size: " + QString::number(mSettings.fontSize) + "px;");

  emit refreshMainUI(mSettings);

  if (mMustRebootMainApp)
  {
    auto lResult{QMessageBox::question(this, tr("Application settings changed"), tr("All settings have been saved. You changed a setting that needs a restart of the application to be applied. Would you like to restart the application now?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)};

    if (lResult == QMessageBox::Yes)
    {
      // Reboot the application in case the language is changed
      qApp->exit(Settings::EXIT_CODE_REBOOT);
    }
  }
}

void Settings::restoreDefaultSettings()
{
  // Store the current settings
  auto lStoredSettings = mSettings;
  // Create a default settings object and save it
  Struct::Settings lSettings;
  mSettings = lSettings;
  this->loadSettings();
  // Reload the settings that were previously set
  mSettings = lStoredSettings;
}

void Settings::saveSettings()
{
  auto lLang{this->findChild<QComboBox*>("language")->currentIndex()};
  auto lFontFamily{this->findChild<QComboBox*>("font_family")->currentText().trimmed()};
  auto lFontSize{this->findChild<QLineEdit*>("font_size")->text().trimmed()};
  auto lAppTheme{this->findChild<QComboBox*>("app_theme")->currentIndex()};
  auto lWindowWidth{this->findChild<QLineEdit*>("window_width")->text().trimmed()};
  auto lWindowHeight{this->findChild<QLineEdit*>("window_height")->text().trimmed()};
  auto lDefaultCBBE3BBBVersion{this->findChild<QComboBox*>("default_cbbe_3bbb_version")->currentIndex()};
  auto lDefaultUpgradeCBBE3BBBVersion{this->findChild<QComboBox*>("upgrade_cbbe_3bbb_version")->currentIndex()};
  auto lWindowOpeningMode{this->findChild<QComboBox*>("window_opening_mode")->currentIndex()};

  Struct::Settings lSettings;

  // Language
  switch (lLang)
  {
    case static_cast<int>(ApplicationLanguage::English):
      lSettings.language = ApplicationLanguage::English;
      break;
    case static_cast<int>(ApplicationLanguage::French):
      lSettings.language = ApplicationLanguage::French;
      break;
    default:
      lSettings.language = ApplicationLanguage::English;
      break;
  }

  // Font family
  if (lFontFamily.size() > 0)
  {
    lSettings.fontFamily = lFontFamily;
  }

  // Font size
  if (lFontSize.size() > 0)
  {
    lSettings.fontSize = lFontSize.toInt();
  }

  // Application theme
  switch (lAppTheme)
  {
    case static_cast<int>(GUITheme::WindowsVista):
      lSettings.appTheme = GUITheme::WindowsVista;
      break;
    case static_cast<int>(GUITheme::PaperLight):
      lSettings.appTheme = GUITheme::PaperLight;
      break;
    case static_cast<int>(GUITheme::PaperDark):
      lSettings.appTheme = GUITheme::PaperDark;
      break;
    case static_cast<int>(GUITheme::PaperWhiteMono):
      lSettings.appTheme = GUITheme::PaperWhiteMono;
      break;
    case static_cast<int>(GUITheme::PaperBlackMono):
      lSettings.appTheme = GUITheme::PaperBlackMono;
      break;
    case static_cast<int>(GUITheme::AlexhuszaghBreezeLight):
      lSettings.appTheme = GUITheme::AlexhuszaghBreezeLight;
      break;
    case static_cast<int>(GUITheme::AlexhuszaghBreezeDark):
      lSettings.appTheme = GUITheme::AlexhuszaghBreezeDark;
      break;
    case static_cast<int>(GUITheme::QuasarAppDarkStyle):
      lSettings.appTheme = GUITheme::QuasarAppDarkStyle;
      break;
    case static_cast<int>(GUITheme::QuasarAppMaterialStyle):
      lSettings.appTheme = GUITheme::QuasarAppMaterialStyle;
      break;
    case static_cast<int>(GUITheme::QuasarAppVisualStudioDark):
      lSettings.appTheme = GUITheme::QuasarAppVisualStudioDark;
      break;
    default:
      lSettings.appTheme = GUITheme::WindowsVista;
      break;
  }

  // Window width
  if (lWindowWidth.size() > 0)
  {
    lSettings.mainWindowWidth = lWindowWidth.toInt();
  }

  // Window height
  if (lWindowHeight.size() > 0)
  {
    lSettings.mainWindowHeight = lWindowHeight.toInt();
  }

  // Main window opening mode
  switch (lWindowOpeningMode)
  {
    case static_cast<int>(WindowOpeningMode::Minimized):
      lSettings.mainWindowOpeningMode = WindowOpeningMode::Minimized;
      break;
    case static_cast<int>(WindowOpeningMode::Windowed):
      lSettings.mainWindowOpeningMode = WindowOpeningMode::Windowed;
      break;
    case static_cast<int>(WindowOpeningMode::Maximized):
      lSettings.mainWindowOpeningMode = WindowOpeningMode::Maximized;
      break;
    default:
      lSettings.mainWindowOpeningMode = WindowOpeningMode::Windowed;
      break;
  }

  // Default selected CBBE 3BBB version (main)
  switch (lDefaultCBBE3BBBVersion)
  {
    case static_cast<int>(CBBE3BBBVersion::Version1_40):
      lSettings.defaultMainWindowCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_50):
      lSettings.defaultMainWindowCBBE3BBBVersion = CBBE3BBBVersion::Version1_50;
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
      lSettings.defaultMainWindowCBBE3BBBVersion = CBBE3BBBVersion::Version1_51_and_1_52;
      break;
    default:
      lSettings.defaultMainWindowCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
      break;
  }

  // Default selected CBBE 3BBB version (Retargeting tool)
  switch (lDefaultUpgradeCBBE3BBBVersion)
  {
    case static_cast<int>(CBBE3BBBVersion::Version1_40):
      lSettings.defaultRetargetingToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_50):
      lSettings.defaultRetargetingToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_50;
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
      lSettings.defaultRetargetingToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_51_and_1_52;
      break;
    default:
      lSettings.defaultRetargetingToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
      break;
  }

  mMustRebootMainApp = (mSettings.language != lSettings.language) || (mSettings.appTheme != lSettings.appTheme);

  Utils::saveSettingsToFile(lSettings);
  mSettings = lSettings;

  this->refreshUI();
  this->close();
}
