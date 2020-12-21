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
    // User theme accent
    const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

    QMessageBox lConfirmationBox(QMessageBox::Icon::Question, tr("Closing"), tr("Do you want to close the window?"), QMessageBox::StandardButton::NoButton, this);
    lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/help-circle").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

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
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setContentsMargins(10, 10, 10, 10);
  this->setLayout(lMainLayout);

  auto lIndex{0};

  if (Utils::RESTART_PENDING)
  {
    auto lRestartPending{new QLabel(tr("Warning: A restart of the application is pending. You should not modify any value marked with the \"*\" character until you restart the application."), this)};
    lRestartPending->setStyleSheet("color: hsl(4, 90%, 58%);");
    lMainLayout->addWidget(lRestartPending, lIndex++, 0, 1, 2);
  }

  this->setupDisplayGroup(*lMainLayout, lIndex);
  this->setupGeneralGroup(*lMainLayout, lIndex);
  this->setupPresetCreatorGroup(*lMainLayout, lIndex);
  this->setupAssistedConversionGroup(*lMainLayout, lIndex);
  this->setupRetargetingToolGroup(*lMainLayout, lIndex);
  this->setupButtons(*lMainLayout, lIndex);

  // Load the settings into the interface
  this->loadSettings(this->mSettings);
}

void Settings::setupDisplayGroup(QGridLayout& aLayout, const int& aNextRowIndex)
{
  // Display group box
  auto lDisplayGroupBox{new QGroupBox(tr("Display"), this)};
  aLayout.addWidget(lDisplayGroupBox, aNextRowIndex, 0, 4, 1);

  // Container layout
  auto lDisplayLayout{new QVBoxLayout(lDisplayGroupBox)};
  lDisplayLayout->setSpacing(10);
  lDisplayLayout->setContentsMargins(15, 20, 15, 15);
  lDisplayLayout->setAlignment(Qt::AlignTop);

  // LANGUAGE
  auto lLanguageLabel{new QLabel(QString("* " + tr("Language:")), this)};
  lDisplayLayout->addWidget(lLanguageLabel);

  auto lLanguageSelector{new QComboBox(this)};
  lLanguageSelector->setItemDelegate(new QStyledItemDelegate());
  lLanguageSelector->setCursor(Qt::PointingHandCursor);
  lLanguageSelector->addItems(DataLists::getLanguages());
  lLanguageSelector->setObjectName(QString("language"));
  lDisplayLayout->addWidget(lLanguageSelector);

  // FONT FAMILY
  auto lFontLabel{new QLabel(QString("* " + tr("Font:")), this)};
  lDisplayLayout->addWidget(lFontLabel);

  auto lFontChooser{new QPushButton(tr("Choose a font"), this)};
  lFontChooser->setCursor(Qt::PointingHandCursor);
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};
  lFontChooser->setIcon(QIcon(QPixmap(QString(":/%1/text").arg(lIconFolder))));
  lFontChooser->setObjectName("font_chooser");
  lFontChooser->setAutoDefault(false);
  lFontChooser->setDefault(false);
  lDisplayLayout->addWidget(lFontChooser);

  // GUI THEME
  auto lGUIThemeLabel{new QLabel(QString("* " + tr("Application Theme:")), this)};
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

void Settings::setupGeneralGroup(QGridLayout& aLayout, const int& aNextRowIndex)
{
  // Display group box
  auto lGeneralGroupBox{new QGroupBox(tr("General"), this)};
  aLayout.addWidget(lGeneralGroupBox, aNextRowIndex, 1);

  // Container layout
  auto lDisplayLayout{new QVBoxLayout(lGeneralGroupBox)};
  lDisplayLayout->setSpacing(10);
  lDisplayLayout->setContentsMargins(15, 20, 15, 15);
  lDisplayLayout->setAlignment(Qt::AlignTop);

  // CHECK UPDATE AT SOFTWARE STARTUP
  auto lAutoOpenDirCheckbox{new QCheckBox(tr("Check for updates at application startup"), this)};
  lAutoOpenDirCheckbox->setCursor(Qt::PointingHandCursor);
  lAutoOpenDirCheckbox->setObjectName(QString("check_update_startup"));
  lDisplayLayout->addWidget(lAutoOpenDirCheckbox);
}

