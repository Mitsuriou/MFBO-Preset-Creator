﻿#pragma once

#include "DataLists.h"
#include "Utils.h"
#include "stdafx.h"

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
  void setupTabs(QVBoxLayout* aLayout);
  void setupDisplayTab(QTabWidget* aTabs);
  void setupPresetCreatorTab(QTabWidget* aTabs);
  void setupRetargetingToolTab(QTabWidget* aTabs);
  void setupButtons(QHBoxLayout* aLayout);
  void loadSettings();
  Struct::Settings getSettingsFromGUI();
  void refreshUI();

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);

private slots:
  void saveSettings();
  void restoreDefaultSettings();
  void chooseExportDirectory();
};