#include "Utils.h"

void Utils::cleanPathString(QString& aPath)
{
  aPath.replace("\\", "/");
}

QString Utils::getSoftwareVersion()
{
  return "1.8.2";
}

void Utils::displayWarningMessage(const QString& aMessage)
{
  QMessageBox lMessageBox(QMessageBox::Icon::Warning, tr("Warning"), aMessage);
  lMessageBox.exec();
}

int Utils::getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension)
{
  auto lNumber{0};
  auto lAbsFilePath{QString("")};
  auto lRelativeDirs{QString("")};

  QDirIterator it(aRootDir, QStringList() << QString("*." + aFileExtension), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();

    // Ignore FOMOD directory
    lAbsFilePath = it.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.remove(aRootDir, Qt::CaseInsensitive);

    if (lRelativeDirs.contains("fomod", Qt::CaseInsensitive))
    {
      continue;
    }

    lNumber++;
  }

  return lNumber;
}

bool Utils::copyRecursively(const QString& aSourcePath, const QString& aDestinationPath)
{
  auto lIsSuccess{false};
  QDir lSourceDirectory(aSourcePath);

  if (!lSourceDirectory.exists())
  {
    return false;
  }

  QDir lDestinationDirectory(aDestinationPath);
  if (!lDestinationDirectory.exists())
  {
    lDestinationDirectory.mkdir(aDestinationPath);
  }

  auto lFilesList{lSourceDirectory.entryList(QDir::Files)};
  auto lFilesListSize{lFilesList.count()};

  for (int i = 0; i < lFilesListSize; i++)
  {
    auto lSourceName{aSourcePath + QDir::separator() + lFilesList[i]};
    auto lDestinationName{aDestinationPath + QDir::separator() + lFilesList[i]};
    lIsSuccess = QFile::copy(lSourceName, lDestinationName);

    if (!lIsSuccess)
    {
      Utils::displayWarningMessage(tr("An unknown error has occurred while creating the backup. Process aborted."));
      return false;
    }
  }

  lFilesList.clear();
  lFilesList = lSourceDirectory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
  for (int i = 0; i < lFilesListSize; i++)
  {
    auto lSourceName{aSourcePath + QDir::separator() + lFilesList[i]};
    auto lDestinationName{aDestinationPath + QDir::separator() + lFilesList[i]};
    lIsSuccess = copyRecursively(lSourceName, lDestinationName);

    if (!lIsSuccess)
    {
      Utils::displayWarningMessage(tr("An unknown error has occurred while creating the backup. Process aborted."));
      return false;
    }
  }

  return true;
}

bool Utils::isThemeDark(const GUITheme& aTheme)
{
  switch (aTheme)
  {
    case GUITheme::PaperDark:
    case GUITheme::PaperBlackMono:
    case GUITheme::AlexhuszaghBreezeDark:
    case GUITheme::QuasarAppDarkStyle:
    case GUITheme::QuasarAppMaterialStyle:
    case GUITheme::QuasarAppVisualStudioDark:
      return true;
    default:
      return false;
  }
}

QString Utils::getPresetNameFromXMLFile(const QString& aPath)
{
  QFile lReadFile(aPath);
  lReadFile.setPermissions(QFile::WriteUser);

  QDomDocument lXMLDocument;

  if (lReadFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    lXMLDocument.setContent(&lReadFile);
    lReadFile.close();
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to open the file \"") + aPath + tr("\"."));
    return "";
  }

  auto lXMLGroup = lXMLDocument.documentElement().firstChild().toElement();
  auto lPresetName{QString("")};

  if (lXMLGroup.tagName() == "Group")
  {
    // Get the first "Member" tag to read its "name" attribute
    QDomElement lXMLMember = lXMLGroup.firstChild().toElement();
    lPresetName = lXMLMember.attribute("name", "");
  }

  return lPresetName.left(lPresetName.lastIndexOf(QChar('-')) - 1);
}

