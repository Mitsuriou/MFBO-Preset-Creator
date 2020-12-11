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
  void reject() override;

private:
  Struct::Settings mSettings;
  QFont mNewFont;
  bool mMustRebootMainApp;

  void setWindowProperties();
  void initializeGUI();

  void setupDisplayGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupGeneralGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupPresetCreatorGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupAssistedConversionGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupRetargetingToolGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupButtons(QGridLayout& aLayout, const int& aNextRowIndex);

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
