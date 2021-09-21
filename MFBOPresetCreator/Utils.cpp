#include "Utils.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Enum.h"
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDirIterator>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QVersionNumber>
#include <QtXml/QDomDocument>
#include <iostream>
#include <windows.h>

bool Utils::RESTART_PENDING = false;

void Utils::CleanPathString(QString& aPath)
{
  // Remove spaces before and after slashes
  auto lParts{aPath.split('/')};

  for (auto& lPart : lParts)
  {
    lPart = lPart.trimmed();
  }

  aPath = lParts.join('/');

  // Replace backslashes
  aPath.replace("\\", "/");

  // Remove any '\n' character
  Utils::CleanBreaksString(aPath);
}

QString Utils::CleanPathString(const QString& aPath)
{
  QString lPath(aPath);

  // Remove spaces before and after slashes
  auto lParts{lPath.split('/')};

  for (auto& lPart : lParts)
  {
    lPart = lPart.trimmed();
  }

  lPath = lParts.join('/');

  // Replace backslashes
  lPath.replace("\\", "/");

  // Remove any '\n' character
  const QString lCleanedPath(lPath);
  lPath = Utils::CleanBreaksString(lCleanedPath);

  return lPath;
}

void Utils::CleanBreaksString(QString& aString)
{
  aString.replace("\n", "");
}

QString Utils::CleanBreaksString(const QString& aString)
{
  QString lPath(aString);
  lPath.replace("\n", "");
  return lPath;
}

QStringList Utils::SplitString(QString aString, const QString& aSeparator)
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

QString Utils::GetApplicationVersion()
{
  return qApp->applicationVersion();
}

ApplicationVersionRelative Utils::CompareVersionNumbers(const QString& aVersionNumber)
{
  return Utils::CompareVersionNumbers(aVersionNumber, Utils::GetApplicationVersion());
}

ApplicationVersionRelative Utils::CompareVersionNumbers(const QString& aVersionNumber1, const QString& aVersionNumber2)
{
  auto lVersion1{QVersionNumber::fromString(aVersionNumber1).normalized()};
  auto lVersion2{QVersionNumber::fromString(aVersionNumber2).normalized()};

  // MAJOR
  if (lVersion1.majorVersion() < lVersion2.majorVersion())
  {
    return ApplicationVersionRelative::OLDER;
  }
  if (lVersion1.majorVersion() > lVersion2.majorVersion())
  {
    return ApplicationVersionRelative::NEWER;
  }

  // MINOR
  if (lVersion1.minorVersion() < lVersion2.minorVersion())
  {
    return ApplicationVersionRelative::OLDER;
  }
  if (lVersion1.minorVersion() > lVersion2.minorVersion())
  {
    return ApplicationVersionRelative::NEWER;
  }

  // MICRO
  if (lVersion1.microVersion() < lVersion2.microVersion())
  {
    return ApplicationVersionRelative::OLDER;
  }
  if (lVersion1.microVersion() > lVersion2.microVersion())
  {
    return ApplicationVersionRelative::NEWER;
  }

  return ApplicationVersionRelative::EQUIVALENT;
}

void Utils::DisplayWarningMessage(const QString& aMessage)
{
  QMessageBox lBox(QMessageBox::Icon::Warning, tr("Warning"), aMessage);
  lBox.setIconPixmap(QPixmap(":/icons/red-alert-circle").scaledToHeight(48, Qt::SmoothTransformation));

  QPushButton lButton(tr("OK"));
  lButton.setCursor(Qt::PointingHandCursor);
  lBox.addButton(&lButton, QMessageBox::ButtonRole::AcceptRole);

  lBox.exec();
}

ButtonClicked Utils::DisplayQuestionMessage(QWidget* aParent, const QString& aTitle, const QString& aMessage, const QString& aIconFolder, const QString& aIconName, const QString& aTextBtnYes, const QString& aTextBtnNo, const QString& aTextBtnOther, const QString& aColorYesBtn, const QString& aColorNoBtn, const QString& aColorOtherBtn, const bool aIsYesBtnDefault)
{
  QMessageBox lConfirmationBox(QMessageBox::Icon::Question, aTitle, aMessage, QMessageBox::StandardButton::NoButton, aParent);
  lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/%2").arg(aIconFolder, aIconName)).scaledToHeight(48, Qt::SmoothTransformation));

  auto lYesButton{lConfirmationBox.addButton(aTextBtnYes, QMessageBox::ButtonRole::YesRole)};
  lYesButton->setCursor(Qt::PointingHandCursor);
  if (!aColorYesBtn.isEmpty())
  {
    lYesButton->setStyleSheet(QString("color: %1;").arg(aColorYesBtn));
  }

  auto lNoButton{lConfirmationBox.addButton(aTextBtnNo, QMessageBox::ButtonRole::NoRole)};
  lNoButton->setCursor(Qt::PointingHandCursor);
  if (!aColorNoBtn.isEmpty())
  {
    lNoButton->setStyleSheet(QString("color: %1;").arg(aColorNoBtn));
  }

  QPushButton* lOtherButton{nullptr};
  if (!aTextBtnOther.isEmpty())
  {
    lOtherButton = lConfirmationBox.addButton(aTextBtnOther, QMessageBox::ButtonRole::HelpRole);
    lOtherButton->setCursor(Qt::PointingHandCursor);
    if (!aColorOtherBtn.isEmpty())
    {
      lOtherButton->setStyleSheet(QString("color: %1;").arg(aColorOtherBtn));
    }
  }

  lConfirmationBox.setDefaultButton(aIsYesBtnDefault ? lYesButton : lNoButton);
  lConfirmationBox.exec();

  // Return the clicked button
  const auto lClickedButton{lConfirmationBox.clickedButton()};
  if (lClickedButton == lYesButton)
  {
    return ButtonClicked::YES;
  }
  if (lClickedButton == lNoButton)
  {
    return ButtonClicked::NO;
  }
  if (lOtherButton != nullptr && lClickedButton == lOtherButton)
  {
    return ButtonClicked::OTHER;
  }
  return ButtonClicked::CLOSE_WINDOW;
}

VersionsInformation Utils::ParseGitHubReleasesRequestResult(const QString& aResult)
{
  VersionsInformation lVersionsInformation;
  auto lTagName{QString()};

  // Create a JSON from the fetched string and parse the "tag_name" data
  QJsonDocument lJsonDocument{QJsonDocument::fromJson(aResult.toUtf8())};
  QJsonArray lTagsArray{lJsonDocument.array()};

  // Iterate in the versions array
  for (int i = 0; i < lTagsArray.size(); i++)
  {
    // Parse the tag_name
    lTagName = Utils::CleanBreaksString(lTagsArray.at(i)["tag_name"].toString());

    // Check if it is a stable or a BETA version
    if (lTagsArray.at(i)["prerelease"].toBool())
    {
      lVersionsInformation.appendBetaVersion(lTagName, Utils::CleanBreaksString(lTagsArray.at(i)["body"].toString()));
    }
    else
    {
      lVersionsInformation.appendStableVersion(lTagName, Utils::CleanBreaksString(lTagsArray.at(i)["body"].toString()));
    }
  }

  return lVersionsInformation;
}

