#include "TargetMeshesPicker.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "PresetsDatabaseManager.h"
#include "Utils.h"
#include <QCloseEvent>

TargetMeshesPicker::TargetMeshesPicker(QWidget* aParent,
                                       const Struct::Settings& aSettings,
                                       std::map<QString, QString>* aLastPaths,
                                       const BodyNameVersion& aPreSelectedBody,
                                       const FeetNameVersion& aPreSelectedFeet)
  : TitleDialog(aParent, aSettings, aLastPaths)
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

  new PresetsDatabaseManager();
}

void TargetMeshesPicker::closeEvent(QCloseEvent* aEvent)
{
  if (this->getChosenBodyName() == this->mOriginalBody && this->getChosenFeetName() == this->mOriginalFeet)
  {
    aEvent->accept();
    return;
  }

  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->settings().display.applicationTheme)};

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    lIconFolder,
                                    "help-circle",
                                    tr("Close the window"),
                                    tr("Go back to the target meshes picker window"),
                                    "",
                                    this->settings().display.dangerColor,
                                    this->settings().display.successColor,
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

void TargetMeshesPicker::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(1100);
  this->setMinimumHeight(650);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("Target Meshes Picker"));
  this->setWindowIcon(QIcon(QPixmap(":/black/mesh")));
}

void TargetMeshesPicker::initializeGUI()
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->settings().display.applicationTheme)};

  // Main window container
  auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  /*=========================*/
  /* Targeted body group box */
  /*=========================*/
  auto lBodyGroupBox{ComponentFactory::CreateGroupBox(this, tr("Body mod"), "body", lIconFolder, this->settings().display.font.pointSize)};
  lMainLayout->addWidget(lBodyGroupBox);

  // Grid layout
  auto lBodyLayout{new QGridLayout(lBodyGroupBox)};
  lBodyLayout->setSpacing(10);
  lBodyLayout->setContentsMargins(15, 20, 15, 15);
  lBodyLayout->setColumnStretch(0, 2);
  lBodyLayout->setColumnStretch(1, 1);
  lBodyLayout->setColumnStretch(2, 2);
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
  auto lFeetGroupBox{ComponentFactory::CreateGroupBox(this, tr("Feet mod"), "foot", lIconFolder, this->settings().display.font.pointSize)};
  lMainLayout->addWidget(lFeetGroupBox);

  // Grid layout
  auto lFeetLayout{new QGridLayout(lFeetGroupBox)};
  lFeetLayout->setSpacing(10);
  lFeetLayout->setContentsMargins(15, 20, 15, 15);
  lFeetLayout->setColumnStretch(0, 2);
  lFeetLayout->setColumnStretch(1, 1);
  lFeetLayout->setColumnStretch(2, 2);
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

  /*========================================*/
  /* Label for currently chosen meshes mods */
  /*========================================*/
  auto lCurrentlyTargetedBody{new QLabel(tr("Targeted body: -\nTargeted feet: -"), this)};
  lCurrentlyTargetedBody->setObjectName("currently_targeted_body_feet");
  lMainLayout->addWidget(lCurrentlyTargetedBody);

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
  QObject::connect(this->mListBodyName, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::bodyNameIndexChanged);
  QObject::connect(this->mListBodyVersion, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::bodyVersionIndexChanged);
  QObject::connect(this->mListBodyVariantName, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::bodyVariantIndexChanged);
  QObject::connect(this->mListFeetName, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::feetNameIndexChanged);
  QObject::connect(this->mListFeetVersion, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::feetVersionIndexChanged);
  QObject::connect(this->mListFeetVariantName, &QListWidget::currentRowChanged, this, &TargetMeshesPicker::feetVariantIndexChanged);

  QObject::connect(lSaveButton, &QPushButton::clicked, this, &TargetMeshesPicker::validateAndClose);
  QObject::connect(lCloseButton, &QPushButton::clicked, this, &TargetMeshesPicker::close);

  // Post-bind initialization functions for body list widgets
  this->mListBodyName->setCurrentRow(static_cast<int>(DataLists::GetName(this->mOriginalBody)));
  this->mListBodyVersion->setCurrentRow(DataLists::GetVersionIndex(this->mOriginalBody));
  this->mListBodyVariantName->setCurrentRow(DataLists::GetVariantIndex(this->mOriginalBody));

  // Post-bind initialization functions for feet list widgets
  this->mListFeetName->setCurrentRow(DataLists::GetNameIndex(DataLists::GetVariant(this->mOriginalBody), this->mOriginalFeet));
  this->mListFeetVersion->setCurrentRow(DataLists::GetVersionIndex(this->mOriginalFeet));
  this->mListFeetVariantName->setCurrentRow(DataLists::GetVariantIndex(this->mOriginalFeet));

  this->mIsWindowInitialized = true;
}

