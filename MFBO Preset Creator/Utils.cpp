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
  auto lNumber{ 0 };
  auto lAbsFilePath{ QString("") };
  auto lRelativeDirs{ QString("") };

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

  QDomElement lRoot{ lOSPDocument.documentElement() };
  QDomElement lSliderSet{ lRoot.firstChild().toElement() };

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

  if (lFileContent.contains("beast hands", Qt::CaseInsensitive) ||
    lFileContent.contains("hands beast", Qt::CaseInsensitive))
  {
    return true;
  }

  return false;
}

void Utils::checkSettingsFileExistence()
{
  QFile lSettingsFile(QCoreApplication::applicationDirPath() + "/config.json");

  if (!lSettingsFile.exists()) {
    lSettingsFile.open(QIODevice::WriteOnly);
    lSettingsFile.close();
  }
}

QString Utils::parseLanguageFromSettingsFile()
{
  QFile lSettingsFile(QCoreApplication::applicationDirPath() + "/config.json");
  lSettingsFile.open(QIODevice::ReadOnly | QIODevice::Text);
  QString lSettingsData = lSettingsFile.readAll();
  lSettingsFile.close();

  QJsonDocument lJsonDocument(QJsonDocument::fromJson(lSettingsData.toUtf8()));
  QJsonObject lJsonObject = lJsonDocument.object();

  if (lJsonObject.contains("lang") && lJsonObject["lang"].isString()) {
    return lJsonObject["lang"].toString();
  }

  return "en";
}
