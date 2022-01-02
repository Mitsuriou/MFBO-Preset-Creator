#include "TargetMeshesPicker.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Utils.h"
#include <QCloseEvent>

TargetMeshesPicker::TargetMeshesPicker(QWidget* aParent,
                                       const Struct::Settings& aSettings,
                                       const BodyNameVersion& aPreSelectedBody,
                                       const FeetNameVersion& aPreSelectedFeet)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
  , mOriginalBody(aPreSelectedBody)
  , mOriginalFeet(aPreSelectedFeet)
  , mListBodyName(new QListWidget(this))
  , mListBodyVersion(new QListWidget(this))
  , mListBodyVariantName(new QListWidget(this))
  , mListFeetName(new QListWidget(this))
  , mListFeetVersion(new QListWidget(this))
  , mListFeetVariantName(new QListWidget(this))
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
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the target meshes picker window"),
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
  this->setWindowIcon(QIcon(QPixmap(":/black/mesh")));
}

void TargetMeshesPicker::initializeGUI()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Main window container
  auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  /*=========================*/
  /* Targeted body group box */
  /*=========================*/
  auto lBodyGroupBox{ComponentFactory::CreateGroupBox(this, tr("Body mod"), "body", lIconFolder, this->mSettings.display.font.size)};
  lMainLayout->addWidget(lBodyGroupBox);

  // Grid layout
  auto lBodyLayout{new QGridLayout(lBodyGroupBox)};
  lBodyLayout->setSpacing(10);
  lBodyLayout->setContentsMargins(15, 20, 15, 15);
  lBodyLayout->setAlignment(Qt::AlignTop);

  // Add the labels
  auto lLabelBodyBase{new QLabel(tr("Body base mod:"), this)};
  lBodyLayout->addWidget(lLabelBodyBase, 0, 0);
  auto lLabelBodyVersionNumber{new QLabel(tr("Version number:"), this)};
  lBodyLayout->addWidget(lLabelBodyVersionNumber, 0, 1);
  auto lLabelBodyVersionName{new QLabel(tr("Targeted body mesh:"), this)};
  lBodyLayout->addWidget(lLabelBodyVersionName, 0, 2);

  // Add the list widgets
  this->mListBodyName->setAlternatingRowColors(true);
  lBodyLayout->addWidget(this->mListBodyName, 1, 0);
  this->mListBodyName->addItems(DataLists::GetBodyNamesList());

  this->mListBodyVersion->setAlternatingRowColors(true);
  lBodyLayout->addWidget(this->mListBodyVersion, 1, 1);

  this->mListBodyVariantName->setAlternatingRowColors(true);
  lBodyLayout->addWidget(this->mListBodyVariantName, 1, 2);

  /*=========================*/
  /* Targeted feet group box */
  /*=========================*/
  auto lFeetGroupBox{ComponentFactory::CreateGroupBox(this, tr("Feet mod"), "foot", lIconFolder, this->mSettings.display.font.size)};
  lMainLayout->addWidget(lFeetGroupBox);

  // Grid layout
  auto lFeetLayout{new QGridLayout(lFeetGroupBox)};
  lFeetLayout->setSpacing(10);
  lFeetLayout->setContentsMargins(15, 20, 15, 15);
  lFeetLayout->setAlignment(Qt::AlignTop);

  // Add the labels
  auto lLabelFeetBase{new QLabel(tr("Feet base mod:"), this)};
  lFeetLayout->addWidget(lLabelFeetBase, 0, 0);
  auto lLabelFeetVersionNumber{new QLabel(tr("Version number:"), this)};
  lFeetLayout->addWidget(lLabelFeetVersionNumber, 0, 1);
  auto lLabelFeetVersionName{new QLabel(tr("Targeted feet mesh:"), this)};
  lFeetLayout->addWidget(lLabelFeetVersionName, 0, 2);

  // Add the list widgets
  this->mListFeetName->setAlternatingRowColors(true);
  lFeetLayout->addWidget(this->mListFeetName, 1, 0);
  this->mListFeetName->addItems(DataLists::GetFeetNamesList(DataLists::GetVariant(this->mOriginalBody)));

  this->mListFeetVersion->setAlternatingRowColors(true);
  lFeetLayout->addWidget(this->mListFeetVersion, 1, 1);

  this->mListFeetVariantName->setAlternatingRowColors(true);
  lFeetLayout->addWidget(this->mListFeetVariantName, 1, 2);

  /*================*/
  /* Bottom buttons */
  /*================*/
  auto lButtonsLayout{new QHBoxLayout(this)};

  auto lSaveButton{ComponentFactory::CreateButton(this, tr("Save and close"), "", "save", lIconFolder, "save_close", false, true)};
  lButtonsLayout->addWidget(lSaveButton);

  auto lCloseButton{ComponentFactory::CreateButton(this, tr("Cancel"), "", "undo", lIconFolder, "", false, true)};
  lButtonsLayout->addWidget(lCloseButton);

  lMainLayout->addLayout(lButtonsLayout);

  // Event binding
  this->connect(this->mListBodyName, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::bodyNameIndexChanged);
  this->connect(this->mListBodyVersion, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::bodyVersionIndexChanged);
  this->connect(this->mListBodyVariantName, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::bodyVariantIndexChanged);
  this->connect(this->mListFeetName, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::feetNameIndexChanged);
  this->connect(this->mListFeetVersion, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::feetVersionIndexChanged);

  this->connect(lSaveButton, &QPushButton::clicked, this, &TargetMeshesPicker::validateAndClose);
  this->connect(lCloseButton, &QPushButton::clicked, this, &TargetMeshesPicker::close);

  // Post-bind initialization functions for body list widgets
  this->mListBodyName->setCurrentRow(static_cast<int>(DataLists::GetName(this->mOriginalBody)));
  this->mListBodyVersion->setCurrentRow(DataLists::GetVersionIndex(this->mOriginalBody));
  this->mListBodyVariantName->setCurrentRow(DataLists::GetVariantIndex(this->mOriginalBody));

  // Post-bind initialization functions for feet list widgets
  this->mListFeetName->setCurrentRow(DataLists::GetNameIndex(DataLists::GetVariant(this->mOriginalBody), this->mOriginalFeet));
  this->mListFeetVersion->setCurrentRow(DataLists::GetVersionIndex(this->mOriginalFeet));
  this->mListFeetVariantName->setCurrentRow(DataLists::GetVariantIndex(this->mOriginalFeet));
}

