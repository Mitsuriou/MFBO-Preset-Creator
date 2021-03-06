#include "Settings.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Utils.h"
#include <QAbstractSlider>
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QColorDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStyledItemDelegate>

Settings::Settings(QWidget* aParent, const Struct::Settings& aSettings, const std::map<QString, QString>& aLastPaths)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mMustRebootMainApp{false}
  , mNewFont{qApp->font()}
  , mPathEntryCleared(false)
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
  auto lEventButton{qobject_cast<QPushButton*>(sender())};
  auto lSaveButton{this->findChild<QPushButton*>("save_close")};

  if (lEventButton == lSaveButton)
  {
    aEvent->accept();

    if (this->mPathEntryCleared)
    {
      Utils::saveLastPathsToFile(this->mLastPaths);
      emit refreshLastPaths(this->mLastPaths);
    }
    return;
  }

  if (this->getSettingsFromGUI() != this->mSettings || this->mPathEntryCleared)
  {
    // User theme accent
    const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

    if (Utils::displayQuestionMessage(this,
                                      tr("Closing"),
                                      tr("Do you want to close the window?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Close the settings window without saving"),
                                      tr("Go back to the settings window"),
                                      "",
                                      this->mSettings.dangerColor,
                                      this->mSettings.successColor,
                                      "",
                                      false)
        != ButtonClicked::YES)
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
  this->setMinimumWidth(950);
  this->setMinimumHeight(670);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Settings"));
  this->setWindowIcon(QIcon(QPixmap(":/black/cog")));
}

void Settings::initializeGUI()
{
  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::createScrollAreaWindowLayout(this)};
  auto lButtonLayout{this->findChild<QHBoxLayout*>("window_buttons_layout")};

  auto lStarLabel{new QLabel(this)};
  if (Utils::RESTART_PENDING)
  {
    lStarLabel->setText(tr("Warning: A restart of the application is pending. You should not modify any value marked with the \"*\" character until you restart the application."));
    lStarLabel->setStyleSheet(QString("color: %1;").arg(this->mSettings.dangerColor));
  }
  else
  {
    lStarLabel->setText(tr("Note: Modifying a value marked with the \"*\" character will require a restart of the application to be applied correctly."));
    lStarLabel->setStyleSheet(QString("color: %1;").arg(this->mSettings.warningColor));
  }
  lMainLayout->addWidget(lStarLabel, 0, 0);

  // Tab widget
  auto lTabWidget{new QTabWidget(this)};
  lTabWidget->setAutoFillBackground(true);
  lTabWidget->tabBar()->setCursor(Qt::CursorShape::PointingHandCursor);
  lMainLayout->addWidget(lTabWidget, 1, 0);

  this->setupDisplayTab(*lTabWidget);
  this->setupGeneralTab(*lTabWidget);
  this->setupPresetCreatorTab(*lTabWidget);
  this->setupRetargetingToolTab(*lTabWidget);
  this->setupAssistedConversionTab(*lTabWidget);
  this->setupLastPathsTab(*lTabWidget);

  this->setupButtons(*lButtonLayout);

  // Load the settings into the interface
  this->loadSettings(this->mSettings);

  // Cursor change for the scroll bar
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderPressed, this, &Settings::scrollbarPressed);
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderReleased, this, &Settings::scrollbarReleased);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderPressed, this, &Settings::scrollbarPressed);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderReleased, this, &Settings::scrollbarReleased);
}

