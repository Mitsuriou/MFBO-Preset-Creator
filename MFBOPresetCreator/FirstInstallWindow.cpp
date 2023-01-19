#include "FirstInstallWindow.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Struct.h"
#include "ThemeCard.h"
#include "Utils.h"
#include <QApplication>
#include <QGridLayout>
#include <QLibraryInfo>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>

FirstInstallWindow::FirstInstallWindow(QTranslator* aTranslator,
                                       QTranslator* aQtBaseTranslator,
                                       QWidget* aParent)
  : QMainWindow(aParent)
  , mTranslator{aTranslator}
  , mQtBaseTranslator{aQtBaseTranslator}
{
  this->initializeGUI();
  this->resize(1000, 620);
  this->show();
}

void FirstInstallWindow::closeEvent(QCloseEvent*)
{
  const auto lEventSource{qobject_cast<QPushButton*>(this->sender())};
  const auto lValidateButton{this->findChild<QPushButton*>("validate")};

  if (lEventSource == lValidateButton)
  {
    // Create a new settings object and save the chosen theme into it
    Struct::Settings lSettings;
    lSettings.display.applicationTheme = this->mChosenTheme;
    lSettings.display.language = this->mChosenLanguage;

    // Save the settings with the expected theme
    Utils::SaveSettingsToFile(lSettings);

    // Reboot the application
    qApp->exit(Utils::EXIT_CODE_REBOOT);
  }
  else
  {
    // Exit the app
    qApp->exit();
  }
}

void FirstInstallWindow::changeEvent(QEvent* event)
{
  if (event->type() == QEvent::LanguageChange)
  {
    this->retranslateUi();
  }

  QMainWindow::changeEvent(event);
}

void FirstInstallWindow::initializeGUI()
{
  // Central widget
  const auto lCentralWidget{new QWidget(this)};
  this->setCentralWidget(lCentralWidget);

  // Main layout
  const auto lMainLayout{new QGridLayout(lCentralWidget)};
  lMainLayout->setRowStretch(0, 1);
  lCentralWidget->setLayout(lMainLayout);

  // Create the central widget
  const auto lStackedWidget{new QStackedWidget(this)};
  lStackedWidget->setObjectName(QStringLiteral("stacked_widget"));
  lMainLayout->addWidget(lStackedWidget, 0, 0);

  // Stacked widget pages
  this->constructLanguagePage(*lStackedWidget);
  this->constructApplicationThemePage(*lStackedWidget);

  // Buttons bar
  this->setupButtons(*lMainLayout);

  // Text and translations
  QEvent* lEvent{new QEvent(QEvent::LanguageChange)};
  this->changeEvent(lEvent);
}

void FirstInstallWindow::constructLanguagePage(QStackedWidget& aParent)
{
  // Wrapper
  const auto lWrapper{new QWidget(&aParent)};
  aParent.addWidget(lWrapper);

  // Main layout
  const auto lLayout{new QVBoxLayout(this)};
  lLayout->setAlignment(Qt::AlignTop);
  lWrapper->setLayout(lLayout);

  // Main title
  const auto lMainTitle{new QLabel(this)};
  lMainTitle->setObjectName(QStringLiteral("main_title_page_1"));
  lMainTitle->setAlignment(Qt::AlignCenter);
  const auto lFontSize{10}; // Same as Struct::Settings.display.font.size
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(lFontSize * 2));
  lLayout->addWidget(lMainTitle);

  // Language selection
  const auto lLanguages{DataLists::GetLanguages()};
  for (int i = 0; i < lLanguages.size(); i++)
  {
    const auto lButton{new QRadioButton(lLanguages.at(i), this)};
    lButton->setAutoExclusive(true);
    lButton->setCursor(Qt::CursorShape::PointingHandCursor);
    lButton->setObjectName(QStringLiteral("language_button_%1").arg(QString::number(i)));
    lLayout->addWidget(lButton);

    // Event binding
    QObject::connect(lButton, &QRadioButton::clicked, this, [=]() {
      this->loadLanguage(i);
    });
  }

  lLayout->addStretch(1);

  // Current language
  const auto lCurrentLanguageLabel{new QLabel(this)};
  lCurrentLanguageLabel->setObjectName(QStringLiteral("current_language_label"));
  lCurrentLanguageLabel->setAlignment(Qt::AlignCenter);
  lLayout->addWidget(lCurrentLanguageLabel);

  // Apply the system language
  const auto lTargetButton{this->findChild<QRadioButton*>("language_button_" + QString::number(static_cast<int>(this->mChosenLanguage)))};
  if (lTargetButton != nullptr)
  {
    lTargetButton->click();
    lTargetButton->setFocus();
  }
}

