#pragma once

#include "HTTPRequesterFile.h"
#include "HTTPRequesterGet.h"
#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"
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

  struct MemoryStruct
  {
    char* memory;
    size_t size;
  };

  void setWindowProperties();
  void setupInterface();
  void overrideHTMLLinksColor(QString& aHTMLString);

private slots:
  void getLastAvailableVersion();
  void pageFetched(const QString&);
  void downloadLatestUpdate();
  void fileFetched(const bool& aResult);
  void installLatestUpdate();
};
