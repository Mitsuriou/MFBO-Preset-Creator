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

    if (Utils::displayQuestionMessage(this,
                                      tr("Closing"),
                                      tr("Do you want to close the window?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Close the settings window without saving"),
                                      tr("Go back to the settings window"),
                                      this->mSettings.dangerColor,
                                      this->mSettings.successColor,
                                      false)
        != ButtonClicked::Yes)
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
    lRestartPending->setStyleSheet(QString("color: %1;").arg(this->mSettings.dangerColor));
    lMainLayout->addWidget(lRestartPending, lIndex++, 0, 1, 2);
  }

  this->setupDisplayGroup(*lMainLayout, lIndex);
  this->setupGeneralGroup(*lMainLayout, lIndex);
  this->setupPresetCreatorGroup(*lMainLayout, lIndex);
  this->setupRetargetingToolGroup(*lMainLayout, lIndex);
  this->setupAssistedConversionGroup(*lMainLayout, lIndex);
  this->setupButtons(*lMainLayout, lIndex);

  // Load the settings into the interface
  this->loadSettings(this->mSettings);
}

void Settings::setupDisplayGroup(QGridLayout& aLayout, const int& aNextRowIndex)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // Display group box
  auto lDisplayGroupBox{new QGroupBox(tr("Display"), this)};
  Utils::addIconToGroupBox(lDisplayGroupBox, lIconFolder, "monitor");
  this->connect(lDisplayGroupBox, &QGroupBox::toggled, this, &Settings::preventGroupBoxCheckEvent);
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

  // COLORS
  auto lColorsLabel{new QLabel(QString("* " + tr("Texts accent color:")), this)};
  lDisplayLayout->addWidget(lColorsLabel);

  // Success
  auto lSuccessColorChooser{new QPushButton(tr("Choose a success color"), this)};
  lSuccessColorChooser->setCursor(Qt::PointingHandCursor);
  lSuccessColorChooser->setIcon(QIcon(QPixmap(QString(":/%1/color").arg(lIconFolder))));
  lSuccessColorChooser->setObjectName("success_color_chooser");
  lSuccessColorChooser->setAutoDefault(false);
  lSuccessColorChooser->setDefault(false);
  lDisplayLayout->addWidget(lSuccessColorChooser);

  // Warning
  auto lWarningColorChooser{new QPushButton(tr("Choose a warning color"), this)};
  lWarningColorChooser->setCursor(Qt::PointingHandCursor);
  lWarningColorChooser->setIcon(QIcon(QPixmap(QString(":/%1/color").arg(lIconFolder))));
  lWarningColorChooser->setObjectName("warning_color_chooser");
  lWarningColorChooser->setAutoDefault(false);
  lWarningColorChooser->setDefault(false);
  lDisplayLayout->addWidget(lWarningColorChooser);

  // Danger
  auto lDangerColorChooser{new QPushButton(tr("Choose a danger color"), this)};
  lDangerColorChooser->setCursor(Qt::PointingHandCursor);
  lDangerColorChooser->setIcon(QIcon(QPixmap(QString(":/%1/color").arg(lIconFolder))));
  lDangerColorChooser->setObjectName("danger_color_chooser");
  lDangerColorChooser->setAutoDefault(false);
  lDangerColorChooser->setDefault(false);
  lDisplayLayout->addWidget(lDangerColorChooser);

  // Event binding
  this->connect(lFontChooser, &QPushButton::clicked, this, &Settings::chooseFont);
  this->connect(lSuccessColorChooser, &QPushButton::clicked, this, &Settings::chooseSuccessColor);
  this->connect(lWarningColorChooser, &QPushButton::clicked, this, &Settings::chooseWarningColor);
  this->connect(lDangerColorChooser, &QPushButton::clicked, this, &Settings::chooseDangerColor);
}

void Settings::setupGeneralGroup(QGridLayout& aLayout, const int& aNextRowIndex)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // Display group box
  auto lGeneralGroupBox{new QGroupBox(tr("General"), this)};
  Utils::addIconToGroupBox(lGeneralGroupBox, lIconFolder, "tune");
  this->connect(lGeneralGroupBox, &QGroupBox::toggled, this, &Settings::preventGroupBoxCheckEvent);
  aLayout.addWidget(lGeneralGroupBox, aNextRowIndex, 1);

  // Container layout
  auto lDisplayLayout{new QVBoxLayout(lGeneralGroupBox)};
  lDisplayLayout->setSpacing(10);
  lDisplayLayout->setContentsMargins(15, 20, 15, 15);
  lDisplayLayout->setAlignment(Qt::AlignTop);

  // CHECK UPDATE AT APPLICATION STARTUP
  auto lAutoOpenDirCheckbox{new QCheckBox(tr("Check for updates at application startup"), this)};
  lAutoOpenDirCheckbox->setCursor(Qt::PointingHandCursor);
  lAutoOpenDirCheckbox->setObjectName(QString("check_update_startup"));
  lDisplayLayout->addWidget(lAutoOpenDirCheckbox);

  // Each button stores the last opened path
  auto lEachButtonSavesItsLastUsedPath{new QCheckBox(tr("Each directory chooser button stores its own last opened path"), this)};
  lEachButtonSavesItsLastUsedPath->setCursor(Qt::PointingHandCursor);
  lEachButtonSavesItsLastUsedPath->setObjectName(QString("each_button_saves_last_path"));
  lDisplayLayout->addWidget(lEachButtonSavesItsLastUsedPath);
}