int Utils::GetNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension)
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

int Utils::GetNumberFilesByExtensionRecursive(const QString& aRootDir, const QString& aFileExtension)
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

int Utils::GetNumberFilesByExtensionRecursiveIgnoringFOMOD(const QString& aRootDir, const QString& aFileExtension)
{
  auto lNumber{0};
  auto lAbsFilePath{QString()};
  auto lRelativeDirs{QString()};

  QDirIterator it(aRootDir, QStringList() << aFileExtension, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();

    // Ignore FOMOD directory
    lAbsFilePath = it.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.remove(aRootDir, Qt::CaseInsensitive);

    if (aFileExtension.compare("*.xml", Qt::CaseInsensitive) == 0
        && (lRelativeDirs.contains("/fomod/info.xml", Qt::CaseInsensitive) || lRelativeDirs.contains("/fomod/ModuleConfig.xml", Qt::CaseInsensitive)))
    {
      continue;
    }

    lNumber++;
  }

  return lNumber;
}

int Utils::GetNumberFilesByExtensions(const QString& aRootDir, const QStringList& aFileExtensions)
{
  auto lNumber{0};

  QDirIterator lIt(aRootDir, aFileExtensions, QDir::Files, QDirIterator::NoIteratorFlags);
  while (lIt.hasNext())
  {
    lIt.next();
    lNumber++;
  }

  return lNumber;
}

int Utils::GetNumberFilesByExtensionsRecursive(const QString& aRootDir, const QStringList& aFileExtensions)
{
  auto lNumber{0};

  QDirIterator lIt(aRootDir, aFileExtensions, QDir::Files, QDirIterator::Subdirectories);
  while (lIt.hasNext())
  {
    lIt.next();
    lNumber++;
  }

  return lNumber;
}

bool Utils::CopyRecursively(const QString& aSourcePath, const QString& aDestinationPath)
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

    if (!Utils::CopyRecursively(lSourceName, lDestinationName))
    {
      return false;
    }
  }

  return true;
}

bool Utils::RemoveDirectoryAndSubDirs(const QString& aPath)
{
  QFileInfo lFileInfo(aPath);
  if (!lFileInfo.isDir())
  {
    return false;
  }

  return QDir(aPath).removeRecursively();
}

bool Utils::IsThemeDark(const GUITheme& aTheme)
{
  switch (aTheme)
  {
    case GUITheme::MITSURIOU_BLACK_THEME:
    case GUITheme::MITSURIOU_DARK_THEME:
    case GUITheme::ALEXHUSZAGH_BREEZE_DARK:
    case GUITheme::PAPER_DARK:
    case GUITheme::PAPER_BLACK_MONO:
      return true;
    default:
      return false;
  }
}

QString Utils::GetIconRessourceFolder(const GUITheme& aTheme)
{
  return (Utils::IsThemeDark(aTheme) ? QString("white") : QString("black"));
}

BCGroupWidgetCallContext Utils::GetMeshTypeFromFileName(const QString& aFileName)
{
  // Body
  if (aFileName.endsWith("femalebody"))
    return BCGroupWidgetCallContext::BODY;

  // Feet
  if (aFileName.endsWith("femalefeet"))
    return BCGroupWidgetCallContext::FEET;

  // Hands
  if (aFileName.endsWith("femalehands") || aFileName.endsWith("femalehandsargonian"))
    return BCGroupWidgetCallContext::HANDS;

  // Head
  if (aFileName.endsWith("femalehead"))
    return BCGroupWidgetCallContext::HEAD;

  // Eyes
  if (aFileName.endsWith("eyesfemale"))
    return BCGroupWidgetCallContext::EYES;

  // Skeleton
  if (aFileName.mid(aFileName.lastIndexOf("/") + 1).contains("skeleton", Qt::CaseInsensitive))
    return BCGroupWidgetCallContext::SKELETON;

  return BCGroupWidgetCallContext::UNDEFINED;
}

bool Utils::ContainsBodyOrHandsOrFeetMesh(const std::set<QString>& aList)
{
  auto lRessourceType{BCGroupWidgetCallContext::UNDEFINED};

  // Iterate through the list of meshes paths
  for (const auto& lValue : aList)
  {
    lRessourceType = Utils::GetMeshTypeFromFileName(lValue);

    // Check if the ressource if of any useful type
    if (lRessourceType == BCGroupWidgetCallContext::BODY || lRessourceType == BCGroupWidgetCallContext::FEET || lRessourceType == BCGroupWidgetCallContext::HANDS)
    {
      return true;
    }
  }

  return false;
}

bool Utils::IsCBBEBasedBody(const BodyNameVersion& aBody)
{
  switch (aBody)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
    case BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04:
    case BodyNameVersion::CBBE_3BBB_3BA_2_06:
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
      return true;
    default: // UNP-based bodies
      return false;
  }
}

bool Utils::IsCBBEBasedBody(const BodyName& aBody)
{
  switch (aBody)
  {
    case BodyName::CBBE_3BBB_3BA:
    case BodyName::CBBE_SMP_3BBB:
    case BodyName::MIMIR_EBONIC_BODY:
      return true;
    default: // UNP-based bodies
      return false;
  }
}

bool Utils::IsBodySupportingBeastHands(const BodyNameVersion& aBody)
{
  return Utils::IsCBBEBasedBody(aBody);
}

bool Utils::IsRunningStandaloneVersion()
{
  QDir lDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
  return !lDir.exists();
}

QString Utils::GetAppDataPathFolder()
{
#if defined(DEBUG) || !defined(QT_NO_DEBUG)
  return QCoreApplication::applicationDirPath() + QDir::separator();
#else
  QDir lDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
  if (lDir.exists())
  {
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator();
  }

  return QCoreApplication::applicationDirPath() + QDir::separator();
#endif
}

QString Utils::ReadQRCFileContent(const QString& aFilePath)
{
  QString lContent;

  QFile lQRCFile(aFilePath);
  if (lQRCFile.open(QFile::ReadOnly | QFile::Text))
  {
    QTextStream lFileBuffer(&lQRCFile);
    lContent = lFileBuffer.readAll();
  }
  else
  {
    // Display a message in case the file cannot be found
    Utils::DisplayWarningMessage(tr("The internal file \"%1\" could not be found. Please report this issue to the developer with the steps to reproduce this bug.").arg(aFilePath));
  }

  return lContent;
}

