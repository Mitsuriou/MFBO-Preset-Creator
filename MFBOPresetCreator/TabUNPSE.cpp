#include "TabUNPSE.h"

TabUNPSE::TabUNPSE(QWidget* aParent, Struct::Settings aSettings)
  : Tab(aParent, aSettings)
{
  auto lWIPLabel(new QLabel("WIP..."));
  mMainVertical->addWidget(lWIPLabel);
}