std::vector<Struct::SliderSet> Utils::getOutputPathsFromOSPFile(const QString& aPath)
{
  std::vector<Struct::SliderSet> lPaths;

  QFile lReadFile(aPath);
  lReadFile.setPermissions(QFile::WriteUser);

  QDomDocument lOSPDocument;

  if (lReadFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    lOSPDocument.setContent(&lReadFile);
    lReadFile.close();
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to open the file \"") + aPath + tr("\"."));
    return std::vector<Struct::SliderSet>();
  }

  auto lRoot{lOSPDocument.documentElement()};
  auto lSliderSet{lRoot.firstChild().toElement()};

  while (!lSliderSet.isNull())
  {
    if (lSliderSet.tagName() == "SliderSet")
    {
      Struct::SliderSet lTempSet;

      lTempSet.name = lSliderSet.attribute("name", "");
      if (lTempSet.name.contains("body", Qt::CaseInsensitive))
      {
        lTempSet.meshpart = "Body";
      }
      else if (lTempSet.name.contains("hands", Qt::CaseInsensitive))
      {
        lTempSet.meshpart = "Hands";
      }
      else if (lTempSet.name.contains("feet", Qt::CaseInsensitive))
      {
        lTempSet.meshpart = "Feet";
      }

      auto lChild{lSliderSet.firstChild().toElement()};
      while (!lChild.isNull())
      {
        if (lChild.tagName() == "OutputPath")
        {
          lTempSet.outputpath = lChild.firstChild().toText().data();
        }
        else if (lChild.tagName() == "OutputFile")
        {
          lTempSet.outputfile = lChild.firstChild().toText().data();
        }

        if (lTempSet.outputpath != "" && lTempSet.outputfile != "")
        {
          break;
        }

        lChild = lChild.nextSiblingElement();
      }

      lPaths.push_back(lTempSet);
    }

    lSliderSet = lSliderSet.nextSiblingElement();
  }

  return lPaths;
}

bool Utils::isPresetUsingBeastHands(const QString& aPath)
{
  QFile lReadFile(aPath);
  lReadFile.setPermissions(QFile::WriteUser);

  auto lFileContent{QString("")};

  if (lReadFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    lFileContent = static_cast<QString>(lReadFile.readAll());
    lReadFile.close();
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to open the file \"") + aPath + tr("\"."));
    return false;
  }

  if (lFileContent.contains("beast hands", Qt::CaseInsensitive) || lFileContent.contains("hands beast", Qt::CaseInsensitive))
  {
    return true;
  }

  return false;
}

void Utils::checkSettingsFileExistence()
{
  QFile lSettingsFile(QCoreApplication::applicationDirPath() + QDir::separator() + "config.json");

  if (!lSettingsFile.exists())
  {
    // Create a default setting file if it does not exist
    Struct::Settings lSettings;
    Utils::saveSettingsToFile(lSettings);
  }
}

