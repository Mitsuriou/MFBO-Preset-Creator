#pragma once

#include <QString>
#include <QStringList>

#ifdef MAKE_TEST_LIB
#define TEST_LIB_EXPORT Q_DECL_EXPORT
#else
#define TEST_LIB_EXPORT Q_DECL_IMPORT
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  class TEST_LIB_EXPORT DataLists
  {
  public:
    static QStringList getCBBE3BBBVersions();
    static QStringList getWindowOpeningModes();
    static QStringList getAppThemes();
    static QString tt();

  private:
    DataLists(){};
  };

#ifdef __cplusplus
}
#endif