void FirstInstallWindow::constructApplicationThemePage(QStackedWidget& aParent)
{
  // Wrapper
  const auto lWrapper{new QWidget(&aParent)};
  aParent.addWidget(lWrapper);

  // Main layout
  const auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setRowStretch(1, 1); // Make the scroll zone as high as possible
  lMainLayout->setAlignment(Qt::AlignTop);
  lWrapper->setLayout(lMainLayout);

  // Main title
  const auto lMainTitle{new QLabel(this)};
  lMainTitle->setObjectName(QStringLiteral("main_title_page_2"));
  lMainTitle->setAlignment(Qt::AlignCenter);
  const auto lFontSize{10}; // Same as Struct::Settings.display.font.size
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(lFontSize * 2));
  lMainLayout->addWidget(lMainTitle, 0, 0);

  // Scroll zone
  const auto lDataContainer{ComponentFactory::CreateScrollAreaComponentLayout(this, *lMainLayout, 1, 0)};

  // Theme cards
  const auto lThemes{DataLists::GetAppThemes()};
  for (int i = 0; i < lThemes.size(); i++)
  {
    const auto lThemeCard{new ThemeCard(this, lThemes.at(i), static_cast<int>(i))};
    lDataContainer->addWidget(lThemeCard, (i + 2) / 2, (i + 2) % 2);

    // Event binding
    QObject::connect(lThemeCard, &ThemeCard::askThemeChange, this, &FirstInstallWindow::loadQSSTheme);
  }

  // Current theme
  const auto lCurrentThemeLabel{new QLabel(this)};
  lCurrentThemeLabel->setObjectName(QStringLiteral("current_theme_label"));
  lCurrentThemeLabel->setAlignment(Qt::AlignCenter);
  lMainLayout->addWidget(lCurrentThemeLabel, 2, 0);

  // Apply the "Mitsuriou's black theme" stylesheet
  const auto lThemeIndex{static_cast<int>(mChosenTheme)};
  this->loadQSSTheme(DataLists::GetAppThemes().at(lThemeIndex), lThemeIndex);
}

void FirstInstallWindow::setupButtons(QGridLayout& aMainLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mChosenTheme)};

  // Wrapper layout
  const auto lWrapperLayout{new QHBoxLayout(this)};
  aMainLayout.addLayout(lWrapperLayout, 1, 0);

  // Previous step
  auto lGoToPreviousStep{this->findChild<QPushButton*>("previous_step")};

  if (lGoToPreviousStep != nullptr)
    delete lGoToPreviousStep;

  lGoToPreviousStep = ComponentFactory::CreateButton(this,
                                                     "",
                                                     "",
                                                     QStringLiteral("arrow-left"),
                                                     lIconFolder,
                                                     QStringLiteral("previous_step"),
                                                     true);
  lWrapperLayout->addWidget(lGoToPreviousStep);

  // Next step
  auto lGoToNextStep{this->findChild<QPushButton*>("next_step")};

  if (lGoToNextStep != nullptr)
    delete lGoToNextStep;

  lGoToNextStep = ComponentFactory::CreateButton(this,
                                                 "",
                                                 "",
                                                 QStringLiteral("arrow-right"),
                                                 lIconFolder,
                                                 QStringLiteral("next_step"));
  lWrapperLayout->addWidget(lGoToNextStep);

  // Validate
  auto lValidateButton{this->findChild<QPushButton*>("validate")};

  if (lValidateButton != nullptr)
    delete lValidateButton;

  lValidateButton = ComponentFactory::CreateButton(this,
                                                   "",
                                                   "",
                                                   QStringLiteral("done"),
                                                   lIconFolder,
                                                   QStringLiteral("validate"));
  lWrapperLayout->addWidget(lValidateButton);

  // Event binding
  QObject::connect(lGoToPreviousStep, &QPushButton::clicked, this, &FirstInstallWindow::goToPreviousStep);
  QObject::connect(lGoToNextStep, &QPushButton::clicked, this, &FirstInstallWindow::goToNextStep);
  QObject::connect(lValidateButton, &QPushButton::clicked, this, &FirstInstallWindow::close);

  // Update buttons state
  const auto lStackedWidget{this->findChild<QStackedWidget*>("stacked_widget")};
  this->goToStep(lStackedWidget->currentIndex());

  this->retranslateUi();
}

