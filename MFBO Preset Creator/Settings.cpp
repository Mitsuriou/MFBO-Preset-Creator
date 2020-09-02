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
  QObject* lEventSourceObject = sender();
  QPushButton* lEventButton = qobject_cast<QPushButton*>(lEventSourceObject);

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
  auto lLanguageLabel{new QLabel(tr("Language:"))};
  aLayout.addWidget(lLanguageLabel, 0, 0);

  QStringList lSupportedLanguages;
  lSupportedLanguages.append(tr("English"));
  lSupportedLanguages.append(tr("Français"));

  auto lLanguageSelector{new QComboBox()};
  lLanguageSelector->addItems(lSupportedLanguages);
  lLanguageSelector->setObjectName(QString("language"));
  aLayout.addWidget(lLanguageSelector, 0, 1);

  // FONT FAMILY
  auto lFontFamilyLabel{new QLabel(tr("Font family:"))};
  aLayout.addWidget(lFontFamilyLabel, 1, 0);

  auto lFontFamilyInput{new QLineEdit("")};
  lFontFamilyInput->setObjectName(QString("font_family"));
  aLayout.addWidget(lFontFamilyInput, 1, 1);

  // FONT SIZE
  auto lFontSizeLabel{new QLabel(tr("Font size:"))};
  aLayout.addWidget(lFontSizeLabel, 2, 0);

  auto lFontSizeInput{new QLineEdit("")};
  lFontSizeInput->setObjectName(QString("font_size"));
  lFontSizeInput->setValidator(new QIntValidator(1, 99, this));
  aLayout.addWidget(lFontSizeInput, 2, 1);

  // GUI THEME
  auto lGUIThemeLabel{new QLabel(tr("Application Theme:"))};
  aLayout.addWidget(lGUIThemeLabel, 3, 0);

  QStringList lThemes;
  lThemes.append(QString(tr("Windows Vista (Default)")));

  auto lGUIThemeSelector{new QComboBox()};
  lGUIThemeSelector->addItems(lThemes);
  lGUIThemeSelector->setObjectName(QString("app_theme"));
  aLayout.addWidget(lGUIThemeSelector, 3, 1);

  // WINDOW WIDTH
  auto lWinWidthLabel{new QLabel(tr("Default main window width:"))};
  aLayout.addWidget(lWinWidthLabel, 4, 0);

  auto lWinWidthInput{new QLineEdit("")};
  lWinWidthInput->setObjectName(QString("window_width"));
  lWinWidthInput->setValidator(new QIntValidator(0, 9999, this));
  aLayout.addWidget(lWinWidthInput, 4, 1);

  // WINDOW HEIGHT
  auto lWinHeightLabel{new QLabel(tr("Default main window height:"))};
  aLayout.addWidget(lWinHeightLabel, 5, 0);

  auto lWinHeightInput{new QLineEdit("")};
  lWinHeightInput->setObjectName(QString("window_height"));
  lWinHeightInput->setValidator(new QIntValidator(0, 9999, this));
  aLayout.addWidget(lWinHeightInput, 5, 1);

  // DEFAULT SELECTED CBBE 3BBB VERSION (MAIN)
  auto ldefaultCbbe3BBBVersionLabel{new QLabel(tr("Default selected CBBE 3BBB version (main window):"))};
  aLayout.addWidget(ldefaultCbbe3BBBVersionLabel, 6, 0);

  auto ldefaultCbbe3BBBVersionSelector{new QComboBox()};
  ldefaultCbbe3BBBVersionSelector->addItems(Utils::getCBBE3BBBVersions());
  ldefaultCbbe3BBBVersionSelector->setObjectName(QString("default_cbbe_3bbb_version"));
  aLayout.addWidget(ldefaultCbbe3BBBVersionSelector, 6, 1);

  // DEFAULT SELECTED CBBE 3BBB VERSION (UP/DOWNGRADER TOOL)
  auto lupgradeCbbe3BBBVersionLabel{new QLabel(tr("Default selected CBBE 3BBB version (Up/Downgrader tool):"))};
  aLayout.addWidget(lupgradeCbbe3BBBVersionLabel, 7, 0);

  auto lupgradeCbbe3BBBVersionSelector{new QComboBox()};
  lupgradeCbbe3BBBVersionSelector->addItems(Utils::getCBBE3BBBVersions());
  lupgradeCbbe3BBBVersionSelector->setObjectName(QString("upgrade_cbbe_3bbb_version"));
  aLayout.addWidget(lupgradeCbbe3BBBVersionSelector, 7, 1);
}

