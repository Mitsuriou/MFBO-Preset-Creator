#include "BCGroupWidget.h"
#include "BCDropWidget.h"
#include "ComponentFactory.h"
#include "Utils.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

BCGroupWidget::BCGroupWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aSectionTitle, const QString& aSectionIconName)
  : QWidget(aParent)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(aSettings.appTheme)};

  // Main layout
  auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setMargin(0);
  this->setLayout(lMainLayout);

  auto lSection{new QGroupBox(aSectionTitle + QString("  "), this)};
  Utils::addIconToGroupBox(lSection, lIconFolder, aSectionIconName, aSettings.font.size);
  this->connect(lSection, &QGroupBox::toggled, this, &BCGroupWidget::groupBoxChecked);
  Utils::setGroupBoxState(lSection, false);

  auto lSectionLayout{new QGridLayout(this)};
  lSectionLayout->setSpacing(10);
  lSectionLayout->setContentsMargins(15, 20, 15, 15);
  lSectionLayout->setColumnStretch(0, 1);
  lSectionLayout->setColumnStretch(1, 0);
  lSection->setLayout(lSectionLayout);

  lMainLayout->addWidget(lSection);

  // Drop zone
  auto lDropWidget{new BCDropWidget(lSection)};
  lDropWidget->setObjectName("drop_widget");
  lSectionLayout->addWidget(lDropWidget);

  // Remove data entry button
  auto lRemoveButton{ComponentFactory::createButton(this, tr("Remove this data entry"), "", "cross", lIconFolder, "remove_button", false, true)};
  lRemoveButton->hide();
  lSectionLayout->addWidget(lRemoveButton);

  // Event binding
  this->connect(lDropWidget, &BCDropWidget::dropEventTriggered, this, &BCGroupWidget::dropEventTrigerredReceiver);
  this->connect(lRemoveButton, &QPushButton::clicked, this, &BCGroupWidget::removeButtonClicked);
}

void BCGroupWidget::removeButtonClicked()
{
  // Current object treatment
  auto lRemoveButton{this->findChild<QPushButton*>("remove_button")};
  lRemoveButton->hide();

  // Lower treatment
  auto lDropWidget{this->findChild<BCDropWidget*>("drop_widget")};
  auto lOriginFolder{lDropWidget->getOriginFolder()};
  auto lRessourcePath{lDropWidget->getRessourcePath()};
  lDropWidget->resetData();

  // Upper treatment
  emit BCGroupWidget::removePressed(lOriginFolder, lRessourcePath);
}

void BCGroupWidget::dropEventTrigerredReceiver(const QString& aOldOriginFolder, const QString& aOldRessourcePath, const QString& aNewOriginFolder, const QString& aNewRessourcePath)
{
  // Check if any data was already set in the drop widget
  if (aOldOriginFolder.length() > 0 && aNewOriginFolder.length() > 0)
  {
    emit BCGroupWidget::removePressed(aOldOriginFolder, aOldRessourcePath); // Simulate a click on the "remove" button to make the data go in list again
    this->findChild<BCDropWidget*>("drop_widget")->resetData();             // Finally, reset the drop widget data
  }
  else
  {
    // Current object treatment
    auto lRemoveButton{this->findChild<QPushButton*>("remove_button")};
    lRemoveButton->show();
  }

  // Upper treatment
  emit BCGroupWidget::dropEventTriggered(aNewOriginFolder, aNewRessourcePath);
}

void BCGroupWidget::groupBoxChecked(bool aIsChecked)
{
  auto lGroupBox{qobject_cast<QGroupBox*>(this->sender())};
  if (lGroupBox == nullptr)
    return;

  Utils::setGroupBoxState(lGroupBox, !aIsChecked);
}
