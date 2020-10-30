#include "Settings.h"

Settings::Settings(QWidget* parent, Struct::Settings aSettings)
  : QDialog(parent)
  , mSettings(aSettings)
  , mMustRebootMainApp{false}
  , mNewFont{qApp->font()}
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->show();
}

void Settings::closeEvent(QCloseEvent* aEvent)
{
  auto lEventSourceObject{sender()};
  auto lEventButton{qobject_cast<QPushButton*>(lEventSourceObject)};
  auto lSaveButton{this->findChild<QPushButton*>("save_close")};

  if (lEventButton == lSaveButton)
  {
    aEvent->accept();
    return;
  }

  if (this->getSettingsFromGUI() != mSettings)
  {
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
  this->setWindowTitle(tr("Settings"));
}

void Settings::initializeGUI()
{
  // Main layout
  auto lMainContainer{new QVBoxLayout(this)};

  // Tabs
  auto lTabsContainer{new QVBoxLayout()};
  lMainContainer->addLayout(lTabsContainer);
  this->setupTabs(lTabsContainer);

  // Buttons
  auto lButtonsContainer{new QHBoxLayout()};
  lMainContainer->addLayout(lButtonsContainer);
  this->setupButtons(lButtonsContainer);

  // Load the settings in the interface
  this->loadSettings(mSettings);
}

void Settings::setupTabs(QVBoxLayout* aLayout)
{
  auto lTabs{new QTabWidget(this)};
  aLayout->addWidget(lTabs);

  this->setupDisplayTab(lTabs);
  this->setupPresetCreatorTab(lTabs);
  this->setupRetargetingToolTab(lTabs);
}

void Settings::setupDisplayTab(QTabWidget* aTabs)
{
  auto lDisplayTab{new QWidget(aTabs)};
  aTabs->addTab(lDisplayTab, tr("Display"));

  auto lDisplayLayout{new QVBoxLayout(lDisplayTab)};
  lDisplayLayout->setAlignment(Qt::AlignTop);
  lDisplayTab->setLayout(lDisplayLayout);

  // LANGUAGE
  auto lLangLabelText = "* " + tr("Language:");
  auto lLanguageLabel{new QLabel(lLangLabelText, this)};
  lDisplayLayout->addWidget(lLanguageLabel);

  auto lLanguageSelector{new QComboBox(this)};
  lLanguageSelector->addItems(DataLists::getLanguages());
  lLanguageSelector->setObjectName(QString("language"));
  lDisplayLayout->addWidget(lLanguageSelector);

  // FONT FAMILY
  auto lFontLabel{new QLabel(tr("Font:"), this)};
  lDisplayLayout->addWidget(lFontLabel);

  auto lFontChooser{new QPushButton(tr("Choose a font"), this)};
  lFontChooser->setObjectName("font_chooser");
  lDisplayLayout->addWidget(lFontChooser);

  // GUI THEME
  auto lGUIThemeLabel{new QLabel(tr("Application Theme:"), this)};
  lDisplayLayout->addWidget(lGUIThemeLabel);

  auto lGUIThemeSelector{new QComboBox(this)};
  lGUIThemeSelector->addItems(DataLists::getAppThemes());
  lGUIThemeSelector->setObjectName(QString("app_theme"));
  lDisplayLayout->addWidget(lGUIThemeSelector);

  // MAIN WINDOW OPENING MODE
  auto lWindowOpeningModeLabel{new QLabel(tr("Window opening mode:"), this)};
  lDisplayLayout->addWidget(lWindowOpeningModeLabel);

  QStringList lSupportedWindowOpeningMode;
  lSupportedWindowOpeningMode.append(tr("English"));
  lSupportedWindowOpeningMode.append(tr("Français"));

  auto lWindowOpeningModeSelector{new QComboBox(this)};
  lWindowOpeningModeSelector->addItems(DataLists::getWindowOpeningModes());
  lWindowOpeningModeSelector->setObjectName(QString("window_opening_mode"));
  lDisplayLayout->addWidget(lWindowOpeningModeSelector);

  // WINDOW WIDTH
  auto lWinWidthLabel{new QLabel(tr("Default main window width:"), this)};
  lDisplayLayout->addWidget(lWinWidthLabel);

  auto lWinWidthInput{new QLineEdit("", this)};
  lWinWidthInput->setObjectName(QString("window_width"));
  lWinWidthInput->setValidator(new QIntValidator(0, 9999, this));
  lDisplayLayout->addWidget(lWinWidthInput);

  // WINDOW HEIGHT
  auto lWinHeightLabel{new QLabel(tr("Default main window height:"), this)};
  lDisplayLayout->addWidget(lWinHeightLabel);

  auto lWinHeightInput{new QLineEdit("", this)};
  lWinHeightInput->setObjectName(QString("window_height"));
  lWinHeightInput->setValidator(new QIntValidator(0, 9999, this));
  lDisplayLayout->addWidget(lWinHeightInput);

  // Event binding
  connect(lFontChooser, &QPushButton::clicked, this, &Settings::chooseFont);
}

void Settings::setupPresetCreatorTab(QTabWidget* aTabs)
{
  QWidget* lPresetCreatorTab{new QWidget(aTabs)};
  aTabs->addTab(lPresetCreatorTab, tr("Preset Creator"));

  auto lPresetCreatorLayout{new QGridLayout(lPresetCreatorTab)};
  lPresetCreatorLayout->setAlignment(Qt::AlignTop);
  lPresetCreatorTab->setLayout(lPresetCreatorLayout);

  // DEFAULT SELECTED CBBE 3BBB VERSION
  auto ldefaultCbbe3BBBVersionLabel{new QLabel(tr("Default selected CBBE 3BBB version:"), this)};
  lPresetCreatorLayout->addWidget(ldefaultCbbe3BBBVersionLabel, 0, 0, 1, 2);

  auto ldefaultCbbe3BBBVersionSelector{new QComboBox(this)};
  ldefaultCbbe3BBBVersionSelector->addItems(DataLists::getCBBE3BBBVersions());
  ldefaultCbbe3BBBVersionSelector->setObjectName(QString("default_cbbe_3bbb_version"));
  lPresetCreatorLayout->addWidget(ldefaultCbbe3BBBVersionSelector, 1, 0, 1, 2);

  // OUTPUT PATH PREVIEW
  auto lOutputPathLabel{new QLabel(tr("Output directory path:"), this)};
  lPresetCreatorLayout->addWidget(lOutputPathLabel, 2, 0, 1, 2);

  auto lOutputPathLineEdit{new QLineEdit("", this)};
  lOutputPathLineEdit->setReadOnly(true);
  lOutputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lOutputPathLineEdit->setObjectName("output_path_directory");
  lPresetCreatorLayout->addWidget(lOutputPathLineEdit, 3, 0);

  // OUTPUT PATH CHOOSER
  auto lOutputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lPresetCreatorLayout->addWidget(lOutputPathChooser, 3, 1);

  // AUTOMATICALLY OPEN THE GENERATED DIRECTORY
  auto lAutoOpenDirCheckbox{new QCheckBox(tr("Automatically open the generated preset's output directory after a generation"), this)};
  lAutoOpenDirCheckbox->setObjectName(QString("auto_open_generated_dir"));
  lPresetCreatorLayout->addWidget(lAutoOpenDirCheckbox, 4, 0, 1, 2);

  // Event binding
  connect(lOutputPathChooser, &QPushButton::clicked, this, &Settings::chooseExportDirectory);
}

void Settings::setupRetargetingToolTab(QTabWidget* aTabs)
{
  QWidget* lRetargetingToolTab{new QWidget(aTabs)};
  aTabs->addTab(lRetargetingToolTab, tr("Retargeting Tool"));

  auto lRetargetingToolLayout{new QVBoxLayout(lRetargetingToolTab)};
  lRetargetingToolLayout->setAlignment(Qt::AlignTop);
  lRetargetingToolTab->setLayout(lRetargetingToolLayout);

  // DEFAULT SELECTED CBBE 3BBB VERSION (RETARGETING TOOL)
  auto lupgradeCbbe3BBBVersionLabel{new QLabel(tr("Default selected CBBE 3BBB version:"), this)};
  lRetargetingToolLayout->addWidget(lupgradeCbbe3BBBVersionLabel);

  auto lUpgradeCbbe3BBBVersionSelector{new QComboBox(this)};
  lUpgradeCbbe3BBBVersionSelector->addItems(DataLists::getCBBE3BBBVersions());
  lUpgradeCbbe3BBBVersionSelector->setObjectName(QString("upgrade_cbbe_3bbb_version"));
  lRetargetingToolLayout->addWidget(lUpgradeCbbe3BBBVersionSelector);
}

void Settings::setupButtons(QHBoxLayout* aLayout)
{
  // Create the buttons
  auto lRestoreDefaultButton{new QPushButton(tr("Restore default"), this)};
  lRestoreDefaultButton->setAutoDefault(false);
  lRestoreDefaultButton->setDefault(false);
  aLayout->addWidget(lRestoreDefaultButton);

  auto lSaveButton{new QPushButton(tr("Save and close"), this)};
  lSaveButton->setObjectName("save_close");
  lSaveButton->setAutoDefault(false);
  lSaveButton->setDefault(false);
  aLayout->addWidget(lSaveButton);

  auto lCloseButton{new QPushButton(tr("Close"), this)};
  lCloseButton->setAutoDefault(false);
  lCloseButton->setDefault(false);
  aLayout->addWidget(lCloseButton);

  // Event binding
  connect(lRestoreDefaultButton, &QPushButton::clicked, this, &Settings::restoreDefaultSettings);
  connect(lSaveButton, &QPushButton::clicked, this, &Settings::saveSettings);
  connect(lCloseButton, &QPushButton::clicked, this, &Settings::close);
}

void Settings::loadSettings(const Struct::Settings& aSettingsToLoad)
{
  auto lLang{this->findChild<QComboBox*>("language")};
  lLang->setCurrentIndex(static_cast<int>(aSettingsToLoad.language));

  auto lAppTheme{this->findChild<QComboBox*>("app_theme")};
  lAppTheme->setCurrentIndex(static_cast<int>(aSettingsToLoad.appTheme));

  auto lWindowWidth{this->findChild<QLineEdit*>("window_width")};
  lWindowWidth->setText(QString::number(aSettingsToLoad.mainWindowWidth));

  auto lWindowHeight{this->findChild<QLineEdit*>("window_height")};
  lWindowHeight->setText(QString::number(aSettingsToLoad.mainWindowHeight));

  auto lWindowOpeningMode{this->findChild<QComboBox*>("window_opening_mode")};
  lWindowOpeningMode->setCurrentIndex(static_cast<int>(aSettingsToLoad.mainWindowOpeningMode));

  auto lDefaultCBBE3BBBVersion{this->findChild<QComboBox*>("default_cbbe_3bbb_version")};
  lDefaultCBBE3BBBVersion->setCurrentIndex(static_cast<int>(aSettingsToLoad.defaultMainWindowCBBE3BBBVersion));

  auto lDefaultUpgradeCBBE3BBBVersion{this->findChild<QComboBox*>("upgrade_cbbe_3bbb_version")};
  lDefaultUpgradeCBBE3BBBVersion->setCurrentIndex(static_cast<int>(aSettingsToLoad.defaultRetargetingToolCBBE3BBBVersion));

  auto lMainWindowOutputPath{this->findChild<QLineEdit*>("output_path_directory")};
  lMainWindowOutputPath->setText(aSettingsToLoad.mainWindowOutputPath);

  auto lAutoOpenGeneratedDir{this->findChild<QCheckBox*>("auto_open_generated_dir")};
  lAutoOpenGeneratedDir->setChecked(aSettingsToLoad.mainWindowAutomaticallyOpenGeneratedDirectory);
}

Struct::Settings Settings::getSettingsFromGUI()
{
  auto lLang{this->findChild<QComboBox*>("language")->currentIndex()};
  auto lAppTheme{this->findChild<QComboBox*>("app_theme")->currentIndex()};
  auto lWindowWidth{this->findChild<QLineEdit*>("window_width")->text().trimmed()};
  auto lWindowHeight{this->findChild<QLineEdit*>("window_height")->text().trimmed()};
  auto lDefaultCBBE3BBBVersion{this->findChild<QComboBox*>("default_cbbe_3bbb_version")->currentIndex()};
  auto lDefaultUpgradeCBBE3BBBVersion{this->findChild<QComboBox*>("upgrade_cbbe_3bbb_version")->currentIndex()};
  auto lWindowOpeningMode{this->findChild<QComboBox*>("window_opening_mode")->currentIndex()};
  auto lMainWindowOutputPath{this->findChild<QLineEdit*>("output_path_directory")->text()};
  auto lAutoOpenGeneratedDir{this->findChild<QCheckBox*>("auto_open_generated_dir")->isChecked()};

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
  lSettings.font.family = this->mNewFont.family();
  lSettings.font.styleName = this->mNewFont.styleName();
  lSettings.font.size = this->mNewFont.pointSize();
  lSettings.font.weight = this->mNewFont.weight();
  lSettings.font.italic = this->mNewFont.italic();
  lSettings.font.underline = this->mNewFont.underline();
  lSettings.font.strikeOut = this->mNewFont.strikeOut();

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

  // Main window output path
  if (lMainWindowOutputPath.size() > 0)
  {
    lSettings.mainWindowOutputPath = lMainWindowOutputPath;
  }

  // Automatically open generated directory
  lSettings.mainWindowAutomaticallyOpenGeneratedDirectory = lAutoOpenGeneratedDir;

  return lSettings;
}

void Settings::restoreDefaultSettings()
{
  // Create a default settings object and load it into the GUI
  Struct::Settings lSettings;
  this->loadSettings(lSettings);
}

void Settings::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("output_path_directory")};

  auto lSetGUIPath{lLineEdit->text()};
  auto lPreSelectedDirectory{QString("")};
  if (lSetGUIPath == "")
  {
    lPreSelectedDirectory = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
  }
  else
  {
    lPreSelectedDirectory = lSetGUIPath;
  }

  auto lPath{QFileDialog::getExistingDirectory(this, "", lPreSelectedDirectory)};
  lLineEdit->setText(lPath);
}

void Settings::chooseFont()
{
  bool lUserPressedOK{false};
  QFont lFont = QFontDialog::getFont(&lUserPressedOK, this->mNewFont, this);

  if (!lUserPressedOK)
  {
    return;
  }

  // Read and store new font's information
  this->mNewFont = lFont;

  // Display a preview on the font chooser's button directly
  auto lFontChooserButton{this->findChild<QPushButton*>("font_chooser")};
  lFontChooserButton->setFont(lFont);
}

void Settings::saveSettings()
{
  auto lSettings{this->getSettingsFromGUI()};

  mMustRebootMainApp = (mSettings.language != lSettings.language);

  Utils::saveSettingsToFile(lSettings);
  mSettings = lSettings;

  emit refreshMainUI(mSettings, true);

  if (mMustRebootMainApp)
  {
    auto lResult{QMessageBox::question(this, tr("Application settings changed"), tr("All settings have been saved. You changed a setting that needs a restart of the application to be applied. Would you like to restart the application now?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)};

    if (lResult == QMessageBox::Yes)
    {
      // Reboot the application in case the language is changed
      qApp->exit(Settings::EXIT_CODE_REBOOT);
    }
  }

  this->close();
}
