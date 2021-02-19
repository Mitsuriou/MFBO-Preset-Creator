#pragma once

#include "HTTPRequesterFile.h"
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

private slots:
  // Check for updates
  void checkForUpdate();
  void updateCheckSuccess(QNetworkReply* aReply);
  void updateCheckError(QNetworkReply::NetworkError aErrorCode);

  // Download the update
  void downloadLatestUpdate();
  void fileFetched(const bool& aResult);
  void installLatestUpdate();
};
