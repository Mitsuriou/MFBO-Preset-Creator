#pragma once

#include "DataLists.h"
#include "Utils.h"
#include "stdafx.h"

class Settings : public QDialog
{
  Q_OBJECT

public:
  explicit Settings(QWidget* parent, Struct::Settings aSettings);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  Struct::Settings mSettings;
  QFont mNewFont;
  bool mMustRebootMainApp;

  void setWindowProperties();
  void initializeGUI();

  void setupDisplayGroup(QGridLayout& aTabs);
  void setupPresetCreatorGroup(QGridLayout& aTabs);
  void setupRetargetingToolGroup(QGridLayout& aTabs);
  void setupButtons(QGridLayout& aLayout);

  void loadSettings(const Struct::Settings& aSettingsToLoad);
  Struct::Settings getSettingsFromGUI();

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);

private slots:
  void saveSettings();
  void restoreDefaultSettings();
  void chooseExportDirectory();
  void chooseFont();
  void applyFontButtonStyle(const QFont& aFont);
};
