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
  this->adjustSize();
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

  if (this->getSettingsFromGUI() != this->mSettings)
  {
    QMessageBox lConfirmationBox(QMessageBox::Icon::Question, tr("Closing"), tr("Do you want to close the window?"), QMessageBox::StandardButton::NoButton, this);

    auto lCloseButton{lConfirmationBox.addButton(tr("Close the settings window without saving"), QMessageBox::ButtonRole::YesRole)};
    lCloseButton->setCursor(Qt::PointingHandCursor);
    lCloseButton->setStyleSheet("color: hsl(4, 90%, 58%);");

    auto lStayButton{lConfirmationBox.addButton(tr("Go back to the settings window"), QMessageBox::ButtonRole::NoRole)};
    lStayButton->setCursor(Qt::PointingHandCursor);
    lStayButton->setStyleSheet("color: hsl(141, 53%, 53%)");

    lConfirmationBox.setDefaultButton(lStayButton);
    lConfirmationBox.exec();

    if (lConfirmationBox.clickedButton() != lCloseButton)
    {
      aEvent->ignore();
      return;
    }
  }

  aEvent->accept();
}

void Settings::reject()
{
  this->close();
}

void Settings::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Settings"));
  this->setWindowIcon(QIcon(QPixmap(":/black/cog")));
}

void Settings::initializeGUI()
{
  // Main layout
  auto lMainContainer{new QVBoxLayout(this)};
  this->setLayout(lMainContainer);

  // Tabs
  auto lTabsContainer{new QVBoxLayout()};
  lTabsContainer->setContentsMargins(0, 0, 0, 0);

  lMainContainer->addLayout(lTabsContainer);
  this->setupTabs(*lTabsContainer);

  // Buttons
  auto lButtonsContainer{new QHBoxLayout()};
  lButtonsContainer->setSpacing(10);
  lButtonsContainer->setContentsMargins(0, 0, 0, 0);

  lMainContainer->addLayout(lButtonsContainer);
  this->setupButtons(*lButtonsContainer);

  // Load the settings in the interface
  this->loadSettings(this->mSettings);
}

void Settings::setupTabs(QVBoxLayout& aLayout)
{
  auto lTabs{new QTabWidget(this)};
  lTabs->tabBar()->setCursor(Qt::PointingHandCursor);
  aLayout.addWidget(lTabs);

  this->setupDisplayTab(*lTabs);
  this->setupPresetCreatorTab(*lTabs);
  this->setupRetargetingToolTab(*lTabs);
}

void Settings::setupDisplayTab(QTabWidget& aTabs)
{
  auto lDisplayTab{new QWidget(&aTabs)};
  aTabs.addTab(lDisplayTab, tr("Display"));

  auto lDisplayLayout{new QVBoxLayout(lDisplayTab)};
  lDisplayLayout->setSpacing(10);
  lDisplayLayout->setContentsMargins(0, 10, 0, 0);
  lDisplayLayout->setAlignment(Qt::AlignTop);
  lDisplayTab->setLayout(lDisplayLayout);

  // LANGUAGE
  auto lLangLabelText{"* " + tr("Language:")};
  auto lLanguageLabel{new QLabel(lLangLabelText, this)};
  lDisplayLayout->addWidget(lLanguageLabel);

  auto lLanguageSelector{new QComboBox(this)};
  lLanguageSelector->setItemDelegate(new QStyledItemDelegate());
  lLanguageSelector->setCursor(Qt::PointingHandCursor);
  lLanguageSelector->addItems(DataLists::getLanguages());
  lLanguageSelector->setObjectName(QString("language"));
  lDisplayLayout->addWidget(lLanguageSelector);

  // FONT FAMILY
  auto lFontLabelText{"* " + tr("Font:")};
  auto lFontLabel{new QLabel(lFontLabelText, this)};
  lDisplayLayout->addWidget(lFontLabel);

  auto lFontChooser{new QPushButton(tr("Choose a font"), this)};
  lFontChooser->setCursor(Qt::PointingHandCursor);
  lFontChooser->setObjectName("font_chooser");
  lFontChooser->setAutoDefault(false);
  lFontChooser->setDefault(false);
  lDisplayLayout->addWidget(lFontChooser);

  // GUI THEME
  auto lThemeLabelText{"* " + tr("Application Theme:")};
  auto lGUIThemeLabel{new QLabel(lThemeLabelText, this)};
  lDisplayLayout->addWidget(lGUIThemeLabel);

  auto lGUIThemeSelector{new QComboBox(this)};
  lGUIThemeSelector->setItemDelegate(new QStyledItemDelegate());
  lGUIThemeSelector->setCursor(Qt::PointingHandCursor);
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
  lWindowOpeningModeSelector->setItemDelegate(new QStyledItemDelegate());
  lWindowOpeningModeSelector->setCursor(Qt::PointingHandCursor);
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
  this->connect(lFontChooser, &QPushButton::clicked, this, &Settings::chooseFont);
}