void Settings::setupDisplayTab(QTabWidget& aTabWidget)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QVBoxLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setContentsMargins(15, 20, 15, 15);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/monitor").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation)), tr("Display"));

  // LANGUAGE
  lTabLayout->addWidget(new QLabel(QString("* " + tr("Language:")), this));

  auto lLanguageSelector{new QComboBox(this)};
  lLanguageSelector->setItemDelegate(new QStyledItemDelegate());
  lLanguageSelector->setCursor(Qt::PointingHandCursor);
  lLanguageSelector->addItems(DataLists::getLanguages());
  lLanguageSelector->setObjectName(QString("language"));
  lTabLayout->addWidget(lLanguageSelector);

  // FONT FAMILY
  lTabLayout->addWidget(new QLabel(QString("* " + tr("Font:")), this));

  auto lFontChooser{ComponentFactory::createButton(this, tr("Choose a font"), "", "text", lIconFolder, "font_chooser", false, true)};
  lTabLayout->addWidget(lFontChooser);

  // GUI THEME
  lTabLayout->addWidget(new QLabel(QString("* " + tr("Application Theme:")), this));

  auto lGUIThemeSelector{new QComboBox(this)};
  lGUIThemeSelector->setItemDelegate(new QStyledItemDelegate());
  lGUIThemeSelector->setCursor(Qt::PointingHandCursor);
  lGUIThemeSelector->addItems(DataLists::getAppThemes());
  lGUIThemeSelector->setObjectName(QString("app_theme"));
  lTabLayout->addWidget(lGUIThemeSelector);

  // MAIN WINDOW OPENING MODE
  lTabLayout->addWidget(new QLabel(tr("Window opening mode:"), this));

  QStringList lSupportedWindowOpeningMode;
  lSupportedWindowOpeningMode.append(tr("English"));
  lSupportedWindowOpeningMode.append(tr("Français"));

  auto lWindowOpeningModeSelector{new QComboBox(this)};
  lWindowOpeningModeSelector->setItemDelegate(new QStyledItemDelegate());
  lWindowOpeningModeSelector->setCursor(Qt::PointingHandCursor);
  lWindowOpeningModeSelector->addItems(DataLists::getWindowOpeningModes());
  lWindowOpeningModeSelector->setObjectName(QString("window_opening_mode"));
  lTabLayout->addWidget(lWindowOpeningModeSelector);

  // WINDOW WIDTH
  lTabLayout->addWidget(new QLabel(tr("Default main window width:"), this));

  auto lWinWidthInput{new QLineEdit("", this)};
  lWinWidthInput->setObjectName(QString("window_width"));
  lWinWidthInput->setValidator(new QIntValidator(0, 9999, this));
  lTabLayout->addWidget(lWinWidthInput);

  // WINDOW HEIGHT
  lTabLayout->addWidget(new QLabel(tr("Default main window height:"), this));

  auto lWinHeightInput{new QLineEdit("", this)};
  lWinHeightInput->setObjectName(QString("window_height"));
  lWinHeightInput->setValidator(new QIntValidator(0, 9999, this));
  lTabLayout->addWidget(lWinHeightInput);

  // COLORS
  lTabLayout->addWidget(new QLabel(QString("* " + tr("Texts accent color:")), this));

  // Success
  auto lSuccessColorChooser{ComponentFactory::createButton(this, tr("Choose a success color"), "", "color", lIconFolder, "success_color_chooser", false, true)};
  lTabLayout->addWidget(lSuccessColorChooser);

  // Warning
  auto lWarningColorChooser{ComponentFactory::createButton(this, tr("Choose a warning color"), "", "color", lIconFolder, "warning_color_chooser", false, true)};
  lTabLayout->addWidget(lWarningColorChooser);

  // Danger
  auto lDangerColorChooser{ComponentFactory::createButton(this, tr("Choose a danger color"), "", "color", lIconFolder, "danger_color_chooser", false, true)};
  lTabLayout->addWidget(lDangerColorChooser);

  // Event binding
  this->connect(lFontChooser, &QPushButton::clicked, this, &Settings::chooseFont);
  this->connect(lSuccessColorChooser, &QPushButton::clicked, this, &Settings::chooseSuccessColor);
  this->connect(lWarningColorChooser, &QPushButton::clicked, this, &Settings::chooseWarningColor);
  this->connect(lDangerColorChooser, &QPushButton::clicked, this, &Settings::chooseDangerColor);
}

void Settings::setupGeneralTab(QTabWidget& aTabWidget)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QVBoxLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setContentsMargins(15, 20, 15, 15);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/tune").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation)), tr("General"));

  // CHECK UPDATE AT APPLICATION STARTUP
  auto lAutoOpenDirCheckbox{new QCheckBox(tr("Check for updates at application startup"), this)};
  lAutoOpenDirCheckbox->setCursor(Qt::PointingHandCursor);
  lAutoOpenDirCheckbox->setObjectName(QString("check_update_startup"));
  lTabLayout->addWidget(lAutoOpenDirCheckbox);

  // Each button stores the last opened path
  auto lEachButtonSavesItsLastUsedPath{new QCheckBox(tr("Each directory chooser button stores its own last opened path"), this)};
  lEachButtonSavesItsLastUsedPath->setCursor(Qt::PointingHandCursor);
  lEachButtonSavesItsLastUsedPath->setObjectName(QString("each_button_saves_last_path"));
  lTabLayout->addWidget(lEachButtonSavesItsLastUsedPath);
}

