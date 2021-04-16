#pragma once

#include "DataLists.h"
#include "Utils.h"
#include "stdafx.h"

class Settings : public QDialog
{
  Q_OBJECT

public:
  explicit Settings(QWidget* parent, const Struct::Settings& aSettings, const std::map<QString, QString>& aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  Struct::Settings mSettings;
  std::map<QString, QString> mLastPaths;
  QFont mNewFont;
  QString mNewSuccessColor;
  QString mNewWarningColor;
  QString mNewDangerColor;
  bool mMustRebootMainApp;

  void setWindowProperties();
  void initializeGUI();

  void setupDisplayGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupGeneralGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupPresetCreatorGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupRetargetingToolGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupAssistedConversionGroup(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupLastPaths(QGridLayout& aLayout, const int& aNextRowIndex);
  void setupButtons(QGridLayout& aLayout, const int& aNextRowIndex);

  void loadSettings(const Struct::Settings& aSettingsToLoad);
  Struct::Settings getSettingsFromGUI() const;

  // Preview on buttons
  void applyFontButtonStyle(const QFont& aFont) const;
  void applySuccessColorButton(const QString& aColor) const;
  void applyWarningColorButton(const QString& aColor) const;
  void applyDangerColorButton(const QString& aColor) const;

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);

private slots:
  void saveSettings();
  void updateAvailableBodyVersions();
  void updateAvailableUpgradeBodyVersions();
  void restoreDefaultSettings();
  void chooseExportDirectory();
  void chooseFont();
  void chooseSuccessColor();
  void chooseWarningColor();
  void chooseDangerColor();

  // GUI widgets events
  void preventGroupBoxCheckEvent(bool aIsChecked);
};
