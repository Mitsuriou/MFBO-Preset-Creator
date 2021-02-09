#include "TexturesAssistant.h"

TexturesAssistant::TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
  , mHasUserDoneSomething(false)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void TexturesAssistant::closeEvent(QCloseEvent* aEvent)
{
  // Catch the sender of the event
  auto lEventSource{qobject_cast<QPushButton*>(sender())};
  auto lValidationBtn{this->findChild<QPushButton*>("validate_selection")};

  if ((lEventSource && lValidationBtn) || !this->mHasUserDoneSomething)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  QMessageBox lBox(QMessageBox::Icon::Question, tr("Closing"), tr("Do you want to close the window?"), QMessageBox::StandardButton::NoButton, this);
  lBox.setIconPixmap(QPixmap(QString(":/%1/help-circle").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

  auto lCloseBtn{lBox.addButton(tr("Close the window"), QMessageBox::ButtonRole::YesRole)};
  lCloseBtn->setCursor(Qt::PointingHandCursor);
  lCloseBtn->setStyleSheet(QString("color: %1;").arg(this->mSettings.dangerColor));

  auto lStayBtn{lBox.addButton(tr("Go back to the textures assistant tool window"), QMessageBox::ButtonRole::NoRole)};
  lStayBtn->setCursor(Qt::PointingHandCursor);
  lStayBtn->setStyleSheet(QString("color: %1;").arg(this->mSettings.successColor));

  lBox.setDefaultButton(lStayBtn);
  lBox.exec();

  if (lBox.clickedButton() != lCloseBtn)
  {
    aEvent->ignore();
  }
  else
  {
    aEvent->accept();
  }
}

void TexturesAssistant::reject()
{
  this->close();
}

void TexturesAssistant::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Textures Assistant"));
  this->setWindowIcon(QIcon(QPixmap(":/black/textures")));
}

void TexturesAssistant::initializeGUI()
{
  // Main window layout
  auto lMainGrid{new QGridLayout(this)};
  lMainGrid->setAlignment(Qt::AlignTop);
  this->setLayout(lMainGrid);

  this->setupInterface();
}

void TexturesAssistant::setupInterface()
{
  // Get the window's layout
  auto lMainLayout{qobject_cast<QGridLayout*>(this->layout())};

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  //// First line
  //auto lInputPathLabel{new QLabel(tr("Input path:"), this)};
  //lMainLayout->addWidget(lInputPathLabel, 0, 0);

  //// Input label
  //auto lInputPathLineEdit{new QLineEdit("", this)};
  //lInputPathLineEdit->setReadOnly(true);
  //lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  //lInputPathLineEdit->setObjectName("input_path_directory");
  //lInputPathLineEdit->setDisabled(true);
  //lMainLayout->addWidget(lInputPathLineEdit, 0, 1);

  //// Input chooser
  //auto lInputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  //lInputPathChooser->setCursor(Qt::PointingHandCursor);
  //lInputPathChooser->setIcon(QIcon(QPixmap(QString(":/%1/folder").arg(lIconFolder))));
  //lInputPathChooser->setAutoDefault(false);
  //lInputPathChooser->setDefault(false);
  //lMainLayout->addWidget(lInputPathChooser, 0, 2);

  //// Launch search button
  //auto lLaunchSearchButton{new QPushButton(tr("Launch the scan of the mod"), this)};
  //lLaunchSearchButton->setCursor(Qt::PointingHandCursor);
  //lLaunchSearchButton->setIcon(QIcon(QPixmap(QString(":/%1/search").arg(lIconFolder))));
  //lLaunchSearchButton->setObjectName("launch_search_button");
  //lLaunchSearchButton->setAutoDefault(false);
  //lLaunchSearchButton->setDefault(false);
  //lLaunchSearchButton->setDisabled(true);
  //lMainLayout->addWidget(lLaunchSearchButton, 1, 0, 1, 3, Qt::AlignTop);
}
