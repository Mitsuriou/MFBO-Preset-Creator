#include "Utils.h"

void Utils::cleanPathString(QString& aPath)
{
  aPath.replace("\\", "/");
}

QString Utils::cleanPathString(const QString& aPath)
{
  QString lPath(aPath);
  lPath.replace("\\", "/");
  return lPath;
}

QStringList Utils::splitString(QString aString, const QString& aSeparator)
{
  auto lList{aString.split(QString(aSeparator))};

  auto lSize{lList.size()};
  for (int i = 0; i < lSize; i++)
  {
    if (lList.at(i).trimmed().compare("", Qt::CaseInsensitive) == 0)
    {
      lList.removeAt(i);
    }
  }

  return lList;
}

QString Utils::getApplicationVersion()
{
  return "2.0.0";
}

void Utils::displayWarningMessage(const QString& aMessage)
{
  QMessageBox lMessageBox(QMessageBox::Icon::Warning, tr("Warning"), aMessage);

  QPushButton lButton(tr("OK"));
  lButton.setCursor(Qt::PointingHandCursor);
  lMessageBox.addButton(&lButton, QMessageBox::ButtonRole::AcceptRole);

  lMessageBox.exec();
}

int Utils::getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension)
{
  auto lNumber{0};

  QDirIterator it(aRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::NoIteratorFlags);
  while (it.hasNext())
  {
    it.next();
    lNumber++;
  }

  return lNumber;
}

int Utils::getNumberFilesByExtensionRecursive(const QString& aRootDir, const QString& aFileExtension)
{
  auto lNumber{0};

  QDirIterator it(aRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
    lNumber++;
  }

  return lNumber;
}

int Utils::getNumberFilesByExtensions(const QString& aRootDir, const QStringList& aFileExtensions)
{
  auto lNumber{0};

  QDirIterator it(aRootDir, aFileExtensions, QDir::Files, QDirIterator::NoIteratorFlags);
  while (it.hasNext())
  {
    it.next();
    lNumber++;
  }

  return lNumber;
}

int Utils::getNumberFilesByExtensionsRecursive(const QString& aRootDir, const QStringList& aFileExtensions)
{
  auto lNumber{0};

  QDirIterator it(aRootDir, aFileExtensions, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
    lNumber++;
  }

  return lNumber;
}

