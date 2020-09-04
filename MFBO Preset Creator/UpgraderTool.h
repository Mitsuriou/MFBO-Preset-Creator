#pragma once

#include "Utils.h"
#include "Struct.h"
#include "Enum.h"

#include <QApplication>
#include <QString>
#include <QStringList>
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
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QDirIterator>
#include <QFileInfo>
#include <QTextStream>
#include <QPair>

class UpgraderTool : public QDialog
{
  Q_OBJECT

public:
  UpgraderTool(QWidget* parent);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject();

private:
  Struct::Settings mSettings;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
  void refreshUI();

private slots:
  void chooseInputDirectory();
  void chooseBackupDirectory();
  void updateBackupPreview();
  void switchBackupState();
  void launchUpDownGradeProcess();
};
