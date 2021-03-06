﻿#pragma once
#include "Struct.h"
#include <QLineEdit>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <windows.h>

#include "ui_MFBOPresetCreator.h"

class MFBOPresetCreator final : public QMainWindow
{
  Q_OBJECT

public:
  explicit MFBOPresetCreator(const Struct::Settings& aSettings, const QString& aInjectedFilePath, QWidget* aParent = Q_NULLPTR);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void dragEnterEvent(QDragEnterEvent* aEvent);
  void dropEvent(QDropEvent* aEvent);

private:
  Ui::MFBOPresetCreatorClass ui;
  Struct::Settings mSettings;
  QString mInjectedFilePath;
  std::map<QString, QString> mLastPaths;
  bool mNewStableVersionAvailable;
  bool mNewBetaVersionAvailable;
  QNetworkAccessManager mManager;

  void initializeGUI();
  void setupMenuBar();
  void showWindow();
  void applyGlobalStyleSheet();
  void applyFont(QString aFamily, QString aStyleName, int aSize, int aWeight, bool aItalic, bool aUnderline, bool aStrikeOut);
  std::vector<QLineEdit*> disableLineEditPlaceholders();
  void enableLineEditPlaceholders(std::vector<QLineEdit*> aLineEditsToReactivate);

  // Check for updates
  void checkForUpdate();
  void displayUpdateMessage(const QString& aResult);

#pragma region PUBLIC_SLOTS
  void refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings);
#pragma endregion PUBLIC_SLOTS

#pragma region PRIVATE_SLOTS
  // Base functionalities
  void loadProject();
  void saveProject(const bool aIsQuittingContext = false);
  void quickRelaunch();
  // Assisted Conversion
  void launchAssistedConversion();
  void fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList);
  // Other tools
  void launchBatchConversion();
  void launchPresetsRetargeting();
  void launchTexturesAssistant();
  // Dialog
  void launchSettingsDialog();
  void launchUpdateDialog();
  // URL links
  void reportBugNexusMods();
  void reportBugGitHub();
  void reportBugGitLab();
  void openNexusModsPage();
  void openGitHubSourceCodePage();
  void openGitLabSourceCodePage();
  void openGoogleDriveGuide();
  // Dialog
  void launchAboutDialog();

  // Check for updates
  void updateCheckFinished();
#pragma endregion PRIVATE_SLOTS
};
