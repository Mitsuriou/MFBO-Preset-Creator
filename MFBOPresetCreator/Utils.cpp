﻿#include "Utils.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Enum.h"
#include "LineEdit.h"
#include "SliderFileBuilder.h"
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QVersionNumber>
#include <QtXml/QDomDocument>
#include <iostream>
#include <math.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

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
  lPath.replace(QStringLiteral("\\"), QStringLiteral("/"));

  // Remove any '\n' character
  const QString lCleanedPath(lPath);
  lPath = Utils::CleanBreaksString(lCleanedPath);

  return lPath;
}

void Utils::CleanBreaksString(QString& aString)
{
  aString.replace(QStringLiteral("\n"), "");
}

QString Utils::CleanBreaksString(const QString& aString)
{
  QString lPath(aString);
  lPath.replace(QStringLiteral("\n"), "");
  return lPath;
}

std::map<QString, std::vector<QString>> Utils::ToMapQsVecQs(const std::map<QString, std::set<QString>>& aMap)
{
  std::map<QString, std::vector<QString>> lNewMap;

  for (const auto& lMapEntry : aMap)
  {
    lNewMap.insert(std::make_pair(lMapEntry.first, std::vector<QString>(lMapEntry.second.begin(), lMapEntry.second.end())));
  }

  return lNewMap;
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

void Utils::DisplayInfoMessage(QWidget* aParent, const QString& aTitle, const QString& aMessage, const QString& aIconFolder, const QString& aIconName, const QString& aButtonText)
{
  QMessageBox lInformationBox(QMessageBox::Icon::Information, aTitle, aMessage, QMessageBox::StandardButton::NoButton, aParent);
  lInformationBox.setIconPixmap(QPixmap(QString(":/%1/%2").arg(aIconFolder, aIconName)).scaledToHeight(17 * 2)); // TODO: Multiply the size by the DPI scale

  auto lOKButton{lInformationBox.addButton(aButtonText, QMessageBox::ButtonRole::AcceptRole)};
  lOKButton->setCursor(Qt::PointingHandCursor);
  lInformationBox.setDefaultButton(lOKButton);

  lInformationBox.exec();
}

void Utils::DisplayWarningMessage(const QString& aMessage)
{
  QMessageBox lWarningBox(QMessageBox::Icon::Warning, tr("Warning"), aMessage);
  lWarningBox.setIconPixmap(QPixmap(":/icons/orange-warning").scaledToHeight(17 * 2)); // TODO: Multiply the size by the DPI scale

  auto lOKButton{lWarningBox.addButton(tr("OK"), QMessageBox::ButtonRole::AcceptRole)};
  lOKButton->setCursor(Qt::PointingHandCursor);
  lWarningBox.setDefaultButton(lOKButton);

  lWarningBox.exec();
}

ButtonClicked Utils::DisplayQuestionMessage(QWidget* aParent, const QString& aTitle, const QString& aMessage, const QString& aIconFolder, const QString& aIconName, const QString& aTextBtnYes, const QString& aTextBtnNo, const QString& aTextBtnOther, const QString& aColorYesBtn, const QString& aColorNoBtn, const QString& aColorOtherBtn, const bool aIsYesBtnDefault)
{
  QMessageBox lConfirmationBox(QMessageBox::Icon::Question, aTitle, aMessage, QMessageBox::StandardButton::NoButton, aParent);
  lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/%2").arg(aIconFolder, aIconName)).scaledToHeight(17 * 2)); // TODO: Multiply the size by the DPI scale

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

void Utils::DisplayErrorMessage(const QString& aMessage)
{
  QMessageBox lWarningBox(QMessageBox::Icon::Critical, tr("Error"), aMessage);
  lWarningBox.setIconPixmap(QPixmap(":/icons/red-dangerous").scaledToHeight(17 * 2)); // TODO: Multiply the size by the DPI scale

  auto lOKButton{lWarningBox.addButton(tr("OK"), QMessageBox::ButtonRole::AcceptRole)};
  lOKButton->setCursor(Qt::PointingHandCursor);
  lWarningBox.setDefaultButton(lOKButton);

  lWarningBox.exec();
}

VersionsInformation Utils::ParseGitHubReleasesRequestResult(const QString& aResult)
{
  VersionsInformation lVersionsInformation;
  QString lTagName;

  // Create a JSON from the fetched string and parse the "tag_name" data
  QJsonDocument lJsonDocument{QJsonDocument::fromJson(aResult.toUtf8())};
  QJsonArray lTagsArray{lJsonDocument.array()};

  // If no data is found, return the empty set
  if (lTagsArray.size() == 0)
    return lVersionsInformation;

  // In case the data is one level deeper
  if (lTagsArray.size() == 1)
  {
    lTagsArray = lTagsArray.at(0).toArray();

    // If there is still no data, return the empty set
    if (lTagsArray.size() == 0)
      return lVersionsInformation;
  }

  // Iterate in the versions array
  for (int i = 0; i < lTagsArray.size(); i++)
  {
    if (lTagsArray.at(i)["tag_name"].isNull() || !lTagsArray.at(i)["tag_name"].isString())
      continue;

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

  QDirIterator it(aRootDir, QStringList(aFileExtension), QDir::Files, QDirIterator::NoIteratorFlags);
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

  QDirIterator it(aRootDir, QStringList(aFileExtension), QDir::Files, QDirIterator::Subdirectories);
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
  QString lAbsFilePath;
  QString lRelativeDirs;

  QDirIterator it(aRootDir, QStringList(aFileExtension), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();

    // Ignore FOMOD directory
    lAbsFilePath = it.fileInfo().absoluteFilePath();
    lRelativeDirs = lAbsFilePath.remove(aRootDir, Qt::CaseSensitivity::CaseInsensitive);

    if (aFileExtension.compare(QStringLiteral("*.xml"), Qt::CaseSensitivity::CaseInsensitive) == 0
        && (lRelativeDirs.contains("/fomod/info.xml", Qt::CaseSensitivity::CaseInsensitive) || lRelativeDirs.contains("/fomod/ModuleConfig.xml", Qt::CaseSensitivity::CaseInsensitive)))
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

void Utils::ApplyApplicationStyleSheet(const QString& aQSSFileName)
{
  qApp->setStyleSheet("");

  if (!aQSSFileName.isEmpty())
  {
    QFile lQSSFile(":qss/" + aQSSFileName + ".qss");
    if (lQSSFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QTextStream lStream(&lQSSFile);
      qApp->setStyleSheet(lStream.readAll());
      lQSSFile.close();
    }
  }
}

bool Utils::IsThemeDark(const GUITheme aTheme)
{
  switch (aTheme)
  {
    case GUITheme::MITSURIOU_BLACK_THEME:
    case GUITheme::MITSURIOU_DARK_THEME:
    case GUITheme::ALEXHUSZAGH_BREEZE_DARK:
    case GUITheme::PAPER_DARK:
    case GUITheme::PAPER_BLACK_MONO:
      return true;
    case GUITheme::DEFAULT_OS_THEME:
    case GUITheme::MITSURIOU_LIGHT_THEME:
    case GUITheme::ALEXHUSZAGH_BREEZE_LIGHT:
    case GUITheme::PAPER_LIGHT:
    case GUITheme::PAPER_WHITE_MONO:
      return false;
  }

  return false;
}

QString Utils::GetIconResourceFolder(const GUITheme aTheme)
{
  return (Utils::IsThemeDark(aTheme) ? QStringLiteral("white") : QStringLiteral("black"));
}

QString Utils::GetTitleBarResourceFolder(const bool aUseBlackIcons)
{
  return (aUseBlackIcons ? QStringLiteral("black") : QStringLiteral("white"));
}

void Utils::OverrideHTMLLinksColor(QString& aHTMLString, const GUITheme aTheme)
{
  // If no color change is needed
  if (aTheme != GUITheme::MITSURIOU_BLACK_THEME
      && aTheme != GUITheme::MITSURIOU_DARK_THEME
      && aTheme != GUITheme::MITSURIOU_LIGHT_THEME)
  {
    return;
  }

  // Hacky links' colors override for some themes
  const auto lLinksColorOverride{aTheme == GUITheme::MITSURIOU_BLACK_THEME ? QStringLiteral("color:#3991ff") : QStringLiteral("color:#e95985")};

  // Go through the string to find the link colors
  auto i{0};
  while ((i = aHTMLString.indexOf(QStringLiteral("color:#0000ff"), i)) != -1)
  {
    aHTMLString.replace(i, lLinksColorOverride.size(), lLinksColorOverride);
  }
}

bool Utils::CreateXMLFile(const QString& aEntryDirectory, const bool aGenerateFilesInExistingMainDirectory, const QString& aOSPXMLNames, const bool aMustUseBeastHands, const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion, const QString& aBodyslideSlidersetsNames, const std::vector<Struct::Filter>& aBodySlideFilters, const bool aIsBatchConversionPreset, const unsigned char& aTargetBlocks)
{
  // Create the SliderGroups directory
  auto lSliderGroupsDirectory{aEntryDirectory + QDir::separator() + "CalienteTools" + QDir::separator() + "BodySlide" + QDir::separator() + "SliderGroups"};

  if (!QDir(lSliderGroupsDirectory).exists())
  {
    QDir().mkpath(lSliderGroupsDirectory);
  }
  else if (!aGenerateFilesInExistingMainDirectory && !aIsBatchConversionPreset)
  {
    Utils::DisplayWarningMessage(tr("Error while creating the meshes directory: \"%1\" already exists.").arg(lSliderGroupsDirectory));
    return false;
  }

  // Construct the file content
  auto lXMLFileContent{SliderFileBuilder::BuildXMLFileContent(aBodyslideSlidersetsNames, aBodySlideFilters, aBodyNameVersion, aFeetNameVersion, aMustUseBeastHands, aTargetBlocks)};

  // Create the OSP file on disk
  auto lXMLPathName(lSliderGroupsDirectory + QDir::separator() + aOSPXMLNames + ".xml");

  QFile lXMLFile(lXMLPathName);
  if (lXMLFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
  {
    QTextStream lTextStream(&lXMLFile);
    lTextStream << lXMLFileContent;
    lTextStream.flush();

    lXMLFile.close();
  }
  else
  {
    Utils::DisplayWarningMessage(tr("Error while trying to create the XML file \"%1\". Aborting process.").arg(lXMLPathName));
    return false;
  }

  return true;
}

bool Utils::CreateOSPFile(const QString& aEntryDirectory,
                          const bool aGenerateFilesInExistingMainDirectory,
                          const QString& aOSPXMLNames,
                          const bool aMustUseBeastHands,
                          const BodyNameVersion& aBodyNameVersion,
                          const FeetNameVersion& aFeetNameVersion,
                          const QString& aBodyslideSlidersetsNames,
                          QString aMeshesPathBody,
                          QString aMeshesPathFeet,
                          QString aMeshesPathHands,
                          const QString& aBodyName,
                          const QString& aFeetName,
                          const QString& aHandsName,
                          const bool aIsBatchConversionPreset,
                          const unsigned char& aTargetBlocks)
{
  // Create the SliderSets directory
  auto lSliderSetsDirectory{aEntryDirectory + QDir::separator() + "CalienteTools" + QDir::separator() + "BodySlide" + QDir::separator() + "SliderSets"};

  if (!QDir(lSliderSetsDirectory).exists())
  {
    QDir().mkpath(lSliderSetsDirectory);
  }
  else if (!aGenerateFilesInExistingMainDirectory && !aIsBatchConversionPreset)
  {
    Utils::DisplayWarningMessage(tr("Error while creating the meshes directory: \"%1\" already exists.").arg(lSliderSetsDirectory));
    return false;
  }

  // Construct the file content
  auto lOSPFileContent{SliderFileBuilder::BuildOSPFileContent(aBodyslideSlidersetsNames, aBodyNameVersion, aFeetNameVersion, aMustUseBeastHands, aTargetBlocks)};

  // Fill the custom variables
  lOSPFileContent.replace(QString("{%%body_output_path%%}"), aMeshesPathBody.replace("/", "\\"));
  lOSPFileContent.replace(QString("{%%feet_output_path%%}"), aMeshesPathFeet.replace("/", "\\"));
  lOSPFileContent.replace(QString("{%%hands_output_path%%}"), aMeshesPathHands.replace("/", "\\"));
  lOSPFileContent.replace(QString("{%%body_output_file%%}"), !aBodyName.isEmpty() ? aBodyName : "femalebody");
  lOSPFileContent.replace(QString("{%%feet_output_file%%}"), !aFeetName.isEmpty() ? aFeetName : "femalefeet");
  lOSPFileContent.replace(QString("{%%hands_output_file%%}"), !aHandsName.isEmpty() ? aHandsName : "femalehands");

  // Create the OSP file on disk
  auto lOSPPathName(lSliderSetsDirectory + QDir::separator() + aOSPXMLNames + ".osp");

  QFile lOSPFile(lOSPPathName);
  if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
  {
    QTextStream lTextStream(&lOSPFile);
    lTextStream << lOSPFileContent;
    lTextStream.flush();

    lOSPFile.close();
  }
  else
  {
    Utils::DisplayWarningMessage(tr("Error while trying to create the OSP file \"%1\". Aborting process.").arg(lOSPPathName));
    return false;
  }

  return true;
}

void Utils::CreateTextureFile(const QString& aSourcePath, const QString& aDestinationEntryDirectory, const QString& aDestinationRelativePath, const QString& aDestinationFileName)
{
  // Create the directory in which the texture file will be created
  auto lDestinationDirectory{aDestinationEntryDirectory + QDir::separator() + aDestinationRelativePath};
  QDir().mkpath(lDestinationDirectory);

  // Full destination path
  auto lDestinationFullPath{QString("%1%2%3.dds").arg(lDestinationDirectory, QDir::separator(), aDestinationFileName)};

  // If the preset is being generated on an already existing directory, remove the file if it already exists
  if (QFile::exists(lDestinationFullPath))
  {
    QFile lFile(lDestinationFullPath);
    lFile.setPermissions(QFile::ReadUser | QFile::WriteUser);
    lFile.remove();
  }

  // Source file path
  auto lSourceFilPath{QString("%1%2%3.dds").arg(aSourcePath, QDir::separator(), aDestinationFileName)};

  // Try to copy the texture file
  if (!QFile::copy(lSourceFilPath, lDestinationFullPath))
  {
    Utils::DisplayWarningMessage(tr("The texture file \"%1\" was not found or could not be copied.").arg(aDestinationFileName));
  }
}

bool Utils::CreateSkeletonFile(const QString& aSourcePath, const QString& aDestinationEntryDirectory, const QString& aDestinationRelativePath, const QString& aDestinationFileName)
{
  if (!aDestinationRelativePath.isEmpty())
  {
    // Create the directory in which the skeleton file will be created
    auto lDestinationSkeletonDirectory{aDestinationEntryDirectory + QDir::separator() + aDestinationRelativePath};
    QDir().mkpath(lDestinationSkeletonDirectory);

    // Full destination path
    auto lDestinationPath{QString("%1%2%3.nif").arg(lDestinationSkeletonDirectory, QDir::separator(), aDestinationFileName)};

    // If the preset is being generated on an already existing directory, remove the skeleton file if it already exists
    if (QFile::exists(lDestinationPath))
    {
      QFile lFile(lDestinationPath);
      lFile.setPermissions(QFile::ReadUser | QFile::WriteUser);
      lFile.remove();
    }

    // Try to copy the skeleton file
    if (!QFile::copy(aSourcePath, lDestinationPath))
    {
      Utils::DisplayWarningMessage(tr("The custom skeleton file was not found or could not be copied. The application will take with the default XPMSSE (v4.72) skeleton instead..."));

      // Fallback option if the custom skeleton could not be copied
      if (!QFile::copy(":/resources/skeleton_female", lDestinationPath))
      {
        Utils::DisplayWarningMessage(tr("The skeleton file could not be created even using the default skeleton.\nBe sure to not generate the preset in a OneDrive/DropBox space and that you executed the application with sufficient permissions.\nBe sure that you used characters authorized by your OS in the given paths."));
        return false;
      }
    }
  }
  else
  {
    Utils::DisplayWarningMessage(tr("Error: no path given for the custom skeleton."));
    return false;
  }

  return true;
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
  if (aFileName.mid(aFileName.lastIndexOf('/') + 1).contains("skeleton", Qt::CaseSensitivity::CaseInsensitive))
    return BCGroupWidgetCallContext::SKELETON;

  return BCGroupWidgetCallContext::UNDEFINED;
}

void Utils::ClearUselessEntries(std::map<QString, std::set<QString>>& aScannedData)
{
  for (auto lIt = aScannedData.begin(); lIt != aScannedData.end();)
  {
    // If no useful data has been found, delete the current entry from the map
    if (!Utils::ContainsBodyOrHandsOrFeetMesh<std::set<QString>>(lIt->second))
    {
      lIt = aScannedData.erase(lIt);
      continue;
    }

    ++lIt;
  }
}

bool Utils::IsCBBEBasedBody(const BodyName& aBodyName)
{
  switch (aBodyName)
  {
    case BodyName::_INVALID_VALUE:
      return false;
    // CBBE-based bodies
    case BodyName::CBBE_3BA_3BBB:
    case BodyName::CBBE_SMP_3BBB:
    case BodyName::COCO_BODY_CBBE:
    case BodyName::MIMIR_EBONIC_BODY:
      return true;
      // UNP-based bodies
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    case BodyName::COCO_BODY_UUNP:
    case BodyName::BHUNP_LITE_ASDASDF:
      return false;
  }

  return false;
}

bool Utils::IsCBBEBasedBody(const BodyVariant& aBodyVariant)
{
  return Utils::IsCBBEBasedBody(DataLists::GetName(aBodyVariant));
}

bool Utils::IsCBBEBasedBody(const BodyNameVersion& aBodyNameVersion)
{
  return Utils::IsCBBEBasedBody(DataLists::GetName(DataLists::GetVariant(aBodyNameVersion)));
}

bool Utils::IsBodySupportingBeastHands(const BodyNameVersion& aBodyNameVersion)
{
  return Utils::IsCBBEBasedBody(aBodyNameVersion);
}

bool Utils::IsVersionOffsetValid(const BodyVariant& aBodyVariant, const int aRelativeVersion)
{
  // If "2.20" or "2.31"
  if (aBodyVariant == BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE && (aRelativeVersion == 0 || aRelativeVersion == 3))
  {
    return false;
  }

  // If not "3.00 to 3.02", "3.03", "3.04 - 3.05", "3.10" and "3.11 to 3.13" but trying to use ver3 key, this is invalid
  if (aBodyVariant == BodyVariant::BHUNP_3BBB_ADVANCED_VER_3
      && !(aRelativeVersion >= 5 && aRelativeVersion <= 9) // "3.00 to 3.02", "3.03", "3.04 - 3.05", "3.10" and "3.11 to 3.13"
  )
  {
    return false;
  }

  return (aRelativeVersion >= DataLists::GetVersionOffset(aBodyVariant, aRelativeVersion));
}

bool Utils::IsVersionOffsetValid(const FeetVariant& aFeetVariant, const int aRelativeVersion)
{
  return (aRelativeVersion >= DataLists::GetVersionOffset(aFeetVariant));
}

bool Utils::QRCResourceExists(const QString& aResourcePath)
{
  return QFile(aResourcePath).exists();
}

bool Utils::IsRunningStandaloneVersion()
{
  QDir lDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
  return !lDir.exists();
}

QString Utils::GetAppDataFolderPath()
{
#if defined(DEBUG) || !defined(QT_NO_DEBUG)
  return QCoreApplication::applicationDirPath() + QDir::separator();
#else
  QDir lDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
  if (lDir.exists())
  {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator();
  }

  return QCoreApplication::applicationDirPath() + QDir::separator();
#endif
}

QString Utils::GetInstallerLogFilePath()
{
  return Utils::GetAppDataFolderPath().append("installer.log");
}

QString Utils::GetConfigFilePath()
{
  return Utils::SettingsFullFilePath();
}

QString Utils::GetFiltersFilePath()
{
  return Utils::GetAppDataFolderPath().append("filters.json");
}

QString Utils::GetLastPathsFilePath()
{
  return Utils::GetAppDataFolderPath().append("paths.json");
}

QString Utils::GetPrivateKeyFilePath()
{
  return Utils::GetAppDataFolderPath().append("private_key");
}

QString Utils::GetDatabaseFilePath()
{
  return Utils::GetAppDataFolderPath().append("slider_sets_db");
}

QString Utils::GetAssetsFolderPath()
{
  return Utils::GetAppDataFolderPath().append("assets/");
}

QString Utils::GetSkeletonsFolderPath()
{
  return Utils::GetAssetsFolderPath().append("skeletons/");
}

QString Utils::GetTexturesFolderPath()
{
  return Utils::GetAssetsFolderPath().append("textures/");
}

QString Utils::GetSliderSetsFolderPath()
{
  return Utils::GetAssetsFolderPath().append("slidersets/");
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
  QString lPresetName;

  if (lXMLGroup.tagName() == "Group")
  {
    // Get the first "Member" tag to read its "name" attribute
    auto lXMLMember{lXMLGroup.firstChild().toElement()};
    lPresetName = lXMLMember.attribute(QStringLiteral("name"), QStringLiteral(""));
  }

  return lPresetName.left(lPresetName.lastIndexOf('-') - 1);
}

std::vector<Struct::SliderSet> Utils::ReadOSPFileInformation(const QString& aPath, const bool aSkipTypeAnalysis)
{
  std::vector<Struct::SliderSet> lSliderSets;

  QFile lReadFile(aPath);
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

  const auto lRoot{lOSPDocument.documentElement()};
  auto lNextNodeToParse{lRoot.firstChild().toElement()};

  while (!lNextNodeToParse.isNull())
  {
    if (lNextNodeToParse.tagName().compare(QStringLiteral("SliderSet"), Qt::CaseSensitivity::CaseInsensitive) == 0)
    {
      Struct::SliderSet lSliderSetEntry;
      lSliderSetEntry.setName(lNextNodeToParse.attribute(QStringLiteral("name"), QStringLiteral(""))); // Name

      if (lSliderSetEntry.getName().isEmpty())
      {
        lNextNodeToParse = lNextNodeToParse.nextSiblingElement();
        continue;
      }

      const auto lSplittedName{lSliderSetEntry.getName().split('-')};
      if (lSplittedName.isEmpty())
      {
        lNextNodeToParse = lNextNodeToParse.nextSiblingElement();
        continue;
      }

      const QString& lNamePartToAnalyze{lSplittedName.size() == 1 ? lSplittedName.first()
                                                                  : (lSplittedName.size() == 2 ? lSplittedName.at(1)
                                                                                               : lSplittedName.last())};

      // Skip the
      if (aSkipTypeAnalysis)
      {
        lSliderSetEntry.setMeshPart(MeshPartType::UNKNOWN);
      }
      // Try to guess the part type from the name
      // TODO: It should be possible to make this analyze way more precise, by using the whole content of the lNextNodeToParse,
      //       and by giving weights from each parsed data.
      else if (lNamePartToAnalyze.contains("feet", Qt::CaseSensitivity::CaseInsensitive)
               && !Utils::AlreadyContainsMeshPartType(lSliderSets, MeshPartType::FEET))
      {
        lSliderSetEntry.setMeshPart(MeshPartType::FEET);
      }
      else if ((lNamePartToAnalyze.contains("hand", Qt::CaseSensitivity::CaseInsensitive)
                && lNamePartToAnalyze.contains("beast", Qt::CaseSensitivity::CaseInsensitive))
               && (!Utils::AlreadyContainsMeshPartType(lSliderSets, MeshPartType::HANDS)
                   || !Utils::AlreadyContainsMeshPartType(lSliderSets, MeshPartType::BEAST_HANDS)))
      {
        lSliderSetEntry.setMeshPart(MeshPartType::BEAST_HANDS);
      }
      else if (lNamePartToAnalyze.contains("hand", Qt::CaseSensitivity::CaseInsensitive)
               && (!Utils::AlreadyContainsMeshPartType(lSliderSets, MeshPartType::HANDS)
                   || !Utils::AlreadyContainsMeshPartType(lSliderSets, MeshPartType::BEAST_HANDS)))
      {
        lSliderSetEntry.setMeshPart(MeshPartType::HANDS);
      }
      else if (!Utils::AlreadyContainsMeshPartType(lSliderSets, MeshPartType::BODY))
      {
        lSliderSetEntry.setMeshPart(MeshPartType::BODY);
      }
      else
      {
        lNextNodeToParse = lNextNodeToParse.nextSiblingElement();
        continue;
      }

      auto lNextChildNodeToParse{lNextNodeToParse.firstChild().toElement()};
      while (!lNextChildNodeToParse.isNull())
      {
        if (lNextChildNodeToParse.tagName() == "OutputPath")
        {
          lSliderSetEntry.setOutputPath(lNextChildNodeToParse.firstChild().toText().data()); // OutputPath
        }
        else if (lNextChildNodeToParse.tagName() == "OutputFile")
        {
          lSliderSetEntry.setOutputFile(lNextChildNodeToParse.firstChild().toText().data()); // OutputFile
        }

        if (!lSliderSetEntry.getOutputPath().isEmpty() && !lSliderSetEntry.getOutputFile().isEmpty())
        {
          break;
        }

        lNextChildNodeToParse = lNextChildNodeToParse.nextSiblingElement();
      }

      if (lSliderSetEntry.isValid() || (aSkipTypeAnalysis && lSliderSetEntry.isValidNoMeshPartCheck()))
        lSliderSets.push_back(lSliderSetEntry);
    }

    lNextNodeToParse = lNextNodeToParse.nextSiblingElement();
  }

  return lSliderSets;
}

bool Utils::AlreadyContainsMeshPartType(const std::vector<Struct::SliderSet>& aSliderSetsList, const MeshPartType& aMeshPartType)
{
  for (const auto& lSliderSet : aSliderSetsList)
  {
    if (lSliderSet.getMeshPart() == aMeshPartType)
    {
      return true;
    }
  }

  return false;
}

std::pair<bool, Struct::SliderSet> Utils::GetSliderSetByMeshPartType(const std::vector<Struct::SliderSet>& aSliderSetsList, const MeshPartType& aMeshPartType)
{
  for (const auto& lSliderSet : aSliderSetsList)
  {
    if (lSliderSet.getMeshPart() == aMeshPartType)
    {
      return std::make_pair(true, lSliderSet);
    }
  }

  return std::make_pair(false, Struct::SliderSet());
}

bool Utils::IsPresetUsingBeastHands(const QString& aPath)
{
  QFile lReadFile(aPath);

  QString lFileContent;

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

  if (lFileContent.contains("beast hands", Qt::CaseSensitivity::CaseInsensitive) || lFileContent.contains("hands beast", Qt::CaseSensitivity::CaseInsensitive))
  {
    return true; // Preset uses beast hands
  }

  return false; // Preset doesn't use beast hands
}

QString Utils::GetBodySliderValue(const BodyNameVersion& aBodyNameVersion)
{
  return QString("%1 - ").append(DataLists::GetBodySliderValue(aBodyNameVersion)).append(QString("\n"));
}

QString Utils::GetFeetSliderValue(const FeetNameVersion& aFeetNameVersion)
{
  return QString("%1 - ").append(DataLists::GetFeetSliderValue(aFeetNameVersion)).append(QString("\n"));
}

QString Utils::GetHandsSliderValue(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands)
{
  return QString("%1 - ").append(DataLists::GetHandsSliderValue(aBodyNameVersion, aMustUseBeastHands));
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
      Utils::DisplayInfoMessage(aParent, tr("Project successfully saved"), tr("The project file has successfully been saved to \"%1\".").arg(aFilePath), aIconFolder, "done", tr("OK"));
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
  if (lFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    // Convert the text content into a QJsonDocument
    const auto lJsonDocument(QJsonDocument::fromJson(lFile.readAll()));

    lFile.close();

    // Return the QJsonObject containing the read data
    return lJsonDocument.object();
  }

  lFile.close();
  return QJsonObject();
}

QString Utils::SettingsFullFilePath()
{
  return Utils::GetAppDataFolderPath().append("config.json");
}

bool Utils::SettingsFileExists()
{
  return QFile(Utils::SettingsFullFilePath()).exists();
}

Struct::Settings Utils::LoadSettingsFromFile()
{
  // If the settings file does not exist, return a defaulted settings struct
  if (!Utils::SettingsFileExists())
  {
    return Struct::Settings();
  }

  const QJsonObject lSettingsJSON{Utils::LoadFromJsonFile(Utils::SettingsFullFilePath())};
  Struct::Settings lSettings;

  // Compatiblity mod for settings created from an older version than v.3.4.0.0
  if (!lSettingsJSON.contains("applicationVersion")
      || (lSettingsJSON.contains("applicationVersion")
          && Utils::CompareVersionNumbers(lSettingsJSON["applicationVersion"].toString(), "3.4.0.0") == ApplicationVersionRelative::OLDER))
  {
    Utils::ParseSettingsCompatibility(lSettings, lSettingsJSON);
    Utils::SaveSettingsToFile(lSettings); // Save the settings in the new file format
    Utils::PrintMessageStdOut("User settings upgraded!");
  }
  // New file format since v.3.4.0.0
  else
  {
    Utils::ParseSettings(lSettings, lSettingsJSON, lSettingsJSON["applicationVersion"].toString());
  }

  Utils::PrintMessageStdOut("User settings:");
  Utils::PrintMessageStdOut(QJsonDocument(Utils::SettingsStructToJson(lSettings)).toJson(QJsonDocument::JsonFormat::Indented));

  return lSettings;
}

void Utils::ParseSettingsCompatibility(Struct::Settings& aSettings, const QJsonObject& aJSONObject)
{
  // App theme
  if (aJSONObject.contains("appTheme") && aJSONObject["appTheme"].isDouble())
    aSettings.display.applicationTheme = static_cast<GUITheme>(aJSONObject["appTheme"].toInt());

  // Opening mode: assisted conversion
  if (aJSONObject.contains("assistedConversionDialogOpeningMode") && aJSONObject["assistedConversionDialogOpeningMode"].isDouble())
    aSettings.display.assistedConversionDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["assistedConversionDialogOpeningMode"].toInt());

  // Opening mode: batch conversion
  if (aJSONObject.contains("batchConversionDialogOpeningMode") && aJSONObject["batchConversionDialogOpeningMode"].isDouble())
    aSettings.display.batchConversionDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["batchConversionDialogOpeningMode"].toInt());

  // Opening mode: batch conversion picker
  if (aJSONObject.contains("batchConversionPickerDialogOpeningMode") && aJSONObject["batchConversionPickerDialogOpeningMode"].isDouble())
    aSettings.display.batchConversionPickerDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["batchConversionPickerDialogOpeningMode"].toInt());

  // Opening mode: bodySlide presets' retargeting
  if (aJSONObject.contains("bodySlidePresetsRetargetingDialogOpeningMode") && aJSONObject["bodySlidePresetsRetargetingDialogOpeningMode"].isDouble())
    aSettings.display.bodySlidePresetsRetargetingDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["bodySlidePresetsRetargetingDialogOpeningMode"].toInt());

  // Danger color
  if (aJSONObject.contains("dangerColor") && aJSONObject["dangerColor"].isString())
    aSettings.display.dangerColor = aJSONObject["dangerColor"].toString();

  // Preset Creator: Default body name and version
  if (aJSONObject.contains("defaultMainWindowBody") && aJSONObject["defaultMainWindowBody"].isDouble())
    aSettings.presetCreator.defaultBodyFeet.bodyMesh = static_cast<BodyNameVersion>(aJSONObject["defaultMainWindowBody"].toInt());

  // Preset Creator: Default feet mod
  if (aJSONObject.contains("defaultMainFeetMod") && aJSONObject["defaultMainFeetMod"].isDouble())
    aSettings.presetCreator.defaultBodyFeet.feetMesh = static_cast<FeetNameVersion>(aJSONObject["defaultMainFeetMod"].toInt());

  // Default BodySlide Presets' Retargeting body name and version
  if (aJSONObject.contains("defaultRetargetingToolBody") && aJSONObject["defaultRetargetingToolBody"].isDouble())
    aSettings.presetsRetargeting.defaultBodyFeet.bodyMesh = static_cast<BodyNameVersion>(aJSONObject["defaultRetargetingToolBody"].toInt());

  // Retargeting Tool: Default feet mod
  if (aJSONObject.contains("defaultRetargetingToolFeetMod") && aJSONObject["defaultRetargetingToolFeetMod"].isDouble())
    aSettings.presetsRetargeting.defaultBodyFeet.feetMesh = static_cast<FeetNameVersion>(aJSONObject["defaultRetargetingToolFeetMod"].toInt());

  // Each button stores the last opened path
  if (aJSONObject.contains("eachButtonSavesItsLastUsedPath") && aJSONObject["eachButtonSavesItsLastUsedPath"].isBool())
    aSettings.general.eachButtonSavesItsLastUsedPath = aJSONObject["eachButtonSavesItsLastUsedPath"].toBool();

  // Font family
  if (aJSONObject.contains("font") && aJSONObject["font"].isObject())
    Utils::ParseFontSettings(aSettings.display.font, aJSONObject["font"].toObject());

  // Language
  if (aJSONObject.contains("language") && aJSONObject["language"].isDouble())
    aSettings.display.language = static_cast<ApplicationLanguage>(aJSONObject["language"].toInt());

  // Main window: automatically open generated dir
  if (aJSONObject.contains("mainWindowAutomaticallyOpenGeneratedDirectory") && aJSONObject["mainWindowAutomaticallyOpenGeneratedDirectory"].isBool())
    aSettings.presetCreator.automaticallyOpenFinalDirectory = aJSONObject["mainWindowAutomaticallyOpenGeneratedDirectory"].toBool();

  // Default window height
  if (aJSONObject.contains("mainWindowHeight") && aJSONObject["mainWindowHeight"].isDouble())
    aSettings.display.mainWindowHeight = aJSONObject["mainWindowHeight"].toInt();

  // Opening mode: main window
  if (aJSONObject.contains("mainWindowOpeningMode") && aJSONObject["mainWindowOpeningMode"].isDouble())
    aSettings.display.mainWindowOpeningMode = static_cast<WindowOpeningMode>(aJSONObject["mainWindowOpeningMode"].toInt());

  // Default window width
  if (aJSONObject.contains("mainWindowWidth") && aJSONObject["mainWindowWidth"].isDouble())
    aSettings.display.mainWindowWidth = aJSONObject["mainWindowWidth"].toInt();

  // Retargeting tool: automatically open retargeted dir
  if (aJSONObject.contains("retargetingToolAutomaticallyOpenGeneratedDirectory") && aJSONObject["retargetingToolAutomaticallyOpenGeneratedDirectory"].isBool())
    aSettings.presetsRetargeting.automaticallyOpenFinalDirectory = aJSONObject["retargetingToolAutomaticallyOpenGeneratedDirectory"].toBool();

  // Show welcome screen at application startup
  if (aJSONObject.contains("startupAction") && aJSONObject["startupAction"].isDouble())
    aSettings.general.startupAction = static_cast<StartupAction>(aJSONObject["startupAction"].toInt());

  // Success color
  if (aJSONObject.contains("successColor") && aJSONObject["successColor"].isString())
    aSettings.display.successColor = aJSONObject["successColor"].toString();

  // Opening mode: textures assistant
  if (aJSONObject.contains("texturesAssistantDialogOpeningMode") && aJSONObject["texturesAssistantDialogOpeningMode"].isDouble())
    aSettings.display.texturesAssistantDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["texturesAssistantDialogOpeningMode"].toInt());

  // Warning color
  if (aJSONObject.contains("warningColor") && aJSONObject["warningColor"].isString())
    aSettings.display.warningColor = aJSONObject["warningColor"].toString();

  // JSON format upgrade from v2.10.2.0 to 3.0.0.0
  if (!aJSONObject.contains("applicationVersion"))
  {
    // "windowWidth" -> "mainWindowWidth"
    if (aJSONObject.contains("windowWidth") && aJSONObject["windowWidth"].isDouble())
      aSettings.display.mainWindowWidth = aJSONObject["windowWidth"].toInt();

    // "windowHeight" -> "mainWindowHeight"
    if (aJSONObject.contains("windowHeight") && aJSONObject["windowHeight"].isDouble())
      aSettings.display.mainWindowHeight = aJSONObject["windowHeight"].toInt();

    // "retargetingToolDefaultBody" -> "defaultRetargetingToolBody"
    if (aJSONObject.contains("retargetingToolDefaultBody") && aJSONObject["retargetingToolDefaultBody"].isDouble())
      aSettings.presetsRetargeting.defaultBodyFeet.bodyMesh = static_cast<BodyNameVersion>(aJSONObject["retargetingToolDefaultBody"].toInt());

    // "defaultBody" -> "defaultMainWindowBody"
    if (aJSONObject.contains("defaultBody") && aJSONObject["defaultBody"].isDouble())
      aSettings.presetCreator.defaultBodyFeet.bodyMesh = static_cast<BodyNameVersion>(aJSONObject["defaultBody"].toInt());
  }
  // For any JSON format upgrade / settings values changed
  else if (Utils::CompareVersionNumbers(aJSONObject["applicationVersion"].toString(), "3.2.0.0") == ApplicationVersionRelative::OLDER)
  {
    aSettings.presetCreator.defaultBodyFeet.bodyMesh = BodyNameVersion::CBBE_3BA_3BBB_1_50;
    aSettings.presetsRetargeting.defaultBodyFeet.bodyMesh = BodyNameVersion::CBBE_3BA_3BBB_1_50;
    aSettings.batchConversion.defaultBodyFeet.bodyMesh = BodyNameVersion::CBBE_3BA_3BBB_1_50;
    aSettings.display.applicationTheme = GUITheme::MITSURIOU_DARK_THEME;
  }
}

void Utils::ParseSettings(Struct::Settings& aSettings, const QJsonObject& aJSONObject, const QString& aSettingsFileVersion)
{
  // batchConversion
  if (aJSONObject.contains("batchConversion") && aJSONObject["batchConversion"].isObject())
  {
    Utils::ParseGenericDialogSettings(aSettings.batchConversion, aJSONObject["batchConversion"].toObject(), aSettingsFileVersion);
  }

  // display
  if (aJSONObject.contains("display") && aJSONObject["display"].isObject())
  {
    Utils::ParseDisplaySettings(aSettings.display, aJSONObject["display"].toObject());
  }

  // general
  if (aJSONObject.contains("general") && aJSONObject["general"].isObject())
  {
    Utils::ParseGeneralSettings(aSettings.general, aJSONObject["general"].toObject());
  }

  // presetCreator
  if (aJSONObject.contains("presetCreator") && aJSONObject["presetCreator"].isObject())
  {
    Utils::ParseGenericDialogSettings(aSettings.presetCreator, aJSONObject["presetCreator"].toObject(), aSettingsFileVersion);
  }

  // presetsRetargeting
  if (aJSONObject.contains("presetsRetargeting") && aJSONObject["presetsRetargeting"].isObject())
  {
    Utils::ParseGenericDialogSettings(aSettings.presetsRetargeting, aJSONObject["presetsRetargeting"].toObject(), aSettingsFileVersion);
  }

  // TODO: Make something cleaner to avoid reseting users' choices at every single update
  // Override some settings to avoid compatiblity issues when coming from older versions
  if (Utils::CompareVersionNumbers(aSettingsFileVersion, Utils::GetApplicationVersion()) == ApplicationVersionRelative::OLDER)
  {
    // Preset Creator
    aSettings.presetCreator.defaultBodyFeet = Struct::BodyFeetSettings();

    // Presets Retargeting
    aSettings.presetsRetargeting.defaultBodyFeet = Struct::BodyFeetSettings();

    // Batch Conversion
    aSettings.batchConversion.defaultBodyFeet = Struct::BodyFeetSettings();
  }
}

void Utils::ParseDisplaySettings(Struct::DisplaySettings& aSettings, const QJsonObject& aJSONObject)
{
  // App theme
  if (aJSONObject.contains("applicationTheme") && aJSONObject["applicationTheme"].isDouble())
    aSettings.applicationTheme = static_cast<GUITheme>(aJSONObject["applicationTheme"].toInt());

  // Title bar icons color
  if (aJSONObject.contains("titleBarIconsBlack") && aJSONObject["titleBarIconsBlack"].isBool())
    aSettings.titleBarIconsBlack = aJSONObject["titleBarIconsBlack"].toBool();

  // Opening mode: assisted conversion
  if (aJSONObject.contains("assistedConversionDialogOpeningMode") && aJSONObject["assistedConversionDialogOpeningMode"].isDouble())
    aSettings.assistedConversionDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["assistedConversionDialogOpeningMode"].toInt());

  // Opening mode: batch conversion
  if (aJSONObject.contains("batchConversionDialogOpeningMode") && aJSONObject["batchConversionDialogOpeningMode"].isDouble())
    aSettings.batchConversionDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["batchConversionDialogOpeningMode"].toInt());

  // Opening mode: batch conversion picker
  if (aJSONObject.contains("batchConversionPickerDialogOpeningMode") && aJSONObject["batchConversionPickerDialogOpeningMode"].isDouble())
    aSettings.batchConversionPickerDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["batchConversionPickerDialogOpeningMode"].toInt());

  // Opening mode: bodySlide presets' retargeting
  if (aJSONObject.contains("bodySlidePresetsRetargetingDialogOpeningMode") && aJSONObject["bodySlidePresetsRetargetingDialogOpeningMode"].isDouble())
    aSettings.bodySlidePresetsRetargetingDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["bodySlidePresetsRetargetingDialogOpeningMode"].toInt());

  // Danger color
  if (aJSONObject.contains("dangerColor") && aJSONObject["dangerColor"].isString())
    aSettings.dangerColor = aJSONObject["dangerColor"].toString();

  // Font family
  if (aJSONObject.contains("font") && aJSONObject["font"].isObject())
    Utils::ParseFontSettings(aSettings.font, aJSONObject["font"].toObject());

  // Language
  if (aJSONObject.contains("language") && aJSONObject["language"].isDouble())
    aSettings.language = static_cast<ApplicationLanguage>(aJSONObject["language"].toInt());

  // Default window height
  if (aJSONObject.contains("mainWindowHeight") && aJSONObject["mainWindowHeight"].isDouble())
    aSettings.mainWindowHeight = aJSONObject["mainWindowHeight"].toInt();

  // Opening mode: main window
  if (aJSONObject.contains("mainWindowOpeningMode") && aJSONObject["mainWindowOpeningMode"].isDouble())
    aSettings.mainWindowOpeningMode = static_cast<WindowOpeningMode>(aJSONObject["mainWindowOpeningMode"].toInt());

  // Default window width
  if (aJSONObject.contains("mainWindowWidth") && aJSONObject["mainWindowWidth"].isDouble())
    aSettings.mainWindowWidth = aJSONObject["mainWindowWidth"].toInt();

  // Opening mode: slider sets database manager
  if (aJSONObject.contains("sliderSetsDBManagerDialogOpeningMode") && aJSONObject["sliderSetsDBManagerDialogOpeningMode"].isDouble())
    aSettings.sliderSetsDBManagerDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["sliderSetsDBManagerDialogOpeningMode"].toInt());

  // Opening mode: slider sets importer
  if (aJSONObject.contains("sliderSetsImporterDialogOpeningMode") && aJSONObject["sliderSetsImporterDialogOpeningMode"].isDouble())
    aSettings.sliderSetsImporterDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["sliderSetsImporterDialogOpeningMode"].toInt());

  // Success color
  if (aJSONObject.contains("successColor") && aJSONObject["successColor"].isString())
    aSettings.successColor = aJSONObject["successColor"].toString();

  // Opening mode: textures assistant
  if (aJSONObject.contains("texturesAssistantDialogOpeningMode") && aJSONObject["texturesAssistantDialogOpeningMode"].isDouble())
    aSettings.texturesAssistantDialogOpeningMode = static_cast<DialogOpeningMode>(aJSONObject["texturesAssistantDialogOpeningMode"].toInt());

  // Warning color
  if (aJSONObject.contains("warningColor") && aJSONObject["warningColor"].isString())
    aSettings.warningColor = aJSONObject["warningColor"].toString();
}

