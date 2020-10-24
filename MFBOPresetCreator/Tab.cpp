#include "Tab.h"

Tab::Tab(QWidget* aParent, Struct::Settings aSettings)
  : QTabWidget(aParent)
  , mSettings(aSettings)
{
  mMainVertical = new QVBoxLayout();
  this->setLayout(mMainVertical);
}
