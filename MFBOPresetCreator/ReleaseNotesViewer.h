#pragma once
#include "Struct.h"
#include <QDialog>
#include <QNetworkAccessManager>

class ReleaseNotesViewer final : public QDialog
{
  Q_OBJECT

public:
  explicit ReleaseNotesViewer(QWidget* aParent, const Struct::Settings& aSettings);

private:
  const Struct::Settings mSettings;
  QNetworkAccessManager mManager;

  void setWindowProperties();
  void initializeGUI();
  // Check for updates
  void checkForUpdate();
  void updateCheckFinished();
  void displayUpdateMessage(const bool aSucceeded, const QString& aResult);
  // Callback functions
  void viewInDefaultBrowser();

  explicit ReleaseNotesViewer(const ReleaseNotesViewer&) = delete;
  ReleaseNotesViewer& operator=(const ReleaseNotesViewer&) = delete;
};