QString Utils::GetPresetNameFromXMLFile(const QString& aPath)
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
    Utils::DisplayWarningMessage(tr("Error while trying to read the file \"%1\".").arg(aPath));
    return QString();
  }

  auto lXMLGroup{lXMLDocument.documentElement().firstChild().toElement()};
  auto lPresetName{QString()};

  if (lXMLGroup.tagName() == "Group")
  {
    // Get the first "Member" tag to read its "name" attribute
    auto lXMLMember{lXMLGroup.firstChild().toElement()};
    lPresetName = lXMLMember.attribute("name", "");
  }

  return lPresetName.left(lPresetName.lastIndexOf(QChar('-')) - 1);
}

std::vector<Struct::SliderSet> Utils::GetOutputPathsFromOSPFile(const QString& aPath)
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
    Utils::DisplayWarningMessage(tr("Error while trying to read the file \"%1\".").arg(aPath));
    return std::vector<Struct::SliderSet>();
  }

  auto lRoot{lOSPDocument.documentElement()};
  auto lSliderSet{lRoot.firstChild().toElement()};

  while (!lSliderSet.isNull())
  {
    if (lSliderSet.tagName() == "SliderSet")
    {
      Struct::SliderSet lTempSet;
      lTempSet.setName(lSliderSet.attribute("name", "")); // Name

      // TODO: Improve these checks:
      if (lTempSet.getName().endsWith(" - BHUNP 3BBB", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Ver 2", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Ver 2 Nevernude", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP BBP", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP BBP Advanced", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP TBBP", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP TBBP Advanced", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - SE 3BBB Body Amazing", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - 3BBB Body Amazing", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - CBBE 3BBB Body Amazing", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - CBBE Body SMP (3BBB)", Qt::CaseInsensitive))
      {
        lTempSet.setMeshPart("Body"); //Body
      }
      else if (lTempSet.getName().endsWith(" - Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - Beast Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE Beast Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE Hands Beast", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE 3BBB Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE 3BBB Hands Beast", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Hands", Qt::CaseInsensitive))
      {
        lTempSet.setMeshPart("Hands"); // Hands
      }
      else if (lTempSet.getName().endsWith(" - Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE 3BBB Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - Feet (MSF - normal)", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - Feet (MSF - HH)", Qt::CaseInsensitive))
      {
        lTempSet.setMeshPart("Feet"); // Feet
      }

      auto lChild{lSliderSet.firstChild().toElement()};
      while (!lChild.isNull())
      {
        if (lChild.tagName() == "OutputPath")
        {
          lTempSet.setOutputPath(lChild.firstChild().toText().data()); // OutputPath
        }
        else if (lChild.tagName() == "OutputFile")
        {
          lTempSet.setOutputFile(lChild.firstChild().toText().data()); // OutputFile
        }

        if (lTempSet.getOutputPath() != "" && lTempSet.getOutputFile() != "")
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

bool Utils::IsPresetUsingBeastHands(const QString& aPath)
{
  QFile lReadFile(aPath);

  auto lFileContent{QString()};

  if (lReadFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    lFileContent = static_cast<QString>(lReadFile.readAll());
    lReadFile.close();
  }
  else
  {
    Utils::DisplayWarningMessage(tr("Error while trying to read the file \"%1\".").arg(aPath));
    return false;
  }

  if (lFileContent.contains("beast hands", Qt::CaseInsensitive) || lFileContent.contains("hands beast", Qt::CaseInsensitive))
  {
    return true;
  }

  return false;
}

QString Utils::GetHandsSliderValue(const BodyNameVersion& aBody, const bool aMustUseBeastHands)
{
  if (Utils::IsCBBEBasedBody(aBody) && aMustUseBeastHands)
  {
    return QString("%1 - CBBE Hands Beast");
  }

  switch (aBody)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
    case BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04:
    case BodyNameVersion::CBBE_3BBB_3BA_2_06:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
      return QString("%1 - CBBE 3BBB Hands");
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      return QString("%1 - CBBE Hands");
    default: // UNP-based bodies
      return QString("%1 - BHUNP 3BBB Advanced Hands");
  }
}

QString Utils::GetFeetSliderValue(const BodyNameVersion& aBody, const int aFeetModIndex)
{
  auto lFeetValue{QString()};

  if (Utils::IsCBBEBasedBody(aBody))
  {
    switch (aFeetModIndex)
    {
      case 0:
        // Default
        switch (aBody)
        {
          case BodyNameVersion::CBBE_3BBB_3BA_1_50:
          case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
          case BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04:
          case BodyNameVersion::CBBE_3BBB_3BA_2_06:
          case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
          case BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
            lFeetValue = QString("%1 - CBBE 3BBB Feet");
            break;
          case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
            lFeetValue = QString("%1 - CBBE Feet");
            break;
        }
        break;
      case 1:
        // More Sliders for Feet - Normal
        lFeetValue = QString("%1 - Feet (MSF - normal)");
        break;
      case 2:
        // More Sliders for Feet - High Heels
        lFeetValue = QString("%1 - Feet (MSF - HH)");
        break;
    }
  }
  else
  {
    switch (aFeetModIndex)
    {
      case 0:
        // Default
        lFeetValue = QString("%1 - BHUNP 3BBB Advanced Feet");
        break;
      case 1:
        // More Sliders for Feet - Normal
        lFeetValue = QString("%1 - Feet (MSF - normal)");
        break;
      case 2:
        // More Sliders for Feet - High Heels
        lFeetValue = QString("%1 - Feet (MSF - HH)");
        break;
      case 3:
        // HG Feet and Toes BHUNP SE - HGFeet UUNP
        lFeetValue = QString("%1 - HGFeet UUNP");
        break;
      case 4:
        // Khrysamere HG Feet (BHUNP)
        lFeetValue = QString("%1 - [Khrysamere] HG Feet (BHUNP)");
        break;
      case 5:
        // Khrysamere HG Feet (Claws) (BHUNP)
        lFeetValue = QString("%1 - [Khrysamere] HG Feet (Claws) (BHUNP)");
        break;
    }
  }

  return (lFeetValue + "\n");
}

QString Utils::GetBodySliderValue(const BodyNameVersion& aBody)
{
  auto lBodyVersion{DataLists::GetSplittedNameVersionFromBodyVersion(aBody)};
  auto lCastedBodyName{static_cast<BodyName>(lBodyVersion.first)};

  auto lBodyValue{QString()};

  switch (lCastedBodyName)
  {
    case BodyName::CBBE_3BBB_3BA:
    case BodyName::MIMIR_EBONIC_BODY:
      lBodyValue = QString("%1 - CBBE 3BBB Body Amazing");
      break;
    case BodyName::CBBE_SMP_3BBB:
      lBodyValue = QString("%1 - CBBE Body SMP (3BBB)");
      break;
    case BodyName::BHUNP_3BBB:
      lBodyValue = QString("%1 - BHUNP 3BBB");
      break;
    case BodyName::BHUNP_3BBB_ADVANCED:
      lBodyValue = QString("%1 - BHUNP 3BBB Advanced");
      break;
    case BodyName::BHUNP_3BBB_ADVANCED_VER_2:
      lBodyValue = QString("%1 - BHUNP 3BBB Advanced Ver 2");
      break;
    case BodyName::BHUNP_BBP:
      lBodyValue = QString("%1 - BHUNP BBP");
      break;
    case BodyName::BHUNP_BBP_ADVANCED:
      lBodyValue = QString("%1 - BHUNP BBP Advanced");
      break;
    case BodyName::BHUNP_TBBP:
      lBodyValue = QString("%1 - BHUNP TBBP");
      break;
    case BodyName::BHUNP_TBBP_ADVANCED:
      lBodyValue = QString("%1 - BHUNP TBBP Advanced");
      break;
    case BodyName::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      lBodyValue = QString("%1 - BHUNP 3BBB Advanced Ver 2 Nevernude");
      break;
  }

  return (lBodyValue + "\n");
}

void Utils::SaveAsJsonFile(const QJsonObject& aJsonToSave, const QString& aFilePath, QWidget* aParent, const QString& aIconFolder)
{
  // Open (or create and open) the file
  QFile lFile(aFilePath);
  if (lFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
  {
    lFile.write(QJsonDocument(aJsonToSave).toJson());

    // Project file save: success window
    if (aParent && !aIconFolder.isEmpty())
    {
      QMessageBox lConfirmationBox(QMessageBox::Icon::Information,
                                   tr("Project successfully saved"),
                                   tr("The project file has successfully been saved to \"%1\".").arg(aFilePath),
                                   QMessageBox::StandardButton::NoButton,
                                   aParent);
      lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/alert-circle").arg(aIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

      auto lOKButton{lConfirmationBox.addButton(tr("OK"), QMessageBox::ButtonRole::AcceptRole)};
      lOKButton->setCursor(Qt::PointingHandCursor);
      lConfirmationBox.setDefaultButton(lOKButton);
      lConfirmationBox.exec();
    }
  }
  else if (aParent && !aIconFolder.isEmpty())
  {
    // Project file save: fail window
    Utils::DisplayWarningMessage(tr("Could not save the project file to \"%1\".\nBe sure to not save the file in a OneDrive/DropBox space and that you executed the application with sufficient permissions.\nBe sure that you used characters authorized by your OS in the given paths.").arg(aFilePath));
  }

  lFile.close();
}

QJsonObject Utils::LoadFromJsonFile(const QString& aFilePath)
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

void Utils::CheckSettingsFileExistence()
{
  auto lSettingsFilePath{Utils::GetAppDataPathFolder() + "config.json"};
  if (!QFile(lSettingsFilePath).exists())
  {
    // Create a default setting file if it does not exist
    Utils::SaveSettingsToFile(Struct::Settings());
  }
}

Struct::Settings Utils::LoadSettingsFromFile()
{
  Utils::CheckSettingsFileExistence();

  auto lSettingsFilePath{Utils::GetAppDataPathFolder() + "config.json"};
  QJsonObject lSettingsJSON{Utils::LoadFromJsonFile(lSettingsFilePath)};

  Struct::Settings lSettings;

  // Font family
  if (lSettingsJSON.contains("font"))
  {
    auto lFontJSON{lSettingsJSON["font"].toObject()};

    if (lFontJSON.contains("family") && lFontJSON["family"].isString())
      lSettings.font.family = lFontJSON["family"].toString();

    if (lFontJSON.contains("italic") && lFontJSON["italic"].isBool())
      lSettings.font.italic = lFontJSON["italic"].toBool();

    if (lFontJSON.contains("size") && lFontJSON["size"].isDouble())
      lSettings.font.size = lFontJSON["size"].toInt();

    if (lFontJSON.contains("strikeOut") && lFontJSON["strikeOut"].isBool())
      lSettings.font.strikeOut = lFontJSON["strikeOut"].toBool();

    if (lFontJSON.contains("styleName") && lFontJSON["styleName"].isString())
      lSettings.font.styleName = lFontJSON["styleName"].toString();

    if (lFontJSON.contains("underline") && lFontJSON["underline"].isBool())
      lSettings.font.underline = lFontJSON["underline"].toBool();

    if (lFontJSON.contains("weight") && lFontJSON["weight"].isDouble())
      lSettings.font.weight = lFontJSON["weight"].toInt();
  }

  // Dark theme
  if (lSettingsJSON.contains("appTheme") && lSettingsJSON["appTheme"].isDouble())
    lSettings.appTheme = static_cast<GUITheme>(lSettingsJSON["appTheme"].toInt());

  // Opening mode: assisted conversion
  if (lSettingsJSON.contains("assistedConversionDialogOpeningMode") && lSettingsJSON["assistedConversionDialogOpeningMode"].isDouble())
    lSettings.assistedConversionDialogOpeningMode = static_cast<DialogOpeningMode>(lSettingsJSON["assistedConversionDialogOpeningMode"].toInt());

  // Assisted Conversion: only scan the meshes subdir
  if (lSettingsJSON.contains("assistedConversionScanOnlyMeshesSubdir") && lSettingsJSON["assistedConversionScanOnlyMeshesSubdir"].isBool())
    lSettings.assistedConversionScanOnlyMeshesSubdir = lSettingsJSON["assistedConversionScanOnlyMeshesSubdir"].toBool();

  // Opening mode: batch conversion
  if (lSettingsJSON.contains("batchConversionDialogOpeningMode") && lSettingsJSON["batchConversionDialogOpeningMode"].isDouble())
    lSettings.batchConversionDialogOpeningMode = static_cast<DialogOpeningMode>(lSettingsJSON["batchConversionDialogOpeningMode"].toInt());

  // Batch Conversion: output path
  if (lSettingsJSON.contains("batchConversionOutputPath") && lSettingsJSON["batchConversionOutputPath"].isString())
    lSettings.batchConversionOutputPath = lSettingsJSON["batchConversionOutputPath"].toString();

  // Opening mode: batch conversion picker
  if (lSettingsJSON.contains("batchConversionPickerDialogOpeningMode") && lSettingsJSON["batchConversionPickerDialogOpeningMode"].isDouble())
    lSettings.batchConversionPickerDialogOpeningMode = static_cast<DialogOpeningMode>(lSettingsJSON["batchConversionPickerDialogOpeningMode"].toInt());

  // Opening mode: bodySlide presets' retargeting
  if (lSettingsJSON.contains("bodySlidePresetsRetargetingDialogOpeningMode") && lSettingsJSON["bodySlidePresetsRetargetingDialogOpeningMode"].isDouble())
    lSettings.bodySlidePresetsRetargetingDialogOpeningMode = static_cast<DialogOpeningMode>(lSettingsJSON["bodySlidePresetsRetargetingDialogOpeningMode"].toInt());

  // Danger color
  if (lSettingsJSON.contains("dangerColor") && lSettingsJSON["dangerColor"].isString())
    lSettings.dangerColor = lSettingsJSON["dangerColor"].toString();

  // Batch Conversion: Default body name and version
  if (lSettingsJSON.contains("defaultBatchConversionBody") && lSettingsJSON["defaultBatchConversionBody"].isDouble())
    lSettings.defaultBatchConversionBody = static_cast<BodyNameVersion>(lSettingsJSON["defaultMainWindowBody"].toInt());

  // Preset Creator: Default feet mod
  if (lSettingsJSON.contains("defaultMainFeetMod") && lSettingsJSON["defaultMainFeetMod"].isDouble())
    lSettings.defaultMainFeetMod = lSettingsJSON["defaultMainFeetMod"].toInt();

  // Preset Creator: Default body name and version
  if (lSettingsJSON.contains("defaultMainWindowBody") && lSettingsJSON["defaultMainWindowBody"].isDouble())
    lSettings.defaultMainWindowBody = static_cast<BodyNameVersion>(lSettingsJSON["defaultMainWindowBody"].toInt());

  // Default BodySlide Presets' Retargeting body name and version
  if (lSettingsJSON.contains("defaultRetargetingToolBody") && lSettingsJSON["defaultRetargetingToolBody"].isDouble())
    lSettings.defaultRetargetingToolBody = static_cast<BodyNameVersion>(lSettingsJSON["defaultRetargetingToolBody"].toInt());

  // Retargeting Tool: Default feet mod
  if (lSettingsJSON.contains("defaultRetargetingToolFeetMod") && lSettingsJSON["defaultRetargetingToolFeetMod"].isDouble())
    lSettings.defaultRetargetingToolFeetMod = lSettingsJSON["defaultRetargetingToolFeetMod"].toInt();

  // Each button stores the last opened path
  if (lSettingsJSON.contains("eachButtonSavesItsLastUsedPath") && lSettingsJSON["eachButtonSavesItsLastUsedPath"].isBool())
    lSettings.eachButtonSavesItsLastUsedPath = lSettingsJSON["eachButtonSavesItsLastUsedPath"].toBool();

  // Language
  if (lSettingsJSON.contains("language") && lSettingsJSON["language"].isDouble())
    lSettings.language = static_cast<ApplicationLanguage>(lSettingsJSON["language"].toInt());

  // Main window: automatically open generated dir
  if (lSettingsJSON.contains("mainWindowAutomaticallyOpenGeneratedDirectory") && lSettingsJSON["mainWindowAutomaticallyOpenGeneratedDirectory"].isBool())
    lSettings.mainWindowAutomaticallyOpenGeneratedDirectory = lSettingsJSON["mainWindowAutomaticallyOpenGeneratedDirectory"].toBool();

  // Default window height
  if (lSettingsJSON.contains("mainWindowHeight") && lSettingsJSON["mainWindowHeight"].isDouble())
    lSettings.mainWindowHeight = lSettingsJSON["mainWindowHeight"].toInt();

  // Opening mode: main window
  if (lSettingsJSON.contains("mainWindowOpeningMode") && lSettingsJSON["mainWindowOpeningMode"].isDouble())
    lSettings.mainWindowOpeningMode = static_cast<WindowOpeningMode>(lSettingsJSON["mainWindowOpeningMode"].toInt());

  // Main window output path
  if (lSettingsJSON.contains("mainWindowOutputPath") && lSettingsJSON["mainWindowOutputPath"].isString())
    lSettings.mainWindowOutputPath = lSettingsJSON["mainWindowOutputPath"].toString();

  // Default window width
  if (lSettingsJSON.contains("mainWindowWidth") && lSettingsJSON["mainWindowWidth"].isDouble())
    lSettings.mainWindowWidth = lSettingsJSON["mainWindowWidth"].toInt();

  // Retargeting tool: automatically open retargeted dir
  if (lSettingsJSON.contains("retargetingToolAutomaticallyOpenGeneratedDirectory") && lSettingsJSON["retargetingToolAutomaticallyOpenGeneratedDirectory"].isBool())
    lSettings.retargetingToolAutomaticallyOpenGeneratedDirectory = lSettingsJSON["retargetingToolAutomaticallyOpenGeneratedDirectory"].toBool();

  // Show welcome screen at application startup
  if (lSettingsJSON.contains("startupAction") && lSettingsJSON["startupAction"].isDouble())
    lSettings.startupAction = static_cast<StartupAction>(lSettingsJSON["startupAction"].toInt());

  // Success color
  if (lSettingsJSON.contains("successColor") && lSettingsJSON["successColor"].isString())
    lSettings.successColor = lSettingsJSON["successColor"].toString();

  // Opening mode: textures assistant
  if (lSettingsJSON.contains("texturesAssistantDialogOpeningMode") && lSettingsJSON["texturesAssistantDialogOpeningMode"].isDouble())
    lSettings.texturesAssistantDialogOpeningMode = static_cast<DialogOpeningMode>(lSettingsJSON["texturesAssistantDialogOpeningMode"].toInt());

  // Warning color
  if (lSettingsJSON.contains("warningColor") && lSettingsJSON["warningColor"].isString())
    lSettings.warningColor = lSettingsJSON["warningColor"].toString();

  // JSON format upgrade from v2.10.2.0 to 3.0.0.0
  if (!lSettingsJSON.contains("applicationVersion"))
  {
    // "windowWidth" -> "mainWindowWidth"
    if (lSettingsJSON.contains("windowWidth") && lSettingsJSON["windowWidth"].isDouble())
      lSettings.mainWindowWidth = lSettingsJSON["windowWidth"].toInt();

    // "windowHeight" -> "mainWindowHeight"
    if (lSettingsJSON.contains("windowHeight") && lSettingsJSON["windowHeight"].isDouble())
      lSettings.mainWindowHeight = lSettingsJSON["windowHeight"].toInt();

    // "retargetingToolDefaultBody" -> "defaultRetargetingToolBody"
    if (lSettingsJSON.contains("retargetingToolDefaultBody") && lSettingsJSON["retargetingToolDefaultBody"].isDouble())
      lSettings.defaultRetargetingToolBody = static_cast<BodyNameVersion>(lSettingsJSON["retargetingToolDefaultBody"].toInt());

    // "defaultBody" -> "defaultMainWindowBody"
    if (lSettingsJSON.contains("defaultBody") && lSettingsJSON["defaultBody"].isDouble())
      lSettings.defaultMainWindowBody = static_cast<BodyNameVersion>(lSettingsJSON["defaultBody"].toInt());

    Utils::SaveSettingsToFile(lSettings);
    Utils::PrintMessageStdOut("User settings upgraded!");
  }
  // For any JSON format upgrade / settings values changed
  else if (Utils::CompareVersionNumbers(lSettingsJSON["applicationVersion"].toString(), "3.2.0.0") == ApplicationVersionRelative::OLDER)
  {
    lSettings.defaultMainWindowBody = BodyNameVersion::CBBE_3BBB_3BA_1_50;
    lSettings.defaultRetargetingToolBody = BodyNameVersion::CBBE_3BBB_3BA_1_50;
    lSettings.defaultBatchConversionBody = BodyNameVersion::CBBE_3BBB_3BA_1_50;
    lSettings.appTheme = GUITheme::MITSURIOU_DARK_THEME;

    Utils::SaveSettingsToFile(lSettings);
    Utils::PrintMessageStdOut("User settings upgraded!");
  }

  Utils::PrintMessageStdOut("User settings:");
  Utils::PrintMessageStdOut(QJsonDocument(Utils::SettingsStructToJson(lSettings)).toJson(QJsonDocument::JsonFormat::Indented));

  return lSettings;
}

void Utils::SaveSettingsToFile(const Struct::Settings& aSettings)
{
  auto lSettingsFilePath{Utils::GetAppDataPathFolder() + "config.json"};
  Utils::SaveAsJsonFile(Utils::SettingsStructToJson(aSettings), lSettingsFilePath);
}

QJsonObject Utils::SettingsStructToJson(const Struct::Settings& aSettings)
{
  // Construct a font subobject
  QJsonObject lFontObj;
  lFontObj["family"] = aSettings.font.family;
  lFontObj["italic"] = aSettings.font.italic;
  lFontObj["size"] = aSettings.font.size;
  lFontObj["strikeOut"] = aSettings.font.strikeOut;
  lFontObj["styleName"] = aSettings.font.styleName;
  lFontObj["underline"] = aSettings.font.underline;
  lFontObj["weight"] = aSettings.font.weight;

  // Construct the full settings object
  QJsonObject lSettings;
  lSettings["appTheme"] = static_cast<int>(aSettings.appTheme);
  lSettings["assistedConversionDialogOpeningMode"] = static_cast<int>(aSettings.assistedConversionDialogOpeningMode);
  lSettings["assistedConversionScanOnlyMeshesSubdir"] = aSettings.assistedConversionScanOnlyMeshesSubdir;
  lSettings["batchConversionDialogOpeningMode"] = static_cast<int>(aSettings.batchConversionDialogOpeningMode);
  lSettings["batchConversionOutputPath"] = aSettings.batchConversionOutputPath;
  lSettings["batchConversionPickerDialogOpeningMode"] = static_cast<int>(aSettings.batchConversionPickerDialogOpeningMode);
  lSettings["bodySlidePresetsRetargetingDialogOpeningMode"] = static_cast<int>(aSettings.bodySlidePresetsRetargetingDialogOpeningMode);
  lSettings["dangerColor"] = aSettings.dangerColor;
  lSettings["defaultBatchConversionBody"] = static_cast<int>(aSettings.defaultBatchConversionBody);
  lSettings["defaultMainFeetMod"] = aSettings.defaultMainFeetMod;
  lSettings["defaultMainWindowBody"] = static_cast<int>(aSettings.defaultMainWindowBody);
  lSettings["defaultRetargetingToolBody"] = static_cast<int>(aSettings.defaultRetargetingToolBody);
  lSettings["defaultRetargetingToolFeetMod"] = aSettings.defaultRetargetingToolFeetMod;
  lSettings["eachButtonSavesItsLastUsedPath"] = aSettings.eachButtonSavesItsLastUsedPath;
  lSettings["font"] = lFontObj;
  lSettings["language"] = static_cast<int>(aSettings.language);
  lSettings["mainWindowAutomaticallyOpenGeneratedDirectory"] = aSettings.mainWindowAutomaticallyOpenGeneratedDirectory;
  lSettings["mainWindowHeight"] = aSettings.mainWindowHeight;
  lSettings["mainWindowOpeningMode"] = static_cast<int>(aSettings.mainWindowOpeningMode);
  lSettings["mainWindowOutputPath"] = aSettings.mainWindowOutputPath;
  lSettings["mainWindowWidth"] = aSettings.mainWindowWidth;
  lSettings["retargetingToolAutomaticallyOpenGeneratedDirectory"] = aSettings.retargetingToolAutomaticallyOpenGeneratedDirectory;
  lSettings["startupAction"] = static_cast<int>(aSettings.startupAction);
  lSettings["successColor"] = aSettings.successColor;
  lSettings["texturesAssistantDialogOpeningMode"] = static_cast<int>(aSettings.texturesAssistantDialogOpeningMode);
  lSettings["warningColor"] = aSettings.warningColor;

  // Version number string
  lSettings["applicationVersion"] = Utils::GetApplicationVersion();

  return lSettings;
}

void Utils::CheckFiltersFileExistence()
{
  auto lFiltersFilePath{Utils::GetAppDataPathFolder() + "filters.json"};
  if (!QFile(lFiltersFilePath).exists())
  {
    // Create a default filters file if it does not exist
    Utils::SaveFiltersToFile(std::map<QString, QStringList>());
  }
}

std::map<QString, QStringList> Utils::LoadFiltersFromFile()
{
  Utils::CheckFiltersFileExistence();

  auto lFiltersFilePath(Utils::GetAppDataPathFolder() + "filters.json");
  QJsonObject lObtainedJSON{Utils::LoadFromJsonFile(lFiltersFilePath)};

  auto lVariantMap{lObtainedJSON.toVariantMap()};
  std::map<QString, QStringList> lFiltersList;

  for (const auto& lKey : lVariantMap.keys())
  {
    lFiltersList.insert(std::make_pair(lKey, lVariantMap.value(lKey, "").toStringList()));
  }

  return lFiltersList;
}

void Utils::SaveFiltersToFile(const std::map<QString, QStringList>& aList)
{
  auto lFiltersFilePath{Utils::GetAppDataPathFolder() + "filters.json"};
  Utils::SaveAsJsonFile(Utils::FiltersMapToJson(aList), lFiltersFilePath);
}

QJsonObject Utils::FiltersMapToJson(const std::map<QString, QStringList>& aList)
{
  QVariantMap lVarMap;

  for (const auto& lPair : aList)
  {
    lVarMap.insert(lPair.first, lPair.second);
  }

  return QJsonObject::fromVariantMap(lVarMap);
}

QString Utils::GetAdditionalFeetFilter(const BodyNameVersion& aBody, const int aFeetModIndex)
{
  if (aFeetModIndex == 1 || aFeetModIndex == 2)
  {
    if (Utils::IsCBBEBasedBody(aBody))
    {
      return QString("MSF CBBE Feet");
    }

    return QString("MSF BHUNP Feet");
  }
  else if (!Utils::IsCBBEBasedBody(aBody) && (aFeetModIndex == 3 || aFeetModIndex == 4 || aFeetModIndex == 5))
  {
    return QString("HGFeet UUNP");
  }

  return QString();
}

std::vector<Struct::Filter> Utils::GetFiltersForExport(const std::map<QString, QStringList>& aList, const QString& aKey, const BodyNameVersion& aBody, const int aFeetModIndex)
{
  auto lExportFilters{std::vector<Struct::Filter>()};

  for (const auto& lPair : aList)
  {
    if (lPair.first.compare(aKey, Qt::CaseSensitive) == 0)
    {
      // Parse user' sliders
      for (const auto& lFilter : lPair.second)
      {
        lExportFilters.push_back(Struct::Filter(lFilter, true, true, true));
      }

      // Additional feet slider
      lExportFilters.push_back(Struct::Filter(Utils::GetAdditionalFeetFilter(aBody, aFeetModIndex), false, true, false));

      break;
    }
  }

  return lExportFilters;
}

void Utils::CheckLastPathsFileExistence()
{
  auto lLastPathsFilePath{Utils::GetAppDataPathFolder() + "paths.json"};
  if (!QFile(lLastPathsFilePath).exists())
  {
    // Create a default "last paths" file if it does not exist
    std::map<QString, QString> lMap;
    for (const auto& lKey : DataLists::GetLastPathsKeys())
    {
      lMap.insert(std::make_pair(lKey, QString()));
    }

    Utils::SaveLastPathsToFile(lMap);
  }
}

std::map<QString, QString> Utils::LoadLastPathsFromFile()
{
  Utils::CheckLastPathsFileExistence();

  auto lLastPathsFilePath(Utils::GetAppDataPathFolder() + "paths.json");
  QJsonObject lObtainedJSON{Utils::LoadFromJsonFile(lLastPathsFilePath)};

  auto lVariantMap{lObtainedJSON.toVariantMap()};
  std::map<QString, QString> lLastPathsList{
    {"assistedConversionInput", ""},
    {"batchConversionInput", ""},
    {"batchConversionOutput", ""},
    {"general", ""},
    {"lastLoadedProject", ""},
    {"lastSavedProject", ""},
    {"mainWindowOutput", ""},
    {"retargetingToolInput", ""},
    {"retargetingToolOutput", ""},
    {"texturesAssistantInput", ""}};

  for (const auto& lKey : lVariantMap.keys())
  {
    lLastPathsList.find(lKey)->second = lVariantMap.value(lKey, "").toString();
  }

  return lLastPathsList;
}

void Utils::SaveLastPathsToFile(const std::map<QString, QString>& aList)
{
  auto lLastPathsFilePath{Utils::GetAppDataPathFolder() + "paths.json"};
  Utils::SaveAsJsonFile(Utils::LastPathsStructToJson(aList), lLastPathsFilePath);
}

QJsonObject Utils::LastPathsStructToJson(const std::map<QString, QString>& aList)
{
  QVariantMap lVarMap;

  for (const auto& lPair : aList)
  {
    lVarMap.insert(lPair.first, lPair.second);
  }

  return QJsonObject::fromVariantMap(lVarMap);
}

QString Utils::GetPathFromKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aFallbackPath, const bool aUseKeyPath)
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

  if (!lPath.isEmpty() && QFile::exists(lPath))
  {
    return lPath;
  }

  if (!aFallbackPath.isEmpty() && QFile::exists(aFallbackPath))
  {
    return aFallbackPath;
  }

  return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

bool Utils::UpdatePathAtKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aPath, const bool aMustUpdateGeneralKey, const bool aAuthorizeEmptyValue, const bool aMustSaveFile)
{
  if (aKey.isEmpty() || (!aAuthorizeEmptyValue && aPath.isEmpty()))
  {
    // Return that not any modification has been made to the list
    return false;
  }

  const auto lMapCopy{*aMap};
  auto lModifiedPaths{0};
  for (auto lIt = aMap->begin(); lIt != aMap->end(); lIt++)
  {
    if (lIt->first == aKey || (aMustUpdateGeneralKey && lIt->first == "general"))
    {
      lIt->second = aPath;

      if (++lModifiedPaths == 2)
      {
        break;
      }
    }
  }

  // Check if the key does not exist (this happens after updating MFBOPC)
  if (lModifiedPaths < 2 && aMap->find(aKey) == aMap->end())
  {
    // Create the new entry
    aMap->insert(std::pair<QString, QString>(aKey, aPath));
  }

  // Save only if the map are different
  if (aMustSaveFile && lMapCopy != *aMap)
  {
    // Save the new list
    Utils::SaveLastPathsToFile(*aMap);
  }

  // Return if any modification has been made
  return lMapCopy != *aMap;
}

QString Utils::GetShortLanguageNameFromEnum(const int aEnumValue)
{
  auto lEnumLang{static_cast<ApplicationLanguage>(aEnumValue)};
  switch (lEnumLang)
  {
    case ApplicationLanguage::ENGLISH:
      return "en";
    case ApplicationLanguage::FRENCH:
      return "fr";
    case ApplicationLanguage::CHINESE_TRADITIONAL:
      return "zh_TW";
    default:
      return "en";
  }
}

ApplicationLanguage Utils::GetStructLanguageFromName(const QString& aShortName)
{
  if (aShortName.compare("English") == 0)
  {
    return ApplicationLanguage::ENGLISH;
  }

  if (aShortName.compare("French") == 0)
  {
    return ApplicationLanguage::FRENCH;
  }

  if (aShortName.compare("Chinese") == 0)
  {
    return ApplicationLanguage::CHINESE_TRADITIONAL;
  }

  // Default language if no supported language has been found
  return ApplicationLanguage::ENGLISH;
}

QAction* Utils::BuildQAction(QWidget* aParent, const QString& aText, const QKeySequence& aKeysCombination, const QString& aIconName, const QString& aIconFolder)
{
  auto lAction{new QAction(aParent)};
  lAction->setText(aText);
  lAction->setShortcut(aKeysCombination);
  lAction->setIcon(QIcon(QPixmap(QString(":/%1/%2").arg(aIconFolder, aIconName))));
  return lAction;
}

void Utils::AddIconToGroupBox(QGroupBox* aGroupBox, const QString& aIconFolder, const QString& aIconName, const int aFontSize)
{
  aGroupBox->setCheckable(true);
  aGroupBox->setStyleSheet(QString("QGroupBox{font-size: %1pt;}"
                                   "QGroupBox::indicator{width: 16px; height: 16px; image: url(:/%2/%3);}")
                             .arg(static_cast<int>(floor(aFontSize * 1.25)))
                             .arg(aIconFolder)
                             .arg(aIconName));
}

void Utils::AddLastPathLine(QWidget* aParent, QGridLayout* aLayout, const int aRow, const QString& aLabel, const QString& aValue, const QString& aIconFolder, const QString& aIconName)
{
  aLayout->addWidget(new QLabel(aLabel, aParent), aRow, 0);

  auto lGeneralValue{new QLineEdit(aValue, aParent)};
  lGeneralValue->setReadOnly(true);
  lGeneralValue->setCursor(Qt::CursorShape::IBeamCursor);
  lGeneralValue->setObjectName(QString("line_edit_path_%1").arg(aRow));
  aLayout->addWidget(lGeneralValue, aRow, 1);

  auto lGeneralEmptyButton{ComponentFactory::CreateButton(aParent, tr("Remove from history"), "", aIconName, aIconFolder, QString("clear_path_%1").arg(aRow), aValue.isEmpty(), true)};
  aLayout->addWidget(lGeneralEmptyButton, aRow, 2);
}

void Utils::SelectComboBoxAt(QComboBox* aComboBox, const int aIndex)
{
  if (aIndex < 0 || aIndex >= aComboBox->count())
  {
    aComboBox->setCurrentIndex(0);
  }
  else
  {
    aComboBox->setCurrentIndex(aIndex);
  }
}

void Utils::SetGroupBoxState(QGroupBox* aGroupBox, const bool aIsCollapsed)
{
  auto lTitle{aGroupBox->title()};

  if (aIsCollapsed)
  {
    aGroupBox->setChecked(false);
    aGroupBox->setMaximumHeight(qApp->fontMetrics().height() * 1.5);
    lTitle.replace(lTitle.length() - 1, 1, QChar(0x23F5));
  }
  else
  {
    aGroupBox->setMaximumHeight(QWIDGETSIZE_MAX);
    lTitle.replace(lTitle.length() - 1, 1, QChar(0x23F7));
  }

  // Update the title with the updated chevron state
  aGroupBox->setTitle(lTitle);
}

void Utils::UpdateComboBoxBodyslideFiltersList(const std::map<QString, QStringList>& aFilterList, QComboBox* aComboBox, QLabel* aFiltersLabel)
{
  // Disable the combobox if there is not any available filter
  if (aFilterList.size() == 0)
  {
    aComboBox->clear();
    aComboBox->setDisabled(true);
    aFiltersLabel->setText("");
    return;
  }

  aComboBox->setDisabled(false);

  auto lPrevKey{aComboBox->itemText(aComboBox->currentIndex())};

  // Fill the combobox
  aComboBox->clear();
  for (const auto& lPair : aFilterList)
  {
    aComboBox->addItem(lPair.first);
  }

  if (lPrevKey == "" || aFilterList.count(lPrevKey) == 0)
  {
    aComboBox->setCurrentIndex(0);
  }
  else
  {
    auto lPrevIndex{0};
    for (const auto& lPair : aFilterList)
    {
      if (lPair.first.compare(lPrevKey, Qt::CaseSensitive) == 0)
      {
        break;
      }
      lPrevIndex++;
    }

    if (lPrevIndex == aFilterList.size())
    {
      lPrevIndex = 0;
    }

    aComboBox->setCurrentIndex(lPrevIndex);
  }
}

void Utils::UpdateOutputPreview(QLineEdit* aMainDirTextEdit, const QString& aSubDirectory, const bool aUseOnlySubdir, const QString& aSuccessColor, const QString& aWarningColor, const QString& aDangerColor, QLabel* aOutputPathsPreview)
{
  auto lMainDirectory{aMainDirTextEdit->text().trimmed()};
  Utils::CleanPathString(lMainDirectory);
  Utils::CleanPathString(aSubDirectory);
  auto lIsValidPath{true};

  // Construct full path
  auto lFullPath{QString()};
  if (aUseOnlySubdir)
  {
    aMainDirTextEdit->setDisabled(true);

    if (!aSubDirectory.isEmpty())
    {
      lFullPath = aSubDirectory;
    }
    else
    {
      lFullPath = tr("No path given or invalid path given.");
      lIsValidPath = false;
    }
  }
  else
  {
    if (!lMainDirectory.isEmpty() && !aSubDirectory.isEmpty())
    {
      lFullPath = lMainDirectory + "/" + aSubDirectory;
      aMainDirTextEdit->setDisabled(false);
    }
    else if (!lMainDirectory.isEmpty() && aSubDirectory.isEmpty())
    {
      lFullPath = lMainDirectory;
      aMainDirTextEdit->setDisabled(false);
    }
    else if (lMainDirectory.isEmpty() && !aSubDirectory.isEmpty())
    {
      lFullPath = tr("You must choose a directory through the file chooser. Current path defined: \" /%1\".").arg(aSubDirectory);
      aMainDirTextEdit->setDisabled(true);
      lIsValidPath = false;
    }
    else
    {
      lFullPath = tr("No path given or invalid path given.");
      aMainDirTextEdit->setDisabled(true);
      lIsValidPath = false;
    }
  }

  // Set the full path value in the preview label
  auto lNewTextColor{aSuccessColor};

  if (lIsValidPath)
  {
    if (QDir(lFullPath).exists() || aUseOnlySubdir)
    {
      lNewTextColor = aWarningColor;
    }
  }
  else
  {
    lNewTextColor = aDangerColor;
  }

  aOutputPathsPreview->setStyleSheet(QString("QLabel{color:%1;}").arg(lNewTextColor));
  aOutputPathsPreview->setText(lFullPath);
}

void Utils::BindConsoleToStdOut()
{
  FreeConsole();
  AllocConsole();
  FILE* lOutFile{NULL};
  freopen_s(&lOutFile, "CONOUT$", "w", stdout);
  SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
}

void Utils::PrintMessageStdOut(const QString& aMessage)
{
  if (aMessage.isEmpty())
  {
    std::cout << std::endl;
  }
  else if (aMessage.endsWith(".") || aMessage.endsWith(":") || aMessage.endsWith("!") || aMessage.endsWith("?") || aMessage.endsWith("\n"))
  {
    std::cout << aMessage.toStdString() << std::endl;
  }
  else
  {
    std::cout << aMessage.toStdString() << "." << std::endl;
  }
}
