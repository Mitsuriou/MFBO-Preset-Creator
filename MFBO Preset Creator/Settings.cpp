#include "Settings.h"

Settings::Settings(QWidget* parent)
  : QDialog(parent)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->show();
}

void Settings::closeEvent(QCloseEvent* aEvent)
{
  auto lResult{ QMessageBox::question(this, tr("Closing"),
    tr("Are you sure you want to close the Settings window?"),
    QMessageBox::Yes | QMessageBox::No, QMessageBox::No) };

  if (lResult != QMessageBox::Yes)
  {
    aEvent->ignore();
  }
  else
  {
    aEvent->accept();
  }
}

void Settings::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setMinimumWidth(650);
  this->setWindowTitle(tr("Settings"));
}

void Settings::initializeGUI()
{
  // Main window container
  auto lMainVertical{ new QGridLayout(this) };

  this->setupInterface(*lMainVertical);
  this->loadSettings();
}

void Settings::setupInterface(QGridLayout& aLayout)
{
  // First line
  auto lInputPathLabel{ new QLabel(tr("Language:")) };
  aLayout.addWidget(lInputPathLabel, 0, 0);

  auto lInputPathLineEdit{ new QLineEdit("") };
  aLayout.addWidget(lInputPathLineEdit, 0, 1);

  // Window buttons
  auto lSaveButton{ new QPushButton(tr("Save")) };
  aLayout.addWidget(lSaveButton, 1, 0, Qt::AlignBottom);

  auto lCloseButton{ new QPushButton(tr("Close")) };
  aLayout.addWidget(lSaveButton, 1, 0, 1, 3, Qt::AlignBottom);

  // Event binding
  connect(lSaveButton, SIGNAL(clicked()), this, SLOT(applySettings()));
  connect(lCloseButton, SIGNAL(clicked()), this, SLOT(close()));
}

void Settings::loadSettings()
{
  // TODO
}

void Settings::applySettings()
{
  // TODO
}