bool Utils::copyRecursively(const QString& aSourcePath, const QString& aDestinationPath)
{
  QDir lSourceDirectory(aSourcePath);
  if (!lSourceDirectory.exists())
  {
    return false;
  }

  QDir lDestinationDirectory(aDestinationPath);
  if (lDestinationDirectory.exists())
  {
    return false;
  }
  if (!lDestinationDirectory.exists())
  {
    lDestinationDirectory.mkdir(aDestinationPath);
  }

  QStringList lFilesList{lSourceDirectory.entryList(QDir::Files)};
  auto lFilesListSize{lFilesList.count()};

  for (int i = 0; i < lFilesListSize; i++)
  {
    auto lSourceName{aSourcePath + QDir::separator() + lFilesList[i]};
    auto lDestinationName{aDestinationPath + QDir::separator() + lFilesList[i]};

    if (!QFile::copy(lSourceName, lDestinationName))
    {
      return false;
    }
  }

  lFilesList.clear();
  lFilesList = lSourceDirectory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
  lFilesListSize = lFilesList.count();

  for (int i = 0; i < lFilesListSize; i++)
  {
    auto lSourceName{aSourcePath + QDir::separator() + lFilesList[i]};
    auto lDestinationName{aDestinationPath + QDir::separator() + lFilesList[i]};

    if (!copyRecursively(lSourceName, lDestinationName))
    {
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
    case GUITheme::QuasarAppVisualStudioDark:
    case GUITheme::MitsuriouDarkTheme:
      return true;
    default:
      return false;
  }
}

QString Utils::getIconRessourceFolder(const GUITheme& aTheme)
{
  return (Utils::isThemeDark(aTheme) ? QString("white") : QString("black"));
}

QString Utils::getBodyRessourceFolder(const BodyNameVersion& aBody)
{
  switch (aBody)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_40:
      return "cbbe_3bbb_1.40";
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
      return "cbbe_3bbb_1.50";
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_and_1_52:
      return "cbbe_3bbb_1.51_1.52";
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      return "cbbe_smp_3bbb_1.2.0";
    case BodyNameVersion::BHUNP_3BBB_2_13:
      return "bhunp_2.13/bhunp_3bbb";
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_13:
      return "bhunp_2.13/bhunp_3bbb_advanced";
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_13:
      return "bhunp_2.13/bhunp_3bbb_advanced_ver_2";
    case BodyNameVersion::BHUNP_BBP_2_13:
      return "bhunp_2.13/bhunp_bbp";
    case BodyNameVersion::BHUNP_BBP_Advanced_2_13:
      return "bhunp_2.13/bhunp_bbp_advanced";
    case BodyNameVersion::BHUNP_TBBP_2_13:
      return "bhunp_2.13/bhunp_tbbp";
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_13:
      return "bhunp_2.13/bhunp_tbbp_advanced";
    default:
      Utils::displayWarningMessage(tr("Error while searching for the targeted body. If it happens, try restarting the application. If the error is still here after restarting the application, contact the developer."));
      return "";
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

void Utils::saveAsJsonFile(const QJsonObject& aJsonToSave, const QString& aFilePath)
{
  // Open (or create and open) the file
  QFile lFile(aFilePath);
  lFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
  lFile.write(QJsonDocument(aJsonToSave).toJson());
  lFile.close();
}

QJsonObject Utils::loadFromJsonFile(const QString& aFilePath)
{
  // Open and read the file
  QFile lFile(aFilePath);
  lFile.open(QIODevice::ReadOnly | QIODevice::Text);
  const QString lReadData = lFile.readAll();
  lFile.close();

  // Convert the text content into a QJsonDocument
  QJsonDocument lJsonDocument(QJsonDocument::fromJson(lReadData.toUtf8()));

  // Return the QJsonObject containing the read data
  return lJsonDocument.object();
}

void Utils::checkSettingsFileExistence()
{
  auto lSettingsFilePath{QCoreApplication::applicationDirPath() + QDir::separator() + "config.json"};
  if (!QFile(lSettingsFilePath).exists())
  {
    // Create a default setting file if it does not exist
    Struct::Settings lSettings;
    Utils::saveSettingsToFile(lSettings);
  }
}

Struct::Settings Utils::loadSettingsFromFile()
{
  Utils::checkSettingsFileExistence();

  auto lSettingsFilePath{QCoreApplication::applicationDirPath() + QDir::separator() + "config.json"};
  QJsonObject lSettingsJSON{Utils::loadFromJsonFile(lSettingsFilePath)};

  Struct::Settings lSettings;

  // Language
  if (lSettingsJSON.contains("lang") && lSettingsJSON["lang"].isDouble())
  {
    auto lFoundLanguage{lSettingsJSON["lang"].toInt()};
    lSettings.language = static_cast<ApplicationLanguage>(lFoundLanguage);
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
    lSettings.appTheme = static_cast<GUITheme>(lFoundAppTheme);
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
    lSettings.mainWindowOpeningMode = static_cast<WindowOpeningMode>(lFoundWindowOpeningMode);
  }

  // Default CBBE 3BBB Version
  if (lSettingsJSON.contains("default_body") && lSettingsJSON["default_body"].isDouble())
  {
    auto lFoundBody{lSettingsJSON["default_body"].toInt()};
    lSettings.defaultMainWindowBody = static_cast<BodyNameVersion>(lFoundBody);
  }

  // Default Retargeting Tool CBBE 3BBB Version
  if (lSettingsJSON.contains("retargeting_tool_default_body") && lSettingsJSON["retargeting_tool_default_body"].isDouble())
  {
    auto lFoundBody{lSettingsJSON["retargeting_tool_default_body"].toInt()};
    lSettings.defaultRetargetingToolBody = static_cast<BodyNameVersion>(lFoundBody);
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

  // Check for updates at startup
  if (lSettingsJSON.contains("checkForUpdatesAtStartup") && lSettingsJSON["checkForUpdatesAtStartup"].isBool())
  {
    lSettings.checkForUpdatesAtStartup = lSettingsJSON["checkForUpdatesAtStartup"].toBool();
  }

  return lSettings;
}

void Utils::saveSettingsToFile(Struct::Settings aSettings)
{
  auto lSettingsFilePath{QCoreApplication::applicationDirPath() + QDir::separator() + "config.json"};
  Utils::saveAsJsonFile(Utils::settingsStructToJson(aSettings), lSettingsFilePath);
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
  lObj["default_body"] = static_cast<int>(aSettings.defaultMainWindowBody);
  lObj["retargeting_tool_default_body"] = static_cast<int>(aSettings.defaultRetargetingToolBody);
  lObj["main_window_opening_mode"] = static_cast<int>(aSettings.mainWindowOpeningMode);
  lObj["mainWindowOutputPath"] = aSettings.mainWindowOutputPath;
  lObj["mainWindowAutomaticallyOpenGeneratedDirectory"] = aSettings.mainWindowAutomaticallyOpenGeneratedDirectory;
  lObj["checkForUpdatesAtStartup"] = aSettings.checkForUpdatesAtStartup;

  return lObj;
}

QStringList Utils::loadFiltersFromFile()
{
  QFile lFiltersFile(QCoreApplication::applicationDirPath() + QDir::separator() + "filters.txt");
  lFiltersFile.open(QIODevice::ReadOnly | QIODevice::Text);
  QStringList lSettingsData = QString::fromUtf8(lFiltersFile.readAll()).split(",");
  lFiltersFile.close();

  return lSettingsData;
}

void Utils::saveFiltersToFile(QStringList aList)
{
  QFile lFiltersFile(QCoreApplication::applicationDirPath() + QDir::separator() + "filters.txt");
  lFiltersFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
  lFiltersFile.write(aList.join(",").toUtf8());
  lFiltersFile.close();
}

QString Utils::getXMLFilterBlockFromBody(const int& aBody, const int& aBeastHands, const QString& aGroupName)
{
  auto lXMLBlock{QString("    <Group name=\"%1\">\n").arg(aGroupName)};

  auto lBody = static_cast<BodyNameVersion>(aBody);
  switch (lBody)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_40:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - 3BBB Body Amazing\"/>\n");
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - Feet\"/>\n");

      if (aBeastHands)
      {
        lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - Beast Hands\"/>\n");
      }
      else
      {
        lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - Hands\"/>\n");
      }

      break;
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE 3BBB Body Amazing\"/>\n");
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE 3BBB Feet\"/>\n");

      if (aBeastHands)
      {
        lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE Beast Hands\"/>\n");
      }
      else
      {
        lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE 3BBB Hands\"/>\n");
      }

      break;
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_and_1_52:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE 3BBB Body Amazing\"/>\n");
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE 3BBB Feet\"/>\n");

      if (aBeastHands)
      {
        lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE 3BBB Hands Beast\"/>\n");
      }
      else
      {
        lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE 3BBB Hands\"/>\n");
      }

      break;
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE Body SMP (3BBB)\"/>\n");

      break;
    case BodyNameVersion::BHUNP_3BBB_2_13:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_13:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_13:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Ver 2\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_BBP_2_13:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP BBP\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_BBP_Advanced_2_13:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP BBP Advanced\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_TBBP_2_13:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP TBBP\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_13:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP TBBP Advanced\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
  }

  lXMLBlock.append("    </Group>\n");
  return lXMLBlock;
}

QString Utils::getShortLanguageNameFromEnum(const int& aEnumValue)
{
  auto lEnumLang{static_cast<ApplicationLanguage>(aEnumValue)};
  switch (lEnumLang)
  {
    case ApplicationLanguage::English:
      return "en";
    case ApplicationLanguage::French:
      return "fr";
    default:
      return "en";
  }
}

QString Utils::getLongLanguageNameFromEnum(const int& aEnumValue)
{
  auto lEnumLang{static_cast<ApplicationLanguage>(aEnumValue)};
  switch (lEnumLang)
  {
    case ApplicationLanguage::English:
      return "English";
    case ApplicationLanguage::French:
      return "Français";
    default:
      return "English";
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
