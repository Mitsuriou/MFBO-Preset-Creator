#pragma once
#include "Struct.h"
#include <QDialog>
#include <QLabel>
#include <QNetworkAccessManager>

class WelcomeScreen final : public QDialog
{
  Q_OBJECT

public:
  explicit WelcomeScreen(QWidget* aParent, const Struct::Settings& aSettings);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  const StartupAction mInitializationStartupAction;
  QNetworkAccessManager mManager;

  // GUI functions
  void setWindowProperties();
  void initializeGUI();
  // Utils functions
  QLabel* createTitleLabel(QWidget* aParent, const QString& aText, const int aAppFontSize);
  void overrideHTMLLinksColor(QString& aHTMLString);
  // Dialog
  void launchUpdateDialog();
  void openGoogleDriveGuide();
  // Check for updates
  void checkForUpdate();
  void updateCheckFinished();
  void displayUpdateMessage(const QString& aResult);

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);
};