void Settings::setupPresetCreatorTab(QTabWidget& aTabs)
{
  QWidget* lPresetCreatorTab{new QWidget(&aTabs)};
  aTabs.addTab(lPresetCreatorTab, tr("Preset Creator"));

  auto lPresetCreatorLayout{new QGridLayout(lPresetCreatorTab)};
  lPresetCreatorLayout->setSpacing(10);
  lPresetCreatorLayout->setContentsMargins(0, 10, 0, 0);
  lPresetCreatorLayout->setAlignment(Qt::AlignTop);
  lPresetCreatorTab->setLayout(lPresetCreatorLayout);

  // DEFAULT SELECTED CBBE 3BBB VERSION
  auto ldefaultCbbe3BBBVersionLabel{new QLabel(tr("Default selected CBBE 3BBB version:"), this)};
  lPresetCreatorLayout->addWidget(ldefaultCbbe3BBBVersionLabel, 0, 0, 1, 2);

  auto ldefaultCbbe3BBBVersionSelector{new QComboBox(this)};
  ldefaultCbbe3BBBVersionSelector->setItemDelegate(new QStyledItemDelegate());
  ldefaultCbbe3BBBVersionSelector->setCursor(Qt::PointingHandCursor);
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
  lOutputPathChooser->setCursor(Qt::PointingHandCursor);
  lOutputPathChooser->setAutoDefault(false);
  lOutputPathChooser->setDefault(false);
  lPresetCreatorLayout->addWidget(lOutputPathChooser, 3, 1);

  // AUTOMATICALLY OPEN THE GENERATED DIRECTORY
  auto lAutoOpenDirCheckbox{new QCheckBox(tr("Automatically open the generated preset's output directory after a generation"), this)};
  lAutoOpenDirCheckbox->setCursor(Qt::PointingHandCursor);
  lAutoOpenDirCheckbox->setObjectName(QString("auto_open_generated_dir"));
  lPresetCreatorLayout->addWidget(lAutoOpenDirCheckbox, 4, 0, 1, 2);

  // Event binding
  this->connect(lOutputPathChooser, &QPushButton::clicked, this, &Settings::chooseExportDirectory);
}

void Settings::setupRetargetingToolTab(QTabWidget& aTabs)
{
  QWidget* lRetargetingToolTab{new QWidget(&aTabs)};
  aTabs.addTab(lRetargetingToolTab, tr("Retargeting Tool"));

  auto lRetargetingToolLayout{new QVBoxLayout(lRetargetingToolTab)};
  lRetargetingToolLayout->setSpacing(10);
  lRetargetingToolLayout->setContentsMargins(0, 10, 0, 0);
  lRetargetingToolLayout->setAlignment(Qt::AlignTop);
  lRetargetingToolTab->setLayout(lRetargetingToolLayout);

  // DEFAULT SELECTED CBBE 3BBB VERSION (RETARGETING TOOL)
  auto lupgradeCbbe3BBBVersionLabel{new QLabel(tr("Default selected CBBE 3BBB version:"), this)};
  lRetargetingToolLayout->addWidget(lupgradeCbbe3BBBVersionLabel);

  auto lUpgradeCbbe3BBBVersionSelector{new QComboBox(this)};
  lUpgradeCbbe3BBBVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lUpgradeCbbe3BBBVersionSelector->setCursor(Qt::PointingHandCursor);
  lUpgradeCbbe3BBBVersionSelector->addItems(DataLists::getCBBE3BBBVersions());
  lUpgradeCbbe3BBBVersionSelector->setObjectName(QString("upgrade_cbbe_3bbb_version"));
  lRetargetingToolLayout->addWidget(lUpgradeCbbe3BBBVersionSelector);
}

