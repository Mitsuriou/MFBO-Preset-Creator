#pragma once

#include "About.h"
#include "AssistedConversion.h"
#include "PresetCreator.h"
#include "RetargetingTool.h"
#include "Settings.h"
#include "TexturesAssistant.h"
#include "Update.h"
#include "Utils.h"
#include "WinUser.h"
#include "stdafx.h"
#include "windows.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "ui_MFBOPresetCreator.h"

class MFBOPresetCreator : public QMainWindow
{
  Q_OBJECT

public:
  explicit MFBOPresetCreator(const Struct::Settings& aSettings, const QString& aInjectedFilePath, QWidget* parent = Q_NULLPTR);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  Ui::MFBOPresetCreatorClass ui;
  Struct::Settings mSettings;
  QString mInjectedFilePath;
  std::map<QString, QString> mLastPaths;
  bool mNewVersionAvailable;
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

public slots:
  void refreshUI(Struct::Settings aSettings, bool aMustUpdateSettings);

private slots:
  // Base functionalities
  void loadProject();
  void saveProject();
  void quickRelaunch();
  // Assisted Conversion
  void launchAssistedConversion();
  void fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList);
  // Other tools
  void launchPresetsRetargeting();
  void launchTexturesAssistant();
  // Dialog
  void launchSettingsDialog();
  void launchUpdateDialog();
  // URL links
  void openNexusPageInDefaultBrowser();
  void openGitHubSourceCodePageInDefaultBrowser();
  void openGitLabSourceCodePageInDefaultBrowser();
  void openGuideInDefaultBrowser();
  // Dialog
  void launchAboutDialog();

  // Check for updates
  void updateCheckFinished();
};
