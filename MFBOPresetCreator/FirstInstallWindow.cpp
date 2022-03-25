#include "FirstInstallWindow.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "ThemeCard.h"
#include "Utils.h"
#include <QPushButton>
#include <QWidget>

FirstInstallWindow::FirstInstallWindow(QWidget* aParent)
  : QMainWindow(aParent)
{
  this->initializeGUI();
  this->resize(620, 620);
  this->show();
}

void FirstInstallWindow::closeEvent(QCloseEvent*)
{
}

void FirstInstallWindow::initializeGUI()
{
  // Apply the Mitsuriou's black theme stylesheet
  this->loadQSSTheme("Mitsuriou's Black Theme");

  // Main widget and layout
  auto lCentralWidget{new QWidget(this)};
  auto lMainLayout{ComponentFactory::CreateScrollAreaWindowLayout(lCentralWidget)};
  this->setCentralWidget(lCentralWidget);
  lCentralWidget->setLayout(lMainLayout);

  // Theme cards
  const auto lThemes{DataLists::GetAppThemes()};
  for (int i = 0; i < lThemes.size(); i++)
  {
    auto lThemeCard{new ThemeCard(lThemes.at(i), this)};
    QObject::connect(lThemeCard, &ThemeCard::loadQSSTheme, this, &FirstInstallWindow::loadQSSTheme);
    lMainLayout->addWidget(lThemeCard, i / 2, i % 2);
  }

  // Buttons bar
  auto lButtonLayout{this->findChild<QHBoxLayout*>(QString("window_buttons_layout"))};
  this->setupButtons(*lButtonLayout);
}

void FirstInstallWindow::setupButtons(QHBoxLayout& aLayout)
{
  // User theme accent
  // const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Generate button
  auto lGenerateButton{ComponentFactory::CreateButton(this, tr("Generate the files on my computer"), "", "build", "" /*lIconFolder*/)};
  aLayout.addWidget(lGenerateButton);

  // Event binding
  // this->connect(lGenerateButton, &QPushButton::clicked, this, &PresetCreator::generateDirectoryStructure);
}

void FirstInstallWindow::loadQSSTheme(const QString& aThemeName)
{
  Utils::ApplyApplicationStyleSheet(aThemeName);
}
