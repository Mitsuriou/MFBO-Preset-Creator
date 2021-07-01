#pragma once

#include "ui_BCDropWidget.h"
#include <QWidget>

class BCDropWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCDropWidget(QWidget* parent = Q_NULLPTR);

private:
  Ui::BCDropWidget ui;
};
