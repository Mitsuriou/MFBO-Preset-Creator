#pragma once
#include "TitleDialog.h"
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QVBoxLayout>

class Settings final : public TitleDialog
{
  Q_OBJECT

public:
  explicit Settings(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  bool mMustRebootMainApp{false};
  bool mPathEntryCleared{false};
  QFont mNewFont{qApp->font()};
  QString mNewSuccessColor;
  QString mNewWarningColor;
  QString mNewDangerColor;
  BodyNameVersion mNewPresetCreatorTargetBodyMesh;
  FeetNameVersion mNewPresetCreatorTargetFeetMesh;
  BodyNameVersion mNewBatchConversionTargetBodyMesh;
  FeetNameVersion mNewBatchConversionTargetFeetMesh;
  BodyNameVersion mNewRetargetingToolTargetBodyMesh;
  FeetNameVersion mNewRetargetingToolTargetFeetMesh;

  void initializeGUI();

  void setupDisplayTab(QTabWidget& aTabWidget);
  void setupGeneralTab(QTabWidget& aTabWidget);
  void setupPresetCreatorTab(QTabWidget& aTabWidget);
  void setupBatchConversionToolTab(QTabWidget& aTabWidget);
  void setupRetargetingToolTab(QTabWidget& aTabWidget);
  void setupLastPathsTab(QTabWidget& aTabWidget);

  void setupButtons(QHBoxLayout& aLayout);

  void createDialogOpeningModeBlock(QVBoxLayout& aLayout, const QString& aLabelTitle, const QString& aObjectName);

  void loadSettings(const Struct::Settings& aSettingsToLoad);
  void loadDisplayTabSettings(const Struct::DisplaySettings& aSettingsToLoad);
  void loadGeneralTabSettings(const Struct::GeneralSettings& aSettingsToLoad);
  void loadGenericDialogSettings(const Struct::GenericDialogSettings& aSettingsToLoad, const QString& aObjectNamePostActionTask);

  Struct::Settings getSettingsFromGUI() const;

  // Preview on buttons
  void applyFontButtonStyle(const QFont& aFont) const;
  void applySuccessColorButton(const QString& aColor) const;
  void applyWarningColorButton(const QString& aColor) const;
  void applyDangerColorButton(const QString& aColor) const;

  // Dialog buttons
  void saveSettings();
  void restoreDefaultSettings();

  // Meshes choices
  void openPresetCreatorTargetMeshesPicker();
  void openBatchConversionTargetMeshesPicker();
  void openRetargetingToolTargetMeshesPicker();
  void presetCreatorTargetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);
  void batchConversionTargetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);
  void retargetingToolTargetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);
  void targetMeshesChanged(BodyNameVersion& aBodyToUpdate, FeetNameVersion& aFeetToUpdate, const BodyNameVersion& aBody, const FeetNameVersion& aFeet, const QString& aObjectNamePrefix);

  // Style choices
  void chooseFont();
  void chooseSuccessColor();
  void chooseWarningColor();
  void chooseDangerColor();

  // Shortcut
  void goToLastPathsTab();

  // Clear paths
  void clearPathButtonClicked();
  void clearAllPaths();
  void toggleClearAllButtonState();

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);
  void refreshLastPaths(const std::map<QString, QString>& aLastPaths);
};
