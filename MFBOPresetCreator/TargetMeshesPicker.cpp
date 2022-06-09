#include "TargetMeshesPicker.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "SliderSetsDBManager.h"
#include "Utils.h"
#include <QCloseEvent>

TargetMeshesPicker::TargetMeshesPicker(QWidget* aParent,
                                       const Struct::Settings& aSettings,
                                       std::map<QString, QString>* aLastPaths,
                                       const BodyNameVersion& aPreSelectedBody,
                                       const FeetNameVersion& aPreSelectedFeet)
  : TitleDialog(aParent, tr("Target Meshes Picker"), "mesh", aSettings, aLastPaths, 1100, 700)
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
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void TargetMeshesPicker::closeEvent(QCloseEvent* aEvent)
{
  if (this->getChosenBodyName() == this->mOriginalBody && this->getChosenFeetName() == this->mOriginalFeet)
  {
    aEvent->accept();
    return;
  }

  if (Utils::DisplayQuestionMessage(this,
                                    tr("Closing"),
                                    tr("Do you want to close the window?"),
                                    this->getThemedResourcePath(),
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

void TargetMeshesPicker::initializeGUI()
{
  // Main window container
  const auto lMainLayout{new QVBoxLayout(this->getCentralWidget())};
  lMainLayout->setAlignment(Qt::AlignTop);

  /*=============*/
  /* Tab widgets */
  /*=============*/
  this->setupBodyTabWidget(*lMainLayout);
  this->setupFeetTabWidget(*lMainLayout);
  this->setupHandsTabWidget(*lMainLayout);

  /*========================================*/
  /* Label for currently chosen meshes mods */
  /*========================================*/
  auto lCurrentlyTargetedBody{new QLabel(tr("Targeted body: -\nTargeted feet: -"), this)};
  lCurrentlyTargetedBody->setObjectName("currently_targeted_body_feet");
  lMainLayout->addWidget(lCurrentlyTargetedBody);

  /*================*/
  /* Bottom buttons */
  /*================*/
  const auto lButtonsLayout{new QHBoxLayout()};

  const auto lSaveButton{ComponentFactory::CreateButton(this, tr("Save and close"), "", "save", this->getThemedResourcePath(), "save_close", false, true)};
  lButtonsLayout->addWidget(lSaveButton);

  const auto lCloseButton{ComponentFactory::CreateButton(this, tr("Cancel"), "", "undo", this->getThemedResourcePath(), "", false, true)};
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

void TargetMeshesPicker::setupBodyTabWidget(QVBoxLayout& aMainLayout)
{
  // Sub-title
  const auto lBodyModLabel{new QLabel(tr("Body mod:"), this)};
  lBodyModLabel->setStyleSheet(QString("font-size: %1pt").arg(this->settings().display.font.pointSize * 1.5));
  aMainLayout.addWidget(lBodyModLabel);

  // Tab widget
  auto lBodyTabWidget{new QTabWidget(this)};
  lBodyTabWidget->setObjectName(QString("body_tab_widget"));
  lBodyTabWidget->setAutoFillBackground(true);
  lBodyTabWidget->tabBar()->setCursor(Qt::CursorShape::PointingHandCursor);
  aMainLayout.addWidget(lBodyTabWidget);

  this->setupEmbeddedBodyTab(*lBodyTabWidget);
  this->setupCustomBodyTab(*lBodyTabWidget);
}

void TargetMeshesPicker::setupEmbeddedBodyTab(QTabWidget& aTabWidget)
{
  // Tab widget
  const auto lTabContent{new QWidget(this)};
  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/body").arg(this->getThemedResourcePath()))), tr("Embedded body mods"));

  // Layout
  const auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setColumnStretch(0, 2);
  lTabLayout->setColumnStretch(1, 1);
  lTabLayout->setColumnStretch(2, 2);
  lTabLayout->setAlignment(Qt::AlignTop);

  // Add the labels
  const auto lLabelBodyBase{new QLabel(tr("Base mod:"), this)};
  lTabLayout->addWidget(lLabelBodyBase, 0, 0);
  const auto lLabelBodyVersionNumber{new QLabel(tr("Version number:"), this)};
  lTabLayout->addWidget(lLabelBodyVersionNumber, 0, 1);
  const auto lLabelBodyVersionName{new QLabel(tr("Targeted mesh:"), this)};
  lTabLayout->addWidget(lLabelBodyVersionName, 0, 2);

  // Add the list widgets
  this->mListBodyName->setAlternatingRowColors(true);
  lTabLayout->addWidget(this->mListBodyName, 1, 0);
  this->mListBodyName->addItems(DataLists::GetBodyNamesList());

  this->mListBodyVersion->setAlternatingRowColors(true);
  lTabLayout->addWidget(this->mListBodyVersion, 1, 1);

  this->mListBodyVariantName->setAlternatingRowColors(true);
  lTabLayout->addWidget(this->mListBodyVariantName, 1, 2);
}

void TargetMeshesPicker::setupCustomBodyTab(QTabWidget& aTabWidget)
{
  // Tab widget
  auto lTabContent{new QWidget(this)};
  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/body").arg(this->getThemedResourcePath()))), tr("Custom body mods"));

  // Layout
  auto lTabLayout{new QVBoxLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);

  // Database manager button
  auto lOpenDatabaseManager{ComponentFactory::CreateButton(this, "Manage custom slider sets", "", "database", this->getThemedResourcePath())};
  lTabLayout->addWidget(lOpenDatabaseManager);

  // Event binding
  QObject::connect(lOpenDatabaseManager, &QPushButton::clicked, this, &TargetMeshesPicker::openSliderSetsDatabaseManager);
}

void TargetMeshesPicker::setupFeetTabWidget(QVBoxLayout& aMainLayout)
{
  // Sub-title
  const auto lFeetModLabel{new QLabel(tr("Feet mod:"), this)};
  lFeetModLabel->setStyleSheet(QString("font-size: %1pt").arg(this->settings().display.font.pointSize * 1.5));
  aMainLayout.addWidget(lFeetModLabel);

  // Tab widget
  auto lFeetTabWidget{new QTabWidget(this)};
  lFeetTabWidget->setObjectName(QString("feet_tab_widget"));
  lFeetTabWidget->setAutoFillBackground(true);
  lFeetTabWidget->tabBar()->setCursor(Qt::CursorShape::PointingHandCursor);
  aMainLayout.addWidget(lFeetTabWidget);

  this->setupEmbeddedFeetTab(*lFeetTabWidget);
  this->setupCustomFeetTab(*lFeetTabWidget);
}

void TargetMeshesPicker::setupEmbeddedFeetTab(QTabWidget& aTabWidget)
{
  // Tab widget
  const auto lTabContent{new QWidget(this)};
  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/foot").arg(this->getThemedResourcePath()))), tr("Embedded feet mods"));

  // Layout
  const auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setColumnStretch(0, 2);
  lTabLayout->setColumnStretch(1, 1);
  lTabLayout->setColumnStretch(2, 2);
  lTabLayout->setAlignment(Qt::AlignTop);

  // Add the labels
  const auto lLabelFeetBase{new QLabel(tr("Base mod:"), this)};
  lTabLayout->addWidget(lLabelFeetBase, 0, 0);
  const auto lLabelFeetVersionNumber{new QLabel(tr("Version number:"), this)};
  lTabLayout->addWidget(lLabelFeetVersionNumber, 0, 1);
  const auto lLabelFeetVersionName{new QLabel(tr("Targeted mesh:"), this)};
  lTabLayout->addWidget(lLabelFeetVersionName, 0, 2);

  // Add the list widgets
  this->mListFeetName->setAlternatingRowColors(true);
  lTabLayout->addWidget(this->mListFeetName, 1, 0);
  this->mListFeetName->addItems(DataLists::GetFeetNamesList(DataLists::GetVariant(this->mOriginalBody)));

  this->mListFeetVersion->setAlternatingRowColors(true);
  lTabLayout->addWidget(this->mListFeetVersion, 1, 1);

  this->mListFeetVariantName->setAlternatingRowColors(true);
  lTabLayout->addWidget(this->mListFeetVariantName, 1, 2);
}

void TargetMeshesPicker::setupCustomFeetTab(QTabWidget& aTabWidget)
{
  // Tab widget
  auto lTabContent{new QWidget(this)};
  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/foot").arg(this->getThemedResourcePath()))), tr("Custom feet mods"));

  // Layout
  auto lTabLayout{new QVBoxLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);

  // Database manager button
  auto lOpenDatabaseManager{ComponentFactory::CreateButton(this, "Manage custom slider sets", "", "database", this->getThemedResourcePath())};
  lTabLayout->addWidget(lOpenDatabaseManager);

  // Event binding
  QObject::connect(lOpenDatabaseManager, &QPushButton::clicked, this, &TargetMeshesPicker::openSliderSetsDatabaseManager);
}

void TargetMeshesPicker::setupHandsTabWidget(QVBoxLayout& aMainLayout)
{
  // Sub-title
  const auto lHandsModLabel{new QLabel(tr("Hands mod:"), this)};
  lHandsModLabel->setStyleSheet(QString("font-size: %1pt").arg(this->settings().display.font.pointSize * 1.5));
  aMainLayout.addWidget(lHandsModLabel);

  // Tab widget
  auto lHandsTabWidget{new QTabWidget(this)};
  lHandsTabWidget->setObjectName(QString("hands_tab_widget"));
  lHandsTabWidget->setAutoFillBackground(true);
  lHandsTabWidget->tabBar()->setCursor(Qt::CursorShape::PointingHandCursor);
  aMainLayout.addWidget(lHandsTabWidget);

  this->setupEmbeddedHandsTab(*lHandsTabWidget);
  this->setupCustomHandsTab(*lHandsTabWidget);
}

void TargetMeshesPicker::setupEmbeddedHandsTab(QTabWidget& aTabWidget)
{
  // Tab widget
  const auto lTabContent{new QWidget(this)};
  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/hand").arg(this->getThemedResourcePath()))), tr("Embedded hands mods"));

  // Layout
  const auto lTabLayout{new QGridLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setColumnStretch(0, 2);
  lTabLayout->setColumnStretch(1, 1);
  lTabLayout->setColumnStretch(2, 2);
  lTabLayout->setAlignment(Qt::AlignTop);

  // Add the labels
  const auto lLabelHandsBase{new QLabel(tr("Base mod:"), this)};
  lTabLayout->addWidget(lLabelHandsBase, 0, 0);
  const auto lLabelHandsVersionNumber{new QLabel(tr("Version number:"), this)};
  lTabLayout->addWidget(lLabelHandsVersionNumber, 0, 1);
  const auto lLabelHandsVersionName{new QLabel(tr("Targeted mesh:"), this)};
  lTabLayout->addWidget(lLabelHandsVersionName, 0, 2);

  //// Add the list widgets
  // this->mListHandsName->setAlternatingRowColors(true);
  // lTabLayout->addWidget(this->mListHandsName, 1, 0);
  // this->mListHandsName->addItems(DataLists::GetHandsNamesList(DataLists::GetVariant(this->mOriginalBody)));

  // this->mListHandsVersion->setAlternatingRowColors(true);
  // lTabLayout->addWidget(this->mListHandsVersion, 1, 1);

  // this->mListHandsVariantName->setAlternatingRowColors(true);
  // lTabLayout->addWidget(this->mListHandsVariantName, 1, 2);
}

void TargetMeshesPicker::setupCustomHandsTab(QTabWidget& aTabWidget)
{
  // Tab widget
  auto lTabContent{new QWidget(this)};
  aTabWidget.addTab(lTabContent, QIcon(QPixmap(QString(":/%1/hand").arg(this->getThemedResourcePath()))), tr("Custom hands mods"));

  // Layout
  auto lTabLayout{new QVBoxLayout(lTabContent)};
  lTabLayout->setSpacing(10);
  lTabLayout->setAlignment(Qt::AlignTop);

  // Database manager button
  auto lOpenDatabaseManager{ComponentFactory::CreateButton(this, "Manage custom slider sets", "", "database", this->getThemedResourcePath())};
  lTabLayout->addWidget(lOpenDatabaseManager);

  // Event binding
  QObject::connect(lOpenDatabaseManager, &QPushButton::clicked, this, &TargetMeshesPicker::openSliderSetsDatabaseManager);
}

void TargetMeshesPicker::openSliderSetsDatabaseManager()
{
  new SliderSetsDBManager(this, this->settings(), this->lastPaths());
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
  const auto lAvailableFeetVariants{DataLists::GetFeetVariantsList(DataLists::GetName(getChosenBodyVariant(),
                                                                                      lSelectedFeetName),
                                                                   aNewIndex,
                                                                   Utils::IsCBBEBasedBody(lBodyVariant))};
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