Struct::Settings Utils::loadSettingsFromFile()
{
  Utils::checkSettingsFileExistence();

  // Open and read the settings file
  QFile lSettingsFile(QCoreApplication::applicationDirPath() + QDir::separator() + "config.json");
  lSettingsFile.open(QIODevice::ReadOnly | QIODevice::Text);
  QString lSettingsData = lSettingsFile.readAll();
  lSettingsFile.close();

  QJsonDocument lJsonDocument(QJsonDocument::fromJson(lSettingsData.toUtf8()));
  QJsonObject lSettingsJSON = lJsonDocument.object();

  Struct::Settings lSettings;

  // Language
  if (lSettingsJSON.contains("lang") && lSettingsJSON["lang"].isDouble())
  {
    auto lFoundLanguage{lSettingsJSON["lang"].toInt()};

    switch (lFoundLanguage)
    {
      case static_cast<int>(ApplicationLanguage::English):
        lSettings.language = ApplicationLanguage::English;
        break;
      case static_cast<int>(ApplicationLanguage::French):
        lSettings.language = ApplicationLanguage::French;
        break;
      default:
        lSettings.language = ApplicationLanguage::English;
        break;
    }
  }

  // Font family
  if (lSettingsJSON.contains("font"))
  {
    auto lFontJSON = lSettingsJSON["font"].toObject();

    if (lFontJSON.contains("family") && lFontJSON["family"].isString())
    {
      lSettings.font.family = lFontJSON["family"].toString();
    }

    if (lFontJSON.contains("styleName") && lFontJSON["styleName"].isString())
    {
      lSettings.font.styleName = lFontJSON["styleName"].toString();
    }

    if (lFontJSON.contains("size") && lFontJSON["size"].isDouble())
    {
      lSettings.font.size = lFontJSON["size"].toInt();
    }

    if (lFontJSON.contains("weight") && lFontJSON["weight"].isDouble())
    {
      lSettings.font.weight = lFontJSON["weight"].toInt();
    }

    if (lFontJSON.contains("italic") && lFontJSON["italic"].isBool())
    {
      lSettings.font.italic = lFontJSON["italic"].toBool();
    }

    if (lFontJSON.contains("underline") && lFontJSON["underline"].isBool())
    {
      lSettings.font.underline = lFontJSON["underline"].toBool();
    }

    if (lFontJSON.contains("strikeOut") && lFontJSON["strikeOut"].isBool())
    {
      lSettings.font.strikeOut = lFontJSON["strikeOut"].toBool();
    }
  }

  // Dark theme
  if (lSettingsJSON.contains("appTheme") && lSettingsJSON["appTheme"].isDouble())
  {
    auto lFoundAppTheme{lSettingsJSON["appTheme"].toInt()};

    switch (lFoundAppTheme)
    {
      case static_cast<int>(GUITheme::WindowsVista):
        lSettings.appTheme = GUITheme::WindowsVista;
        break;
      case static_cast<int>(GUITheme::PaperLight):
        lSettings.appTheme = GUITheme::PaperLight;
        break;
      case static_cast<int>(GUITheme::PaperDark):
        lSettings.appTheme = GUITheme::PaperDark;
        break;
      case static_cast<int>(GUITheme::PaperWhiteMono):
        lSettings.appTheme = GUITheme::PaperWhiteMono;
        break;
      case static_cast<int>(GUITheme::PaperBlackMono):
        lSettings.appTheme = GUITheme::PaperBlackMono;
        break;
      case static_cast<int>(GUITheme::AlexhuszaghBreezeLight):
        lSettings.appTheme = GUITheme::AlexhuszaghBreezeLight;
        break;
      case static_cast<int>(GUITheme::AlexhuszaghBreezeDark):
        lSettings.appTheme = GUITheme::AlexhuszaghBreezeDark;
        break;
      case static_cast<int>(GUITheme::QuasarAppDarkStyle):
        lSettings.appTheme = GUITheme::QuasarAppDarkStyle;
        break;
      case static_cast<int>(GUITheme::QuasarAppMaterialStyle):
        lSettings.appTheme = GUITheme::QuasarAppMaterialStyle;
        break;
      case static_cast<int>(GUITheme::QuasarAppVisualStudioDark):
        lSettings.appTheme = GUITheme::QuasarAppVisualStudioDark;
        break;
      default:
        lSettings.appTheme = GUITheme::WindowsVista;
        break;
    }
  }

  // Default window width
  if (lSettingsJSON.contains("windowWidth") && lSettingsJSON["windowWidth"].isDouble())
  {
    lSettings.mainWindowWidth = lSettingsJSON["windowWidth"].toInt();
  }

  // Default window height
  if (lSettingsJSON.contains("windowHeight") && lSettingsJSON["windowHeight"].isDouble())
  {
    lSettings.mainWindowHeight = lSettingsJSON["windowHeight"].toInt();
  }

  // Main window opening mode
  if (lSettingsJSON.contains("main_window_opening_mode") && lSettingsJSON["main_window_opening_mode"].isDouble())
  {
    auto lFoundWindowOpeningMode{lSettingsJSON["main_window_opening_mode"].toInt()};

    switch (lFoundWindowOpeningMode)
    {
      case static_cast<int>(WindowOpeningMode::Minimized):
        lSettings.mainWindowOpeningMode = WindowOpeningMode::Minimized;
        break;
      case static_cast<int>(WindowOpeningMode::Windowed):
        lSettings.mainWindowOpeningMode = WindowOpeningMode::Windowed;
        break;
      case static_cast<int>(WindowOpeningMode::Maximized):
        lSettings.mainWindowOpeningMode = WindowOpeningMode::Maximized;
        break;
      default:
        lSettings.mainWindowOpeningMode = WindowOpeningMode::Windowed;
    }
  }

  // Default CBBE 3BBB Version
  if (lSettingsJSON.contains("default_3bbb_version") && lSettingsJSON["default_3bbb_version"].isDouble())
  {
    auto lFoundVersion{lSettingsJSON["default_3bbb_version"].toInt()};

    switch (lFoundVersion)
    {
      case static_cast<int>(CBBE3BBBVersion::Version1_40):
        lSettings.defaultMainWindowCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
        break;
      case static_cast<int>(CBBE3BBBVersion::Version1_50):
        lSettings.defaultMainWindowCBBE3BBBVersion = CBBE3BBBVersion::Version1_50;
        break;
      case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
        lSettings.defaultMainWindowCBBE3BBBVersion = CBBE3BBBVersion::Version1_51_and_1_52;
        break;
      default:
        lSettings.defaultMainWindowCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
    }
  }

  // Default Retargeting Tool CBBE 3BBB Version
  if (lSettingsJSON.contains("retargeting_tool_3bbb_version") && lSettingsJSON["retargeting_tool_3bbb_version"].isDouble())
  {
    auto lFoundVersion{lSettingsJSON["retargeting_tool_3bbb_version"].toInt()};

    switch (lFoundVersion)
    {
      case static_cast<int>(CBBE3BBBVersion::Version1_40):
        lSettings.defaultRetargetingToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
        break;
      case static_cast<int>(CBBE3BBBVersion::Version1_50):
        lSettings.defaultRetargetingToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_50;
        break;
      case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
        lSettings.defaultRetargetingToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_51_and_1_52;
        break;
      default:
        lSettings.defaultRetargetingToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
    }
  }

  // Main window output path
  if (lSettingsJSON.contains("mainWindowOutputPath") && lSettingsJSON["mainWindowOutputPath"].isString())
  {
    lSettings.mainWindowOutputPath = lSettingsJSON["mainWindowOutputPath"].toString();
  }

  // Main window auto open generated dir
  if (lSettingsJSON.contains("mainWindowAutomaticallyOpenGeneratedDirectory") && lSettingsJSON["mainWindowAutomaticallyOpenGeneratedDirectory"].isBool())
  {
    lSettings.mainWindowAutomaticallyOpenGeneratedDirectory = lSettingsJSON["mainWindowAutomaticallyOpenGeneratedDirectory"].toBool();
  }

  return lSettings;
}

