#include "Utils.h"

bool Utils::RESTART_PENDING = false;

void Utils::cleanPathString(QString& aPath)
{
  // Replace backslashes
  aPath.replace("\\", "/");

  // Remove any '\n' character
  Utils::cleanBreaksString(aPath);
}

QString Utils::cleanPathString(const QString& aPath)
{
  // Replace backslashes
  QString lPath(aPath);
  lPath.replace("\\", "/");

  // Remove any '\n' character
  const QString lCleanedPath(lPath);
  lPath = Utils::cleanBreaksString(lCleanedPath);

  return lPath;
}

void Utils::cleanBreaksString(QString& aPath)
{
  aPath.replace("\n", "");
}

QString Utils::cleanBreaksString(const QString& aPath)
{
  QString lPath(aPath);
  lPath.replace("\n", "");
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
  return "2.7.3";
}

void Utils::displayWarningMessage(const QString& aMessage)
{
  QMessageBox lBox(QMessageBox::Icon::Warning, tr("Warning"), aMessage);
  lBox.setIconPixmap(QPixmap(":/icons/red-alert-circle").scaledToHeight(48, Qt::SmoothTransformation));

  QPushButton lButton(tr("OK"));
  lButton.setCursor(Qt::PointingHandCursor);
  lBox.addButton(&lButton, QMessageBox::ButtonRole::AcceptRole);

  lBox.exec();
}

ButtonClicked Utils::displayQuestionMessage(QWidget* aParent,
                                            const QString& aTitle,
                                            const QString& aMessage,
                                            const QString& aIconFolder,
                                            const QString& aIconName,
                                            const QString& aTextBtnYes,
                                            const QString& aTextBtnNo,
                                            const QString& aColorYesBtn,
                                            const QString& aColorNoBtn,
                                            const bool& aIsYesBtnDefault)
{
  QMessageBox lConfirmationBox(QMessageBox::Icon::Question, aTitle, aMessage, QMessageBox::StandardButton::NoButton, aParent);
  lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/%2").arg(aIconFolder).arg(aIconName)).scaledToHeight(48, Qt::SmoothTransformation));

  auto lYesButton{lConfirmationBox.addButton(aTextBtnYes, QMessageBox::ButtonRole::YesRole)};
  lYesButton->setCursor(Qt::PointingHandCursor);
  lYesButton->setStyleSheet(QString("color: %1;").arg(aColorYesBtn));

  auto lNoButton{lConfirmationBox.addButton(aTextBtnNo, QMessageBox::ButtonRole::NoRole)};
  lNoButton->setCursor(Qt::PointingHandCursor);
  lNoButton->setStyleSheet(QString("color: %1;").arg(aColorNoBtn));

  lConfirmationBox.setDefaultButton(aIsYesBtnDefault ? lYesButton : lNoButton);
  lConfirmationBox.exec();

  // Return the clicked button
  const auto lClickedButton{lConfirmationBox.clickedButton()};
  if (lClickedButton == lYesButton)
  {
    return ButtonClicked::Yes;
  }
  if (lClickedButton == lNoButton)
  {
    return ButtonClicked::No;
  }
  return ButtonClicked::CloseWindow;
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
    lDestinationDirectory.mkpath(aDestinationPath);
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
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_and_1_55:
      return "cbbe_3bbb_1.51_1.55";
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
    case BodyNameVersion::BHUNP_3BBB_2_15:
      return "bhunp_2.15/bhunp_3bbb";
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_15:
      return "bhunp_2.15/bhunp_3bbb_advanced";
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_15:
      return "bhunp_2.15/bhunp_3bbb_advanced_ver_2";
    case BodyNameVersion::BHUNP_BBP_2_15:
      return "bhunp_2.15/bhunp_bbp";
    case BodyNameVersion::BHUNP_BBP_Advanced_2_15:
      return "bhunp_2.15/bhunp_bbp_advanced";
    case BodyNameVersion::BHUNP_TBBP_2_15:
      return "bhunp_2.15/bhunp_tbbp";
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_15:
      return "bhunp_2.15/bhunp_tbbp_advanced";
    case BodyNameVersion::BHUNP_3BBB_2_20:
      return "bhunp_2.20/bhunp_3bbb";
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_20:
      return "bhunp_2.20/bhunp_3bbb_advanced";
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_20:
      return "bhunp_2.20/bhunp_3bbb_advanced_ver_2";
    case BodyNameVersion::BHUNP_BBP_2_20:
      return "bhunp_2.20/bhunp_bbp";
    case BodyNameVersion::BHUNP_BBP_Advanced_2_20:
      return "bhunp_2.20/bhunp_bbp_advanced";
    case BodyNameVersion::BHUNP_TBBP_2_20:
      return "bhunp_2.20/bhunp_tbbp";
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_20:
      return "bhunp_2.20/bhunp_tbbp_advanced";
    default:
      Utils::displayWarningMessage(tr("Error while searching for the targeted body. If it happens, try restarting the application. If the error is still here after restarting the application, contact the developer."));
      return "";
  }
}

