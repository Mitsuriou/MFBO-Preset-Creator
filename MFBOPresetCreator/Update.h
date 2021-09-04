#pragma once
#include "Struct.h"
#include <QDialog>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>

// File downloading made with the great help of
// https://www.bogotobogo.com/Qt/Qt5_QNetworkRequest_Http_File_Download.php

class Update final : public QDialog
{
  Q_OBJECT

public:
  explicit Update(QWidget* aParent, const Struct::Settings& aSettings, const bool aForceStableContext, const bool aForceBetaContext);
  ~Update();

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const bool mForceStableContext;
  const bool mForceBetaContext;
  const Struct::Settings mSettings;
  QString mSaveFilePath;
  QNetworkAccessManager mManager;
  QWinTaskbarButton* mTaskbarAppButton;
  QWinTaskbarProgress* mTaskbarProgressBar;

  // File download attributes
  QUrl mDownloadURL;
  bool mHasDownloadBeenCanceled;
  QFile* mDownloadedFile;
  QNetworkReply* mReply;

  // GUI functions
  void setWindowProperties();
  void initializeGUI();
  void overrideHTMLLinksColor(QString& aHTMLString);

  // Check for updates
  void checkForUpdate();
  void updateCheckFinished();
  void displayUpdateMessage(const QString& aResult);

  // Download the update file
  void downloadLatestUpdate();
  void cancelCurrentDownload();
  void fileChunkReceived();
  void chunkSizeUpdated(qint64 aBytesRead, qint64 aTotal);
  void fileDownloadEnded();
  void displayFileDownloadEndStatus(const bool aResult);

  // Install the update file
  void installLatestUpdate();
  void openStandaloneOutputDirectory();
};
