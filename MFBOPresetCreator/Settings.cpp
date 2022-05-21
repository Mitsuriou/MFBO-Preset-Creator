#include "Settings.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "TargetMeshesPicker.h"
#include "Utils.h"
#include <QAbstractSlider>
#include <QCheckBox>
#include <QCloseEvent>
#include <QColorDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QStandardPaths>
#include <QStyledItemDelegate>

Settings::Settings(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, tr("Settings"), "cog", aSettings, aLastPaths, 1000, 700)
  , mNewFont(qApp->font())
{
  // Build the window's interface
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void Settings::closeEvent(QCloseEvent* aEvent)
{
  auto lEventButton{qobject_cast<QPushButton*>(this->sender())};
  auto lSaveButton{this->findChild<QPushButton*>(QString("save_close"))};

  if (lEventButton == lSaveButton)
  {
    aEvent->accept();

    if (this->mPathEntryCleared)
    {
      Utils::SaveLastPathsToFile(*this->lastPaths());
      emit refreshLastPaths(*this->lastPaths());
    }
    return;
  }

  if (this->getSettingsFromGUI() != this->settings() || this->mPathEntryCleared)
  {
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Closing"),
                                      tr("Do you want to close the window?"),
                                      this->getThemedResourcePath(),
                                      "help-circle",
                                      tr("Close the settings window without saving"),
                                      tr("Go back to the settings window"),
                                      "",
                                      this->settings().display.dangerColor,
                                      this->settings().display.successColor,
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

void Settings::initializeGUI()
{
  // Main layout with scroll area
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(this->getCentralWidget())};
  const auto lButtonsLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};

  auto lStarLabel{new QLabel(this)};
  if (Utils::RESTART_PENDING)
  {
    lStarLabel->setText(tr("Warning: A restart of the application is pending. You should not modify any value marked with the \"*\" character until you restart the application."));
    lStarLabel->setStyleSheet(QString("color: %1;").arg(this->settings().display.dangerColor));
  }
  else
  {
    lStarLabel->setText(tr("Note: Modifying a value marked with the \"*\" character will require a restart of the application to be applied correctly."));
    lStarLabel->setStyleSheet(QString("color: %1;").arg(this->settings().display.warningColor));
  }
  lMainLayout->addWidget(lStarLabel, 0, 0);

  // Tab widget
  auto lTabWidget{new QTabWidget(this)};
  lTabWidget->setObjectName(QString("tab_widget"));
  lTabWidget->setAutoFillBackground(true);
  lTabWidget->tabBar()->setCursor(Qt::CursorShape::PointingHandCursor);
  lMainLayout->addWidget(lTabWidget, 1, 0);

  this->setupDisplayTab(*lTabWidget);
  this->setupGeneralTab(*lTabWidget);
  this->setupPresetCreatorTab(*lTabWidget);
  this->setupBatchConversionToolTab(*lTabWidget);
  this->setupRetargetingToolTab(*lTabWidget);
  this->setupLastPathsTab(*lTabWidget);

  this->setupButtons(*lButtonsLayout);

  // Load the settings into the interface
  this->loadSettings(this->settings());
}

void Settings::setupDisplayTab(QTabWidget& aTabWidget)
{
  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setColumnStretch(0, 1);
  lTabLayout->setColumnStretch(1, 1);
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);

  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/monitor").arg(this->getThemedResourcePath()))), tr("Display"));

  //
  // First column
  //
  auto lLeftColumnLayout{new QVBoxLayout(this)};
  lLeftColumnLayout->setSpacing(10);
  lTabLayout->addLayout(lLeftColumnLayout, 0, 0);

  // LANGUAGE
  lLeftColumnLayout->addWidget(new QLabel(QString("* " + tr("Language:")), this));

  auto lLanguageSelector{new QComboBox(this)};
  lLanguageSelector->setItemDelegate(new QStyledItemDelegate());
  lLanguageSelector->setCursor(Qt::PointingHandCursor);
  lLanguageSelector->addItems(DataLists::GetLanguages());
  lLanguageSelector->setObjectName(QString("language"));
  lLeftColumnLayout->addWidget(lLanguageSelector);

  // GUI THEME
  lLeftColumnLayout->addWidget(new QLabel(QString("* " + tr("Application Theme:")), this));

  auto lGUIThemeSelector{new QComboBox(this)};
  lGUIThemeSelector->setItemDelegate(new QStyledItemDelegate());
  lGUIThemeSelector->setCursor(Qt::PointingHandCursor);
  lGUIThemeSelector->addItems(DataLists::GetAppThemes());
  lGUIThemeSelector->setObjectName(QString("app_theme"));
  lLeftColumnLayout->addWidget(lGUIThemeSelector);

  // FONT FAMILY
  lLeftColumnLayout->addWidget(new QLabel(QString(tr("Font:")), this));

  auto lFontChooser{ComponentFactory::CreateButton(this, tr("Change font properties"), "", "text", this->getThemedResourcePath(), "font_chooser", false, true)};
  lLeftColumnLayout->addWidget(lFontChooser);

  // WINDOW WIDTH
  lLeftColumnLayout->addWidget(new QLabel(tr("Default main window width:"), this));

  auto lWinWidthInput{new QLineEdit(this)};
  lWinWidthInput->setObjectName(QString("window_width"));
  lWinWidthInput->setValidator(new QIntValidator(0, 9999, this));
  lLeftColumnLayout->addWidget(lWinWidthInput);

  // WINDOW HEIGHT
  lLeftColumnLayout->addWidget(new QLabel(tr("Default main window height:"), this));

  auto lWinHeightInput{new QLineEdit(this)};
  lWinHeightInput->setObjectName(QString("window_height"));
  lWinHeightInput->setValidator(new QIntValidator(0, 9999, this));
  lLeftColumnLayout->addWidget(lWinHeightInput);

  // COLORS
  lLeftColumnLayout->addWidget(new QLabel(QString("* " + tr("Texts accent color:")), this));

  // Success
  auto lSuccessColorChooser{ComponentFactory::CreateButton(this,
                                                           tr("Choose a success color"),
                                                           "",
                                                           "color",
                                                           this->getThemedResourcePath(),
                                                           "success_color_chooser",
                                                           false,
                                                           true)};
  lLeftColumnLayout->addWidget(lSuccessColorChooser);

  // Warning
  auto lWarningColorChooser{ComponentFactory::CreateButton(this,
                                                           tr("Choose a warning color"),
                                                           "",
                                                           "color",
                                                           this->getThemedResourcePath(),
                                                           "warning_color_chooser",
                                                           false,
                                                           true)};
  lLeftColumnLayout->addWidget(lWarningColorChooser);

  // Danger
  auto lDangerColorChooser{ComponentFactory::CreateButton(this,
                                                          tr("Choose a danger color"),
                                                          "",
                                                          "color",
                                                          this->getThemedResourcePath(),
                                                          "danger_color_chooser",
                                                          false,
                                                          true)};
  lLeftColumnLayout->addWidget(lDangerColorChooser);

  lLeftColumnLayout->addStretch();

  //
  // Second column
  //
  auto lRightColumnLayout{new QVBoxLayout(this)};
  lRightColumnLayout->setSpacing(10);
  lTabLayout->addLayout(lRightColumnLayout, 0, 1);

  // Opening mode: main window
  lRightColumnLayout->addWidget(new QLabel(tr("Preset creator - opening mode:"), this));

  auto lWindowOpeningModeSelector{new QComboBox(this)};
  lWindowOpeningModeSelector->setItemDelegate(new QStyledItemDelegate());
  lWindowOpeningModeSelector->setCursor(Qt::PointingHandCursor);
  lWindowOpeningModeSelector->addItems(DataLists::GetWindowOpeningModes());
  lWindowOpeningModeSelector->setObjectName(QString("main_window_opening_mode"));
  lRightColumnLayout->addWidget(lWindowOpeningModeSelector);

  // Opening mode: batch conversion
  this->createDialogOpeningModeBlock(*lRightColumnLayout, tr("Batch conversion - opening mode:"), "batch_conversion_opening_mode");

  // Opening mode: batch conversion picker
  this->createDialogOpeningModeBlock(*lRightColumnLayout, tr("Batch conversion: results picker - opening mode:"), "batch_conversion_picker_opening_mode");

  // Opening mode: textures assistant
  this->createDialogOpeningModeBlock(*lRightColumnLayout, tr("Textures assistant - opening mode:"), "textures_assistant_opening_mode");

  // Opening mode: assisted conversion
  this->createDialogOpeningModeBlock(*lRightColumnLayout, tr("Assisted conversion - opening mode:"), "assisted_conversion_opening_mode");

  // Opening mode: bodySlide presets' retargeting
  this->createDialogOpeningModeBlock(*lRightColumnLayout, tr("BodySlide presets' retargeting - opening mode:"), "bodyslide_presets_retargeting_opening_mode");

  // Opening mode: slider sets database manager
  this->createDialogOpeningModeBlock(*lRightColumnLayout, tr("Slider Sets DB Manager - opening mode:"), "slider_sets_db_manager_opening_mode");

  // Opening mode: slider sets importer
  this->createDialogOpeningModeBlock(*lRightColumnLayout, tr("Slider Sets Importer - opening mode:"), "slider_sets_importer_opening_mode");

  lRightColumnLayout->addStretch();

  // Event binding
  QObject::connect(lFontChooser, &QPushButton::clicked, this, &Settings::chooseFont);
  QObject::connect(lSuccessColorChooser, &QPushButton::clicked, this, &Settings::chooseSuccessColor);
  QObject::connect(lWarningColorChooser, &QPushButton::clicked, this, &Settings::chooseWarningColor);
  QObject::connect(lDangerColorChooser, &QPushButton::clicked, this, &Settings::chooseDangerColor);
}

