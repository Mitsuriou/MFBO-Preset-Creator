#include "DataLists.h"

QStringList DataLists::getBodiesNameVersions()
{
  QStringList lBodies;
  lBodies.append(QString("CBBE 3BBB 3BA [v.1.40]"));
  lBodies.append(QString("CBBE 3BBB 3BA [v.1.50]"));
  lBodies.append(QString("CBBE 3BBB 3BA [v.1.51 -> v.1.52]"));
  lBodies.append(QString("CBBE SMP (3BBB) [v.1.2.0]"));

  return lBodies;
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
  lAppThemes.append(QString("QuasarApp's Visual Studio Dark"));
  lAppThemes.append(QString("Mitsuriou's Light Theme"));
  lAppThemes.append(QString("Mitsuriou's Dark Theme"));

  return lAppThemes;
}

QStringList DataLists::getLanguages()
{
  QStringList lLanguages;
  lLanguages.append("English");
  lLanguages.append("Français");

  return lLanguages;
}

QStringList DataLists::getAssistedConversionActions()
{
  QStringList lActions;
  lActions.append(tr("[ignore]"));
  lActions.append(tr("Body mesh"));
  lActions.append(tr("Feet mesh"));
  lActions.append(tr("Hands mesh"));
  lActions.append(tr("Skeleton mesh"));

  return lActions;
}