void Settings::setupPresetCreatorTab(QTabWidget& aTabWidget)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setContentsMargins(15, 20, 15, 15);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/home").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation)), tr("Preset Creator"));

  // DEFAULT SELECTED BODY AND VERSION
  auto lDefaultBodyVersionSettings{DataLists::getSplittedNameVersionFromBodyVersion(mSettings.defaultRetargetingToolBody)};

  lTabLayout->addWidget(new QLabel(tr("Default selected body:"), this), 0, 0, 1, 2);

  auto lBodyNameSelector{new QComboBox(this)};
  lBodyNameSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyNameSelector->setCursor(Qt::PointingHandCursor);
  lBodyNameSelector->addItems(DataLists::getBodiesNames());
  lBodyNameSelector->setCurrentIndex(lDefaultBodyVersionSettings.first);
  lBodyNameSelector->setObjectName(QString("default_body_selector_name"));
  lTabLayout->addWidget(lBodyNameSelector, 1, 0);

  auto lBodyVersionSelector{new QComboBox(this)};
  lBodyVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lBodyVersionSelector->setCursor(Qt::PointingHandCursor);
  lBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lBodyVersionSelector->setCurrentIndex(lDefaultBodyVersionSettings.second);
  lBodyVersionSelector->setObjectName(QString("default_body_selector_version"));
  lTabLayout->addWidget(lBodyVersionSelector, 1, 1);

  // Feet mod
  lTabLayout->addWidget(new QLabel(tr("Default selected feet mod:"), this), 2, 0, 1, 2);

  auto lFeetSelector{new QComboBox(this)};
  lFeetSelector->setItemDelegate(new QStyledItemDelegate());
  lFeetSelector->setCursor(Qt::PointingHandCursor);
  lFeetSelector->addItems(DataLists::getFeetModsEntries());
  lFeetSelector->setCurrentIndex(mSettings.defaultMainFeetMod);
  lFeetSelector->setObjectName(QString("feet_mod_main"));
  lTabLayout->addWidget(lFeetSelector, 3, 0, 1, 2);

  // OUTPUT PATH PREVIEW
  lTabLayout->addWidget(new QLabel(tr("Output directory path:"), this), 4, 0, 1, 2);

  auto lOutputPathLineEdit{new QLineEdit("", this)};
  lOutputPathLineEdit->setReadOnly(true);
  lOutputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lOutputPathLineEdit->setObjectName("output_path_directory");
  lTabLayout->addWidget(lOutputPathLineEdit, 5, 0);

  // OUTPUT PATH CHOOSER
  auto lOutputPathChooser{ComponentFactory::createButton(this, tr("Choose a directory..."), "", "folder", lIconFolder, "", false, true)};
  lTabLayout->addWidget(lOutputPathChooser, 5, 1);

  // AUTOMATICALLY OPEN THE GENERATED DIRECTORY
  auto lAutoOpenDirCheckbox{new QCheckBox(tr("Automatically open the generated preset's output directory after a generation"), this)};
  lAutoOpenDirCheckbox->setCursor(Qt::PointingHandCursor);
  lAutoOpenDirCheckbox->setObjectName(QString("auto_open_generated_dir"));
  lTabLayout->addWidget(lAutoOpenDirCheckbox, 6, 0, 1, 2);

  // Event binding
  this->connect(lOutputPathChooser, &QPushButton::clicked, this, &Settings::chooseExportDirectory);
  this->connect(lBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &Settings::updateAvailableBodyVersions);
}