void Settings::setupGeneralTab(QTabWidget& aTabWidget)
{
  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setColumnStretch(0, 1);
  lTabLayout->setColumnStretch(1, 0);
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/tune").arg(this->getThemedResourcePath()))), tr("General"));

  // Show welcome screen at application startup
  lTabLayout->addWidget(new QLabel(tr("Startup actions:"), this), 0, 0, 1, 2);

  auto lWelcomeActionWelcomeScreen{new QRadioButton(tr("Open the welcome screen (pop-up window)"), this)};
  lWelcomeActionWelcomeScreen->setCursor(Qt::PointingHandCursor);
  lWelcomeActionWelcomeScreen->setObjectName(QString("welcome_action_welcome_screen"));
  lTabLayout->addWidget(lWelcomeActionWelcomeScreen, 1, 0, 1, 2);

  auto lWelcomeActionUpdater{new QRadioButton(tr("Check for updates only (pop-up window only if there is an update available)"), this)};
  lWelcomeActionUpdater->setCursor(Qt::PointingHandCursor);
  lWelcomeActionUpdater->setObjectName(QString("welcome_action_updater"));
  lTabLayout->addWidget(lWelcomeActionUpdater, 2, 0, 1, 2);

  auto lWelcomeActionNone{new QRadioButton(tr("Skip any check for updates and go directly to the main window"), this)};
  lWelcomeActionNone->setCursor(Qt::PointingHandCursor);
  lWelcomeActionNone->setObjectName(QString("welcome_action_none"));
  lTabLayout->addWidget(lWelcomeActionNone, 3, 0, 1, 2);

  // Each button stores the last opened path
  lTabLayout->addWidget(new QLabel(tr("Smarter buttons:"), this), 4, 0, 1, 2);

  auto lEachButtonSavesItsLastUsedPath{ComponentFactory::CreateCheckBox(this,
                                                                        tr("Each directory chooser button stores its own last opened path"),
                                                                        "",
                                                                        "each_button_saves_last_path")};
  lTabLayout->addWidget(lEachButtonSavesItsLastUsedPath, 5, 0);

  auto lCheckPathsHistory{ComponentFactory::CreateButton(this,
                                                         tr("Check/clear my browsing history"),
                                                         "",
                                                         "tab",
                                                         this->getThemedResourcePath(),
                                                         "",
                                                         false,
                                                         true)};
  lTabLayout->addWidget(lCheckPathsHistory, 5, 1);

  // Event binding
  QObject::connect(lCheckPathsHistory, &QPushButton::clicked, this, &Settings::goToLastPathsTab);
}

