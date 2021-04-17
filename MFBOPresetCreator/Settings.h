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
  bool mPathEntryCleared;

  void setWindowProperties();
  void initializeGUI();

  void setupDisplayGroup(QGridLayout& aLayout);
  void setupGeneralGroup(QGridLayout& aLayout);
  void setupPresetCreatorGroup(QGridLayout& aLayout);
  void setupRetargetingToolGroup(QGridLayout& aLayout);
  void setupAssistedConversionGroup(QGridLayout& aLayout);
  void setupLastPaths(QGridLayout& aLayout);
  void setupButtons(QGridLayout& aLayout);

  void loadSettings(const Struct::Settings& aSettingsToLoad);
  Struct::Settings getSettingsFromGUI() const;

  // Preview on buttons
  void applyFontButtonStyle(const QFont& aFont) const;
  void applySuccessColorButton(const QString& aColor) const;
  void applyWarningColorButton(const QString& aColor) const;
  void applyDangerColorButton(const QString& aColor) const;

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);
  void refreshLastPaths(const std::map<QString, QString>& aLastPaths);

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
  void groupBoxChecked(bool aIsChecked);
  void clearPathButtonClicked();
};
