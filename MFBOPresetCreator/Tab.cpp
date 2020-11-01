#include "Tab.h"

Tab::Tab(QWidget* aParent, const Struct::Settings& aSettings)
  : QWidget(aParent)
  , mSettings(aSettings)
  , mMainVertical(new QVBoxLayout(this))
{
  this->setLayout(mMainVertical);
}

void Tab::updateSettings(Struct::Settings aSettings)
{
  this->mSettings = aSettings;
}
