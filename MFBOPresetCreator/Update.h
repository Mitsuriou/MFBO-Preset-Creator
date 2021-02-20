#pragma once

#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <iostream>
#include <string>

class Update : public QDialog
{
  Q_OBJECT

public:
  explicit Update(QWidget* parent, const Struct::Settings& aSettings);

private:
  const Struct::Settings mSettings;
  QString mNewVersionTag;
  QString mSaveFilePath;
  QNetworkAccessManager mManager;
  // WIP:
  bool mHasDownloadBeenCanceled;
  QFile* mDownloadedFile;
  qint64 mDownloadedFileSize;
  QNetworkReply* mReply;

  QUrl mDownloadURL;

  struct MemoryStruct
  {
    char* memory;
    size_t size;
  };

  void setWindowProperties();
  void setupInterface();
  void overrideHTMLLinksColor(QString& aHTMLString);

  // Check for updates
  void displayUpdateMessage(const QString& aResult);

  // Download the update
  void aaaa(const bool& aResult);

private slots:
  // Check for updates
  void checkForUpdate();
  void updateCheckSuccess();
  void updateCheckError(QNetworkReply::NetworkError aErrorCode);

  // Download the update
  void downloadLatestUpdate();
  void cancelDownload();
  void installLatestUpdate();
  void fileChunkDownloaded();
  void chunckSizeUpdated(qint64, qint64);
  void fileDownloadSuccess();
};
