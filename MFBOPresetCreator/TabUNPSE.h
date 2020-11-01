#pragma once
#include "Tab.h"

class TabUNPSE : public Tab
{
  Q_OBJECT

public:
  explicit TabUNPSE(QWidget* aParent, const Struct::Settings& aSettings);
};
