#pragma once
#include "TitleDialog.h"
#include <QLabel>
#include <QNetworkAccessManager>

class WelcomeScreen final : public TitleDialog
{
  Q_OBJECT

public:
  explicit WelcomeScreen(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  QNetworkAccessManager mManager;

  // GUI functions
  void initializeGUI();

  // Utils functions
  QLabel* createTitleLabel(QWidget* aParent, const QString& aText, const int aAppFontSize);

  // Dialog
  void openKoFiPage();
  void launchUpdateDialog();
  void openIncomingFeatures();
  void openKnownIssues();
  void openGoogleDriveGuide();

  // Check for updates
  void checkForUpdate();
  void updateCheckFinished();
  void displayUpdateMessage(const bool aSucceeded, const QString& aResult);

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);
};
