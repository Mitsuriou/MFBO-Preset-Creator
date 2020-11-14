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
  static const QString getSoftwareVersion();
  static void displayWarningMessage(const QString& aMessage);
  static const int getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension);
  static const bool copyRecursively(const QString& aSourcePath, const QString& aDestinationPath);
  static const bool isThemeDark(const GUITheme& aTheme);
  static const QString getIconFolder(const GUITheme& aTheme);

  // XML and OSP file parse
  static const QString getPresetNameFromXMLFile(const QString& aPath);
  static const std::vector<Struct::SliderSet> getOutputPathsFromOSPFile(const QString& aPath);
  static const bool isPresetUsingBeastHands(const QString& aPath);

  // Settings
  static void checkSettingsFileExistence();
  static const Struct::Settings loadSettingsFromFile();
  static void saveSettingsToFile(Struct::Settings aSettings);
  static const QJsonObject settingsStructToJson(Struct::Settings aSettings);

  // Filters
  static const QStringList loadFiltersFromFile();
  static void saveFiltersToFile(QStringList aList);
  static const QString getFilterBlockFromBody(const int& aBody, const int& aBeastHands, const QString& aGroupName);

  // Language
  static const QString getShortLanguageNameFromEnum(const int aEnumValue);
  static const QString getLongLanguageNameFromEnum(const int aEnumValue);
  static const ApplicationLanguage getStructLanguageFromName(const QString& aShortName);

private:
  Utils(){};
};
