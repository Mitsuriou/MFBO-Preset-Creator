#pragma once

#include "Struct.h"
#include "Enum.h"

#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QDirIterator>
#include <QStringList>
#include <QFile>
#include <QPair>
#include <QtXml/QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>

class Utils : public QObject
{
  Q_OBJECT

public:
  // General functions
  static void cleanPathString(QString& aPath);
  static QString getProgramVersion();
  static void displayWarningMessage(QString aMessage);
  static int getNumberFilesByExtension(QString aRootDir, QString aFileExtension);
  static bool copyRecursively(QString sourceFolder, QString destFolder);
  static bool isThemeDark(GUITheme aTheme);

  // XML and OSP file parse
  static QString getPresetNameFromXMLFile(QString aPath);
  static std::vector<Struct::SliderSet> getOutputPathsFromOSPFile(QString aPath);
  static bool isPresetUsingBeastHands(QString aPath);

  // Settings
  static void checkSettingsFileExistence();
  static Struct::Settings loadSettingsFromFile();
  static void saveSettingsToFile(Struct::Settings aSettings);
  static QJsonObject settingsStructToJson(Struct::Settings aSettings);

  // List
  static QStringList getCBBE3BBBVersions();
  static QStringList getWindowOpeningModes();
  static QStringList getAppThemes();

  // Language
  static QString getShortLanguageNameFromEnum(int aEnumValue);
  static QString getLongLanguageNameFromEnum(int aEnumValue);
  static ApplicationLanguage getStructLanguageFromName(QString aShortName);
};
