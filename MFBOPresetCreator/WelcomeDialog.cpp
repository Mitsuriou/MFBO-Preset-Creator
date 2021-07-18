#include "WelcomeDialog.h"
#include <QCloseEvent>
#include <QIcon>
#include <QLabel>
#include <QVBoxLayout>

WelcomeDialog::WelcomeDialog(QWidget* aParent, const Struct::Settings& aSettings)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void WelcomeDialog::closeEvent(QCloseEvent* aEvent)
{
  aEvent->accept();
}

void WelcomeDialog::reject()
{
  this->close();
}

void WelcomeDialog::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Welcome screen"));
  this->setWindowIcon(QIcon(QPixmap(":/black/home")));
}

void WelcomeDialog::initializeGUI()
{
  // Main dialog layout
  auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  // Main title
  auto lMainTitle{new QLabel(tr("Welcome to MFBO: Preset Creator"), this)};
  lMainTitle->setAlignment(Qt::AlignCenter);
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(this->mSettings.font.size * 2));
  lMainLayout->addWidget(lMainTitle);
}
