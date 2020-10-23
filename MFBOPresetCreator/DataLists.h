#pragma once

#include "qstring.h"
#include "qstringlist.h"

class DataLists
{
public:
  static QStringList getCBBE3BBBVersions();
  static QStringList getWindowOpeningModes();
  static QStringList getAppThemes();

private:
  DataLists(){};
};
