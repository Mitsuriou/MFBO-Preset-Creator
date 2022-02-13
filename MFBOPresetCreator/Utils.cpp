#include "Utils.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Enum.h"
#include "SliderFileBuilder.h"
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDirIterator>
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

std::map<QString, std::vector<QString>> Utils::ToMapQsVecQs(const std::map<QString, std::set<QString>>& aMap)
{
  std::map<QString, std::vector<QString>> lNewMap;

  for (const auto& lMapEntry : aMap)
  {
    lNewMap.insert({lMapEntry.first, std::vector<QString>(lMapEntry.second.begin(), lMapEntry.second.end())});
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
  auto lTagName{QString()};

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
    case GUITheme::WINDOWS_VISTA:
    case GUITheme::MITSURIOU_LIGHT_THEME:
    case GUITheme::ALEXHUSZAGH_BREEZE_LIGHT:
    case GUITheme::PAPER_LIGHT:
    case GUITheme::PAPER_WHITE_MONO:
      return false;
  }

  return false;
}

QString Utils::GetIconResourceFolder(const GUITheme& aTheme)
{
  return (Utils::IsThemeDark(aTheme) ? QString("white") : QString("black"));
}

void Utils::OverrideHTMLLinksColor(QString& aHTMLString, const GUITheme& aTheme)
{
  // If no color change is needed
  if (aTheme != GUITheme::MITSURIOU_BLACK_THEME
      && aTheme != GUITheme::MITSURIOU_DARK_THEME
      && aTheme != GUITheme::MITSURIOU_LIGHT_THEME)
  {
    return;
  }

  // Hacky links' colors override for some themes
  const auto lLinksColorOverride(aTheme == GUITheme::MITSURIOU_BLACK_THEME ? QString("color:#3991ff") : QString("color:#e95985"));

  // Go through the string to find the link colors
  auto i{0};
  while ((i = aHTMLString.indexOf("color:#0000ff", i)) != -1)
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
  if (aFileName.midRef(aFileName.lastIndexOf('/') + 1).contains("skeleton", Qt::CaseInsensitive))
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

  // If not "3.00 to 3.02", "3.03" or "3.04"
  if (aBodyVariant == BodyVariant::BHUNP_3BBB_ADVANCED_VER_3
      && !(aRelativeVersion >= 5 && aRelativeVersion <= 7) // "3.00 to 3.02", "3.03", "3.04"
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

bool Utils::QRCResourceExists(const QString& aRessourcePath)
{
  return QFile(aRessourcePath).exists();
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

  return lPresetName.left(lPresetName.lastIndexOf('-') - 1);
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
      if (lTempSet.getName().endsWith(" - CBBE 3BBB Body Amazing", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - CBBE 3BBB Amazing NeverNude", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - CBBE 3BBB Amazing Underwear", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP 3BBB", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Ver 2", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Ver 3", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP BBP", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP BBP Advanced", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP TBBP", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP TBBP Advanced", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Ver 2 Nevernude", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - CBBE Body SMP (3BBB)", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - [COCO]3Bsmp_BodyV4_A", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - [COCO 3BBB V6]Body_A", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - [COCO]body_B_v3", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - [COCO]3Bsmp_BodyV4_B", Qt::CaseInsensitive)
          || lTempSet.getName().endsWith(" - [COCO 3BBB V6]Body_B", Qt::CaseInsensitive))
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
               || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Hands Ver 3", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO]3Bsmp_HandV4_A", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO 3BBB V6]Hands_A", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO]bodyHands_B_v3", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO]3Bsmp_HandV4_B", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO 3BBB V6]Hands_B", Qt::CaseInsensitive))
      {
        lTempSet.setMeshPart("Hands"); // Hands
      }
      else if (lTempSet.getName().endsWith(" - Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE 3BBB Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO]3Bsmp_FeetV4_A", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO 3BBB V6]Feet_A", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO]bodyFeets_B_v3", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO]3Bsmp_FeetV4_B", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [COCO 3BBB V6]Feet_B", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - Feet (MSF - normal)", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE MSF Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - BHUNP Feet MSF", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - Feet (MSF - HH)", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE MSF Feet High Heel", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - BHUNP Feet High Heel MSF", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - HGFeet UUNP", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [Khrysamere] HG Feet (BHUNP)", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [Khrysamere] K Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - [Khrysamere] K Feet v5.0", Qt::CaseInsensitive))
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

        if (!lTempSet.getOutputPath().isEmpty() && !lTempSet.getOutputFile().isEmpty())
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

  // Override some settings to avoid compatiblity issues when coming from older versions
  if (Utils::CompareVersionNumbers(aSettingsFileVersion, "3.6.3.0") == ApplicationVersionRelative::OLDER)
  {
    // Preset Creator
    const auto lPresetCreatorDefaultBodyMesh{static_cast<int>(aSettings.presetCreator.defaultBodyFeet.bodyMesh)};
    if (lPresetCreatorDefaultBodyMesh == 4)
      aSettings.presetCreator.defaultBodyFeet.bodyMesh = BodyNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12;
    else if (lPresetCreatorDefaultBodyMesh == 5)
      aSettings.presetCreator.defaultBodyFeet.bodyMesh = BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18;

    const auto lPresetCreatorDefaultFeetMesh{static_cast<int>(aSettings.presetCreator.defaultBodyFeet.feetMesh)};
    if (lPresetCreatorDefaultFeetMesh == 10)
      aSettings.presetCreator.defaultBodyFeet.feetMesh = FeetNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12;
    else if (lPresetCreatorDefaultFeetMesh == 11)
      aSettings.presetCreator.defaultBodyFeet.feetMesh = FeetNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18;

    // Presets Retargeting
    const auto lPresetsRetargetingDefaultBodyMesh{static_cast<int>(aSettings.presetsRetargeting.defaultBodyFeet.bodyMesh)};
    if (lPresetsRetargetingDefaultBodyMesh == 4)
      aSettings.presetsRetargeting.defaultBodyFeet.bodyMesh = BodyNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12;
    else if (lPresetsRetargetingDefaultBodyMesh == 5)
      aSettings.presetsRetargeting.defaultBodyFeet.bodyMesh = BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18;

    const auto lPresetsRetargetingDefaultFeetMesh{static_cast<int>(aSettings.presetsRetargeting.defaultBodyFeet.feetMesh)};
    if (lPresetsRetargetingDefaultFeetMesh == 10)
      aSettings.presetsRetargeting.defaultBodyFeet.feetMesh = FeetNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12;
    else if (lPresetsRetargetingDefaultFeetMesh == 11)
      aSettings.presetsRetargeting.defaultBodyFeet.feetMesh = FeetNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18;

    // Batch Conversion
    const auto lBatchConversionDefaultBodyMesh{static_cast<int>(aSettings.batchConversion.defaultBodyFeet.bodyMesh)};
    if (lBatchConversionDefaultBodyMesh == 4)
      aSettings.batchConversion.defaultBodyFeet.bodyMesh = BodyNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12;
    else if (lBatchConversionDefaultBodyMesh == 5)
      aSettings.batchConversion.defaultBodyFeet.bodyMesh = BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18;

    const auto lBatchConversionDefaultFeetMesh{static_cast<int>(aSettings.batchConversion.defaultBodyFeet.feetMesh)};
    if (lBatchConversionDefaultFeetMesh == 10)
      aSettings.batchConversion.defaultBodyFeet.feetMesh = FeetNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12;
    else if (lBatchConversionDefaultFeetMesh == 11)
      aSettings.batchConversion.defaultBodyFeet.feetMesh = FeetNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18;
  }
}

