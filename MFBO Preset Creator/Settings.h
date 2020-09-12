#pragma once

#include "Utils.h"
#include "DataLists.h"

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
#include <QIntValidator>
#include <QTranslator>
#include <QFontDatabase>
#include <QRegularExpression>

class Settings : public QDialog
{
  Q_OBJECT

public:
  Settings(QWidget* parent);
  static int const EXIT_CODE_REBOOT{-123456789};

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject();

private:
  Struct::Settings mSettings;
  bool mMustRebootMainApp;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
  void setupButtons(QGridLayout& aLayout);
  void loadSettings();
  void refreshUI();

signals:
  void refreshMainUI(Struct::Settings aSettings);

private slots:
  void saveSettings();
  void restoreDefaultSettings();
};
