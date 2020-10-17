#pragma once

#include <QStringList>

#ifdef CPLUSPLUS
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

class __declspec(dllexport) DataLists
{
public:
  static QStringList getCBBE3BBBVersions();
  static QStringList getWindowOpeningModes();
  static QStringList getAppThemes();
};
