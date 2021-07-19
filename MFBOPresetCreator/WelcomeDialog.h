#pragma once
#include "Struct.h"
#include <QDialog>
#include <QLabel>
#include <QNetworkAccessManager>

class WelcomeDialog final : public QDialog
{
  Q_OBJECT

public:
  explicit WelcomeDialog(QWidget* aParent, const Struct::Settings& aSettings);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  // Check for updates
  QNetworkAccessManager mManager;

  void setWindowProperties();
  void initializeGUI();
  // Utils functions
  QLabel* createTitleLabel(QWidget* aParent, const QString& aText, const int aAppFontSize);
  void overrideHTMLLinksColor(QString& aHTMLString);
  // Dialog
  void launchUpdateDialog();
  // Check for updates
  void checkForUpdate();
  void displayUpdateMessage(const QString& aResult);
  void updateCheckFinished();

  // GUI widgets events
  void scrollbarPressed();
  void scrollbarReleased();
  void groupBoxChecked(bool aIsChecked);
};
