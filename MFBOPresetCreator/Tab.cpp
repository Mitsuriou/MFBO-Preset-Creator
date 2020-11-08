#include "Tab.h"

Tab::Tab(QWidget* aParent, const Struct::Settings& aSettings)
  : QWidget(aParent)
  , mSettings(aSettings)
{
  auto lBaseLayout{new QVBoxLayout(this)};

  auto lScrollArea{new QScrollArea(this)};
  lScrollArea->setWidgetResizable(true);

  auto lMainWidget{new QWidget(this)};
  mMainVertical = new QVBoxLayout(lMainWidget);
  mMainVertical->setContentsMargins(0, 0, 0, 0);

  lScrollArea->setWidget(lMainWidget);
  lBaseLayout->addWidget(lScrollArea);
}

void Tab::updateSettings(Struct::Settings aSettings)
{
  this->mSettings = aSettings;
}

QVBoxLayout* Tab::getMainLayout()
{
  return this->mMainVertical;
}
