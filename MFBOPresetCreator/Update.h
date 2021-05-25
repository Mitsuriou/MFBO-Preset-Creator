#pragma once

#include "ComponentFactory.h"
#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <iostream>
#include <string>

// File downloading made with the great help of
// https://www.bogotobogo.com/Qt/Qt5_QNetworkRequest_Http_File_Download.php

class Update : public QDialog
{
  Q_OBJECT

public:
  explicit Update(QWidget* parent, const Struct::Settings& aSettings);

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
  void setupInterface();
  void overrideHTMLLinksColor(QString& aHTMLString);

  // Check for updates
  void displayUpdateMessage(const QString& aResult);

  // Download the update file
  void displayFileDownloadEndStatus(const bool aResult);

private slots:
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
};
