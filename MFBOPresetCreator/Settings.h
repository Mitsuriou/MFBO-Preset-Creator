#pragma once

#include "DataLists.h"
#include "Utils.h"
#include "stdafx.h"

class __declspec(dllexport) Settings : public QDialog
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