void Settings::setupPresetCreatorGroup(QGridLayout& aLayout, const int& aNextRowIndex)
{
  // Preset Creator group box
  auto lPresetCreatorGroupBox{new QGroupBox(tr("Preset Creator"), this)};
  aLayout.addWidget(lPresetCreatorGroupBox, aNextRowIndex + 1, 1);

  auto lPresetCreatorLayout{new QGridLayout(lPresetCreatorGroupBox)};
  lPresetCreatorLayout->setSpacing(10);
  lPresetCreatorLayout->setContentsMargins(15, 20, 15, 15);
  lPresetCreatorLayout->setAlignment(Qt::AlignTop);

  // DEFAULT SELECTED BODY AND VERSION
  auto lDefaultBodyVersionLabel{new QLabel(tr("Default selected body:"), this)};
  lPresetCreatorLayout->addWidget(lDefaultBodyVersionLabel, 0, 0, 1, 2);

  auto lDefaultBodyVersionLabelSelector{new QComboBox(this)};
  lDefaultBodyVersionLabelSelector->setItemDelegate(new QStyledItemDelegate());
  lDefaultBodyVersionLabelSelector->setCursor(Qt::PointingHandCursor);
  lDefaultBodyVersionLabelSelector->addItems(DataLists::getBodiesNameVersions());
  lDefaultBodyVersionLabelSelector->setObjectName(QString("default_body_selector"));
  lPresetCreatorLayout->addWidget(lDefaultBodyVersionLabelSelector, 1, 0, 1, 2);

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
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};
  lOutputPathChooser->setIcon(QIcon(QPixmap(QString(":/%1/folder").arg(lIconFolder))));
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

void Settings::setupAssistedConversionGroup(QGridLayout& aLayout, const int& aNextRowIndex)
{
  // Assisted Conversion group box
  auto lAssistedConversionGroupBox{new QGroupBox(tr("Assisted Conversion"), this)};
  aLayout.addWidget(lAssistedConversionGroupBox, aNextRowIndex + 2, 1);

  // Container layout
  auto lAssistedConversionLayout{new QVBoxLayout(lAssistedConversionGroupBox)};
  lAssistedConversionLayout->setSpacing(10);
  lAssistedConversionLayout->setContentsMargins(15, 20, 15, 15);
  lAssistedConversionLayout->setAlignment(Qt::AlignTop);

  // ONLY SCAN THE MESHES SUBDIRECTORY
  auto lScanOnlyMeshesFolder{new QCheckBox(tr("Only scan the \"meshes\" subdirectory"), this)};
  lScanOnlyMeshesFolder->setCursor(Qt::PointingHandCursor);
  lScanOnlyMeshesFolder->setObjectName(QString("assis_conv_only_scan_meshes_dir"));
  lAssistedConversionLayout->addWidget(lScanOnlyMeshesFolder);
}

void Settings::setupRetargetingToolGroup(QGridLayout& aLayout, const int& aNextRowIndex)
{
  // Retargeting Tool group box
  auto lRetToolGroupBox{new QGroupBox(tr("Retargeting Tool"), this)};
  aLayout.addWidget(lRetToolGroupBox, aNextRowIndex + 3, 1);

  auto lRetargetingToolLayout{new QVBoxLayout(lRetToolGroupBox)};
  lRetargetingToolLayout->setSpacing(10);
  lRetargetingToolLayout->setContentsMargins(15, 20, 15, 15);
  lRetargetingToolLayout->setAlignment(Qt::AlignTop);

  // DEFAULT SELECTED BODY AND VERSION (RETARGETING TOOL)
  auto lUpgradeBodyNameVersionLabel{new QLabel(tr("Default selected body:"), this)};
  lRetargetingToolLayout->addWidget(lUpgradeBodyNameVersionLabel);

  auto lUpgradeBodyNameVersionSelector{new QComboBox(this)};
  lUpgradeBodyNameVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lUpgradeBodyNameVersionSelector->setCursor(Qt::PointingHandCursor);
  lUpgradeBodyNameVersionSelector->addItems(DataLists::getBodiesNameVersions());
  lUpgradeBodyNameVersionSelector->setObjectName(QString("upgrade_body_selector"));
  lRetargetingToolLayout->addWidget(lUpgradeBodyNameVersionSelector);
}

