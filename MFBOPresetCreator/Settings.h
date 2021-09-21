#pragma once
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
  QFont mNewFont;
  QString mNewSuccessColor;
  QString mNewWarningColor;
  QString mNewDangerColor;
  bool mMustRebootMainApp;
  bool mPathEntryCleared;

  void setWindowProperties();
  void initializeGUI();

  void setupDisplayTab(QTabWidget& aTabWidget);
  void setupGeneralTab(QTabWidget& aTabWidget);
  void setupPresetCreatorTab(QTabWidget& aTabWidget);
  void setupRetargetingToolTab(QTabWidget& aTabWidget);
  void setupAssistedConversionTab(QTabWidget& aTabWidget);
  void setupLastPathsTab(QTabWidget& aTabWidget);
  void setupButtons(QHBoxLayout& aLayout);

  void createDialogOpeningModeBlock(QGridLayout& aLayout, const QString& aLabelTitle, const QString& aObjectName, const int aRow, const int aCol);

  void loadSettings(const Struct::Settings& aSettingsToLoad);
  Struct::Settings getSettingsFromGUI() const;

  // Preview on buttons
  void applyFontButtonStyle(const QFont& aFont) const;
  void applySuccessColorButton(const QString& aColor) const;
  void applyWarningColorButton(const QString& aColor) const;
  void applyDangerColorButton(const QString& aColor) const;

  //#pragma region PRIVATE_SLOTS
  void saveSettings();
  void updateAvailableBodyVersions();
  void updateAvailableUpgradeBodyVersions();
  void restoreDefaultSettings();
  void chooseExportDirectory();
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