bool Utils::isBodySupportingBeastHands(const BodyNameVersion& aBody)
{
  switch (aBody)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_40:
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_and_1_55:
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      return true;
    default:
      return false;
  }
}

QString Utils::getAppDataPathFolder()
{
#ifdef DEBUG
  return QCoreApplication::applicationDirPath() + QDir::separator();
#else
  QDir lDir;
  lDir.mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
  return QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator();
#endif
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
    Utils::displayWarningMessage(tr("Error while trying to read the file \"%1\".").arg(aPath));
    return "";
  }

  auto lXMLGroup{lXMLDocument.documentElement().firstChild().toElement()};
  auto lPresetName{QString("")};

  if (lXMLGroup.tagName() == "Group")
  {
    // Get the first "Member" tag to read its "name" attribute
    auto lXMLMember{lXMLGroup.firstChild().toElement()};
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
    Utils::displayWarningMessage(tr("Error while trying to read the file \"%1\".").arg(aPath));
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
        lTempSet.meshPart = "Body";
      }
      else if (lTempSet.name.contains("hands", Qt::CaseInsensitive))
      {
        lTempSet.meshPart = "Hands";
      }
      else if (lTempSet.name.contains("feet", Qt::CaseInsensitive))
      {
        lTempSet.meshPart = "Feet";
      }

      auto lChild{lSliderSet.firstChild().toElement()};
      while (!lChild.isNull())
      {
        if (lChild.tagName() == "OutputPath")
        {
          lTempSet.outputPath = lChild.firstChild().toText().data();
        }
        else if (lChild.tagName() == "OutputFile")
        {
          lTempSet.outputFile = lChild.firstChild().toText().data();
        }

        if (lTempSet.outputPath != "" && lTempSet.outputFile != "")
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
    Utils::displayWarningMessage(tr("Error while trying to read the file \"%1\".").arg(aPath));
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
  const QString lReadData{lFile.readAll()};
  lFile.close();

  // Convert the text content into a QJsonDocument
  auto lJsonDocument(QJsonDocument::fromJson(lReadData.toUtf8()));

  // Return the QJsonObject containing the read data
  return lJsonDocument.object();
}

void Utils::checkSettingsFileExistence()
{
  auto lSettingsFilePath{Utils::getAppDataPathFolder() + "config.json"};
  if (!QFile(lSettingsFilePath).exists())
  {
    // Create a default setting file if it does not exist
    Utils::saveSettingsToFile(Struct::Settings());
  }
}