void Settings::setupButtons(QGridLayout& aLayout, const int& aNextRowIndex)
{
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // Vertical layout for the buttons
  auto lButtonsContainer{new QHBoxLayout()};
  lButtonsContainer->setSpacing(10);
  aLayout.addLayout(lButtonsContainer, aNextRowIndex + 4, 0, 1, 2);

  // Create the buttons
  auto lRestoreDefaultButton{new QPushButton(tr("Restore default"), this)};
  lRestoreDefaultButton->setCursor(Qt::PointingHandCursor);
  lRestoreDefaultButton->setIcon(QIcon(QPixmap(QString(":/%1/restore").arg(lIconFolder))));
  lRestoreDefaultButton->setAutoDefault(false);
  lRestoreDefaultButton->setDefault(false);
  lButtonsContainer->addWidget(lRestoreDefaultButton);

  auto lSaveButton{new QPushButton(tr("Save and close"), this)};
  lSaveButton->setCursor(Qt::PointingHandCursor);
  lSaveButton->setIcon(QIcon(QPixmap(QString(":/%1/save").arg(lIconFolder))));
  lSaveButton->setObjectName("save_close");
  lSaveButton->setAutoDefault(false);
  lSaveButton->setDefault(false);
  lButtonsContainer->addWidget(lSaveButton);

  auto lCloseButton{new QPushButton(tr("Cancel"), this)};
  lCloseButton->setCursor(Qt::PointingHandCursor);
  lCloseButton->setIcon(QIcon(QPixmap(QString(":/%1/undo").arg(lIconFolder))));
  lCloseButton->setAutoDefault(false);
  lCloseButton->setDefault(false);
  lButtonsContainer->addWidget(lCloseButton);

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

  auto lDefaultBody{this->findChild<QComboBox*>("default_body_selector")};
  lDefaultBody->setCurrentIndex(static_cast<int>(aSettingsToLoad.defaultMainWindowBody));

  auto lDefaultUpgradeBody{this->findChild<QComboBox*>("upgrade_body_selector")};
  lDefaultUpgradeBody->setCurrentIndex(static_cast<int>(aSettingsToLoad.defaultRetargetingToolBody));

  auto lMainWindowOutputPath{this->findChild<QLineEdit*>("output_path_directory")};
  lMainWindowOutputPath->setText(aSettingsToLoad.mainWindowOutputPath);

  auto lAutoOpenGeneratedDir{this->findChild<QCheckBox*>("auto_open_generated_dir")};
  lAutoOpenGeneratedDir->setChecked(aSettingsToLoad.mainWindowAutomaticallyOpenGeneratedDirectory);

  auto lCheckUpdateStartup{this->findChild<QCheckBox*>("check_update_startup")};
  lCheckUpdateStartup->setChecked(aSettingsToLoad.checkForUpdatesAtStartup);

  auto lAssisConvScanOnlyMeshesDir{this->findChild<QCheckBox*>("assis_conv_only_scan_meshes_dir")};
  lAssisConvScanOnlyMeshesDir->setChecked(aSettingsToLoad.assistedConversionScanOnlyMeshesSubdir);
}

