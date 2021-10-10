﻿#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTabWidget>

class Settings final : public QDialog
{
  Q_OBJECT

public:
  explicit Settings(QWidget* aParent, const Struct::Settings& aSettings, const std::map<QString, QString>& aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  Struct::Settings mSettings;
  std::map<QString, QString> mLastPaths;
  bool mMustRebootMainApp;
  bool mPathEntryCleared;
  QFont mNewFont;
  QString mNewSuccessColor;
  QString mNewWarningColor;
  QString mNewDangerColor;

  void setWindowProperties();
  void initializeGUI();

  void setupDisplayTab(QTabWidget& aTabWidget);
  void setupGeneralTab(QTabWidget& aTabWidget);
  void setupPresetCreatorTab(QTabWidget& aTabWidget);
  void setupBatchConversionToolTab(QTabWidget& aTabWidget);
  void setupRetargetingToolTab(QTabWidget& aTabWidget);
  void setupLastPathsTab(QTabWidget& aTabWidget);

  void setupButtons(QHBoxLayout& aLayout);

  void createDialogOpeningModeBlock(QGridLayout& aLayout, const QString& aLabelTitle, const QString& aObjectName, const int aRow, const int aCol);

  void loadSettings(const Struct::Settings& aSettingsToLoad);
  void loadDisplayTabSettings(const Struct::DisplaySettings& aSettingsToLoad);
  void loadGeneralTabSettings(const Struct::GeneralSettings& aSettingsToLoad);
  void loadGenericDialogSettings(const Struct::GenericDialogSettings& aSettingsToLoad, 
                                 const QString& aObjectNameBodyNameSelector,
                                 const QString& aObjectNameBodyVersionSelector,
                                 const QString& aObjectNameFeetSelector,
                                 const QString& aObjectNamePostActionTask);
  

  Struct::Settings getSettingsFromGUI() const;

  // Preview on buttons
  void applyFontButtonStyle(const QFont& aFont) const;
  void applySuccessColorButton(const QString& aColor) const;
  void applyWarningColorButton(const QString& aColor) const;
  void applyDangerColorButton(const QString& aColor) const;

  //#pragma region PRIVATE_SLOTS
  void saveSettings();
  void updateAvailableBodyVersions();
  void updateAvailableBatchConversionBodyVersions();
  void updateAvailableUpgradeBodyVersions();
  void restoreDefaultSettings();
  void chooseFont();
  void chooseSuccessColor();
  void chooseWarningColor();
  void chooseDangerColor();
  void goToLastPathsTab();
  //#pragma endregion PRIVATE_SLOTS

  // Clear paths
  void clearPathButtonClicked();
  void clearAllPaths();
  void toggleClearAllButtonState();

  explicit Settings(const Settings&) = delete;
  Settings& operator=(const Settings&) = delete;

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);
  void refreshLastPaths(const std::map<QString, QString>& aLastPaths);
};
