#pragma once

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>

#define CURL_STATICLIB
#include <curl\curl.h>

class HTTPRequesterGet : public QThread
{
  Q_OBJECT

public:
  explicit HTTPRequesterGet(const QString& aURL, QWidget* aParent);
  ~HTTPRequesterGet();

protected:
  void run() override;

private:
  void* mCURL;
  std::string mURL;

  std::string download();

signals:
  void resultReady(const QString&);
};
