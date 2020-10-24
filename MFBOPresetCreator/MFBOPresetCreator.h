#pragma once

#include "RetargetingTool.h"
#include "Settings.h"
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
  int mMinimumFirstColmunWidth;

  void initializeGUI();
  void setupMenuBar();
  void setupBodyMeshesGUI(QVBoxLayout& aLayout);
  void setupBodySlideGUI(QVBoxLayout& aLayout);
  void setupOptionsGUI(QVBoxLayout& aLayout);
  void setupOutputGUI(QVBoxLayout& aLayout);
  void setupRemainingGUI(QVBoxLayout& aLayout);
  void showWindow();
  void applyStyleSheet();

public slots:
  void refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings);

private slots:
  void updateMeshesPreview();
  void updateOutputPreview();
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);
  void updateSkeletonPathState(int aState);
  void updateSkeletonPreview(QString aText);
  void chooseExportDirectory();
  void generateDirectoryStructure();
  void refreshAllPreviewFields(QString aText);
  void refreshAllPreviewFields(int);
  void refreshAllPreviewFields();

  // Menu dialogs
  void quickRelaunch();
  void launchRetargetingTool();
  void showSettingsDialog();
  void showAboutDialog();
};
