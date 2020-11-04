#pragma once

#include "HTTPDownloader.hpp"
#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"
#include <iostream>
#include <string>

class Update : public QDialog
{
  Q_OBJECT

public:
  Update(QWidget* parent, const Struct::Settings& aSettings);

private:
  const Struct::Settings mSettings;

  struct MemoryStruct
  {
    char* memory;
    size_t size;
  };

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QVBoxLayout& aLayout);

private slots:
  void getLastAvailableVersion();
  void pageFetched(const QString&);
  void openGithubInBrowser();
};