void Utils::ParseDisplaySettings(Struct::DisplaySettings& aSettings, const QJsonObject& aJSONObject)
{
  // App theme
  if (aJSONObject.contains("applicationTheme") && aJSONObject["applicationTheme"].isDouble())
    aSettings.applicationTheme = static_cast<GUITheme>(aJSONObject["applicationTheme"].toInt());

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
    aSettings.size = aJSONObject["size"].toInt();

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
  auto lSettingsFilePath{Utils::GetAppDataPathFolder() + "config.json"};
  Utils::SaveAsJsonFile(Utils::SettingsStructToJson(aSettings), lSettingsFilePath);
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
    if (lPair.first.compare(aKey, Qt::CaseSensitive) == 0)
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
  const auto lLastPathsFilePath{Utils::GetAppDataPathFolder() + "paths.json"};
  if (!QFile(lLastPathsFilePath).exists())
  {
    // Create a default "last paths" file if it does not exist
    std::map<QString, QString> lMap;
    const auto lKeys{DataLists::GetLastPathsKeys()};

    for (const auto& lKey : lKeys)
    {
      lMap.insert(std::make_pair(lKey, QString()));
    }

    Utils::SaveLastPathsToFile(lMap);
  }
}

std::map<QString, QString> Utils::LoadLastPathsFromFile()
{
  Utils::CheckLastPathsFileExistence();

  const auto lLastPathsFilePath(Utils::GetAppDataPathFolder() + "paths.json");
  const QJsonObject lObtainedJSON{Utils::LoadFromJsonFile(lLastPathsFilePath)};

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
    {"texturesAssistantInput", ""},
    {"texturesAssistantOutput", ""}};

  const auto lKeys{lVariantMap.keys()};
  for (const auto& lKey : lKeys)
  {
    lLastPathsList.find(lKey)->second = lVariantMap.value(lKey, "").toString();
  }

  return lLastPathsList;
}

void Utils::SaveLastPathsToFile(const std::map<QString, QString>& aList)
{
  const auto lLastPathsFilePath{Utils::GetAppDataPathFolder() + "paths.json"};
  Utils::SaveAsJsonFile(Utils::LastPathsStructToJson(aList), lLastPathsFilePath);
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
  const auto lEnumLang{static_cast<ApplicationLanguage>(aEnumValue)};
  switch (lEnumLang)
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
  // TODO: Find a way to set the size of the icons + multiply the size by the DPI scale
  return lAction;
}

void Utils::AddLastPathLine(QWidget* aParent, QGridLayout* aLayout, const int aRow, const QString& aLabel, const QString& aValue, const QString& aIconFolder, const QString& aIconName)
{
  aLayout->addWidget(new QLabel(aLabel, aParent), aRow, 0);

  const auto lGeneralValue{new QLineEdit(aValue, aParent)};
  lGeneralValue->setReadOnly(true);
  lGeneralValue->setCursor(Qt::CursorShape::IBeamCursor);
  lGeneralValue->setObjectName(QString("line_edit_path_%1").arg(aRow));
  aLayout->addWidget(lGeneralValue, aRow, 1);

  const auto lGeneralEmptyButton{ComponentFactory::CreateButton(aParent, tr("Remove from history"), "", aIconName, aIconFolder, QString("clear_path_%1").arg(aRow), aValue.isEmpty(), true)};
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
      if (lPair.first.compare(lPrevKey, Qt::CaseSensitive) == 0)
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
  //freopen_s(&lOutFile, "CONIN$", "r", stdin);
  freopen_s(&lOutFile, "CONOUT$", "w", stdout);
  //freopen_s(&lOutFile, "CONOUT$", "w", stderr);
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
