#pragma once

#include "About.h"
#include "AssistedConversion.h"
#include "PresetCreator.h"
#include "RetargetingTool.h"
#include "Settings.h"
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
  explicit MFBOPresetCreator(Struct::Settings aSettings, QWidget* parent = Q_NULLPTR);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  Ui::MFBOPresetCreatorClass ui;
  Struct::Settings mSettings;
  std::map<QString, QString> mLastPaths;
  bool mNewVersionAvailable;

  void initializeGUI();
  void setupMenuBar();
  void showWindow();
  void applyGlobalStyleSheet();
  void applyFont(QString aFamily, QString aStyleName, int aSize, int aWeight, bool aItalic, bool aUnderline, bool aStrikeOut);
  std::vector<QLineEdit*> disableLineEditPlaceholders();
  void enableLineEditPlaceholders(std::vector<QLineEdit*> aLineEditsToReactivate);

public slots:
  void refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings);
  void checkForUpdate();
  void pageFetched(const QString& aResult);

private slots:
  void quickRelaunch();
  void launchAssistedConversion();
  void fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList);
  void launchPresetsRetargeting();
  void launchSettingsDialog();
  void launchUpdateDialog();
  void launchAboutDialog();
};
