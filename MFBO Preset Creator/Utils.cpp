#include "Utils.h"

void Utils::cleanPathString(QString& aPath)
{
  aPath.replace("\\", "/");
}

QString Utils::getProgramVersion()
{
  return QString("1.6.0.0");
}

void Utils::displayWarningMessage(QString aMessage)
{
  QMessageBox lMessageBox(QMessageBox::Icon::Warning, tr("Warning"), aMessage);
  lMessageBox.exec();
}

int Utils::getNumberFilesByExtension(QString aRootDir, QString aFileExtension)
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

bool Utils::copyRecursively(QString sourceFolder, QString destFolder)
{
  // Taken from https://forum.qt.io/topic/59245/is-there-any-api-to-recursively-copy-a-directory-and-all-it-s-sub-dirs-and-files/3

  bool success = false;
  QDir sourceDir(sourceFolder);

  if (!sourceDir.exists())
    return false;

  QDir destDir(destFolder);
  if (!destDir.exists())
    destDir.mkdir(destFolder);

  QStringList files = sourceDir.entryList(QDir::Files);
  for (int i = 0; i < files.count(); i++)
  {
    QString srcName = sourceFolder + QDir::separator() + files[i];
    QString destName = destFolder + QDir::separator() + files[i];
    success = QFile::copy(srcName, destName);

    if (!success)
    {
      Utils::displayWarningMessage(tr("An unknown error has occured while creating the backup. Process aborted."));
      return false;
    }
  }

  files.clear();
  files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
  for (int i = 0; i < files.count(); i++)
  {
    QString srcName = sourceFolder + QDir::separator() + files[i];
    QString destName = destFolder + QDir::separator() + files[i];
    success = copyRecursively(srcName, destName);

    if (!success)
    {
      Utils::displayWarningMessage(tr("An unknown error has occured while creating the backup. Process aborted."));
      return false;
    }
  }

  return true;
}

QString Utils::getPresetNameFromXMLFile(QString aPath)
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

  QDomElement lXMLGroup = lXMLDocument.documentElement().firstChild().toElement();
  QString lPresetName;

  if (lXMLGroup.tagName() == "Group")
  {
    // Get the first "Member" tag to read its "name" attribute
    QDomElement lXMLMember = lXMLGroup.firstChild().toElement();
    lPresetName = lXMLMember.attribute("name", "");
  }

  return lPresetName.left(lPresetName.lastIndexOf(QChar('-')) - 1);
}

std::vector<Struct::SliderSet> Utils::getOutputPathsFromOSPFile(QString aPath)
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

  QDomElement lRoot{lOSPDocument.documentElement()};
  QDomElement lSliderSet{lRoot.firstChild().toElement()};

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

      QDomElement lChild = lSliderSet.firstChild().toElement();
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