void Utils::ParseFontSettings(Struct::Font& aSettings, const QJsonObject& aJSONObject)
{
  // family
  if (aJSONObject.contains("family") && aJSONObject["family"].isString())
    aSettings.family = aJSONObject["family"].toString();

  // italic
  if (aJSONObject.contains("italic") && aJSONObject["italic"].isBool())
    aSettings.italic = aJSONObject["italic"].toBool();

  // size
  if (aJSONObject.contains("size") && aJSONObject["size"].isDouble())
    aSettings.pointSize = aJSONObject["size"].toInt();

  // strikeOut
  if (aJSONObject.contains("strikeOut") && aJSONObject["strikeOut"].isBool())
    aSettings.strikeOut = aJSONObject["strikeOut"].toBool();

  // styleName
  if (aJSONObject.contains("styleName") && aJSONObject["styleName"].isString())
    aSettings.styleName = aJSONObject["styleName"].toString();

  // underline
  if (aJSONObject.contains("underline") && aJSONObject["underline"].isBool())
    aSettings.underline = aJSONObject["underline"].toBool();

  // weight
  if (aJSONObject.contains("weight") && aJSONObject["weight"].isDouble())
    aSettings.weight = aJSONObject["weight"].toInt();
}

void Utils::ParseGeneralSettings(Struct::GeneralSettings& aSettings, const QJsonObject& aJSONObject)
{
  // eachButtonSavesItsLastUsedPath
  if (aJSONObject.contains("eachButtonSavesItsLastUsedPath") && aJSONObject["eachButtonSavesItsLastUsedPath"].isBool())
  {
    aSettings.eachButtonSavesItsLastUsedPath = aJSONObject["eachButtonSavesItsLastUsedPath"].toBool();
  }

  // startupAction
  if (aJSONObject.contains("startupAction") && aJSONObject["startupAction"].isDouble())
  {
    aSettings.startupAction = static_cast<StartupAction>(aJSONObject["startupAction"].toDouble());
  }
}

