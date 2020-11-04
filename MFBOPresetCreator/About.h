#pragma once

#include "Enum.h"
#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"

class About : public QDialog
{
  Q_OBJECT

public:
  About(QWidget* parent, const Struct::Settings& aSettings);

private:
  const Struct::Settings mSettings;

  void setWindowProperties();
  void initializeGUI();
};
