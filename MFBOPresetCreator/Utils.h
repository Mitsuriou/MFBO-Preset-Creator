﻿#pragma once

#include "Enum.h"
#include "Struct.h"
#include "buildnumber.h"
#include "stdafx.h"
#include <QtXml/QDomDocument>

class Utils : public QObject
{
  Q_OBJECT

public:
  // General functions
  static void cleanPathString(QString& aPath);
  static QString getSoftwareVersion();
  static void displayWarningMessage(const QString& aMessage);
  static int getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension);
  static bool copyRecursively(const QString& aSourcePath, const QString& aDestinationPath);
  static bool isThemeDark(const GUITheme aTheme);

  // XML and OSP file parse
  static QString getPresetNameFromXMLFile(const QString& aPath);
  static std::vector<Struct::SliderSet> getOutputPathsFromOSPFile(const QString& aPath);
  static bool isPresetUsingBeastHands(const QString& aPath);

  // Settings
  static void checkSettingsFileExistence();
  static Struct::Settings loadSettingsFromFile();
  static void saveSettingsToFile(Struct::Settings aSettings);
  static QJsonObject settingsStructToJson(Struct::Settings aSettings);

  // Language
  static QString getShortLanguageNameFromEnum(const int aEnumValue);
  static QString getLongLanguageNameFromEnum(const int aEnumValue);
  static ApplicationLanguage getStructLanguageFromName(const QString& aShortName);

private:
  Utils(){};
};