BodyVariant TargetMeshesPicker::getChosenBodyVariant() const
{
  // Get the selected body name
  const auto lSelectedBodyName{static_cast<BodyName>(this->mListBodyName->currentRow())};

  // Get the selected relative version number
  const auto lSelectedVersionNumber{this->mListBodyVersion->currentRow()};

  // Get the selected relative variant name
  const auto lSelectedVariant{this->mListBodyVariantName->currentRow()};

  return DataLists::GetVariant(lSelectedBodyName, lSelectedVersionNumber, lSelectedVariant);
}

BodyNameVersion TargetMeshesPicker::getChosenBodyName() const
{
  // Get the selected body name
  const auto lSelectedBodyName{static_cast<BodyName>(this->mListBodyName->currentRow())};

  // Get the selected relative version number
  const auto lSelectedVersionNumber{this->mListBodyVersion->currentRow()};

  // Get the selected relative variant name
  const auto lSelectedVariant{this->mListBodyVariantName->currentRow()};

  return DataLists::GetBodyNameVersion(lSelectedBodyName, lSelectedVersionNumber, lSelectedVariant);
}

FeetNameVersion TargetMeshesPicker::getChosenFeetName() const
{
  // Get the dependent body
  const auto lBodyVariant{DataLists::GetVariant(this->getChosenBodyName())};

  // Get the selected feet name
  const auto lSelectedFeetName{this->mListFeetName->currentRow()};
  const auto lFeetName{DataLists::GetName(lBodyVariant, lSelectedFeetName)};

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

  // Reselect the first available variant automatically
  this->mListBodyVariantName->setCurrentRow(0);
}

void TargetMeshesPicker::bodyVariantIndexChanged(const int)
{
  this->mListFeetName->clear();
  const auto lAvailableFeetNames{DataLists::GetFeetNamesList(DataLists::GetVariant(getChosenBodyName()))};
  this->mListFeetName->addItems(lAvailableFeetNames);

  // Smarter lists behavior
  if (this->mIsWindowInitialized)
  {
    // If the feet name is still available with the newly select body-name combo, restore it
    const auto lFeetVersionNumberListIndex{lAvailableFeetNames.indexOf(this->mLastSelectedFeetName)};
    if (lFeetVersionNumberListIndex != -1)
    {
      this->mListFeetName->setCurrentRow(lFeetVersionNumberListIndex);
    }
    // If it is not available anymore, reselect the first available name
    else
    {
      this->mListFeetName->setCurrentRow(0);
    }
  }
  // Window initialization: select the first available name
  else
  {
    this->mListFeetName->setCurrentRow(0);
  }
}

void TargetMeshesPicker::feetNameIndexChanged(const int aNewIndex)
{
  // Get the dependent body
  const auto lBodyVariant{DataLists::GetVariant(this->getChosenBodyName())};

  // Refresh the content of the second list
  this->mListFeetVersion->clear();
  const auto lAvailableFeetVersions{DataLists::GetFeetVersionsList(DataLists::GetName(lBodyVariant, aNewIndex), Utils::IsCBBEBasedBody(lBodyVariant))};
  this->mListFeetVersion->addItems(lAvailableFeetVersions);

  // Smarter lists behavior
  if (this->mListBodyVersion->currentRow() != -1)
  {
    const auto lSelectedBodyVersionVersionNumber{this->mListBodyVersion->currentItem()->text()};
    const auto lFeetVersionNumberListIndex{lAvailableFeetVersions.indexOf(lSelectedBodyVersionVersionNumber)};

    if (lFeetVersionNumberListIndex != -1)
    {
      // Make an version number being written in italic if it is the recommended value
      auto lListItem{this->mListFeetVersion->item(lFeetVersionNumberListIndex)};
      auto lFont{lListItem->font()};
      lFont.setItalic(true);
      lListItem->setFont(lFont);

      // Smart feet version selection, based on the selected body version
      if (this->mIsWindowInitialized)
      {
        this->mListFeetVersion->setCurrentRow(lFeetVersionNumberListIndex);
      }
      else
      {
        this->mListFeetVersion->setCurrentRow(0);
      }
    }
    else
    {
      this->mListFeetVersion->setCurrentRow(0);
    }
  }
  else
  {
    this->mListFeetVersion->setCurrentRow(0);
  }

  // Save the currently selected feet name
  if (aNewIndex != -1)
  {
    auto lCurrentlySelectedFeetName{this->mListFeetName->currentItem()->text()};
    if (!lCurrentlySelectedFeetName.isEmpty())
    {
      this->mLastSelectedFeetName = lCurrentlySelectedFeetName;
    }
  }
}