void Utils::saveSettingsToFile(Struct::Settings aSettings)
{
  QFile lSettingsFile(QCoreApplication::applicationDirPath() + QDir::separator() + "config.json");

  lSettingsFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

  auto gameObject{Utils::settingsStructToJson(aSettings)};
  QJsonDocument saveDoc(gameObject);
  lSettingsFile.write(saveDoc.toJson());

  lSettingsFile.close();
}

QJsonObject Utils::settingsStructToJson(Struct::Settings aSettings)
{
  // Construct a font subobject
  QJsonObject lFontObj;
  lFontObj["family"] = aSettings.font.family;
  lFontObj["styleName"] = aSettings.font.styleName;
  lFontObj["size"] = aSettings.font.size;
  lFontObj["weight"] = aSettings.font.weight;
  lFontObj["italic"] = aSettings.font.italic;
  lFontObj["underline"] = aSettings.font.underline;
  lFontObj["strikeOut"] = aSettings.font.strikeOut;

  // Construct the full settings object
  QJsonObject lObj;
  lObj["lang"] = static_cast<int>(aSettings.language);
  lObj["font"] = lFontObj;
  lObj["appTheme"] = static_cast<int>(aSettings.appTheme);
  lObj["windowWidth"] = aSettings.mainWindowWidth;
  lObj["windowHeight"] = aSettings.mainWindowHeight;
  lObj["default_3bbb_version"] = static_cast<int>(aSettings.defaultMainWindowCBBE3BBBVersion);
  lObj["retargeting_tool_3bbb_version"] = static_cast<int>(aSettings.defaultRetargetingToolCBBE3BBBVersion);
  lObj["main_window_opening_mode"] = static_cast<int>(aSettings.mainWindowOpeningMode);
  lObj["mainWindowOutputPath"] = aSettings.mainWindowOutputPath;
  lObj["mainWindowAutomaticallyOpenGeneratedDirectory"] = aSettings.mainWindowAutomaticallyOpenGeneratedDirectory;

  return lObj;
}

QString Utils::getShortLanguageNameFromEnum(const int aEnumValue)
{
  switch (aEnumValue)
  {
    case static_cast<int>(ApplicationLanguage::English):
      return "en";
    case static_cast<int>(ApplicationLanguage::French):
      return "fr";
    default:
      return "en";
      break;
  }
}

QString Utils::getLongLanguageNameFromEnum(const int aEnumValue)
{
  switch (aEnumValue)
  {
    case static_cast<int>(ApplicationLanguage::English):
      return "English";
    case static_cast<int>(ApplicationLanguage::French):
      return "Français";
    default:
      return "English";
      break;
  }
}

ApplicationLanguage Utils::getStructLanguageFromName(const QString& aShortName)
{
  if (aShortName.compare("English") == 0)
  {
    return ApplicationLanguage::English;
  }

  if (aShortName.compare("French") == 0)
  {
    return ApplicationLanguage::French;
  }

  // Default language if no supported language has been found
  return ApplicationLanguage::English;
}
