#pragma once

#include "Enum.h"
#include "Struct.h"
#include "stdafx.h"
#include <QtXml/QDomDocument>

class Utils : public QObject
{
  Q_OBJECT

public:
  // General functions
  static void cleanPathString(QString& aPath);
  static QString cleanPathString(const QString& aPath);
  static QString getApplicationVersion();
  static void displayWarningMessage(const QString& aMessage);
  static int getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension);
  static int getNumberFilesByExtensions(const QString& aRootDir, const QStringList& aFileExtensions);
  static bool copyRecursively(const QString& aSourcePath, const QString& aDestinationPath);
  static bool isThemeDark(const GUITheme& aTheme);
  static QString getIconFolder(const GUITheme& aTheme);

  // XML and OSP file parse
  static QString getPresetNameFromXMLFile(const QString& aPath);
  static std::vector<Struct::SliderSet> getOutputPathsFromOSPFile(const QString& aPath);
  static bool isPresetUsingBeastHands(const QString& aPath);

  // Settings
  static void checkSettingsFileExistence();
  static Struct::Settings loadSettingsFromFile();
  static void saveSettingsToFile(Struct::Settings aSettings);
  static QJsonObject settingsStructToJson(Struct::Settings aSettings);

  // Filters
  static QStringList loadFiltersFromFile();
  static void saveFiltersToFile(QStringList aList);
  static QString getFilterBlockFromBody(const int& aBody, const int& aBeastHands, const QString& aGroupName);

  // Language
  static QString getShortLanguageNameFromEnum(const int& aEnumValue);
  static QString getLongLanguageNameFromEnum(const int& aEnumValue);
  static ApplicationLanguage getStructLanguageFromName(const QString& aShortName);

private:
  Utils(){};
};
