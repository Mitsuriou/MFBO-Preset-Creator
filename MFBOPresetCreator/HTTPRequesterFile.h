#pragma once

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define CURL_STATICLIB
#include <curl\curl.h>

class HTTPRequesterFile : public QThread
{
  Q_OBJECT

public:
  explicit HTTPRequesterFile(const QString& aURL, QWidget* aParent);
  ~HTTPRequesterFile();

protected:
  void run() override;

private:
  void* mCURL;
  std::string mURL;

  bool download();

signals:
  void resultReady(const bool&);
};
