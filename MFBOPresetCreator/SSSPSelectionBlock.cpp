#include "SSSPSelectionBlock.h"
#include <QLabel>
#include <QVBoxLayout>

SSSPSelectionBlock::SSSPSelectionBlock(QWidget* aParent, const Struct::SliderSet& aSliderSet)
  : QGroupBox(aParent)
{
  auto lLayout = new QVBoxLayout(this);
  this->setLayout(lLayout);

  lLayout->addWidget(new QLabel(aSliderSet.getName(), this));
}
