#pragma once

#include "DataLists.h"
#include "Settings.h"
#include "Utils.h"
#include "stdafx.h"

class Tab : public QWidget
{
  Q_OBJECT

public:
  explicit Tab(QWidget* aParent, const Struct::Settings& aSettings);
  void updateSettings(Struct::Settings aSettings);

protected:
  Struct::Settings mSettings;

  QVBoxLayout* getMainLayout();

private:
  QVBoxLayout* mMainVertical;
};