void Settings::setupPresetCreatorTab(QTabWidget& aTabWidget)
{
  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/home").arg(this->getThemedResourcePath()))), tr("Preset Creator"));

  // DEFAULT TARGETED BODY AND FEET VERSION
  auto lTargetMeshesPicker{ComponentFactory::CreateTargetMeshesPickerLine(this,
                                                                          *lTabLayout,
                                                                          false,
                                                                          0,
                                                                          this->getThemedResourcePath(),
                                                                          QString("main_target_meshes_picker_button"),
                                                                          QString("main_currently_targeted_body_feet"))};

  // AUTOMATICALLY OPEN THE GENERATED DIRECTORY
  lTabLayout->addWidget(new QLabel(tr("Post-generation task:"), this), 2, 0);

  auto lAutoOpenDirCheckbox{ComponentFactory::CreateCheckBox(this,
                                                             tr("Automatically open the generated preset's output directory after a generation"),
                                                             "",
                                                             "auto_open_generated_dir")};
  lTabLayout->addWidget(lAutoOpenDirCheckbox, 3, 0);

  // Event binding
  QObject::connect(lTargetMeshesPicker, &QPushButton::clicked, this, &Settings::openPresetCreatorTargetMeshesPicker);
}

void Settings::setupBatchConversionToolTab(QTabWidget& aTabWidget)
{
  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/reorder").arg(this->getThemedResourcePath()))), tr("Batch Conversion"));

  // DEFAULT TARGETED BODY AND FEET VERSION (BATCH CONVERSION)
  auto lTargetMeshesPicker{ComponentFactory::CreateTargetMeshesPickerLine(this,
                                                                          *lTabLayout,
                                                                          false,
                                                                          0,
                                                                          this->getThemedResourcePath(),
                                                                          QString("batch_conversion_target_meshes_picker_button"),
                                                                          QString("batch_conversion_currently_targeted_body_feet"))};

  // AUTOMATICALLY OPEN THE GENERATED DIRECTORY
  lTabLayout->addWidget(new QLabel(tr("Post-generation task:"), this), 2, 0);

  auto lAutoOpenRetargetedDirCheckbox{ComponentFactory::CreateCheckBox(this,
                                                                       tr("Automatically open the output directory after a batch generation"),
                                                                       "",
                                                                       "auto_open_batch_generated_dir")};
  lTabLayout->addWidget(lAutoOpenRetargetedDirCheckbox, 3, 0);

  // Event binding
  QObject::connect(lTargetMeshesPicker, &QPushButton::clicked, this, &Settings::openBatchConversionTargetMeshesPicker);
}

void Settings::setupRetargetingToolTab(QTabWidget& aTabWidget)
{
  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/arrow-up").arg(this->getThemedResourcePath()))), tr("BodySlide Presets' Retargeting"));

  // DEFAULT SELECTED BODY AND VERSION (RETARGETING TOOL)
  auto lTargetMeshesPicker{ComponentFactory::CreateTargetMeshesPickerLine(this,
                                                                          *lTabLayout,
                                                                          false,
                                                                          0,
                                                                          this->getThemedResourcePath(),
                                                                          QString("retargeting_tool_target_meshes_picker_button"),
                                                                          QString("retargeting_tool_currently_targeted_body_feet"))};

  // AUTOMATICALLY OPEN THE GENERATED DIRECTORY
  lTabLayout->addWidget(new QLabel(tr("Post-processing task:"), this), 2, 0);

  auto lAutoOpenRetargetedDirCheckbox{ComponentFactory::CreateCheckBox(this,
                                                                       tr("Automatically open the retargeted directory after the retargeting process succeeded"),
                                                                       "",
                                                                       "auto_open_retargeted_dir")};
  lTabLayout->addWidget(lAutoOpenRetargetedDirCheckbox, 3, 0);

  // Event binding
  QObject::connect(lTargetMeshesPicker, &QPushButton::clicked, this, &Settings::openRetargetingToolTargetMeshesPicker);
}