void Settings::setupRetargetingToolTab(QTabWidget& aTabWidget)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setContentsMargins(15, 20, 15, 15);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/arrow-up").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation)), tr("BodySlide Presets' Retargeting"));

  // DEFAULT SELECTED BODY AND VERSION (RETARGETING TOOL)
  auto lDefaultBodyVersionSettings{DataLists::getSplittedNameVersionFromBodyVersion(mSettings.defaultRetargetingToolBody)};

  lTabLayout->addWidget(new QLabel(tr("Default selected body:"), this), 0, 0, 1, 2);

  auto lUpgradeBodyNameSelector{new QComboBox(this)};
  lUpgradeBodyNameSelector->setItemDelegate(new QStyledItemDelegate());
  lUpgradeBodyNameSelector->setCursor(Qt::PointingHandCursor);
  lUpgradeBodyNameSelector->addItems(DataLists::getBodiesNames());
  lUpgradeBodyNameSelector->setCurrentIndex(lDefaultBodyVersionSettings.first);
  lUpgradeBodyNameSelector->setObjectName(QString("upgrade_body_selector_name"));
  lTabLayout->addWidget(lUpgradeBodyNameSelector, 1, 0);

  auto lUpgradeBodyVersionSelector{new QComboBox(this)};
  lUpgradeBodyVersionSelector->setItemDelegate(new QStyledItemDelegate());
  lUpgradeBodyVersionSelector->setCursor(Qt::PointingHandCursor);
  lUpgradeBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(static_cast<BodyName>(lDefaultBodyVersionSettings.first)));
  lUpgradeBodyVersionSelector->setCurrentIndex(lDefaultBodyVersionSettings.second);
  lUpgradeBodyVersionSelector->setObjectName(QString("upgrade_body_selector_version"));
  lTabLayout->addWidget(lUpgradeBodyVersionSelector, 1, 1);

  // Feet mod
  lTabLayout->addWidget(new QLabel(tr("Default selected feet mod:"), this), 2, 0, 1, 2);

  auto lFeetSelector{new QComboBox(this)};
  lFeetSelector->setItemDelegate(new QStyledItemDelegate());
  lFeetSelector->setCursor(Qt::PointingHandCursor);
  lFeetSelector->addItems(DataLists::getFeetModsEntries());
  lFeetSelector->setCurrentIndex(mSettings.defaultRetargetingToolFeetMod);
  lFeetSelector->setObjectName(QString("feet_mod_retargeting_tool"));
  lTabLayout->addWidget(lFeetSelector, 3, 0, 1, 2);

  // Event binding
  this->connect(lUpgradeBodyNameSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &Settings::updateAvailableUpgradeBodyVersions);
}

void Settings::setupAssistedConversionTab(QTabWidget& aTabWidget)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QVBoxLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setContentsMargins(15, 20, 15, 15);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/pencil").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation)), tr("Assisted Conversion"));

  // ONLY SCAN THE MESHES SUBDIRECTORY
  auto lScanOnlyMeshesFolder{new QCheckBox(tr("Only scan the \"meshes\" subdirectory"), this)};
  lScanOnlyMeshesFolder->setCursor(Qt::PointingHandCursor);
  lScanOnlyMeshesFolder->setObjectName(QString("assis_conv_only_scan_meshes_dir"));
  lTabLayout->addWidget(lScanOnlyMeshesFolder);
}

void Settings::setupLastPathsTab(QTabWidget& aTabWidget)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setContentsMargins(15, 20, 15, 15);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/folder").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation)), tr("Last used paths"));

  // "Clear all" button
  auto lClearAllButton{ComponentFactory::createButton(this, tr("Remove all the history"), "", "trash-lines", lIconFolder, "remove_all_filters", false, true)};
  lClearAllButton->setStyleSheet("text-align:left;");
  lTabLayout->addWidget(lClearAllButton, 0, 2);

  // Create a line for each path
  auto lRowIndex{1};
  Utils::addLastPathLine(this, lTabLayout, lRowIndex++, tr("General"), this->mLastPaths.find("general")->second, lIconFolder, QString("cross"));
  Utils::addLastPathLine(this, lTabLayout, lRowIndex++, tr("Main window: output"), this->mLastPaths.find("mainWindowOutput")->second, lIconFolder, QString("cross"));
  Utils::addLastPathLine(this, lTabLayout, lRowIndex++, tr("Batch Conv.: input"), this->mLastPaths.find("batchConversionInput")->second, lIconFolder, QString("cross"));
  Utils::addLastPathLine(this, lTabLayout, lRowIndex++, tr("Batch Conv.: output"), this->mLastPaths.find("batchConversionOutput")->second, lIconFolder, QString("cross"));
  Utils::addLastPathLine(this, lTabLayout, lRowIndex++, tr("Assist. Conv.: input"), this->mLastPaths.find("assistedConversionInput")->second, lIconFolder, QString("cross"));
  Utils::addLastPathLine(this, lTabLayout, lRowIndex++, tr("Presets' Ret.: input"), this->mLastPaths.find("retargetingToolInput")->second, lIconFolder, QString("cross"));
  Utils::addLastPathLine(this, lTabLayout, lRowIndex++, tr("Presets' Ret.: output"), this->mLastPaths.find("retargetingToolOutput")->second, lIconFolder, QString("cross"));
  Utils::addLastPathLine(this, lTabLayout, lRowIndex++, tr("Textures Assist.: input"), this->mLastPaths.find("texturesAssistantInput")->second, lIconFolder, QString("cross"));
  Utils::addLastPathLine(this, lTabLayout, lRowIndex++, tr("Loaded project"), this->mLastPaths.find("lastLoadedProject")->second, lIconFolder, QString("cross"));
  Utils::addLastPathLine(this, lTabLayout, lRowIndex, tr("Saved project"), this->mLastPaths.find("lastSavedProject")->second, lIconFolder, QString("cross"));

  // Bind all the clear buttons
  this->connect(lClearAllButton, &QPushButton::clicked, this, &Settings::clearAllPaths);

  const auto lButtons{this->findChildren<QPushButton*>(QRegularExpression("clear_path_*"))};
  for (const auto lButton : lButtons)
  {
    this->connect(lButton, &QPushButton::clicked, this, &Settings::clearPathButtonClicked);
  }
}

