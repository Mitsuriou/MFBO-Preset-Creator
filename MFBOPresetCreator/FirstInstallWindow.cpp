#include "FirstInstallWindow.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "ThemeCard.h"
#include "Utils.h"
#include <QApplication>
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
  // Main widget and layout
  auto lCentralWidget{new QWidget(this)};
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(lCentralWidget)};
  this->setCentralWidget(lCentralWidget);
  lCentralWidget->setLayout(lMainLayout);

  /*============*/
  /* Main title */
  /*============*/
  auto lMainTitle{new QLabel(tr("Application theme"), this)};
  lMainTitle->setAlignment(Qt::AlignCenter);
  const auto lFontSize{10}; // Same as Struct::Settings.display.font.size
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(lFontSize * 2));
  lMainLayout->addWidget(lMainTitle, 0, 0, 1, 2);

  // Theme cards
  const auto lThemes{DataLists::GetAppThemes()};
  for (int i = 0; i < lThemes.size(); i++)
  {
    auto lThemeCard{new ThemeCard(lThemes.at(i), this)};
    QObject::connect(lThemeCard, &ThemeCard::loadQSSTheme, this, &FirstInstallWindow::loadQSSTheme);
    lMainLayout->addWidget(lThemeCard, (i + 2) / 2, (i + 2) % 2);
  }

  // Buttons bar
  this->setupButtons();

  // Apply the Mitsuriou's black theme stylesheet
  this->loadQSSTheme("Mitsuriou's Black Theme");
}

void FirstInstallWindow::setupButtons()
{
  auto lButtonLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};

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
  lButtonLayout->addWidget(lValidateButton);

  // Event binding
  this->connect(lValidateButton, &QPushButton::clicked, this, &FirstInstallWindow::close);
}

void FirstInstallWindow::loadQSSTheme(const QString& aThemeName)
{
  Utils::ApplyApplicationStyleSheet(aThemeName);

  // Store the last loaded theme
  const auto lThemes{DataLists::GetAppThemes()};
  const auto lLastLoadedTheme = this->mLastLoadedTheme;
  this->mLastLoadedTheme = static_cast<GUITheme>(lThemes.indexOf(aThemeName));

  // Rebuild the buttons bar, since the icon color might need to be changed
  const auto lHasDarkModeChanged{Utils::IsThemeDark(lLastLoadedTheme) != Utils::IsThemeDark(this->mLastLoadedTheme)};
  if (lHasDarkModeChanged)
  {
    this->setupButtons();
  }
}