void Settings::setupLastPathsTab(QTabWidget& aTabWidget)
{
  // Tab widget
  auto lTabContent{new QWidget(this)};

  // Layout
  auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);
  lTabContent->setLayout(lTabLayout);

  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/folder").arg(this->getThemedResourcePath()))), tr("Last used paths"));

  // "Clear all" button
  auto lClearAllButton{ComponentFactory::CreateButton(this,
                                                      tr("Remove all the history"),
                                                      "",
                                                      "trash-lines",
                                                      this->getThemedResourcePath(),
                                                      "remove_all_filters",
                                                      false,
                                                      true)};
  lClearAllButton->setStyleSheet("text-align:left;");
  lTabLayout->addWidget(lClearAllButton, 0, 2);

  // Bind the "remove all" button
  QObject::connect(lClearAllButton, &QPushButton::clicked, this, &Settings::clearAllPaths);
  this->toggleClearAllButtonState();

  // Create a line for each path
  auto lRowIndex{1};
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("General"), this->lastPaths()->find("general")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Main window: output"), this->lastPaths()->find("mainWindowOutput")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Batch Conv.: input"), this->lastPaths()->find("batchConversionInput")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Batch Conv.: output"), this->lastPaths()->find("batchConversionOutput")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Assist. Conv.: input"), this->lastPaths()->find("assistedConversionInput")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Presets' Ret.: input"), this->lastPaths()->find("retargetingToolInput")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Presets' Ret.: output"), this->lastPaths()->find("retargetingToolOutput")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Textures Assist.: input"), this->lastPaths()->find("texturesAssistantInput")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Textures Assist.: output"), this->lastPaths()->find("texturesAssistantOutput")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Slider Sets Importer: input"), this->lastPaths()->find("sliderSetsImporterInput")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Last injected OSP file"), this->lastPaths()->find("lastInjectedOSPFile")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Loaded project"), this->lastPaths()->find("lastLoadedProject")->second, this->getThemedResourcePath(), QString("cross"));
  Utils::AddLastPathLine(this, lTabLayout, lRowIndex++, tr("Saved project"), this->lastPaths()->find("lastSavedProject")->second, this->getThemedResourcePath(), QString("cross"));

  // Bind every single "clear path" button
  const auto lButtons{this->findChildren<QPushButton*>(QRegularExpression("clear_path_*"))};
  for (const auto lButton : lButtons)
  {
    QObject::connect(lButton, &QPushButton::clicked, this, &Settings::clearPathButtonClicked);
  }
}

void Settings::setupButtons(QHBoxLayout& aLayout)
{
  // Create the buttons
  auto lRestoreDefaultButton{ComponentFactory::CreateButton(this, tr("Restore default"), "", "restore", this->getThemedResourcePath(), "", false, true)};
  aLayout.addWidget(lRestoreDefaultButton);

  auto lSaveButton{ComponentFactory::CreateButton(this, tr("Save and close"), "", "save", this->getThemedResourcePath(), "save_close", false, true)};
  aLayout.addWidget(lSaveButton);

  auto lCloseButton{ComponentFactory::CreateButton(this, tr("Cancel"), "", "undo", this->getThemedResourcePath(), "", false, true)};
  aLayout.addWidget(lCloseButton);

  // Event binding
  QObject::connect(lRestoreDefaultButton, &QPushButton::clicked, this, &Settings::restoreDefaultSettings);
  QObject::connect(lSaveButton, &QPushButton::clicked, this, &Settings::saveSettings);
  QObject::connect(lCloseButton, &QPushButton::clicked, this, &Settings::close);
}

void Settings::createDialogOpeningModeBlock(QVBoxLayout& aLayout, const QString& aLabelTitle, const QString& aObjectName)
{
  aLayout.addWidget(new QLabel(aLabelTitle, this));

  auto lSelector{new QComboBox(this)};
  lSelector->setItemDelegate(new QStyledItemDelegate());
  lSelector->setCursor(Qt::PointingHandCursor);
  lSelector->addItems(DataLists::GetDialogOpeningModes());
  lSelector->setObjectName(QString(aObjectName));
  aLayout.addWidget(lSelector);
}

void Settings::loadSettings(const Struct::Settings& aSettingsToLoad)
{
  // Display
  this->loadDisplayTabSettings(aSettingsToLoad.display);

  // General
  this->loadGeneralTabSettings(aSettingsToLoad.general);

  // TODO: Optimize the calls below, with the wanted function as a parameter of loadGenericDialogSettings()

  // Preset Creator
  this->presetCreatorTargetMeshesChanged(aSettingsToLoad.presetCreator.defaultBodyFeet.bodyMesh, aSettingsToLoad.presetCreator.defaultBodyFeet.feetMesh);
  this->loadGenericDialogSettings(aSettingsToLoad.presetCreator, "auto_open_generated_dir");

  // Batch conversion
  this->batchConversionTargetMeshesChanged(aSettingsToLoad.batchConversion.defaultBodyFeet.bodyMesh, aSettingsToLoad.batchConversion.defaultBodyFeet.feetMesh);
  this->loadGenericDialogSettings(aSettingsToLoad.batchConversion, "auto_open_batch_generated_dir");

  // BodySlide Presets' Retargeting
  this->retargetingToolTargetMeshesChanged(aSettingsToLoad.presetsRetargeting.defaultBodyFeet.bodyMesh, aSettingsToLoad.presetsRetargeting.defaultBodyFeet.feetMesh);
  this->loadGenericDialogSettings(aSettingsToLoad.presetsRetargeting, "auto_open_retargeted_dir");
}