void Settings::setupButtons(QHBoxLayout& aLayout)
{
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

  // Create the buttons
  auto lRestoreDefaultButton{ComponentFactory::createButton(this, tr("Restore default"), "", "restore", lIconFolder, "", false, true)};
  aLayout.addWidget(lRestoreDefaultButton);

  auto lSaveButton{ComponentFactory::createButton(this, tr("Save and close"), "", "save", lIconFolder, "save_close", false, true)};
  aLayout.addWidget(lSaveButton);

  auto lCloseButton{ComponentFactory::createButton(this, tr("Cancel"), "", "undo", lIconFolder, "", false, true)};
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

  auto lDefaultBody{DataLists::getSplittedNameVersionFromBodyVersion(aSettingsToLoad.defaultMainWindowBody)};
  this->findChild<QComboBox*>(QString("default_body_selector_name"))->setCurrentIndex(lDefaultBody.first);
  this->findChild<QComboBox*>(QString("default_body_selector_version"))->setCurrentIndex(lDefaultBody.second);

  auto lDefaultUpgradeBody{DataLists::getSplittedNameVersionFromBodyVersion(aSettingsToLoad.defaultRetargetingToolBody)};
  this->findChild<QComboBox*>(QString("upgrade_body_selector_name"))->setCurrentIndex(lDefaultUpgradeBody.first);
  this->findChild<QComboBox*>(QString("upgrade_body_selector_version"))->setCurrentIndex(lDefaultUpgradeBody.second);

  auto lDefaultMainFeetMod{this->findChild<QComboBox*>("feet_mod_main")};
  lDefaultMainFeetMod->setCurrentIndex(static_cast<int>(aSettingsToLoad.defaultMainFeetMod));

  auto lDefaultRetargetingToolFeetMod{this->findChild<QComboBox*>("feet_mod_retargeting_tool")};
  lDefaultRetargetingToolFeetMod->setCurrentIndex(static_cast<int>(aSettingsToLoad.defaultRetargetingToolFeetMod));

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
  auto lDefaultMainFeetMod{this->findChild<QComboBox*>(QString("feet_mod_main"))->currentIndex()};
  auto lDefaultRetargetingToolFeetMod{this->findChild<QComboBox*>(QString("feet_mod_retargeting_tool"))->currentIndex()};
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
    case ApplicationLanguage::ENGLISH:
      lSettings.language = ApplicationLanguage::ENGLISH;
      break;
    case ApplicationLanguage::FRENCH:
      lSettings.language = ApplicationLanguage::FRENCH;
      break;
    case ApplicationLanguage::CHINESE_TRADITIONAL:
      lSettings.language = ApplicationLanguage::CHINESE_TRADITIONAL;
      break;
    default:
      lSettings.language = ApplicationLanguage::ENGLISH;
      break;
  }

  // Font family
  lSettings.font.family = this->mNewFont.family();
  lSettings.font.italic = this->mNewFont.italic();
  lSettings.font.size = this->mNewFont.pointSize();
  lSettings.font.strikeOut = this->mNewFont.strikeOut();
  lSettings.font.styleName = this->mNewFont.styleName();
  lSettings.font.underline = this->mNewFont.underline();
  lSettings.font.weight = this->mNewFont.weight();

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

  // Default selected feet mod (main)
  lSettings.defaultMainFeetMod = lDefaultMainFeetMod;

  // Default selected feet mod (Retargeting tool)
  lSettings.defaultRetargetingToolFeetMod = lDefaultRetargetingToolFeetMod;

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

  // TODO: Add the settings GUI for Batch Conversion

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
    const auto& lIconFolder{Utils::getIconRessourceFolder(this->mSettings.appTheme)};

    if (Utils::displayQuestionMessage(this,
                                      tr("Application settings changed"),
                                      tr("All settings have been saved. You changed a setting that needs a restart of the application to be applied.\n\nWould you like to restart the application now (you will lose all unsaved data)?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Restart now"),
                                      tr("Go back to the application and restart later"),
                                      "",
                                      this->mSettings.dangerColor,
                                      this->mSettings.warningColor,
                                      "",
                                      false)
        == ButtonClicked::YES)
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

