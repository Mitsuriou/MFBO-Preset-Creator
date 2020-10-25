#include "Tab.h"

Tab::Tab(QWidget* aParent, Struct::Settings aSettings)
  : QWidget(aParent)
  , mSettings(aSettings)
{
  mMainVertical = new QVBoxLayout(this);
  this->setLayout(mMainVertical);
}
