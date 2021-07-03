#include "BCGroupWidget.h"
#include "BCDropWidget.h"
#include <QGroupBox>
#include <QVBoxLayout>

BCGroupWidget::BCGroupWidget(QWidget* aParent, const QString& aSectionTitle)
  : QWidget(aParent)
{
  // Create the layout
  auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setMargin(0);
  this->setLayout(lMainLayout);

  auto lSection{new QGroupBox(aSectionTitle, this)};

  auto lSectionLayout{new QGridLayout(this)};
  lSectionLayout->setSpacing(10);
  lSectionLayout->setContentsMargins(15, 20, 15, 15);
  lSectionLayout->setAlignment(Qt::AlignTop);
  lSection->setLayout(lSectionLayout);

  lMainLayout->addWidget(lSection);

  auto lDropWidget{new BCDropWidget(lSection)};
  lSectionLayout->addWidget(lDropWidget);
}