void TargetMeshesPicker::feetVersionIndexChanged(const int aNewIndex)
{
  // Get the dependent body
  const auto lBodyVariant{DataLists::GetVariant(this->getChosenBodyName())};

  // Get the selected feet
  const auto lSelectedFeetName{this->mListFeetName->currentRow()};

  // Refresh the content of the third list
  this->mListFeetVariantName->clear();
  const auto lAvailableFeetVariants{DataLists::GetFeetVariantsList(DataLists::GetName(getChosenBodyVariant(), lSelectedFeetName), aNewIndex, Utils::IsCBBEBasedBody(lBodyVariant))};
  this->mListFeetVariantName->addItems(lAvailableFeetVariants);

  // Smarter lists behavior
  if (this->mIsWindowInitialized)
  {
    // If the feet name is still available with the newly select body-name combo, restore it
    const auto lFeetVersionNumberListIndex{lAvailableFeetVariants.indexOf(this->mLastSelectedFeetVariant)};
    if (lFeetVersionNumberListIndex != -1)
    {
      this->mListFeetVariantName->setCurrentRow(lFeetVersionNumberListIndex);
    }
    // If it is not available anymore, reselect the first available variant
    else
    {
      this->mListFeetVariantName->setCurrentRow(0);
    }
  }
  // Window initialization: select the first available variant
  else
  {
    this->mListFeetVariantName->setCurrentRow(0);
  }
}

void TargetMeshesPicker::feetVariantIndexChanged(const int aNewIndex)
{
  // Save the currently selected feet variant
  if (aNewIndex != -1)
  {
    auto lCurrentlySelectedFeetName{this->mListFeetVariantName->currentItem()->text()};
    if (!lCurrentlySelectedFeetName.isEmpty())
    {
      this->mLastSelectedFeetVariant = lCurrentlySelectedFeetName;
    }
  }

  const auto lChosenBodyName{this->getChosenBodyName()};
  const auto lChosenFeetName{this->getChosenFeetName()};

  if (lChosenBodyName != BodyNameVersion::_INVALID_VALUE && lChosenFeetName != FeetNameVersion::_INVALID_VALUE)
  {
    // Update the "targeted body mesh" text content
    const auto lBodyText{
      QString("%1 [v.%2]").arg(DataLists::GetBodyVariantsList(DataLists::GetName(lChosenBodyName), DataLists::GetVersionIndex(lChosenBodyName)).at(DataLists::GetVariantIndex(lChosenBodyName)), DataLists::GetVersionString(lChosenBodyName))};

    // Update the "targeted feet mesh" text content
    const auto lFeetText{
      QString("%1 [v.%2]").arg(DataLists::GetFeetVariantsList(DataLists::GetName(lChosenFeetName), DataLists::GetVersionIndex(lChosenFeetName), Utils::IsCBBEBasedBody(lChosenBodyName)).at(DataLists::GetVariantIndex(lChosenFeetName)), DataLists::GetVersionString(lChosenBodyName, lChosenFeetName))};

    auto lCurrentlyTargetedBody{this->findChild<QLabel*>("currently_targeted_body_feet")};
    lCurrentlyTargetedBody->setText(tr("Targeted body: %1\nTargeted feet: %2").arg(lBodyText, lFeetText));
  }
}
