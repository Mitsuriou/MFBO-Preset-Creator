#pragma once

#include "RetargetingTool.h"
#include "Settings.h"
#include "Tab.h"
#include "TabCBBESE.h"
#include "Update.h"
#include "Utils.h"
#include "WinUser.h"
#include "stdafx.h"
#include "windows.h"

#include "ui_MFBOPresetCreator.h"

class MFBOPresetCreator : public QMainWindow
{
  Q_OBJECT

public:
  MFBOPresetCreator(QWidget* parent = Q_NULLPTR);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  Ui::MFBOPresetCreatorClass ui;
  Struct::Settings mSettings;
  std::vector<Tab*> mTabs;

  void initializeGUI();
  void setupMenuBar();
  void showWindow();
  void applyStyleSheet();

public slots:
  void refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings);

private slots:
  void quickRelaunch();
  void launchRetargetingTool();
  void showSettingsDialog();
  void showUpdateDialog();
  void showAboutDialog();
};
