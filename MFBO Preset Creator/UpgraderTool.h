#pragma once

#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>

class UpgraderTool : public QDialog
{
  Q_OBJECT

public:
  UpgraderTool(QWidget* parent);

private:
  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
};
