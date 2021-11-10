#include "TargetMeshesPicker.h"
#include "Utils.h"
#include <QCloseEvent>

TargetMeshesPicker::TargetMeshesPicker(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
  , mLastPaths(aLastPaths)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void TargetMeshesPicker::closeEvent(QCloseEvent* aEvent)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the textures assistant window"),
                                    "",
                                    this->mSettings.display.dangerColor,
                                    this->mSettings.display.successColor,
                                    "",
                                    false)
      == ButtonClicked::YES)
  {
    aEvent->accept();
  }
  else
  {
    aEvent->ignore();
  }
}

void TargetMeshesPicker::reject()
{
  this->close();
}

void TargetMeshesPicker::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Target Meshes Picker"));
  this->setWindowIcon(QIcon(QPixmap(":/black/bodyslide-logo")));
}

void TargetMeshesPicker::initializeGUI()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(this->mSettings.display.applicationTheme)};

  // Main window container
  auto lMainLayout{new QVBoxLayout(this)};
  this->setLayout(lMainLayout);

  /*=========================*/
  /* Targeted body group box */
  /*=========================*/
  auto lBodyGroupBox{new QGroupBox(tr("Body mod").append("  "), this)};
  Utils::AddIconToGroupBox(lBodyGroupBox, lIconFolder, "body", this->mSettings.display.font.size);
  this->connect(lBodyGroupBox, &QGroupBox::toggled, this, &TargetMeshesPicker::groupBoxChecked);
  Utils::SetGroupBoxState(lBodyGroupBox, false);
  lMainLayout->addWidget(lBodyGroupBox);

  // Grid layout
  auto lBodyGridLayout{new QGridLayout(lBodyGroupBox)};
  lBodyGridLayout->setSpacing(10);
  lBodyGridLayout->setContentsMargins(15, 20, 15, 15);
  lBodyGridLayout->setAlignment(Qt::AlignTop);

  /*=========================*/
  /* Targeted feet group box */
  /*=========================*/
  auto lFeetGroupBox{new QGroupBox(tr("Feet mod").append("  "), this)};
  Utils::AddIconToGroupBox(lFeetGroupBox, lIconFolder, "foot", this->mSettings.display.font.size);
  this->connect(lFeetGroupBox, &QGroupBox::toggled, this, &TargetMeshesPicker::groupBoxChecked);
  Utils::SetGroupBoxState(lFeetGroupBox, false);
  lMainLayout->addWidget(lFeetGroupBox);

  // Grid layout
  auto lFeetGridLayout{new QGridLayout(lBodyGroupBox)};
  lFeetGridLayout->setSpacing(10);
  lFeetGridLayout->setContentsMargins(15, 20, 15, 15);
  lFeetGridLayout->setAlignment(Qt::AlignTop);
}

void TargetMeshesPicker::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::SetGroupBoxState(lGroupBox, !aIsChecked);
}