Struct::Settings Settings::getSettingsFromGUI()
{
  auto lLang{this->findChild<QComboBox*>("language")->currentIndex()};
  auto lAppTheme{this->findChild<QComboBox*>("app_theme")->currentIndex()};
  auto lWindowWidth{this->findChild<QLineEdit*>("window_width")->text().trimmed()};
  auto lWindowHeight{this->findChild<QLineEdit*>("window_height")->text().trimmed()};
  auto lDefaultBodyVersion{this->findChild<QComboBox*>("default_body_selector")->currentIndex()};
  auto lDefaultRetargetBodyVersion{this->findChild<QComboBox*>("upgrade_body_selector")->currentIndex()};
  auto lWindowOpeningMode{this->findChild<QComboBox*>("window_opening_mode")->currentIndex()};
  auto lMainWindowOutputPath{this->findChild<QLineEdit*>("output_path_directory")->text()};
  auto lAutoOpenGeneratedDir{this->findChild<QCheckBox*>("auto_open_generated_dir")->isChecked()};
  auto lCheckUpdateStartup{this->findChild<QCheckBox*>("check_update_startup")->isChecked()};
  auto lAssisConvScanOnlyMeshesDir{this->findChild<QCheckBox*>("assis_conv_only_scan_meshes_dir")->isChecked()};

  Struct::Settings lSettings;

  // Language
  auto lEnumLang{static_cast<ApplicationLanguage>(lLang)};
  switch (lEnumLang)
  {
    case ApplicationLanguage::English:
      lSettings.language = ApplicationLanguage::English;
      break;
    case ApplicationLanguage::French:
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
  lSettings.appTheme = static_cast<GUITheme>(lAppTheme);

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
  lSettings.mainWindowOpeningMode = static_cast<WindowOpeningMode>(lWindowOpeningMode);

  // Default selected body and version (main)
  lSettings.defaultMainWindowBody = static_cast<BodyNameVersion>(lDefaultBodyVersion);

  // Default selected body and version (Retargeting tool)
  lSettings.defaultRetargetingToolBody = static_cast<BodyNameVersion>(lDefaultRetargetBodyVersion);

  // Main window output path
  if (lMainWindowOutputPath.size() > 0)
  {
    lSettings.mainWindowOutputPath = lMainWindowOutputPath;
  }

  // Automatically open generated directory
  lSettings.mainWindowAutomaticallyOpenGeneratedDirectory = lAutoOpenGeneratedDir;

  // Check for updates at startup
  lSettings.checkForUpdatesAtStartup = lCheckUpdateStartup;

  // Assisted Conversion: only scan the meshes subdir
  lSettings.assistedConversionScanOnlyMeshesSubdir = lAssisConvScanOnlyMeshesDir;

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

  mMustRebootMainApp = (this->mSettings.language != lSettings.language || this->mSettings.appTheme != lSettings.appTheme || this->mSettings.font != lSettings.font);

  if (mMustRebootMainApp)
  {
    // User theme accent
    const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

    QMessageBox lConfirmationBox(QMessageBox::Icon::Question, tr("Application settings changed"), tr("All settings have been saved. You changed a setting that needs a restart of the application to be applied. Would you like to restart the application now?"), QMessageBox::StandardButton::NoButton, this);
    lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/help-circle").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

    auto lRestartNowButton{lConfirmationBox.addButton(tr("Restart now"), QMessageBox::ButtonRole::YesRole)};
    lRestartNowButton->setCursor(Qt::PointingHandCursor);
    lRestartNowButton->setStyleSheet("color: hsl(4, 90%, 58%);");

    auto lRestartLaterButton{lConfirmationBox.addButton(tr("Go back to the application and restart later"), QMessageBox::ButtonRole::NoRole)};
    lRestartLaterButton->setCursor(Qt::PointingHandCursor);
    lRestartLaterButton->setStyleSheet("color: hsl(33, 100%, 71%);");

    lConfirmationBox.setDefaultButton(lRestartLaterButton);
    lConfirmationBox.exec();

    if (lConfirmationBox.clickedButton() == lRestartNowButton)
    {
      // Reboot the application in case the language is changed
      qApp->exit(Utils::EXIT_CODE_REBOOT);
    }
    else if (!Utils::RESTART_PENDING)
    {
      Utils::RESTART_PENDING = true;
    }
  }

  Utils::saveSettingsToFile(lSettings);
  this->mSettings = lSettings;

  emit refreshMainUI(this->mSettings, true);

  this->close();
}