void Settings::setupPresetCreatorGroup(QGridLayout& aLayout, const int& aNextRowIndex)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // Preset Creator group box
  auto lPresetCreatorGroupBox{new QGroupBox(tr("Preset Creator"), this)};
  Utils::addIconToGroupBox(lPresetCreatorGroupBox, lIconFolder, "home");
  this->connect(lPresetCreatorGroupBox, &QGroupBox::toggled, this, &Settings::preventGroupBoxCheckEvent);
  aLayout.addWidget(lPresetCreatorGroupBox, aNextRowIndex + 1, 1);

  auto lPresetCreatorLayout{new QGridLayout(lPresetCreatorGroupBox)};
  lPresetCreatorLayout->setSpacing(10);
  lPresetCreatorLayout->setContentsMargins(15, 20, 15, 15);
  lPresetCreatorLayout->setAlignment(Qt::AlignTop);

  // DEFAULT SELECTED BODY AND VERSION
  auto lDefaultBodyVersionSettings{DataLists::getSplittedNameVersionFromBodyVersion(mSettings.defaultRetargetingToolBody)};

  auto lDefaultBodyVersionLabel{new QLabel(tr("Default selected body:"), this)};
  lPresetCreatorLayout->addWidget(lDefaultBodyVersionLabel, 0, 0, 1, 2);

  auto lBodyNameSelector{new QComboBox(this)};
  lBodyNameSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyNameSelector->setCursor(Qt::PointingHandCursor);
  lBodyNameSelector->addItems(DataLists::getBodiesNames());
  lBodyNameSelector->setCurrentIndex(lDefaultBodyVersionSettings.first);
  lBodyNameSelector->setObjectName(QString("default_body_selector_name"));
  lPresetCreatorLayout->addWidget(lBodyNameSelector, 1, 0);

  auto lBodyVersionSelector{new QComboBox(this)};
  lBodyVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyVersionSelector->setCursor(Qt::PointingHandCursor);
  lBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lBodyVersionSelector->setCurrentIndex(lDefaultBodyVersionSettings.second);
  lBodyVersionSelector->setObjectName(QString("default_body_selector_version"));
  lPresetCreatorLayout->addWidget(lBodyVersionSelector, 1, 1);

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
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // Assisted Conversion group box
  auto lAssistedConversionGroupBox{new QGroupBox(tr("Assisted Conversion"), this)};
  Utils::addIconToGroupBox(lAssistedConversionGroupBox, lIconFolder, "pencil");
  this->connect(lAssistedConversionGroupBox, &QGroupBox::toggled, this, &Settings::preventGroupBoxCheckEvent);
  aLayout.addWidget(lAssistedConversionGroupBox, aNextRowIndex + 3, 1);

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
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // BodySlide Presets' Retargeting group box
  auto lRetToolGroupBox{new QGroupBox(tr("BodySlide Presets' Retargeting"), this)};
  Utils::addIconToGroupBox(lRetToolGroupBox, lIconFolder, "arrow-up");
  this->connect(lRetToolGroupBox, &QGroupBox::toggled, this, &Settings::preventGroupBoxCheckEvent);
  aLayout.addWidget(lRetToolGroupBox, aNextRowIndex + 2, 1);

  auto lRetargetingToolLayout{new QVBoxLayout(lRetToolGroupBox)};
  lRetargetingToolLayout->setSpacing(10);
  lRetargetingToolLayout->setContentsMargins(15, 20, 15, 15);
  lRetargetingToolLayout->setAlignment(Qt::AlignTop);

  // DEFAULT SELECTED BODY AND VERSION (RETARGETING TOOL)
  auto lDefaultBodyVersionSettings{DataLists::getSplittedNameVersionFromBodyVersion(mSettings.defaultRetargetingToolBody)};

  auto lUpgradeBodyNameVersionLabel{new QLabel(tr("Default selected body:"), this)};
  lRetargetingToolLayout->addWidget(lUpgradeBodyNameVersionLabel);

  auto lUpgradeBodyNameSelector{new QComboBox(this)};
  lUpgradeBodyNameSelector->setItemDelegate(new QStyledItemDelegate());
  lUpgradeBodyNameSelector->setCursor(Qt::PointingHandCursor);
  lUpgradeBodyNameSelector->addItems(DataLists::getBodiesNames());
  lUpgradeBodyNameSelector->setCurrentIndex(lDefaultBodyVersionSettings.first);
  lUpgradeBodyNameSelector->setObjectName(QString("upgrade_body_selector_name"));
  lRetargetingToolLayout->addWidget(lUpgradeBodyNameSelector);

  auto lUpgradeBodyVersionSelector{new QComboBox(this)};
  lUpgradeBodyVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lUpgradeBodyVersionSelector->setCursor(Qt::PointingHandCursor);
  lUpgradeBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lUpgradeBodyVersionSelector->setCurrentIndex(lDefaultBodyVersionSettings.second);
  lUpgradeBodyVersionSelector->setObjectName(QString("upgrade_body_selector_version"));
  lRetargetingToolLayout->addWidget(lUpgradeBodyVersionSelector);
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

  auto lDefaultBody{DataLists::getSplittedNameVersionFromBodyVersion(aSettingsToLoad.defaultMainWindowBody)};
  this->findChild<QComboBox*>(QString("default_body_selector_name"))->setCurrentIndex(lDefaultBody.first);
  this->findChild<QComboBox*>(QString("default_body_selector_version"))->setCurrentIndex(lDefaultBody.second);

  auto lDefaultUpgradeBody{DataLists::getSplittedNameVersionFromBodyVersion(aSettingsToLoad.defaultRetargetingToolBody)};
  this->findChild<QComboBox*>(QString("upgrade_body_selector_name"))->setCurrentIndex(lDefaultUpgradeBody.first);
  this->findChild<QComboBox*>(QString("upgrade_body_selector_version"))->setCurrentIndex(lDefaultUpgradeBody.second);

  auto lMainWindowOutputPath{this->findChild<QLineEdit*>("output_path_directory")};
  lMainWindowOutputPath->setText(aSettingsToLoad.mainWindowOutputPath);

  auto lAutoOpenGeneratedDir{this->findChild<QCheckBox*>("auto_open_generated_dir")};
  lAutoOpenGeneratedDir->setChecked(aSettingsToLoad.mainWindowAutomaticallyOpenGeneratedDirectory);

  auto lCheckUpdateStartup{this->findChild<QCheckBox*>("check_update_startup")};
  lCheckUpdateStartup->setChecked(aSettingsToLoad.checkForUpdatesAtStartup);

  auto lAssisConvScanOnlyMeshesDir{this->findChild<QCheckBox*>("assis_conv_only_scan_meshes_dir")};
  lAssisConvScanOnlyMeshesDir->setChecked(aSettingsToLoad.assistedConversionScanOnlyMeshesSubdir);

  auto lEachButtonSavesItsLastUsedPath{this->findChild<QCheckBox*>("each_button_saves_last_path")};
  lEachButtonSavesItsLastUsedPath->setChecked(aSettingsToLoad.eachButtonSavesItsLastUsedPath);

  this->mNewSuccessColor = aSettingsToLoad.successColor;
  this->applySuccessColorButton(this->mNewSuccessColor);

  this->mNewWarningColor = aSettingsToLoad.warningColor;
  this->applyWarningColorButton(this->mNewWarningColor);

  this->mNewDangerColor = aSettingsToLoad.dangerColor;
  this->applyDangerColorButton(this->mNewDangerColor);
}

