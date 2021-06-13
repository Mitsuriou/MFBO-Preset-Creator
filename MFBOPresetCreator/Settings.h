#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>

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
  void setupButtons(QHBoxLayout& aLayout);

  void loadSettings(const Struct::Settings& aSettingsToLoad);
  Struct::Settings getSettingsFromGUI() const;

  // Preview on buttons
  void applyFontButtonStyle(const QFont& aFont) const;
  void applySuccessColorButton(const QString& aColor) const;
  void applyWarningColorButton(const QString& aColor) const;
  void applyDangerColorButton(const QString& aColor) const;

#pragma region PRIVATE_SLOTS
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
  void scrollbarPressed();
  void scrollbarReleased();
  void groupBoxChecked(bool aIsChecked);
  void clearPathButtonClicked();
  void clearAllPaths();
#pragma endregion PRIVATE_SLOTS

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);
  void refreshLastPaths(const std::map<QString, QString>& aLastPaths);
};