void Utils::ParseGenericDialogSettings(Struct::GenericDialogSettings& aSettings, const QJsonObject& aJSONObject, const QString& aSettingsFileVersion)
{
  // automaticallyOpenFinalDirectory
  if (aJSONObject.contains("automaticallyOpenFinalDirectory") && aJSONObject["automaticallyOpenFinalDirectory"].isBool())
    aSettings.automaticallyOpenFinalDirectory = aJSONObject["automaticallyOpenFinalDirectory"].toBool();

  // defaultBodyFeet
  if (aJSONObject.contains("defaultBodyFeet") && aJSONObject["defaultBodyFeet"].isObject())
  {
    Utils::ParseBodyFeetSettings(aSettings.defaultBodyFeet, aJSONObject["defaultBodyFeet"].toObject(), aSettingsFileVersion);
  }
}

void Utils::ParseBodyFeetSettings(Struct::BodyFeetSettings& aSettings, const QJsonObject& aJSONObject, const QString& aSettingsFileVersion)
{
  // Compatibility mode
  if (Utils::CompareVersionNumbers(aSettingsFileVersion, "3.5.0.0") == ApplicationVersionRelative::OLDER)
  {
    const auto lCompatibilityBodyMod{DataLists::GetSplittedNameVersionFromBodyVersionCompatibility(aJSONObject["bodyMod"].toInt())};
    const auto lCompatibilityData{DataLists::ReadBodyFeetModsCompatibility(lCompatibilityBodyMod.first,
                                                                           lCompatibilityBodyMod.second,
                                                                           aJSONObject["feetMod"].toInt())};

    aSettings.bodyMesh = lCompatibilityData.first;
    aSettings.feetMesh = lCompatibilityData.second;
  }
  // Last format version
  else
  {
    // bodyMod
    if (aJSONObject.contains("bodyMesh") && aJSONObject["bodyMesh"].isDouble())
      aSettings.bodyMesh = static_cast<BodyNameVersion>(aJSONObject["bodyMesh"].toInt());

    // feetMod
    if (aJSONObject.contains("feetMesh") && aJSONObject["feetMesh"].isDouble())
      aSettings.feetMesh = static_cast<FeetNameVersion>(aJSONObject["feetMesh"].toInt());
  }
}

