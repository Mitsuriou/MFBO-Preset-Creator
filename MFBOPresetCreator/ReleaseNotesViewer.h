#pragma once
#include "TitleDialog.h"
#include <QNetworkAccessManager>

class ReleaseNotesViewer final : public TitleDialog
{
  Q_OBJECT

public:
  explicit ReleaseNotesViewer(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  QNetworkAccessManager mManager;

  void setWindowProperties();
  void initializeGUI();
  // Check for updates
  void checkForUpdate();
  void updateCheckFinished();
  void displayUpdateMessage(const bool aSucceeded, const QString& aResult);
  // Callback functions
  void viewInDefaultBrowser();
};
