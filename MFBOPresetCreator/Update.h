#pragma once

#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"

class Update : public QDialog
{
  Q_OBJECT

public:
  Update(QWidget* parent);

private:
  Struct::Settings mSettings;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
  void refreshUI();
};