void Settings::loadDisplayTabSettings(const Struct::DisplaySettings& aSettingsToLoad)
{
  // Language
  auto lLanguage{this->findChild<QComboBox*>(QString("language"))};
  lLanguage->setCurrentIndex(static_cast<int>(aSettingsToLoad.language));

  // Application theme
  auto lApplicationTheme{this->findChild<QComboBox*>(QString("app_theme"))};
  lApplicationTheme->setCurrentIndex(static_cast<int>(aSettingsToLoad.applicationTheme));

  // Font
  QFont lFont(aSettingsToLoad.font.family,
              aSettingsToLoad.font.pointSize,
              aSettingsToLoad.font.weight,
              aSettingsToLoad.font.italic);
  lFont.setStyleName(aSettingsToLoad.font.styleName);
  lFont.setUnderline(aSettingsToLoad.font.underline);
  lFont.setStrikeOut(aSettingsToLoad.font.strikeOut);
  lFont.setStyleStrategy(QFont::PreferAntialias);
  this->mNewFont = lFont;
  this->applyFontButtonStyle(this->mNewFont);

  // Window width
  auto lWindowWidth{this->findChild<QLineEdit*>(QString("window_width"))};
  lWindowWidth->setText(QString::number(aSettingsToLoad.mainWindowWidth));

  // Window height
  auto lWindowHeight{this->findChild<QLineEdit*>(QString("window_height"))};
  lWindowHeight->setText(QString::number(aSettingsToLoad.mainWindowHeight));

  // Colors
  this->mNewSuccessColor = aSettingsToLoad.successColor;
  this->applySuccessColorButton(this->mNewSuccessColor);

  this->mNewWarningColor = aSettingsToLoad.warningColor;
  this->applyWarningColorButton(this->mNewWarningColor);

  this->mNewDangerColor = aSettingsToLoad.dangerColor;
  this->applyDangerColorButton(this->mNewDangerColor);

  // Windows' default opening modes
  auto lWindowOpeningMode{this->findChild<QComboBox*>(QString("main_window_opening_mode"))};
  lWindowOpeningMode->setCurrentIndex(static_cast<int>(aSettingsToLoad.mainWindowOpeningMode));

  auto lBatchConversionDialogOpeningMode{this->findChild<QComboBox*>(QString("batch_conversion_opening_mode"))};
  lBatchConversionDialogOpeningMode->setCurrentIndex(static_cast<int>(aSettingsToLoad.batchConversionDialogOpeningMode));

  auto lBatchConversionPickerDialogOpeningMode{this->findChild<QComboBox*>(QString("batch_conversion_picker_opening_mode"))};
  lBatchConversionPickerDialogOpeningMode->setCurrentIndex(static_cast<int>(aSettingsToLoad.batchConversionPickerDialogOpeningMode));

  auto lTexturesAssistantDialogOpeningMode{this->findChild<QComboBox*>(QString("textures_assistant_opening_mode"))};
  lTexturesAssistantDialogOpeningMode->setCurrentIndex(static_cast<int>(aSettingsToLoad.texturesAssistantDialogOpeningMode));

  auto lAssistedConversionDialogOpeningMode{this->findChild<QComboBox*>(QString("assisted_conversion_opening_mode"))};
  lAssistedConversionDialogOpeningMode->setCurrentIndex(static_cast<int>(aSettingsToLoad.assistedConversionDialogOpeningMode));

  auto lBodySlidePresetsRetargetingDialogOpeningMode{this->findChild<QComboBox*>(QString("bodyslide_presets_retargeting_opening_mode"))};
  lBodySlidePresetsRetargetingDialogOpeningMode->setCurrentIndex(static_cast<int>(aSettingsToLoad.bodySlidePresetsRetargetingDialogOpeningMode));

  auto lSliderSetsDBManagerDialogOpeningMode{this->findChild<QComboBox*>(QString("slider_sets_db_manager_opening_mode"))};
  lSliderSetsDBManagerDialogOpeningMode->setCurrentIndex(static_cast<int>(aSettingsToLoad.sliderSetsDBManagerDialogOpeningMode));

  auto lSliderSetsImporterDialogOpeningMode{this->findChild<QComboBox*>(QString("slider_sets_importer_opening_mode"))};
  lSliderSetsImporterDialogOpeningMode->setCurrentIndex(static_cast<int>(aSettingsToLoad.sliderSetsImporterDialogOpeningMode));
}

void Settings::loadGeneralTabSettings(const Struct::GeneralSettings& aSettingsToLoad)
{
  auto lEachButtonSavesItsLastUsedPath{this->findChild<QCheckBox*>(QString("each_button_saves_last_path"))};
  lEachButtonSavesItsLastUsedPath->setChecked(aSettingsToLoad.eachButtonSavesItsLastUsedPath);

  // Security in case the switch below does not get any verified condition
  this->findChild<QRadioButton*>(QString("welcome_action_welcome_screen"))->setChecked(true);

  switch (aSettingsToLoad.startupAction)
  {
    case StartupAction::OPEN_WELCOME_SCREEN:
      this->findChild<QRadioButton*>(QString("welcome_action_welcome_screen"))->setChecked(true);
      break;
    case StartupAction::CHECK_FOR_UPDATES:
      this->findChild<QRadioButton*>(QString("welcome_action_updater"))->setChecked(true);
      break;
    case StartupAction::SKIP_UPDATE_CHECKS:
      this->findChild<QRadioButton*>(QString("welcome_action_none"))->setChecked(true);
      break;
  }
}

void Settings::loadGenericDialogSettings(const Struct::GenericDialogSettings& aSettingsToLoad, const QString& aObjectNamePostActionTask)
{
  auto lPostGenTask{this->findChild<QCheckBox*>(aObjectNamePostActionTask)};
  lPostGenTask->setChecked(aSettingsToLoad.automaticallyOpenFinalDirectory);
}