void Settings::setupButtons(QGridLayout& aLayout)
{
  // Create the buttons
  auto lRestoreDefaultButton{new QPushButton(tr("Restore default without saving"))};
  aLayout.addWidget(lRestoreDefaultButton, 0, 0);

  auto lSaveButton{new QPushButton(tr("Save and close"))};
  lSaveButton->setObjectName("save_close");
  aLayout.addWidget(lSaveButton, 0, 1);

  auto lCloseButton{new QPushButton(tr("Close without saving"))};
  aLayout.addWidget(lCloseButton, 0, 2);

  // Event binding
  connect(lRestoreDefaultButton, SIGNAL(clicked()), this, SLOT(restoreDefaultSettings()));
  connect(lSaveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
  connect(lCloseButton, SIGNAL(clicked()), this, SLOT(close()));
}

void Settings::loadSettings()
{
  auto lLang = this->findChild<QComboBox*>("language");
  lLang->setCurrentIndex(static_cast<int>(mSettings.language));

  auto lFontFamily = this->findChild<QLineEdit*>("font_family");
  lFontFamily->setText(mSettings.fontFamily);

  auto lFontSize = this->findChild<QLineEdit*>("font_size");
  lFontSize->setText(QString::number(mSettings.fontSize));

  auto lAppTheme = this->findChild<QComboBox*>("app_theme");
  lAppTheme->setCurrentIndex(static_cast<int>(mSettings.appTheme));

  auto lWindowWidth = this->findChild<QLineEdit*>("window_width");
  lWindowWidth->setText(QString::number(mSettings.mainWindowWidth));

  auto lWindowHeight = this->findChild<QLineEdit*>("window_height");
  lWindowHeight->setText(QString::number(mSettings.mainWindowHeight));

  auto lDefaultCBBE3BBBVersion = this->findChild<QComboBox*>("default_cbbe_3bbb_version");
  lDefaultCBBE3BBBVersion->setCurrentIndex(static_cast<int>(mSettings.defaultMainWindowCBBE3BBBVersion));

  auto lDefaultUpgradeCBBE3BBBVersion = this->findChild<QComboBox*>("upgrade_cbbe_3bbb_version");
  lDefaultUpgradeCBBE3BBBVersion->setCurrentIndex(static_cast<int>(mSettings.defaultUpgradeToolCBBE3BBBVersion));
}

void Settings::refreshUI()
{
  // Set the font properties
  QFont lFont(mSettings.fontFamily, mSettings.fontSize, -1, false);
  this->setFont(lFont);
  this->setStyleSheet("font-family: \"" + mSettings.fontFamily + "\"; font-size: " + QString::number(mSettings.fontSize) + "px;");

  if (mMustRebootMainApp)
  {
    auto lResult{QMessageBox::question(this, tr("Application language changed"), tr("Settings saved. You changed the language of the application. You need to restart the latter to apply the chosen language. Would you like to restart the application now?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)};

    if (lResult == QMessageBox::Yes)
    {
      // Reboot the application in case the language is changed
      qApp->exit(Settings::EXIT_CODE_REBOOT);
    }
  }
  emit refreshMainUI(mSettings);
}

void Settings::restoreDefaultSettings()
{
  // Create a default settings object and save it
  Struct::Settings lSettings;
  mSettings = lSettings;
  this->loadSettings();
}

void Settings::saveSettings()
{
  auto lLang = this->findChild<QComboBox*>("language")->currentIndex();
  auto lFontFamily = this->findChild<QLineEdit*>("font_family")->text().trimmed();
  auto lFontSize = this->findChild<QLineEdit*>("font_size")->text().trimmed();
  auto lAppTheme = this->findChild<QComboBox*>("app_theme")->currentIndex();
  auto lWindowWidth = this->findChild<QLineEdit*>("window_width")->text().trimmed();
  auto lWindowHeight = this->findChild<QLineEdit*>("window_height")->text().trimmed();
  auto lDefaultCBBE3BBBVersion = this->findChild<QComboBox*>("default_cbbe_3bbb_version")->currentIndex();
  auto lDefaultUpgradeCBBE3BBBVersion = this->findChild<QComboBox*>("upgrade_cbbe_3bbb_version")->currentIndex();

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

  // Default selected CBBE 3BBB version (up/downgrader tool)
  switch (lDefaultUpgradeCBBE3BBBVersion)
  {
    case static_cast<int>(CBBE3BBBVersion::Version1_40):
      lSettings.defaultUpgradeToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_50):
      lSettings.defaultUpgradeToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_50;
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
      lSettings.defaultUpgradeToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_51_and_1_52;
      break;
    default:
      lSettings.defaultUpgradeToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
      break;
  }

  mMustRebootMainApp = mSettings.language != lSettings.language;

  Utils::saveSettingsToFile(lSettings);
  mSettings = lSettings;

  this->refreshUI();
  this->close();
}