void FirstInstallWindow::swapTranslators(const QString& aLanguageToSet)
{
  // Remove the previous translators
  qApp->removeTranslator(this->mTranslator);       // Automatically deleted by Qt
  qApp->removeTranslator(this->mQtBaseTranslator); // Automatically deleted by Qt

  // Load the new translators
  const auto lTranslator{new QTranslator()};
  if (lTranslator->load(QString(":/translations/mfbopc_%1.qm").arg(aLanguageToSet)))
  {
    qApp->installTranslator(lTranslator);
  }

  // Apply default Qt language and translation
  const auto lQtBaseTranslator{new QTranslator()};
  if (lQtBaseTranslator->load(QString("qt_%1.qm").arg(aLanguageToSet), QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
  {
    qApp->installTranslator(lQtBaseTranslator);
  }
}

void FirstInstallWindow::goToPreviousStep()
{
  const auto lStackedWidget{this->findChild<QStackedWidget*>("stacked_widget")};
  if (lStackedWidget != nullptr)
  {
    this->goToStep(lStackedWidget->currentIndex() - 1);
  }
}

void FirstInstallWindow::goToNextStep()
{
  const auto lStackedWidget{this->findChild<QStackedWidget*>("stacked_widget")};
  if (lStackedWidget != nullptr)
  {
    this->goToStep(lStackedWidget->currentIndex() + 1);
  }
}

void FirstInstallWindow::goToStep(int aStep)
{
  const auto lStackedWidget{this->findChild<QStackedWidget*>("stacked_widget")};
  const auto lPreviousStepButton{this->findChild<QPushButton*>("previous_step")};
  const auto lNextStepButton{this->findChild<QPushButton*>("next_step")};
  const auto lValidateButton{this->findChild<QPushButton*>("validate")};

  if (lStackedWidget == nullptr || lPreviousStepButton == nullptr
      || lNextStepButton == nullptr || lValidateButton == nullptr)
    return;

  // Is it the very first step?
  if (aStep <= 0)
  {
    lPreviousStepButton->setEnabled(false);
    lNextStepButton->setEnabled(true);
    lValidateButton->setEnabled(false);
  }
  // Is it the very last step?
  else if (aStep >= lStackedWidget->count() - 1)
  {
    lPreviousStepButton->setEnabled(true);
    lNextStepButton->setEnabled(false);
    lValidateButton->setEnabled(true);
  }
  // Valid cases
  else
  {
    lPreviousStepButton->setEnabled(true);
    lNextStepButton->setEnabled(true);
    lValidateButton->setEnabled(false);
  }

  lStackedWidget->setCurrentIndex(aStep);
}

void FirstInstallWindow::loadLanguage(const int aLanguage)
{
  // Reset the language to English
  this->swapTranslators(Utils::GetShortLanguageNameFromEnum(ApplicationLanguage::ENGLISH));

  // Load the new language (if not English)
  if (static_cast<ApplicationLanguage>(aLanguage) != ApplicationLanguage::ENGLISH)
  {
    this->swapTranslators(Utils::GetShortLanguageNameFromEnum(aLanguage));
  }

  this->mChosenLanguage = static_cast<ApplicationLanguage>(aLanguage);
}

void FirstInstallWindow::retranslateUi()
{
  // Window title
  this->setWindowTitle(tr("First Installation Wizard"));

  //--------//
  // STEP 1 //
  //--------//

  // Step 1's main title
  const auto lMainTitle{this->findChild<QLabel*>("main_title_page_1")};
  if (lMainTitle != nullptr)
  {
    lMainTitle->setText(tr("Language"));
  }

  // Current language
  const auto lCurrentLanguageLabel{this->findChild<QLabel*>("current_language_label")};
  if (lCurrentLanguageLabel != nullptr)
  {
    lCurrentLanguageLabel->setText(tr("Current language: %1").arg(DataLists::GetLanguages().at(static_cast<int>(this->mChosenLanguage))));
  }

  //--------//
  // STEP 2 //
  //--------//

  // Step 2's main title
  const auto lMainTitlePage2{this->findChild<QLabel*>("main_title_page_2")};
  if (lMainTitlePage2 != nullptr)
  {
    lMainTitlePage2->setText(tr("Application theme"));
  }

  const auto lCurrentThemeLabel{this->findChild<QLabel*>("current_theme_label")};
  if (lCurrentThemeLabel != nullptr)
  {
    const auto lThemes{DataLists::GetAppThemes()};
    lCurrentThemeLabel->setText(tr("Current theme: %1").arg(lThemes.at(static_cast<int>(this->mChosenTheme))));
  }

  //-------------//
  // BUTTONS BAR //
  //-------------//

  // Previous step
  const auto lGoToPreviousStep{this->findChild<QPushButton*>("previous_step")};
  if (lGoToPreviousStep != nullptr)
  {
    lGoToPreviousStep->setText(tr("Previous step"));
  }

  // Next step
  const auto lGoToNextStep{this->findChild<QPushButton*>("next_step")};
  if (lGoToNextStep != nullptr)
  {
    lGoToNextStep->setText(tr("Next step"));
  }

  // Validate
  const auto lValidateButton{this->findChild<QPushButton*>("validate")};
  if (lValidateButton != nullptr)
  {
    lValidateButton->setText(tr("Validate and launch MFBO: Preset Creator"));
  }
}

void FirstInstallWindow::loadQSSTheme(const QString& aThemeName, const int aCardIndex)
{
  // Clear every single checked radio button
  const auto lRadioButtons{this->findChildren<QRadioButton*>(QRegularExpression("selector_*"))};
  for (const auto& lRadioButton : lRadioButtons)
  {
    lRadioButton->setChecked(false);
  }

  // Set the new checked radio button
  const auto lCheckedRadioButton{this->findChild<QRadioButton*>(QStringLiteral("selector_%1").arg(QString::number(aCardIndex)))};
  lCheckedRadioButton->setChecked(true);

  // Apply the new theme on the GUI
  Utils::ApplyApplicationStyleSheet(aThemeName);

  // Store the last loaded theme
  const auto lThemes{DataLists::GetAppThemes()};
  const auto lPreviousTheme{this->mChosenTheme};
  this->mChosenTheme = static_cast<GUITheme>(lThemes.indexOf(aThemeName));

  // Display the name of the current theme
  const auto lCurrentThemeLabel{this->findChild<QLabel*>("current_theme_label")};
  if (lCurrentThemeLabel != nullptr)
  {
    lCurrentThemeLabel->setText(tr("Current theme: %1").arg(aThemeName));
  }

  // Rebuild the buttons bar, since the icon color might need to be changed
  const auto lHasDarkModeChanged{Utils::IsThemeDark(lPreviousTheme) != Utils::IsThemeDark(this->mChosenTheme)};
  if (lHasDarkModeChanged)
  {
    const auto lMainLayout{qobject_cast<QGridLayout*>(this->centralWidget()->layout())};
    this->setupButtons(*lMainLayout);
  }
}