bool Utils::isPresetUsingBeastHands(QString aPath)
{
  QFile lReadFile(aPath);
  lReadFile.setPermissions(QFile::WriteUser);

  QString lFileContent;

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
  QFile lSettingsFile(QCoreApplication::applicationDirPath() + "/config.json");

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
  QFile lSettingsFile(QCoreApplication::applicationDirPath() + "/config.json");
  lSettingsFile.open(QIODevice::ReadOnly | QIODevice::Text);
  QString lSettingsData = lSettingsFile.readAll();
  lSettingsFile.close();

  QJsonDocument lJsonDocument(QJsonDocument::fromJson(lSettingsData.toUtf8()));
  QJsonObject lJsonObject = lJsonDocument.object();

  Struct::Settings lSettings;

  // Language
  if (lJsonObject.contains("lang") && lJsonObject["lang"].isDouble())
  {
    auto lFoundLanguage = lJsonObject["lang"].toInt();

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
  if (lJsonObject.contains("fontFamily") && lJsonObject["fontFamily"].isString())
  {
    lSettings.fontFamily = lJsonObject["fontFamily"].toString();
  }

  // Font size
  if (lJsonObject.contains("fontSize") && lJsonObject["fontSize"].isDouble())
  {
    lSettings.fontSize = lJsonObject["fontSize"].toInt();
  }

  // Dark theme
  if (lJsonObject.contains("appTheme") && lJsonObject["appTheme"].isDouble())
  {
    auto lFoundAppTheme = lJsonObject["appTheme"].toInt();

    switch (lFoundAppTheme)
    {
      case static_cast<int>(GUITheme::WindowsVista):
        lSettings.appTheme = GUITheme::WindowsVista;
        break;
      default:
        lSettings.appTheme = GUITheme::WindowsVista;
        break;
    }
  }

  // Default window width
  if (lJsonObject.contains("windowWidth") && lJsonObject["windowWidth"].isDouble())
  {
    lSettings.mainWindowWidth = lJsonObject["windowWidth"].toInt();
  }

  // Default window height
  if (lJsonObject.contains("windowHeight") && lJsonObject["windowHeight"].isDouble())
  {
    lSettings.mainWindowHeight = lJsonObject["windowHeight"].toInt();
  }

  // Default CBBE 3BBB Version
  if (lJsonObject.contains("default_3bbb_version") && lJsonObject["default_3bbb_version"].isDouble())
  {
    auto lFoundVersion = lJsonObject["default_3bbb_version"].toInt();

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

  // Default upgrade or downgrade CBBE 3BBB Version
  if (lJsonObject.contains("up_downgrade_3bbb_version") && lJsonObject["up_downgrade_3bbb_version"].isDouble())
  {
    auto lFoundVersion = lJsonObject["up_downgrade_3bbb_version"].toInt();

    switch (lFoundVersion)
    {
      case static_cast<int>(CBBE3BBBVersion::Version1_40):
        lSettings.defaultUpgradeToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
        break;
      case static_cast<int>(CBBE3BBBVersion::Version1_50):
        lSettings.defaultUpgradeToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_50;
        break;
      case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
        lSettings.defaultUpgradeToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_51_and_1_52;
        break;
      default:
        lSettings.defaultUpgradeToolCBBE3BBBVersion = CBBE3BBBVersion::Version1_40;
    }
  }

  return lSettings;
}

void Utils::saveSettingsToFile(Struct::Settings aSettings)
{
  QFile lSettingsFile(QCoreApplication::applicationDirPath() + "/config.json");

  lSettingsFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

  auto gameObject{Utils::settingsStructToJson(aSettings)};
  QJsonDocument saveDoc(gameObject);
  lSettingsFile.write(saveDoc.toJson());

  lSettingsFile.close();
}

QJsonObject Utils::settingsStructToJson(Struct::Settings aSettings)
{
  QJsonObject lObj;

  lObj["lang"] = static_cast<int>(aSettings.language);
  lObj["fontFamily"] = aSettings.fontFamily;
  lObj["fontSize"] = static_cast<int>(aSettings.fontSize);
  lObj["appTheme"] = static_cast<int>(aSettings.appTheme);
  lObj["windowWidth"] = aSettings.mainWindowWidth;
  lObj["windowHeight"] = aSettings.mainWindowHeight;
  lObj["default_3bbb_version"] = static_cast<int>(aSettings.defaultMainWindowCBBE3BBBVersion);
  lObj["up_downgrade_3bbb_version"] = static_cast<int>(aSettings.defaultUpgradeToolCBBE3BBBVersion);

  return lObj;
}

QStringList Utils::getCBBE3BBBVersions()
{
  QStringList lVersions;
  lVersions.append(QString("1.40"));
  lVersions.append(QString("1.50"));
  lVersions.append(QString("1.51 - 1.52"));

  return lVersions;
}

QString Utils::getShortLanguageNameFromEnum(int aEnumValue)
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

QString Utils::getLongLanguageNameFromEnum(int aEnumValue)
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
