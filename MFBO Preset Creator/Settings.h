#pragma once

#include "Utils.h"

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

class Settings : public QDialog
{
  Q_OBJECT

public:
  Settings(QWidget* parent);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  Struct::Settings mSettings;

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
