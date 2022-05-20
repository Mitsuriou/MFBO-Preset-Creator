#pragma once
#include "TitleDialog.h"
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

// File downloading made with the great help of
// https://www.bogotobogo.com/Qt/Qt5_QNetworkRequest_Http_File_Download.php

class Update final : public TitleDialog
{
  Q_OBJECT

public:
  explicit Update(QWidget* aParent,
                  const Struct::Settings& aSettings,
                  std::map<QString, QString>* aLastPaths,
                  const bool aForceStableContext,
                  const bool aForceBetaContext);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  const bool mForceStableContext;
  const bool mForceBetaContext;
  QString mSaveFilePath;
  QNetworkAccessManager mManager;

  // File download attributes
  QUrl mDownloadURL;
  bool mHasDownloadBeenCanceled{false};
  QFile* mDownloadedFile{nullptr};
  QNetworkReply* mReply{nullptr};

  // GUI functions
  void initializeGUI();

  // Check for updates
  void checkForUpdate();
  void updateCheckFinished();
  void displayUpdateMessage(const bool aSucceeded, const QString& aResult);

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