void Settings::updateAvailableBodyVersions()
{
  auto lBodyName{static_cast<BodyName>(this->findChild<QComboBox*>(QString("default_body_selector_name"))->currentIndex())};
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("default_body_selector_version"))};
  lBodyVersionSelector->clear();
  lBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(lBodyName));
  lBodyVersionSelector->setCurrentIndex(0);
}

void Settings::updateAvailableUpgradeBodyVersions()
{
  auto lBodyName{static_cast<BodyName>(this->findChild<QComboBox*>(QString("upgrade_body_selector_name"))->currentIndex())};
  auto lBodyVersionSelector{this->findChild<QComboBox*>(QString("upgrade_body_selector_version"))};
  lBodyVersionSelector->clear();
  lBodyVersionSelector->addItems(DataLists::getVersionsFromBodyName(lBodyName));
  lBodyVersionSelector->setCurrentIndex(0);
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

  // The QColor is only valid if the user did not click on "cancel"
  if (lSuccessColor.isValid())
  {
    // Read and store new color's information
    this->mNewSuccessColor = lSuccessColor.name();

    // Display a preview on the color chooser's button directly
    this->applySuccessColorButton(this->mNewSuccessColor);
  }
}

void Settings::chooseWarningColor()
{
  // Show a color piocker dialog to the user
  auto lWarningColor{QColorDialog::getColor(QColor(this->mNewWarningColor), this)};

  // The QColor is only valid if the user did not click on "cancel"
  if (lWarningColor.isValid())
  {
    // Read and store new color's information
    this->mNewWarningColor = lWarningColor.name();

    // Display a preview on the color chooser's button directly
    this->applyWarningColorButton(this->mNewWarningColor);
  }
}

void Settings::chooseDangerColor()
{
  // Show a color piocker dialog to the user
  auto lDangerColor{QColorDialog::getColor(QColor(this->mNewDangerColor), this)};

  // The QColor is only valid if the user did not click on "cancel"
  if (lDangerColor.isValid())
  {
    // Read and store new color's information
    this->mNewDangerColor = lDangerColor.name();

    // Display a preview on the color chooser's button directly
    this->applyDangerColorButton(this->mNewDangerColor);
  }
}

void Settings::scrollbarPressed()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::ClosedHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::ClosedHandCursor);
}

void Settings::scrollbarReleased()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
}

void Settings::clearPathButtonClicked()
{
  auto lButton{qobject_cast<QPushButton*>(this->sender())};
  if (lButton == nullptr)
    return;

  auto lRowIndex{lButton->objectName().remove(QString("clear_path_"), Qt::CaseSensitivity::CaseSensitive).toInt()};
  auto lKey{DataLists::getLastPathsKeys().at(lRowIndex - 1)};

  // If the path is already empty, return instantly
  if (this->mLastPaths.find(lKey)->second.compare("", Qt::CaseInsensitive) == 0)
  {
    return;
  }

  Utils::updatePathAtKey(&this->mLastPaths, lKey, QString(), true, false);

  this->findChild<QLineEdit*>(QString("line_edit_path_%1").arg(lRowIndex))->setText(this->mLastPaths.find(lKey)->second);

  this->mPathEntryCleared = true;
}

void Settings::clearAllPaths()
{
  auto lKeys{DataLists::getLastPathsKeys()};
  auto lSize{lKeys.length()};
  for (int i = 0; i < lSize; i++)
  {
    auto lKey{lKeys.at(i)};
    Utils::updatePathAtKey(&this->mLastPaths, lKey, QString(), true, false);

    this->findChild<QLineEdit*>(QString("line_edit_path_%1").arg(i + 1))->setText(this->mLastPaths.find(lKey)->second);
  }

  this->mPathEntryCleared = true;
}