Struct::Settings Settings::getSettingsFromGUI() const
{
  auto lLang{this->findChild<QComboBox*>(QString("language"))->currentIndex()};
  auto lAppTheme{this->findChild<QComboBox*>(QString("app_theme"))->currentIndex()};
  auto lWindowWidth{this->findChild<QLineEdit*>(QString("window_width"))->text().trimmed()};
  auto lWindowHeight{this->findChild<QLineEdit*>(QString("window_height"))->text().trimmed()};
  auto lWindowOpeningMode{this->findChild<QComboBox*>(QString("main_window_opening_mode"))->currentIndex()};
  auto lBatchConversionDialogOpeningMode{this->findChild<QComboBox*>(QString("batch_conversion_opening_mode"))->currentIndex()};
  auto lBatchConversionPickerDialogOpeningMode{this->findChild<QComboBox*>(QString("batch_conversion_picker_opening_mode"))->currentIndex()};
  auto lTexturesAssistantDialogOpeningMode{this->findChild<QComboBox*>(QString("textures_assistant_opening_mode"))->currentIndex()};
  auto lAssistedConversionDialogOpeningMode{this->findChild<QComboBox*>(QString("assisted_conversion_opening_mode"))->currentIndex()};
  auto lBodySlidePresetsRetargetingDialogOpeningMode{this->findChild<QComboBox*>(QString("bodyslide_presets_retargeting_opening_mode"))->currentIndex()};
  auto lSliderSetsDBManagerDialogOpeningMode{this->findChild<QComboBox*>(QString("slider_sets_db_manager_opening_mode"))->currentIndex()};
  auto lSliderSetsImporterDialogOpeningMode{this->findChild<QComboBox*>(QString("slider_sets_importer_opening_mode"))->currentIndex()};
  auto lWelcomeActionWelcomeScreen{this->findChild<QRadioButton*>(QString("welcome_action_welcome_screen"))->isChecked()};
  auto lWelcomeActionUpdater{this->findChild<QRadioButton*>(QString("welcome_action_updater"))->isChecked()};
  auto lWelcomeActionNone{this->findChild<QRadioButton*>(QString("welcome_action_none"))->isChecked()};
  auto lAutoOpenGeneratedDir{this->findChild<QCheckBox*>(QString("auto_open_generated_dir"))->isChecked()};
  auto lAutoOpenRetargetedDir{this->findChild<QCheckBox*>(QString("auto_open_retargeted_dir"))->isChecked()};
  auto lEachButtonSavesItsLastUsedPath{this->findChild<QCheckBox*>(QString("each_button_saves_last_path"))->isChecked()};

  Struct::Settings lSettings;

  // Language
  auto lEnumLang{static_cast<ApplicationLanguage>(lLang)};
  switch (lEnumLang)
  {
    case ApplicationLanguage::ENGLISH:
      lSettings.display.language = ApplicationLanguage::ENGLISH;
      break;
    case ApplicationLanguage::FRENCH:
      lSettings.display.language = ApplicationLanguage::FRENCH;
      break;
    case ApplicationLanguage::CHINESE_TRADITIONAL:
      lSettings.display.language = ApplicationLanguage::CHINESE_TRADITIONAL;
      break;
  }

  // Font family
  lSettings.display.font.family = this->mNewFont.family();
  lSettings.display.font.italic = this->mNewFont.italic();
  lSettings.display.font.pointSize = this->mNewFont.pointSize();
  lSettings.display.font.strikeOut = this->mNewFont.strikeOut();
  lSettings.display.font.styleName = this->mNewFont.styleName();
  lSettings.display.font.underline = this->mNewFont.underline();
  lSettings.display.font.weight = this->mNewFont.weight();

  // Application theme
  lSettings.display.applicationTheme = static_cast<GUITheme>(lAppTheme);

  // Window width
  if (lWindowWidth.size() > 0)
  {
    lSettings.display.mainWindowWidth = lWindowWidth.toInt();
  }

  // Window height
  if (lWindowHeight.size() > 0)
  {
    lSettings.display.mainWindowHeight = lWindowHeight.toInt();
  }

  // Opening mode: main window
  lSettings.display.mainWindowOpeningMode = static_cast<WindowOpeningMode>(lWindowOpeningMode);

  // Opening mode: batch conversion
  lSettings.display.batchConversionDialogOpeningMode = static_cast<DialogOpeningMode>(lBatchConversionDialogOpeningMode);

  // Opening mode: batch conversion picker
  lSettings.display.batchConversionPickerDialogOpeningMode = static_cast<DialogOpeningMode>(lBatchConversionPickerDialogOpeningMode);

  // Opening mode: textures assistant
  lSettings.display.texturesAssistantDialogOpeningMode = static_cast<DialogOpeningMode>(lTexturesAssistantDialogOpeningMode);

  // Opening mode: assisted conversion
  lSettings.display.assistedConversionDialogOpeningMode = static_cast<DialogOpeningMode>(lAssistedConversionDialogOpeningMode);

  // Opening mode: bodySlide presets' retargeting
  lSettings.display.bodySlidePresetsRetargetingDialogOpeningMode = static_cast<DialogOpeningMode>(lBodySlidePresetsRetargetingDialogOpeningMode);

  // Opening mode: slider sets database manager
  lSettings.display.sliderSetsDBManagerDialogOpeningMode = static_cast<DialogOpeningMode>(lSliderSetsDBManagerDialogOpeningMode);

  // Opening mode: slider sets importer
  lSettings.display.sliderSetsImporterDialogOpeningMode = static_cast<DialogOpeningMode>(lSliderSetsImporterDialogOpeningMode);

  // Default selected body and version (main)
  lSettings.presetCreator.defaultBodyFeet.bodyMesh = this->mNewPresetCreatorTargetBodyMesh;

  // Default selected body and version (Batch conversion)
  lSettings.batchConversion.defaultBodyFeet.bodyMesh = this->mNewBatchConversionTargetBodyMesh;

  // Default selected body and version (Retargeting tool)
  lSettings.presetsRetargeting.defaultBodyFeet.bodyMesh = this->mNewRetargetingToolTargetBodyMesh;

  // Default selected feet mod (main)
  lSettings.presetCreator.defaultBodyFeet.feetMesh = this->mNewPresetCreatorTargetFeetMesh;

  // Default selected feet mod (Batch conversion)
  lSettings.batchConversion.defaultBodyFeet.feetMesh = this->mNewBatchConversionTargetFeetMesh;

  // Default selected feet mod (Retargeting tool)
  lSettings.presetsRetargeting.defaultBodyFeet.feetMesh = this->mNewRetargetingToolTargetFeetMesh;

  // Show welcome screen at application startup
  if (lWelcomeActionWelcomeScreen)
  {
    lSettings.general.startupAction = StartupAction::OPEN_WELCOME_SCREEN;
  }
  else if (lWelcomeActionUpdater)
  {
    lSettings.general.startupAction = StartupAction::CHECK_FOR_UPDATES;
  }
  else if (lWelcomeActionNone)
  {
    lSettings.general.startupAction = StartupAction::SKIP_UPDATE_CHECKS;
  }

  // Automatically open generated directory
  lSettings.presetCreator.automaticallyOpenFinalDirectory = lAutoOpenGeneratedDir;

  // Automatically open retargeted directory
  lSettings.presetsRetargeting.automaticallyOpenFinalDirectory = lAutoOpenRetargetedDir;

  // Each button stores the last opened path
  lSettings.general.eachButtonSavesItsLastUsedPath = lEachButtonSavesItsLastUsedPath;

  // Colors
  lSettings.display.successColor = this->mNewSuccessColor;
  lSettings.display.warningColor = this->mNewWarningColor;
  lSettings.display.dangerColor = this->mNewDangerColor;

  return lSettings;
}

