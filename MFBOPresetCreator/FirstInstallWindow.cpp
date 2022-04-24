#include "FirstInstallWindow.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Struct.h"
#include "ThemeCard.h"
#include "Utils.h"
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>

FirstInstallWindow::FirstInstallWindow(QWidget* aParent)
  : QMainWindow(aParent)
{
  this->setWindowTitle(tr("First Installation Wizard"));
  this->initializeGUI();
  this->resize(620, 620);
  this->show();
}

void FirstInstallWindow::closeEvent(QCloseEvent*)
{
  auto lEventSource{qobject_cast<QPushButton*>(this->sender())};
  auto lValidateButton{this->findChild<QPushButton*>("validate")};

  if (lEventSource == lValidateButton)
  {
    // Create a new settings object and save the chosen theme into it
    Struct::Settings lSettings;
    lSettings.display.applicationTheme = this->mLastLoadedTheme;

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

void FirstInstallWindow::initializeGUI()
{
  // Create the central widget
  auto mainContainer{new QWidget(this)};
  this->setCentralWidget(mainContainer);

  // Main layout
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setRowStretch(1, 1); // Make the scroll zone as high as possible
  lMainLayout->setAlignment(Qt::AlignTop);
  mainContainer->setLayout(lMainLayout);

  // Main title
  auto lMainTitle{new QLabel(tr("Application theme"), this)};
  lMainTitle->setAlignment(Qt::AlignCenter);
  const auto lFontSize{10}; // Same as Struct::Settings.display.font.size
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(lFontSize * 2));
  lMainLayout->addWidget(lMainTitle, 0, 0);

  // Scroll zone
  auto lDataContainer{ComponentFactory::CreateScrollAreaComponentLayout(this, 1, 0)};

  // Theme cards
  const auto lThemes{DataLists::GetAppThemes()};
  for (int i = 0; i < lThemes.size(); i++)
  {
    auto lThemeCard{new ThemeCard(lThemes.at(i), this)};
    QObject::connect(lThemeCard, &ThemeCard::loadQSSTheme, this, &FirstInstallWindow::loadQSSTheme);
    lDataContainer->addWidget(lThemeCard, (i + 2) / 2, (i + 2) % 2);
  }

  // Current theme
  auto lCurrentThemeLabel{new QLabel(tr("Current theme: %1").arg("-"), this)};
  lCurrentThemeLabel->setObjectName("current_theme_label");
  lCurrentThemeLabel->setAlignment(Qt::AlignCenter);
  lMainLayout->addWidget(lCurrentThemeLabel, 2, 0);

  // Buttons bar
  this->setupButtons();

  // Apply the "Mitsuriou's black theme" stylesheet
  this->loadQSSTheme(lThemes.at(static_cast<int>(GUITheme::MITSURIOU_BLACK_THEME)));
}

void FirstInstallWindow::setupButtons()
{
  auto lMainLayout{qobject_cast<QGridLayout*>(this->centralWidget()->layout())};

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mLastLoadedTheme)};

  // Previous step
  auto lValidateButton{this->findChild<QPushButton*>("validate")};
  if (lValidateButton != nullptr)
  {
    delete lValidateButton;
    lValidateButton = nullptr;
  }

  lValidateButton = ComponentFactory::CreateButton(this, tr("Validate and launch MFBO: Preset Creator"), "", "done", lIconFolder, "validate");
  lMainLayout->addWidget(lValidateButton, 3, 0);

  // Event binding
  QObject::connect(lValidateButton, &QPushButton::clicked, this, &FirstInstallWindow::close);
}

void FirstInstallWindow::loadQSSTheme(const QString& aThemeName)
{
  Utils::ApplyApplicationStyleSheet(aThemeName);

  // Store the last loaded theme
  const auto lThemes{DataLists::GetAppThemes()};
  const auto lLastLoadedTheme = this->mLastLoadedTheme;
  this->mLastLoadedTheme = static_cast<GUITheme>(lThemes.indexOf(aThemeName));

  // Display the name of the current theme
  auto lCurrentThemeLabel{this->findChild<QLabel*>("current_theme_label")};
  lCurrentThemeLabel->setText(tr("Current theme: %1").arg(aThemeName));

  // Rebuild the buttons bar, since the icon color might need to be changed
  const auto lHasDarkModeChanged{Utils::IsThemeDark(lLastLoadedTheme) != Utils::IsThemeDark(this->mLastLoadedTheme)};
  if (lHasDarkModeChanged)
  {
    this->setupButtons();
  }
}
