#include "BCGroupWidget.h"
#include "BCDropWidget.h"
#include "ComponentFactory.h"
#include "Utils.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

BCGroupWidget::BCGroupWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aSectionTitle, const QString& aSectionIconName, const BCGroupWidgetCallContext& aCallContext)
  : QWidget(aParent)
  , mCallContext(aCallContext)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconRessourceFolder(aSettings.appTheme)};

  // Main layout
  auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setMargin(0);
  this->setLayout(lMainLayout);

  auto lSection{new QGroupBox(aSectionTitle + QString("  "), this)};
  Utils::AddIconToGroupBox(lSection, lIconFolder, aSectionIconName, aSettings.font.size);
  this->connect(lSection, &QGroupBox::toggled, this, &BCGroupWidget::groupBoxChecked);
  Utils::SetGroupBoxState(lSection, false);

  auto lSectionLayout{new QGridLayout(this)};
  lSectionLayout->setSpacing(10);
  lSectionLayout->setContentsMargins(15, 20, 15, 15);
  lSectionLayout->setColumnStretch(0, 1);
  lSectionLayout->setColumnStretch(1, 0);
  lSection->setLayout(lSectionLayout);

  lMainLayout->addWidget(lSection);

  // Drop zone
  auto lDropWidget{new BCDropWidget(lSection, this->mCallContext)};
  lDropWidget->setObjectName(QString("drop_widget"));
  lSectionLayout->addWidget(lDropWidget, 0, 0, 2, 1);

  // Remove data entry button
  auto lRemoveButton{ComponentFactory::CreateButton(this, tr("Remove this data entry"), "", "cross", lIconFolder, "remove_button", false, true)};
  lRemoveButton->hide();
  lSectionLayout->addWidget(lRemoveButton, 0, 1);

  auto lDuplicateButton{ComponentFactory::CreateButton(this, tr("Duplicate this data entry"), "", "duplicate", lIconFolder, "duplicate_button", false, true)};
  lDuplicateButton->hide();
  lSectionLayout->addWidget(lDuplicateButton, 1, 1);

  // Event binding
  this->connect(lDropWidget, &BCDropWidget::dropEventTriggered, this, &BCGroupWidget::dropEventTrigerredReceiver);
  this->connect(lDropWidget, &BCDropWidget::checkBoxStateChangedTriggered, this, &BCGroupWidget::checkBoxStateChangedReceiver);
  this->connect(lRemoveButton, &QPushButton::clicked, this, &BCGroupWidget::removeData);
  this->connect(lDuplicateButton, &QPushButton::clicked, this, &BCGroupWidget::duplicateData);
}

void BCGroupWidget::setData(const Struct::BatchConversionPresetData& aData)
{
  auto lRemoveButton{this->findChild<QPushButton*>(QString("remove_button"))};
  auto lDropWidget{this->findChild<BCDropWidget*>(QString("drop_widget"))};

  if (this->mCallContext == BCGroupWidgetCallContext::BODY)
  {
    auto lData{aData.getBodyData()};
    if (lData.second.isEmpty())
    {
      lRemoveButton->hide();
      lDropWidget->resetData();
    }
    else
    {
      lRemoveButton->show();
      lDropWidget->setData(lData.first, lData.second);
    }
  }
  else if (this->mCallContext == BCGroupWidgetCallContext::FEET)
  {
    auto lData{aData.getFeetData()};
    if (lData.second.isEmpty())
    {
      lRemoveButton->hide();
      lDropWidget->resetData();
    }
    else
    {
      lRemoveButton->show();
      lDropWidget->setData(lData.first, lData.second);
    }
  }
  else if (this->mCallContext == BCGroupWidgetCallContext::HANDS)
  {
    auto lData{aData.getHandsData()};
    if (lData.getRessourcePath().isEmpty())
    {
      lRemoveButton->hide();
      lDropWidget->resetData();
    }
    else
    {
      lRemoveButton->show();
      lDropWidget->setData(lData.getOriginFolder(), lData.getRessourcePath(), lData.mustUseAlternativeModel());
    }
  }
  else if (this->mCallContext == BCGroupWidgetCallContext::SKELETON)
  {
    auto lData{aData.getSkeletonData()};
    if (lData.getRessourcePath().isEmpty())
    {
      lRemoveButton->hide();
      lDropWidget->resetData();
    }
    else
    {
      lRemoveButton->show();
      lDropWidget->setData(lData.getOriginFolder(), lData.getRessourcePath(), lData.mustUseAlternativeModel());
    }
  }
}

void BCGroupWidget::removeData()
{
  // Current object treatment
  this->findChild<QPushButton*>(QString("remove_button"))->hide();
  this->findChild<QPushButton*>(QString("duplicate_button"))->hide();

  // Lower treatment
  auto lDropWidget{this->findChild<BCDropWidget*>(QString("drop_widget"))};
  auto lOriginFolder{lDropWidget->getOriginFolder()};
  auto lRessourcePath{lDropWidget->getRessourcePath()};
  lDropWidget->resetData();

  // Upper treatment
  emit BCGroupWidget::removePressed(lOriginFolder, lRessourcePath);
}

void BCGroupWidget::duplicateData()
{
  auto lDropWidget{this->findChild<BCDropWidget*>(QString("drop_widget"))};
  auto lOriginFolder{lDropWidget->getOriginFolder()};
  auto lRessourcePath{lDropWidget->getRessourcePath()};

  emit BCGroupWidget::duplicatePressed(lOriginFolder, lRessourcePath);
}

void BCGroupWidget::dropEventTrigerredReceiver(const QString& aOldOriginFolder, const QString& aOldRessourcePath, const QString& aNewOriginFolder, const QString& aNewRessourcePath, const bool isCheckBoxChecked)
{
  // Check if any data was already set in the drop widget
  if (!aOldOriginFolder.isEmpty() && !aNewOriginFolder.isEmpty())
  {
    emit BCGroupWidget::removePressed(aOldOriginFolder, aOldRessourcePath); // Simulate a click on the "remove" button to make the data go in list again
    this->findChild<BCDropWidget*>(QString("drop_widget"))->resetData();    // Finally, reset the drop widget data
  }
  else
  {
    // Current object treatment
    this->findChild<QPushButton*>(QString("remove_button"))->show();
    this->findChild<QPushButton*>(QString("duplicate_button"))->show();
  }

  // Upper treatment
  emit BCGroupWidget::dropEventTriggered(aNewOriginFolder, aNewRessourcePath, isCheckBoxChecked);
}

void BCGroupWidget::checkBoxStateChangedReceiver(const bool aIsActive)
{
  emit BCGroupWidget::checkBoxStateChangedTriggered(aIsActive);
}

void BCGroupWidget::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::SetGroupBoxState(lGroupBox, !aIsChecked);
}
