#pragma once

#include "Struct.h"
#include "stdafx.h"

class AssistedConversion : public QDialog
{
  Q_OBJECT

public:
  explicit AssistedConversion(QWidget* parent, const Struct::Settings& aSettings);

private:
  const Struct::Settings mSettings;

  void setWindowProperties();
};