Struct::Settings Utils::loadSettingsFromFile()
{
  Utils::checkSettingsFileExistence();

  auto lSettingsFilePath{Utils::getAppDataPathFolder() + "config.json"};
  QJsonObject lSettingsJSON{Utils::loadFromJsonFile(lSettingsFilePath)};

  Struct::Settings lSettings;

  // Language
  if (lSettingsJSON.contains("language") && lSettingsJSON["language"].isDouble())
  {
    auto lFoundLanguage{lSettingsJSON["language"].toInt()};
    lSettings.language = static_cast<ApplicationLanguage>(lFoundLanguage);
  }

  // Font family
  if (lSettingsJSON.contains("font"))
  {
    auto lFontJSON{lSettingsJSON["font"].toObject()};

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
  if (lSettingsJSON.contains("mainWindowOpeningMode") && lSettingsJSON["mainWindowOpeningMode"].isDouble())
  {
    auto lFoundWindowOpeningMode{lSettingsJSON["mainWindowOpeningMode"].toInt()};
    lSettings.mainWindowOpeningMode = static_cast<WindowOpeningMode>(lFoundWindowOpeningMode);
  }

  // Default body name and version
  if (lSettingsJSON.contains("defaultBody") && lSettingsJSON["defaultBody"].isDouble())
  {
    auto lFoundBody{lSettingsJSON["defaultBody"].toInt()};
    lSettings.defaultMainWindowBody = static_cast<BodyNameVersion>(lFoundBody);
  }

  // Default BodySlide Presets' Retargeting body name and version
  if (lSettingsJSON.contains("retargetingToolDefaultBody") && lSettingsJSON["retargetingToolDefaultBody"].isDouble())
  {
    auto lFoundBody{lSettingsJSON["retargetingToolDefaultBody"].toInt()};
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

  // Assisted Conversion: only scan the meshes subdir
  if (lSettingsJSON.contains("assistedConversionScanOnlyMeshesSubdir") && lSettingsJSON["assistedConversionScanOnlyMeshesSubdir"].isBool())
  {
    lSettings.assistedConversionScanOnlyMeshesSubdir = lSettingsJSON["assistedConversionScanOnlyMeshesSubdir"].toBool();
  }

  // Each button stores the last opened path
  if (lSettingsJSON.contains("eachButtonSavesItsLastUsedPath") && lSettingsJSON["eachButtonSavesItsLastUsedPath"].isBool())
  {
    lSettings.eachButtonSavesItsLastUsedPath = lSettingsJSON["eachButtonSavesItsLastUsedPath"].toBool();
  }

  // Success color
  if (lSettingsJSON.contains("successColor") && lSettingsJSON["successColor"].isString())
  {
    lSettings.successColor = lSettingsJSON["successColor"].toString();
  }

  // Warning color
  if (lSettingsJSON.contains("warningColor") && lSettingsJSON["warningColor"].isString())
  {
    lSettings.warningColor = lSettingsJSON["warningColor"].toString();
  }

  // Danger color
  if (lSettingsJSON.contains("dangerColor") && lSettingsJSON["dangerColor"].isString())
  {
    lSettings.dangerColor = lSettingsJSON["dangerColor"].toString();
  }

  Utils::printMessageStdOut("User settings:");
  Utils::printMessageStdOut(QJsonDocument(lSettingsJSON).toJson(QJsonDocument::JsonFormat::Indented));

  return lSettings;
}

void Utils::saveSettingsToFile(const Struct::Settings& aSettings)
{
  auto lSettingsFilePath{Utils::getAppDataPathFolder() + "config.json"};
  Utils::saveAsJsonFile(Utils::settingsStructToJson(aSettings), lSettingsFilePath);
}

QJsonObject Utils::settingsStructToJson(const Struct::Settings& aSettings)
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
  lObj["language"] = static_cast<int>(aSettings.language);
  lObj["font"] = lFontObj;
  lObj["appTheme"] = static_cast<int>(aSettings.appTheme);
  lObj["windowWidth"] = aSettings.mainWindowWidth;
  lObj["windowHeight"] = aSettings.mainWindowHeight;
  lObj["defaultBody"] = static_cast<int>(aSettings.defaultMainWindowBody);
  lObj["retargetingToolDefaultBody"] = static_cast<int>(aSettings.defaultRetargetingToolBody);
  lObj["mainWindowOpeningMode"] = static_cast<int>(aSettings.mainWindowOpeningMode);
  lObj["mainWindowOutputPath"] = aSettings.mainWindowOutputPath;
  lObj["mainWindowAutomaticallyOpenGeneratedDirectory"] = aSettings.mainWindowAutomaticallyOpenGeneratedDirectory;
  lObj["checkForUpdatesAtStartup"] = aSettings.checkForUpdatesAtStartup;
  lObj["assistedConversionScanOnlyMeshesSubdir"] = aSettings.assistedConversionScanOnlyMeshesSubdir;
  lObj["eachButtonSavesItsLastUsedPath"] = aSettings.eachButtonSavesItsLastUsedPath;
  lObj["successColor"] = aSettings.successColor;
  lObj["warningColor"] = aSettings.warningColor;
  lObj["dangerColor"] = aSettings.dangerColor;

  return lObj;
}

void Utils::checkFiltersFileExistence()
{
  auto lFiltersFilePath{Utils::getAppDataPathFolder() + "filters.json"};
  if (!QFile(lFiltersFilePath).exists())
  {
    // Create a default filters file if it does not exist
    Utils::saveFiltersToFile(std::map<QString, QStringList>());
  }
}

std::map<QString, QStringList> Utils::loadFiltersFromFile()
{
  Utils::checkFiltersFileExistence();

  auto lFiltersFilePath(Utils::getAppDataPathFolder() + "filters.json");
  QJsonObject lObtainedJSON{Utils::loadFromJsonFile(lFiltersFilePath)};

  auto lVariantMap{lObtainedJSON.toVariantMap()};
  std::map<QString, QStringList> lFiltersList;

  for (const auto& lKey : lVariantMap.keys())
  {
    lFiltersList.insert({lKey, lVariantMap.value(lKey, "").toStringList()});
  }

  return lFiltersList;
}

void Utils::saveFiltersToFile(const std::map<QString, QStringList>& aList)
{
  auto lFiltersFilePath{Utils::getAppDataPathFolder() + "filters.json"};
  Utils::saveAsJsonFile(Utils::filtersMapToJson(aList), lFiltersFilePath);
}

QJsonObject Utils::filtersMapToJson(const std::map<QString, QStringList>& aList)
{
  QVariantMap lVarMap;

  for (const auto& lPair : aList)
  {
    lVarMap.insert(lPair.first, lPair.second);
  }

  return QJsonObject::fromVariantMap(lVarMap);
}

QString Utils::getXMLFilterBlockFromBody(const int& aBody, const int& aBeastHands, const QString& aGroupName)
{
  auto lXMLBlock{QString("    <Group name=\"%1\">\n").arg(aGroupName)};

  auto lBody{static_cast<BodyNameVersion>(aBody)};
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
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_and_1_55:
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
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE Feet\"/>\n");

      if (aBeastHands)
      {
        lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE Hands Beast\"/>\n");
      }
      else
      {
        lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - CBBE Hands\"/>\n");
      }

      break;
    case BodyNameVersion::BHUNP_3BBB_2_13:
    case BodyNameVersion::BHUNP_3BBB_2_15:
    case BodyNameVersion::BHUNP_3BBB_2_20:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_13:
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_15:
    case BodyNameVersion::BHUNP_3BBB_Advanced_2_20:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_13:
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_15:
    case BodyNameVersion::BHUNP_3BBB_Advanced_ver_2_2_20:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Ver 2\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_BBP_2_13:
    case BodyNameVersion::BHUNP_BBP_2_15:
    case BodyNameVersion::BHUNP_BBP_2_20:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP BBP\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_BBP_Advanced_2_13:
    case BodyNameVersion::BHUNP_BBP_Advanced_2_15:
    case BodyNameVersion::BHUNP_BBP_Advanced_2_20:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP BBP Advanced\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_TBBP_2_13:
    case BodyNameVersion::BHUNP_TBBP_2_15:
    case BodyNameVersion::BHUNP_TBBP_2_20:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP TBBP\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_13:
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_15:
    case BodyNameVersion::BHUNP_TBBP_Advanced_2_20:
      lXMLBlock.append("        <Member name=\"{%%bodyslide_set_name%%} - BHUNP TBBP Advanced\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Feet\"/>\n"
                       "        <Member name=\"{%%bodyslide_set_name%%} - BHUNP 3BBB Advanced Hands\"/>\n");

      break;
  }

  lXMLBlock.append("    </Group>\n");
  return lXMLBlock;
}

