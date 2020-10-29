#pragma once

#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"
#include "Enum.h"

class About : public QDialog
{
  Q_OBJECT

public:
  About(QWidget* parent, Struct::Settings aSettings);

private:
  Struct::Settings mSettings;

  void setWindowProperties();
  void initializeGUI();
};