void Utils::SaveSettingsToFile(const Struct::Settings& aSettings)
{
  Utils::SaveAsJsonFile(Utils::SettingsStructToJson(aSettings), Utils::SettingsFullFilePath());
}

QJsonObject Utils::SettingsStructToJson(const Struct::Settings& aSettings)
{
  // Construct the full settings object
  auto lSettings{aSettings.toJson()};

  // Add the version number string
  lSettings["applicationVersion"] = Utils::GetApplicationVersion();

  return lSettings;
}

void Utils::CheckFiltersFileExistence()
{
  if (!QFile(Utils::GetFiltersFilePath()).exists())
  {
    // Create a default filters file if it does not exist
    Utils::SaveFiltersToFile(std::map<QString, QStringList>());
  }
}

std::map<QString, QStringList> Utils::LoadFiltersFromFile()
{
  Utils::CheckFiltersFileExistence();

  QJsonObject lObtainedJSON{Utils::LoadFromJsonFile(Utils::GetFiltersFilePath())};

  std::map<QString, QStringList> lFiltersList;
  const auto lVariantMap{lObtainedJSON.toVariantMap()};
  const auto lKeys{lVariantMap.keys()};

  for (const auto& lKey : lKeys)
  {
    lFiltersList.insert(std::make_pair(lKey, lVariantMap.value(lKey, "").toStringList()));
  }

  return lFiltersList;
}