BodyVariant TargetMeshesPicker::getChosenBodyVariant() const
{
  // Get the selected body name
  auto lSelectedBodyName{static_cast<BodyName>(this->mListBodyName->currentRow())};

  // Get the selected relative version number
  const auto lSelectedVersionNumber{this->mListBodyVersion->currentRow()};

  // Get the selected relative variant name
  const auto lSelectedVariant{this->mListBodyVariantName->currentRow()};

  return DataLists::GetVariant(lSelectedBodyName, lSelectedVersionNumber, lSelectedVariant);
}

BodyNameVersion TargetMeshesPicker::getChosenBodyName() const
{
  // Get the selected body name
  auto lSelectedBodyName{static_cast<BodyName>(this->mListBodyName->currentRow())};

  // Get the selected relative version number
  const auto lSelectedVersionNumber{this->mListBodyVersion->currentRow()};

  // Get the selected relative variant name
  const auto lSelectedVariant{this->mListBodyVariantName->currentRow()};

  return DataLists::GetBodyNameVersion(lSelectedBodyName, lSelectedVersionNumber, lSelectedVariant);
}

FeetNameVersion TargetMeshesPicker::getChosenFeetName() const
{
  // Get the dependent body
  auto lBodyVariant{DataLists::GetVariant(this->getChosenBodyName())};

  // Get the selected feet name
  auto lSelectedFeetName{this->mListFeetName->currentRow()};
  auto lFeetName{DataLists::GetName(lBodyVariant, lSelectedFeetName)};

  // Get the selected relative version number
  const auto lSelectedVersionNumber{this->mListFeetVersion->currentRow()};

  // Get the selected relative variant name
  const auto lSelectedVariant{this->mListFeetVariantName->currentRow()};

  // DataLists::GetVariant(lFeetName, lSelectedVersionNumber, lSelectedVariant)
  return DataLists::GetFeetNameVersion(lFeetName, lSelectedVersionNumber, lSelectedVariant, Utils::IsCBBEBasedBody(lBodyVariant));
}

void TargetMeshesPicker::validateAndClose()
{
  emit valuesChosen(getChosenBodyName(), getChosenFeetName());
  this->accept();
}

void TargetMeshesPicker::bodyNameIndexChanged(const int aNewIndex)
{
  // Get the selected body name
  const auto lSelectedBodyName{static_cast<BodyName>(aNewIndex)};

  // Refresh the content of the second list
  this->mListBodyVersion->clear();
  this->mListBodyVersion->addItems(DataLists::GetBodyVersionsList(lSelectedBodyName));

  // Reselect the first available version automatically
  this->mListBodyVersion->setCurrentRow(0);
}

void TargetMeshesPicker::bodyVersionIndexChanged(const int aNewIndex)
{
  // Get the selected body name
  const auto lSelectedBodyName{static_cast<BodyName>(this->mListBodyName->currentRow())};

  // Refresh the content of the third list
  this->mListBodyVariantName->clear();
  this->mListBodyVariantName->addItems(DataLists::GetBodyVariantsList(lSelectedBodyName, aNewIndex));

  // Reselect the first available version automatically
  this->mListBodyVariantName->setCurrentRow(0);
}

void TargetMeshesPicker::bodyVariantIndexChanged(const int)
{
  this->mListFeetName->clear();
  this->mListFeetName->addItems(DataLists::GetFeetNamesList(DataLists::GetVariant(getChosenBodyName())));

  // Reselect the first available version automatically
  this->mListFeetName->setCurrentRow(0);
}

void TargetMeshesPicker::feetNameIndexChanged(const int aNewIndex)
{
  // Get the dependent body
  auto lBodyVariant{DataLists::GetVariant(this->getChosenBodyName())};

  // Refresh the content of the second list
  this->mListFeetVersion->clear();
  this->mListFeetVersion->addItems(DataLists::GetFeetVersionsList(DataLists::GetName(lBodyVariant, aNewIndex), Utils::IsCBBEBasedBody(lBodyVariant)));

  // Reselect the first available version automatically
  this->mListFeetVersion->setCurrentRow(0);
}

void TargetMeshesPicker::feetVersionIndexChanged(const int)
{
  // Get the selected feet
  auto lSelectedFeetName{this->mListFeetName->currentRow()};

  // Refresh the content of the third list
  this->mListFeetVariantName->clear();
  this->mListFeetVariantName->addItems(DataLists::GetFeetVariantsList(DataLists::GetName(getChosenBodyVariant(), lSelectedFeetName)));

  // Reselect the first available version automatically
  this->mListFeetVariantName->setCurrentRow(0);
}