void Settings::setupButtons(QHBoxLayout& aLayout)
{
  // Create the buttons
  auto lRestoreDefaultButton{new QPushButton(tr("Restore default"), this)};
  lRestoreDefaultButton->setCursor(Qt::PointingHandCursor);
  lRestoreDefaultButton->setAutoDefault(false);
  lRestoreDefaultButton->setDefault(false);
  aLayout.addWidget(lRestoreDefaultButton);

  auto lSaveButton{new QPushButton(tr("Save and close"), this)};
  lSaveButton->setCursor(Qt::PointingHandCursor);
  lSaveButton->setObjectName("save_close");
  lSaveButton->setAutoDefault(false);
  lSaveButton->setDefault(false);
  aLayout.addWidget(lSaveButton);

  auto lCloseButton{new QPushButton(tr("Cancel"), this)};
  lCloseButton->setCursor(Qt::PointingHandCursor);
  lCloseButton->setAutoDefault(false);
  lCloseButton->setDefault(false);
  aLayout.addWidget(lCloseButton);

  // Event binding
  this->connect(lRestoreDefaultButton, &QPushButton::clicked, this, &Settings::restoreDefaultSettings);
  this->connect(lSaveButton, &QPushButton::clicked, this, &Settings::saveSettings);
  this->connect(lCloseButton, &QPushButton::clicked, this, &Settings::close);
}

void Settings::loadSettings(const Struct::Settings& aSettingsToLoad)
{
  auto lLang{this->findChild<QComboBox*>("language")};
  lLang->setCurrentIndex(static_cast<int>(aSettingsToLoad.language));

  // Reset the font
  QFont lFont(aSettingsToLoad.font.family, aSettingsToLoad.font.size, aSettingsToLoad.font.weight, aSettingsToLoad.font.italic);
  lFont.setStyleName(aSettingsToLoad.font.styleName);
  lFont.setUnderline(aSettingsToLoad.font.underline);
  lFont.setStrikeOut(aSettingsToLoad.font.strikeOut);
  lFont.setStyleStrategy(QFont::PreferAntialias);
  this->mNewFont = lFont;
  this->applyFontButtonStyle(this->mNewFont);

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
    case static_cast<int>(GUITheme::MitsuriouLightTheme):
      lSettings.appTheme = GUITheme::MitsuriouLightTheme;
      break;
    case static_cast<int>(GUITheme::MitsuriouDarkTheme):
      lSettings.appTheme = GUITheme::MitsuriouDarkTheme;
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
  auto lPath{QFileDialog::getExistingDirectory(this, "", lLineEdit->text().size() > 0 ? lLineEdit->text() : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))};
  lLineEdit->setText(lPath);
}

void Settings::chooseFont()
{
  bool lUserPressedOK{false};
  auto lFont{QFontDialog::getFont(&lUserPressedOK, this->mNewFont, this)};

  if (!lUserPressedOK)
  {
    return;
  }

  lFont.setStyleStrategy(QFont::PreferAntialias);

  // Read and store new font's information
  this->mNewFont = lFont;

  // Display a preview on the font chooser's button directly
  this->applyFontButtonStyle(this->mNewFont);
}

void Settings::applyFontButtonStyle(const QFont& aFont)
{
  auto lFontChooserButton{this->findChild<QPushButton*>("font_chooser")};
  lFontChooserButton->setFont(aFont);
}

void Settings::saveSettings()
{
  auto lSettings{this->getSettingsFromGUI()};

  mMustRebootMainApp = (this->mSettings.language != lSettings.language
                        || this->mSettings.appTheme != lSettings.appTheme
                        || this->mSettings.font != lSettings.font);

  Utils::saveSettingsToFile(lSettings);
  this->mSettings = lSettings;

  emit refreshMainUI(this->mSettings, true);

  if (mMustRebootMainApp)
  {
    QMessageBox lConfirmationBox(QMessageBox::Icon::Question, tr("Application settings changed"), tr("All settings have been saved. You changed a setting that needs a restart of the software to be applied. Would you like to restart the software now?"), QMessageBox::StandardButton::NoButton, this);

    auto lRestartNowButton{lConfirmationBox.addButton(tr("Restart now"), QMessageBox::ButtonRole::YesRole)};
    lRestartNowButton->setCursor(Qt::PointingHandCursor);

    auto lRestartLaterButton{lConfirmationBox.addButton(tr("Go back to the software and restart later"), QMessageBox::ButtonRole::NoRole)};
    lRestartLaterButton->setCursor(Qt::PointingHandCursor);

    lConfirmationBox.setDefaultButton(lRestartLaterButton);
    lConfirmationBox.exec();

    if (lConfirmationBox.clickedButton() == lRestartNowButton)
    {
      // Reboot the application in case the language is changed
      qApp->exit(Settings::EXIT_CODE_REBOOT);
    }
  }

  this->close();
}