void Utils::SaveFiltersToFile(const std::map<QString, QStringList>& aList)
{
  Utils::SaveAsJsonFile(Utils::FiltersMapToJson(aList), Utils::GetFiltersFilePath());
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

QString Utils::GetAdditionalFeetFilter(const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion)
{
  const auto lIsCBBEBody{Utils::IsCBBEBasedBody(aBodyNameVersion)};
  const auto lFeetName{DataLists::GetName(aFeetNameVersion)};
  switch (lFeetName)
  {
    case FeetName::MORE_SLIDERS_FOR_FEET:
    {
      if (lIsCBBEBody)
        return QString("MSF CBBE Feet");

      return QString("MSF BHUNP Feet");
    }
    case FeetName::HG_FEET:
    {
      if (lIsCBBEBody)
        return QString();

      return QString("HGFeet UUNP");
    }
    case FeetName::_INVALID_VALUE:
    case FeetName::CBBE:
    case FeetName::BHUNP_3BBB_ADVANCED:
    case FeetName::CBBE_3BA_3BBB:
    case FeetName::COCO_BODY:
    case FeetName::MIMIR_EBONIC_BODY:
    case FeetName::ASDASDF:
    case FeetName::KHRYSAMERE_HG_FEET:
      return QString();
  }
  return QString();
}

std::vector<Struct::Filter> Utils::GetFiltersForExport(const std::map<QString, QStringList>& aList, const QString& aKey, const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion)
{
  auto lExportFilters{std::vector<Struct::Filter>()};

  for (const auto& lPair : aList)
  {
    if (lPair.first.compare(aKey, Qt::CaseSensitivity::CaseSensitive) == 0)
    {
      // Parse user' sliders
      for (const auto& lFilter : lPair.second)
      {
        lExportFilters.push_back(Struct::Filter(lFilter, true, true, true));
      }

      // Additional feet slider
      const auto lAdditionalFilter{Utils::GetAdditionalFeetFilter(aBodyNameVersion, aFeetNameVersion)};
      if (!lAdditionalFilter.isEmpty())
      {
        lExportFilters.push_back(Struct::Filter(lAdditionalFilter, false, true, false));
      }

      break;
    }
  }

  return lExportFilters;
}

void Utils::CheckLastPathsFileExistence()
{
  if (!QFile(Utils::GetLastPathsFilePath()).exists())
  {
    // Create a default "last paths" file if it does not exist
    std::map<QString, QString> lMap;
    const auto lKeys{DataLists::GetLastPathsKeys()};

    for (const auto& lKey : lKeys)
    {
      lMap.insert(std::make_pair(lKey, ""));
    }

    Utils::SaveLastPathsToFile(lMap);
  }
}

std::map<QString, QString> Utils::LoadLastPathsFromFile()
{
  Utils::CheckLastPathsFileExistence();

  const QJsonObject lObtainedJSON{Utils::LoadFromJsonFile(Utils::GetLastPathsFilePath())};

  auto lVariantMap{lObtainedJSON.toVariantMap()};
  std::map<QString, QString> lLastPathsList{
    {"assistedConversionInput", ""},
    {"batchConversionInput", ""},
    {"batchConversionOutput", ""},
    {"general", ""},
    {"lastInjectedOSPFile", ""},
    {"lastLoadedProject", ""},
    {"lastSavedProject", ""},
    {"mainWindowOutput", ""},
    {"retargetingToolInput", ""},
    {"retargetingToolOutput", ""},
    {"sliderSetsImporterInput", ""},
    {"texturesAssistantInput", ""},
    {"texturesAssistantOutput", ""}};

  const auto lKeys{lVariantMap.keys()};
  for (const auto& lKey : lKeys)
  {
    const auto lPosition{lLastPathsList.find(lKey)};
    if (lPosition != lLastPathsList.cend())
      lLastPathsList.find(lKey)->second = lVariantMap.value(lKey, "").toString();
  }

  return lLastPathsList;
}

void Utils::SaveLastPathsToFile(const std::map<QString, QString>& aList)
{
  Utils::SaveAsJsonFile(Utils::LastPathsStructToJson(aList), Utils::GetLastPathsFilePath());
}

QJsonObject Utils::LastPathsStructToJson(const std::map<QString, QString>& aList)
{
  QVariantMap lMap;

  for (const auto& lPair : aList)
  {
    lMap.insert(lPair.first, lPair.second);
  }

  return QJsonObject::fromVariantMap(lMap);
}

QString Utils::GetPathFromKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aFallbackPath, const bool aUseKeyPath)
{
  QString lPath;
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
    aMap->insert(std::make_pair(aKey, aPath));
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

bool Utils::SaveAPIKeyToFile(const QString& aAPIKey, QWidget* aParent, const QString& aIconFolder)
{
  const auto lAPIKeyFilePath{Utils::GetPrivateKeyFilePath()};

  // Open (or create and open) the file
  QFile lFile(lAPIKeyFilePath);
  if (lFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
  {
    lFile.write(aAPIKey.toUtf8());

    // API key file save: success window
    if (aParent && !aIconFolder.isEmpty())
    {
      Utils::DisplayInfoMessage(aParent,
                                tr("API key successfully saved"),
                                tr("The API key file has successfully been saved to \"%1\".").arg(lAPIKeyFilePath),
                                aIconFolder,
                                "done",
                                tr("OK"));
    }

    lFile.close();
    return true;
  }
  else if (aParent && !aIconFolder.isEmpty())
  {
    // API key file save: fail window
    Utils::DisplayWarningMessage(tr("Could not save the API key file to \"%1\".\nBe sure to not save the file in a OneDrive/DropBox space and that you executed the application with sufficient permissions.\nBe sure that you used characters authorized by your OS in the given paths.").arg(lAPIKeyFilePath));
  }

  return false;
}

QString Utils::ReadAPIKeyFromFile()
{
  // Check if an installer file needs to be removed at launch
  QFile lAPIKeyFile(Utils::GetPrivateKeyFilePath());
  if (lAPIKeyFile.exists())
  {
    if (lAPIKeyFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      const auto lAPIKey{QString::fromUtf8(lAPIKeyFile.readAll())};
      lAPIKeyFile.close();

      return lAPIKey;
    }
  }

  return QString();
}

QString Utils::GetShortLanguageNameFromEnum(const int aEnumValue)
{
  return Utils::GetShortLanguageNameFromEnum(static_cast<ApplicationLanguage>(aEnumValue));
}

QString Utils::GetShortLanguageNameFromEnum(const ApplicationLanguage& aLanguage)
{
  switch (aLanguage)
  {
    case ApplicationLanguage::ENGLISH:
      return "en";
    case ApplicationLanguage::FRENCH:
      return "fr";
    case ApplicationLanguage::CHINESE_TRADITIONAL:
      return "zh_TW";
  }

  return "en";
}

ApplicationLanguage Utils::GetStructLanguageFromName(const QString& aShortName)
{
  if (aShortName.compare(QStringLiteral("English"), Qt::CaseSensitivity::CaseInsensitive) == 0)
  {
    return ApplicationLanguage::ENGLISH;
  }

  if (aShortName.compare(QStringLiteral("French"), Qt::CaseSensitivity::CaseInsensitive) == 0)
  {
    return ApplicationLanguage::FRENCH;
  }

  if (aShortName.compare(QStringLiteral("Chinese"), Qt::CaseSensitivity::CaseInsensitive) == 0)
  {
    return ApplicationLanguage::CHINESE_TRADITIONAL;
  }

  // Default language if no supported language has been found
  return ApplicationLanguage::ENGLISH;
}

// TODO: Move to ComponentFactory
QAction* Utils::BuildQAction(QWidget* aParent, const QString& aText, const QKeySequence& aKeysCombination, const QString& aIconName, const QString& aIconFolder)
{
  auto lAction{new QAction(aParent)};
  lAction->setText(aText);
  lAction->setShortcut(aKeysCombination);
  lAction->setIcon(QIcon(QPixmap(QString(":/%1/%2").arg(aIconFolder, aIconName))));
  // TODO: Find a way to set the size of the icons + multiply the size by the DPI scale
  return lAction;
}

// TODO: Move to ComponentFactory
void Utils::AddLastPathLine(QWidget* aParent, QGridLayout* aLayout, const int aRow, const QString& aLabel, const QString& aValue, const QString& aIconFolder, const QString& aIconName)
{
  aLayout->addWidget(new QLabel(aLabel, aParent), aRow, 0);

  const auto lGeneralValue{new LineEdit(aValue, aParent)};
  lGeneralValue->setReadOnly(true);
  lGeneralValue->setCursor(Qt::CursorShape::IBeamCursor);
  lGeneralValue->setObjectName(QStringLiteral("line_edit_path_%1").arg(aRow));
  aLayout->addWidget(lGeneralValue, aRow, 1);

  const auto lGeneralEmptyButton{ComponentFactory::CreateButton(aParent,
                                                                tr("Remove from history"),
                                                                "",
                                                                aIconName,
                                                                aIconFolder,
                                                                QString("clear_path_%1").arg(aRow),
                                                                aValue.isEmpty(),
                                                                true)};
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

  if (lPrevKey.isEmpty() || aFilterList.count(lPrevKey) == 0)
  {
    aComboBox->setCurrentIndex(0);
  }
  else
  {
    auto lPrevIndex{0};
    for (const auto& lPair : aFilterList)
    {
      if (lPair.first.compare(lPrevKey, Qt::CaseSensitivity::CaseSensitive) == 0)
      {
        break;
      }
      lPrevIndex++;
    }

    if (lPrevIndex == static_cast<int>(aFilterList.size()))
    {
      lPrevIndex = 0;
    }

    aComboBox->setCurrentIndex(lPrevIndex);
  }
}

void Utils::UpdateOutputPreview(QFileSystemWatcher* aFileWatcher, QLineEdit* aMainDirTextEdit, const QString& aSubDirectory, const bool aUseOnlySubdir, const QString& aSuccessColor, const QString& aWarningColor, const QString& aDangerColor, QLabel* aOutputPathsPreview)
{
  auto lMainDirectory{aMainDirTextEdit->text().trimmed()};
  Utils::CleanPathString(lMainDirectory);
  auto lSubDirectory{aSubDirectory};
  Utils::CleanPathString(lSubDirectory);
  auto lIsValidPath{true};

  // Construct full path
  QString lFullPath;

  if (aUseOnlySubdir)
  {
    aMainDirTextEdit->setDisabled(true);

    if (!lSubDirectory.isEmpty())
    {
      lFullPath = lSubDirectory;
    }
    else
    {
      lFullPath = tr("No path given or invalid path given.");
      lIsValidPath = false;
    }
  }
  else
  {
    if (!lMainDirectory.isEmpty() && !lSubDirectory.isEmpty())
    {
      lFullPath = lMainDirectory + "/" + lSubDirectory;
      aMainDirTextEdit->setDisabled(false);
    }
    else if (!lMainDirectory.isEmpty() && lSubDirectory.isEmpty())
    {
      lFullPath = lMainDirectory;
      aMainDirTextEdit->setDisabled(false);
    }
    else if (lMainDirectory.isEmpty() && !lSubDirectory.isEmpty())
    {
      lFullPath = tr("You must choose a directory through the file chooser. Current path defined: \" /%1\".").arg(lSubDirectory);
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

  // Clear all the watched paths
  aFileWatcher->removePaths(aFileWatcher->files());

  // Set the full path value in the preview label
  auto lNewTextColor{aSuccessColor};

  if (lIsValidPath)
  {
    if (QDir(lFullPath).exists() || aUseOnlySubdir)
    {
      lNewTextColor = aWarningColor;
    }

    // Add a new path to watch to
    aFileWatcher->addPath(lFullPath);
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
#ifdef Q_OS_WIN
  FreeConsole();
  AllocConsole();
  FILE* lOutFile{NULL};
  // freopen_s(&lOutFile, "CONIN$", "r", stdin);
  freopen_s(&lOutFile, "CONOUT$", "w", stdout);
  // freopen_s(&lOutFile, "CONOUT$", "w", stderr);
  SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
#endif
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
