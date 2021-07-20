#pragma once
#include "Struct.h"
#include <QDialog>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

// File downloading made with the great help of
// https://www.bogotobogo.com/Qt/Qt5_QNetworkRequest_Http_File_Download.php

class Update final : public QDialog
{
  Q_OBJECT

public:
  explicit Update(QWidget* aParent, const Struct::Settings& aSettings, const bool aIsBetaContext);

protected:
  void reject() override;

private:
  const Struct::Settings mSettings;
  QString mSaveFilePath;
  QNetworkAccessManager mManager;

  // File download attributes
  QUrl mDownloadURL;
  bool mHasDownloadBeenCanceled;
  QFile* mDownloadedFile;
  QNetworkReply* mReply;

  void setWindowProperties();
  void setupInterface(const bool aIsBetaContext);
  void overrideHTMLLinksColor(QString& aHTMLString);

  // Check for updates
  void displayUpdateMessage(const QString& aResult);

  // Download the update file
  void displayFileDownloadEndStatus(const bool aResult);

#pragma region PRIVATE_SLOTS
  // Check for updates
  void checkForUpdate();
  void updateCheckFinished();

  // Download the update file
  void downloadLatestUpdate();
  void cancelCurrentDownload();
  void fileChunkReceived();
  void chunkSizeUpdated(qint64 aBytesRead, qint64 aTotal);
  void fileDownloadEnded();

  // Install the update file
  void installLatestUpdate();
#pragma endregion PRIVATE_SLOTS
};
