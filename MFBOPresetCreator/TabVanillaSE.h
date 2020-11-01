#pragma once
#include "Tab.h"

class TabVanillaSE : public Tab
{
  Q_OBJECT

public:
  explicit TabVanillaSE(QWidget* aParent, const Struct::Settings& aSettings);
};