void Settings::applyFontButtonStyle(const QFont& aFont) const
{
  auto lFontChooserButton{this->findChild<QPushButton*>(QString("font_chooser"))};
  lFontChooserButton->setFont(aFont);
}

void Settings::applySuccessColorButton(const QString& aColor) const
{
  auto lColorChooser{this->findChild<QPushButton*>(QString("success_color_chooser"))};
  lColorChooser->setStyleSheet(QString("background-color: %1;").arg(aColor));
}

void Settings::applyWarningColorButton(const QString& aColor) const
{
  auto lColorChooser{this->findChild<QPushButton*>(QString("warning_color_chooser"))};
  lColorChooser->setStyleSheet(QString("background-color: %1;").arg(aColor));
}

void Settings::applyDangerColorButton(const QString& aColor) const
{
  auto lColorChooser{this->findChild<QPushButton*>(QString("danger_color_chooser"))};
  lColorChooser->setStyleSheet(QString("background-color: %1;").arg(aColor));
}

void Settings::saveSettings()
{
  auto lSettings{this->getSettingsFromGUI()};

  // Check if a restart of the app is required
  this->mMustRebootMainApp = (this->settings().display.language != lSettings.display.language
                              || this->settings().display.applicationTheme != lSettings.display.applicationTheme
                              || this->settings().display.successColor != lSettings.display.successColor
                              || this->settings().display.warningColor != lSettings.display.warningColor
                              || this->settings().display.dangerColor != lSettings.display.dangerColor);

  if (this->mMustRebootMainApp)
  {
    if (Utils::DisplayQuestionMessage(this,
                                      tr("Application settings changed"),
                                      tr("All settings have been saved. You changed a setting that needs a restart of the application to be applied.\n\nWould you like to restart the application now (you will lose all unsaved data)?"),
                                      this->getThemedResourcePath(),
                                      "help-circle",
                                      tr("Restart now"),
                                      tr("Go back to the application and restart later"),
                                      "",
                                      this->settings().display.dangerColor,
                                      this->settings().display.warningColor,
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

  Utils::SaveSettingsToFile(lSettings);

  emit refreshMainUI(lSettings, true);

  this->close();
}

void Settings::restoreDefaultSettings()
{
  // Create a default settings object and load it into the GUI
  Struct::Settings lSettings;
  this->loadSettings(lSettings);
}

void Settings::openPresetCreatorTargetMeshesPicker()
{
  auto lDialog{new TargetMeshesPicker(this,
                                      this->settings(),
                                      this->lastPaths(),
                                      this->settings().presetCreator.defaultBodyFeet.bodyMesh,
                                      this->settings().presetCreator.defaultBodyFeet.feetMesh)};
  QObject::connect(lDialog, &TargetMeshesPicker::valuesChosen, this, &Settings::presetCreatorTargetMeshesChanged);
}

void Settings::openBatchConversionTargetMeshesPicker()
{
  auto lDialog{new TargetMeshesPicker(this,
                                      this->settings(),
                                      this->lastPaths(),
                                      this->settings().batchConversion.defaultBodyFeet.bodyMesh,
                                      this->settings().batchConversion.defaultBodyFeet.feetMesh)};
  QObject::connect(lDialog, &TargetMeshesPicker::valuesChosen, this, &Settings::batchConversionTargetMeshesChanged);
}

void Settings::openRetargetingToolTargetMeshesPicker()
{
  auto lDialog{new TargetMeshesPicker(this,
                                      this->settings(),
                                      this->lastPaths(),
                                      this->settings().presetsRetargeting.defaultBodyFeet.bodyMesh,
                                      this->settings().presetsRetargeting.defaultBodyFeet.feetMesh)};
  QObject::connect(lDialog, &TargetMeshesPicker::valuesChosen, this, &Settings::retargetingToolTargetMeshesChanged);
}

void Settings::presetCreatorTargetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet)
{
  this->targetMeshesChanged(this->mNewPresetCreatorTargetBodyMesh, this->mNewPresetCreatorTargetFeetMesh, aBody, aFeet, QString("main"));
}

void Settings::batchConversionTargetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet)
{
  this->targetMeshesChanged(this->mNewBatchConversionTargetBodyMesh, this->mNewBatchConversionTargetFeetMesh, aBody, aFeet, QString("batch_conversion"));
}

void Settings::retargetingToolTargetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet)
{
  this->targetMeshesChanged(this->mNewRetargetingToolTargetBodyMesh, this->mNewRetargetingToolTargetFeetMesh, aBody, aFeet, QString("retargeting_tool"));
}

