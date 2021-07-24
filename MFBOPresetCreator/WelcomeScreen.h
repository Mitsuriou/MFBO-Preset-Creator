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
  // Check for updates
  QNetworkAccessManager mManager;

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
  void displayUpdateMessage(const QString& aResult);
  void updateCheckFinished();

  // GUI widgets events
  void groupBoxChecked(bool aIsChecked);

signals:
  void refreshMainUI(Struct::Settings aSettings, bool aMustUpdateSettings);
};
