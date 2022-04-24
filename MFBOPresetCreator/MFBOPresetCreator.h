#pragma once
#include "Struct.h"
#include <QLineEdit>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "ui_MFBOPresetCreator.h"

class MFBOPresetCreator final : public QMainWindow
{
  Q_OBJECT

public:
  explicit MFBOPresetCreator(const Struct::Settings& aSettings,
                             const QString& aInjectedFilePath,
                             QWidget* aParent = nullptr);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void dragEnterEvent(QDragEnterEvent* aEvent) override;
  void dropEvent(QDropEvent* aEvent) override;

private:
  Ui::MFBOPresetCreatorClass ui;
  Struct::Settings mSettings;
  QString mInjectedFilePath;
  std::map<QString, QString> mLastPaths;
  bool mNewStableVersionAvailable;
  bool mNewBetaVersionAvailable;
  QNetworkAccessManager mManager;

  // GUI
  void initializeGUI();
  void setupMenuBar();
  void showWindow();
  void launchWelcomeScreen();
  void applyGlobalStyleSheet();
  void applyFont(const QString& aFamily,
                 const QString& aStyleName,
                 const int aSize,
                 const int aWeight,
                 const bool aItalic,
                 const bool aUnderline,
                 const bool aStrikeOut);
  std::vector<QLineEdit*> disableLineEditPlaceholders();
  void enableLineEditPlaceholders(std::vector<QLineEdit*> aLineEditsToReactivate);

  // Check for updates
  void checkForUpdate();
  void updateCheckFinished();
  void displayUpdateMessage(const bool aSucceeded, const QString& aResult);

  // Base functionalities
  void loadProject();
  void saveProject(const bool aIsQuittingContext = false);
  void quickRelaunch();

  // Assisted Conversion
  void launchAssistedConversion();
  void fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList);

  // Tools
  void launchBatchConversion();
  void launchPresetsRetargeting();
  void launchTexturesAssistant();
  void injectDataFromOSPFile();

  // Dialogs
  void launchSettingsDialog();
  void launchUpdateDialog();
  void launchCurrentVersionReleaseNotes();
  void launchAboutDialog();
  void launchAboutQtDialog();

  // Settings callback
  void refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings);

  // URL links
  void reportBugNexusMods();
  void reportBugGitHub();
  void reportBugGitLab();
  void openNexusModsPage();
  void openGitHubSourceCodePage();
  void openGitLabSourceCodePage();
  void openGoogleDriveGuide();
  void openKoFiPage();
};