Struct::Settings Settings::getSettingsFromGUI() const
{
  auto lLang{this->findChild<QComboBox*>("language")->currentIndex()};
  auto lAppTheme{this->findChild<QComboBox*>("app_theme")->currentIndex()};
  auto lWindowWidth{this->findChild<QLineEdit*>("window_width")->text().trimmed()};
  auto lWindowHeight{this->findChild<QLineEdit*>("window_height")->text().trimmed()};
  auto lBodyNameSelected{this->findChild<QComboBox*>(QString("default_body_selector_name"))->currentIndex()};
  auto lBodyVersionSelected{this->findChild<QComboBox*>(QString("default_body_selector_version"))->currentIndex()};
  auto lDefaultBodyVersion{DataLists::getBodyNameVersion(static_cast<BodyName>(lBodyNameSelected), lBodyVersionSelected)};
  auto lUpBodyNameSelected{this->findChild<QComboBox*>(QString("upgrade_body_selector_name"))->currentIndex()};
  auto lUpBodyVersionSelected{this->findChild<QComboBox*>(QString("upgrade_body_selector_version"))->currentIndex()};
  auto lDefaultRetargetBodyVersion{DataLists::getBodyNameVersion(static_cast<BodyName>(lUpBodyNameSelected), lUpBodyVersionSelected)};
  auto lWindowOpeningMode{this->findChild<QComboBox*>("window_opening_mode")->currentIndex()};
  auto lMainWindowOutputPath{this->findChild<QLineEdit*>("output_path_directory")->text()};
  auto lAutoOpenGeneratedDir{this->findChild<QCheckBox*>("auto_open_generated_dir")->isChecked()};
  auto lCheckUpdateStartup{this->findChild<QCheckBox*>("check_update_startup")->isChecked()};
  auto lAssisConvScanOnlyMeshesDir{this->findChild<QCheckBox*>("assis_conv_only_scan_meshes_dir")->isChecked()};
  auto lEachButtonSavesItsLastUsedPath{this->findChild<QCheckBox*>("each_button_saves_last_path")->isChecked()};

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

  // Each button stores the last opened path
  lSettings.eachButtonSavesItsLastUsedPath = lEachButtonSavesItsLastUsedPath;

  // Colors
  lSettings.successColor = this->mNewSuccessColor;
  lSettings.warningColor = this->mNewWarningColor;
  lSettings.dangerColor = this->mNewDangerColor;

  return lSettings;
}