void Utils::checkLastPathsFileExistence()
{
  auto lLastPathsFilePath{Utils::getAppDataPathFolder() + "paths.json"};
  if (!QFile(lLastPathsFilePath).exists())
  {
    // Create a default "last paths" file if it does not exist
    std::map<QString, QString> lMap;
    for (const auto& lKey : DataLists::getLastPathsKeys())
    {
      lMap.insert({lKey, QString("")});
    }

    Utils::saveLastPathsToFile(lMap);
  }
}

std::map<QString, QString> Utils::loadLastPathsFromFile()
{
  Utils::checkLastPathsFileExistence();

  auto lLastPathsFilePath(Utils::getAppDataPathFolder() + "paths.json");
  QJsonObject lObtainedJSON{Utils::loadFromJsonFile(lLastPathsFilePath)};

  auto lVariantMap{lObtainedJSON.toVariantMap()};
  std::map<QString, QString> lLastPathsList;

  for (const auto& lKey : lVariantMap.keys())
  {
    lLastPathsList.insert({lKey, lVariantMap.value(lKey, "").toString()});
  }

  return lLastPathsList;
}

void Utils::saveLastPathsToFile(const std::map<QString, QString>& aList)
{
  auto lLastPathsFilePath{Utils::getAppDataPathFolder() + "paths.json"};
  Utils::saveAsJsonFile(Utils::lastPathsStructToJson(aList), lLastPathsFilePath);
}

