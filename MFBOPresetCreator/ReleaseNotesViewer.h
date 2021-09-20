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
  // Utils functions
  void overrideHTMLLinksColor(QString& aHTMLString);
  // Check for updates
  void checkForUpdate();
  void updateCheckFinished();
  void displayUpdateMessage(const QString& aResult);

  explicit ReleaseNotesViewer(const ReleaseNotesViewer&) = delete;
  ReleaseNotesViewer& operator=(const ReleaseNotesViewer&) = delete;
};
