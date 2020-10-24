#include "DataLists.h"

QStringList DataLists::getCBBE3BBBVersions()
{
  QStringList lVersions;
  lVersions.append(QString("1.40"));
  lVersions.append(QString("1.50"));
  lVersions.append(QString("1.51 - 1.52"));

  return lVersions;
}

QStringList DataLists::getWindowOpeningModes()
{
  QStringList lWindowModes;
  lWindowModes.append(tr("Minimized"));
  lWindowModes.append(tr("Windowed"));
  lWindowModes.append(tr("Maximized"));

  return lWindowModes;
}

QStringList DataLists::getAppThemes()
{
  QStringList lAppThemes;
  lAppThemes.append(QString("Windows Vista"));
  lAppThemes.append(QString("Paper Light by 6788"));
  lAppThemes.append(QString("Paper Dark by 6788"));
  lAppThemes.append(QString("Paper White Mono"));
  lAppThemes.append(QString("Paper Black Mono"));
  lAppThemes.append(QString("Alexhuszagh's Breeze Light"));
  lAppThemes.append(QString("Alexhuszagh's Breeze Dark"));
  lAppThemes.append(QString("QuasarApp's Dark Style"));
  lAppThemes.append(QString("QuasarApp's Material Style"));
  lAppThemes.append(QString("QuasarApp's Visual Studio Dark"));

  return lAppThemes;
}

QStringList DataLists::getLanguages()
{
  QStringList lLanguages;
  lLanguages.append(tr("English"));
  lLanguages.append(tr("Français"));

  return lLanguages;
}
