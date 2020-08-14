#pragma once

#include "Utils.h"

#include <QApplication>
#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QProgressDialog>
#include <QCheckBox>
#include <QProgressBar>
#include <QStandardPaths>
#include <QCloseEvent>

class UpgraderTool : public QDialog
{
  Q_OBJECT

public:
  UpgraderTool(QWidget* parent);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);

private slots:
  void chooseInputDirectory();
  void chooseBackupDirectory();
  void updateBackupPreview();
  void switchBackupState();
  void launchUpDownGradeProcess();
};