void Settings::applyFontButtonStyle(const QFont& aFont) const
{
  auto lFontChooserButton{this->findChild<QPushButton*>("font_chooser")};
  lFontChooserButton->setFont(aFont);
}

void Settings::applySuccessColorButton(const QString& aColor) const
{
  auto lColorChooser{this->findChild<QPushButton*>("success_color_chooser")};
  lColorChooser->setStyleSheet(QString("background-color: %1;").arg(aColor));
}

void Settings::applyWarningColorButton(const QString& aColor) const
{
  auto lColorChooser{this->findChild<QPushButton*>("warning_color_chooser")};
  lColorChooser->setStyleSheet(QString("background-color: %1;").arg(aColor));
}

void Settings::applyDangerColorButton(const QString& aColor) const
{
  auto lColorChooser{this->findChild<QPushButton*>("danger_color_chooser")};
  lColorChooser->setStyleSheet(QString("background-color: %1;").arg(aColor));
}

void Settings::saveSettings()
{
  auto lSettings{this->getSettingsFromGUI()};

  this->mMustRebootMainApp = (this->mSettings.language != lSettings.language
                              || this->mSettings.appTheme != lSettings.appTheme
                              || this->mSettings.font != lSettings.font
                              || this->mSettings.successColor != lSettings.successColor
                              || this->mSettings.warningColor != lSettings.warningColor
                              || this->mSettings.dangerColor != lSettings.dangerColor);

  if (mMustRebootMainApp)
  {
    // User theme accent
    const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

    if (Utils::displayQuestionMessage(this,
                                      tr("Application settings changed"),
                                      tr("All settings have been saved. You changed a setting that needs a restart of the application to be applied. Would you like to restart the application now?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Restart now"),
                                      tr("Go back to the application and restart later"),
                                      this->mSettings.dangerColor,
                                      this->mSettings.warningColor,
                                      false)
        == ButtonClicked::Yes)
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

void Settings::chooseSuccessColor()
{
  // Show a color piocker dialog to the user
  auto lSuccessColor{QColorDialog::getColor(QColor(this->mNewSuccessColor), this)};

  // Read and store new color's information
  this->mNewSuccessColor = lSuccessColor.name();

  // Display a preview on the color chooser's button directly
  this->applySuccessColorButton(this->mNewSuccessColor);
}

void Settings::chooseWarningColor()
{
  // Show a color piocker dialog to the user
  auto lWarningColor{QColorDialog::getColor(QColor(this->mNewWarningColor), this)};

  // Read and store new color's information
  this->mNewWarningColor = lWarningColor.name();

  // Display a preview on the color chooser's button directly
  this->applyWarningColorButton(this->mNewWarningColor);
}

void Settings::chooseDangerColor()
{
  // Show a color piocker dialog to the user
  auto lDangerColor{QColorDialog::getColor(QColor(this->mNewDangerColor), this)};

  // Read and store new color's information
  this->mNewDangerColor = lDangerColor.name();

  // Display a preview on the color chooser's button directly
  this->applyDangerColorButton(this->mNewDangerColor);
}

void Settings::preventGroupBoxCheckEvent(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (!aIsChecked)
  {
    lGroupBox->setChecked(true);
  }
}
