#pragma once

#include "DataLists.h"
#include "Settings.h"
#include "Utils.h"
#include "stdafx.h"

class Tab : public QWidget
{
  Q_OBJECT

public:
  explicit Tab(QWidget* aParent, Struct::Settings aSettings);

protected:
  QVBoxLayout* mMainVertical = nullptr;
  Struct::Settings mSettings;
};