QJsonObject Utils::lastPathsStructToJson(const std::map<QString, QString>& aList)
{
  QVariantMap lVarMap;

  for (const auto& lPair : aList)
  {
    lVarMap.insert(lPair.first, lPair.second);
  }

  return QJsonObject::fromVariantMap(lVarMap);
}

QString Utils::getPathFromKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aFallbackPath, const bool& aUseKeyPath)
{
  QString lPath{""};
  QString lKey{aKey};

  if (!aUseKeyPath)
  {
    lKey = "general";
  }

  for (auto lIt = aMap->begin(); lIt != aMap->end(); lIt++)
  {
    if (lIt->first == lKey)
    {
      lPath = lIt->second;
      break;
    }
  }

  if (lPath.length() > 0 && QFile::exists(lPath))
  {
    return lPath;
  }

  if (aFallbackPath.length() > 0 && QFile::exists(aFallbackPath))
  {
    return aFallbackPath;
  }

  return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void Utils::updatePathAtKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aPath)
{
  if (aPath.length() == 0)
  {
    return;
  }

  auto lModifiedPaths{0};
  for (auto lIt = aMap->begin(); lIt != aMap->end(); lIt++)
  {
    if (lIt->first == "general" || lIt->first == aKey)
    {
      lIt->second = aPath;

      if (++lModifiedPaths == 2)
      {
        break;
      }
    }
  }

  // The key did not exist, this happens when updating MFBOPC
  if (lModifiedPaths == 1)
  {
    // Create the new entry
    aMap->insert(std::pair<QString, QString>(aKey, aPath));
  }

  // Save the new list
  Utils::saveLastPathsToFile(*aMap);
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

QAction* Utils::buildQAction(QWidget* aParent, const QString& aText, const QKeySequence& aKeysCombination, const QString& aIconName, const QString& aIconFolder)
{
  auto lAction{new QAction(aParent)};
  lAction->setText(aText);
  lAction->setShortcut(aKeysCombination);
  lAction->setIcon(QIcon(QPixmap(QString(":/%1/%2").arg(aIconFolder).arg(aIconName))));
  return lAction;
}

void Utils::addIconToGroupBox(QGroupBox* aGroupBox, const QString& aIconFolder, const QString& aIconName)
{
  aGroupBox->setCheckable(true);
  aGroupBox->setStyleSheet(QString("QGroupBox::indicator{width: 16px; height: 16px; image: url(:/%1/%2)}").arg(aIconFolder).arg(aIconName));
}

void Utils::bindConsoleToStdOut()
{
  FreeConsole();
  AllocConsole();
  FILE* lOutFile{NULL};
  freopen_s(&lOutFile, "CONOUT$", "w", stdout);
  SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
}

void Utils::printMessageStdOut(const QString& aMessage)
{
  if (aMessage.length() == 0)
  {
    std::cout << std::endl;
  }
  else if (aMessage.endsWith("..."))
  {
    std::cout << aMessage.toStdString() << std::endl;
  }
  else
  {
    std::cout << aMessage.toStdString() << "." << std::endl;
  }
}