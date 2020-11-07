#include "TabVanillaSE.h"

TabVanillaSE::TabVanillaSE(QWidget* aParent, const Struct::Settings& aSettings)
  : Tab(aParent, aSettings)
{
  auto lMainLayout{this->getMainLayout()};

  auto lWIPLabel(new QLabel("WIP..."));
  lMainLayout->addWidget(lWIPLabel);
}