void Settings::targetMeshesChanged(BodyNameVersion& aBodyToUpdate,
                                   FeetNameVersion& aFeetToUpdate,
                                   const BodyNameVersion& aBody,
                                   const FeetNameVersion& aFeet,
                                   const QString& aObjectNamePrefix)
{
  // Update the class members
  aBodyToUpdate = aBody;
  aFeetToUpdate = aFeet;

  // Update the "targeted body mesh" text content
  const auto lBodyText{
    QString("%1 [v.%2]").arg(DataLists::GetBodyVariantsList(DataLists::GetName(aBody), DataLists::GetVersionIndex(aBody)).at(DataLists::GetVariantIndex(aBody)), DataLists::GetVersionString(aBody))};

  // Update the "targeted feet mesh" text content
  const auto lFeetText{
    QString("%1 [v.%2]").arg(DataLists::GetFeetVariantsList(DataLists::GetName(aFeet), DataLists::GetVersionIndex(aFeet), Utils::IsCBBEBasedBody(aBody)).at(DataLists::GetVariantIndex(aFeet)), DataLists::GetVersionString(aBody, aFeet))};

  auto lCurrentlyTargetedBody{this->findChild<QLabel*>(QString("%1_currently_targeted_body_feet").arg(aObjectNamePrefix))};
  lCurrentlyTargetedBody->setText(tr("Targeted body: %1\nTargeted feet: %2").arg(lBodyText, lFeetText));
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

void Settings::goToLastPathsTab()
{
  this->findChild<QTabWidget*>(QString("tab_widget"))->setCurrentIndex(5);
}

void Settings::clearPathButtonClicked()
{
  auto lButton{qobject_cast<QPushButton*>(this->sender())};
  if (lButton == nullptr)
    return;

  const auto lRowIndex{lButton->objectName().remove(QString("clear_path_"), Qt::CaseSensitivity::CaseSensitive).toInt()};
  const auto lKey{DataLists::GetLastPathsKeys().at(lRowIndex - 1)};

  // If the path is already empty, return instantly
  if (this->lastPaths()->find(lKey)->second.compare("", Qt::CaseInsensitive) == 0)
  {
    return;
  }

  if (Utils::UpdatePathAtKey(this->lastPaths(), lKey, QString(), false, true, false))
  {
    this->mPathEntryCleared = true;

    // Update the path display in the corresponding QLineEdit
    auto lPathLineEdit{this->findChild<QLineEdit*>(QString("line_edit_path_%1").arg(lRowIndex))};
    lPathLineEdit->setText(this->lastPaths()->find(lKey)->second);
    lPathLineEdit->setFocus();

    // Disable the corresponding button
    lButton->setDisabled(true);

    this->toggleClearAllButtonState();
  }
}

void Settings::clearAllPaths()
{
  auto lButton{qobject_cast<QPushButton*>(this->sender())};
  if (lButton == nullptr)
    return;

  const auto lKeys{DataLists::GetLastPathsKeys()};
  for (int i = 0; i < lKeys.size(); i++)
  {
    if (Utils::UpdatePathAtKey(this->lastPaths(), lKeys.at(i), QString(), false, true, false))
    {
      this->mPathEntryCleared = true;

      // Update the path display in the corresponding QLineEdit
      this->findChild<QLineEdit*>(QString("line_edit_path_%1").arg(i + 1))->setText(this->lastPaths()->find(lKeys.at(i))->second);

      // Disable the corresponding button
      this->findChild<QPushButton*>(QString("clear_path_%1").arg(i + 1))->setDisabled(true);
    }
  }

  // Disable the "remove all" button
  lButton->setDisabled(true);

  // Change the focus
  this->findChild<QTabWidget*>(QString("tab_widget"))->setFocus();
}

void Settings::toggleClearAllButtonState()
{
  // If there is as least one path to clear, keep the "remove all" button active
  auto lKeepEnabledRemoveAllButton{false};
  for (const auto& lPair : *this->lastPaths())
  {
    if (!lPair.second.isEmpty())
    {
      lKeepEnabledRemoveAllButton = true;
      break;
    }
  }

  this->findChild<QPushButton*>(QString("remove_all_filters"))->setDisabled(!lKeepEnabledRemoveAllButton);
}
