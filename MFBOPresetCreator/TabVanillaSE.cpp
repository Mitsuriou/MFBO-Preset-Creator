#include "TabVanillaSE.h"

TabVanillaSE::TabVanillaSE(QWidget* aParent, const Struct::Settings& aSettings)
  : Tab(aParent, aSettings)
{
  auto lWIPLabel(new QLabel("WIP..."));
  mMainVertical->addWidget(lWIPLabel);
}