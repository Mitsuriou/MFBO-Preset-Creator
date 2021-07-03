#include "BCGroupWidget.h"
#include "BCDropWidget.h"
#include <QGroupBox>
#include <QVBoxLayout>

BCGroupWidget::BCGroupWidget(QWidget* aParent, const QString& aSectionTitle)
  : QWidget(aParent)
{
  // Create the layout
  auto lMainLayout{new QVBoxLayout(this)};
  this->setLayout(lMainLayout);

  auto lSection{new QGroupBox(aSectionTitle, this)};

  auto lSectionLayout{new QGridLayout(this)};
  lSection->setLayout(lSectionLayout);

  lMainLayout->addWidget(lSection);

  auto lDropWidget{new BCDropWidget(lSection)};
  lSectionLayout->addWidget(lDropWidget);
}
