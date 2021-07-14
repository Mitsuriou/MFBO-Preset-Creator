#include "Utils.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Enum.h"
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDirIterator>
#include <QGroupBox>
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

void Utils::cleanPathString(QString& aPath)
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
  Utils::cleanBreaksString(aPath);
}

QString Utils::cleanPathString(const QString& aPath)
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
  return qApp->applicationVersion();
}

ApplicationVersionRelative Utils::compareVersionNumbers(const QString& aVersionNumber)
{
  return Utils::compareVersionNumbers(aVersionNumber, Utils::getApplicationVersion());
}

ApplicationVersionRelative Utils::compareVersionNumbers(const QString& aVersionNumber1, const QString& aVersionNumber2)
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

void Utils::displayWarningMessage(const QString& aMessage)
{
  QMessageBox lBox(QMessageBox::Icon::Warning, tr("Warning"), aMessage);
  lBox.setIconPixmap(QPixmap(":/icons/red-alert-circle").scaledToHeight(48, Qt::SmoothTransformation));

  QPushButton lButton(tr("OK"));
  lButton.setCursor(Qt::PointingHandCursor);
  lBox.addButton(&lButton, QMessageBox::ButtonRole::AcceptRole);

  lBox.exec();
}

ButtonClicked Utils::displayQuestionMessage(QWidget* aParent, const QString& aTitle, const QString& aMessage, const QString& aIconFolder, const QString& aIconName, const QString& aTextBtnYes, const QString& aTextBtnNo, const QString& aTextBtnOther, const QString& aColorYesBtn, const QString& aColorNoBtn, const QString& aColorOtherBtn, const bool aIsYesBtnDefault)
{
  QMessageBox lConfirmationBox(QMessageBox::Icon::Question, aTitle, aMessage, QMessageBox::StandardButton::NoButton, aParent);
  lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/%2").arg(aIconFolder).arg(aIconName)).scaledToHeight(48, Qt::SmoothTransformation));

  auto lYesButton{lConfirmationBox.addButton(aTextBtnYes, QMessageBox::ButtonRole::YesRole)};
  lYesButton->setCursor(Qt::PointingHandCursor);
  if (aColorYesBtn.length() > 0)
  {
    lYesButton->setStyleSheet(QString("color: %1;").arg(aColorYesBtn));
  }

  auto lNoButton{lConfirmationBox.addButton(aTextBtnNo, QMessageBox::ButtonRole::NoRole)};
  lNoButton->setCursor(Qt::PointingHandCursor);
  if (aColorNoBtn.length() > 0)
  {
    lNoButton->setStyleSheet(QString("color: %1;").arg(aColorNoBtn));
  }

  QPushButton* lOtherButton{nullptr};
  if (aTextBtnOther.length() > 0)
  {
    lOtherButton = lConfirmationBox.addButton(aTextBtnOther, QMessageBox::ButtonRole::HelpRole);
    lOtherButton->setCursor(Qt::PointingHandCursor);
    if (aColorOtherBtn.length() > 0)
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

int Utils::getNumberFilesByExtensionRecursiveIgnoringFOMOD(const QString& aRootDir, const QString& aFileExtension)
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

int Utils::getNumberFilesByExtensions(const QString& aRootDir, const QStringList& aFileExtensions)
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

int Utils::getNumberFilesByExtensionsRecursive(const QString& aRootDir, const QStringList& aFileExtensions)
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

bool Utils::removeDirectoryAndSubDirs(const QString& aPath)
{
  QFileInfo lFileInfo(aPath);
  if (!lFileInfo.isDir())
  {
    return false;
  }

  return QDir(aPath).removeRecursively();
}

bool Utils::isThemeDark(const GUITheme& aTheme)
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

QString Utils::getIconRessourceFolder(const GUITheme& aTheme)
{
  return (Utils::isThemeDark(aTheme) ? QString("white") : QString("black"));
}

bool Utils::isCBBEBasedBody(const BodyNameVersion& aBody)
{
  switch (aBody)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
    case BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04:
    case BodyNameVersion::CBBE_3BBB_3BA_2_06:
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
      return true;
    default:
      return false;
  }
}

bool Utils::isBodySupportingBeastHands(const BodyNameVersion& aBody)
{
  return Utils::isCBBEBasedBody(aBody);
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

QString Utils::readQRCFileContent(const QString& aFilePath)
{
  QString lContent;

  QFile lQRCFile(aFilePath);
  if (lQRCFile.open(QFile::ReadOnly | QFile::Text))
  {
    QTextStream lFileBuffer(&lQRCFile);
    lContent = lFileBuffer.readAll();
  }

  return lContent;
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
  auto lPresetName{QString()};

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
      lTempSet.setName(lSliderSet.attribute("name", "")); // Name

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
      else if (lTempSet.getName().endsWith(" - Beast Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE Beast Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE 3BBB Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE 3BBB Hands Beast", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE Hands Beast", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE Hands", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - BHUNP 3BBB Advanced Hands", Qt::CaseInsensitive))
      {
        lTempSet.setMeshPart("Hands"); // Hands
      }
      else if (lTempSet.getName().endsWith(" - Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE 3BBB Feet", Qt::CaseInsensitive)
               || lTempSet.getName().endsWith(" - CBBE Feet", Qt::CaseInsensitive)
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

bool Utils::isPresetUsingBeastHands(const QString& aPath)
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
    Utils::displayWarningMessage(tr("Error while trying to read the file \"%1\".").arg(aPath));
    return false;
  }

  if (lFileContent.contains("beast hands", Qt::CaseInsensitive) || lFileContent.contains("hands beast", Qt::CaseInsensitive))
  {
    return true;
  }

  return false;
}

QString Utils::getHandsSliderValue(const BodyNameVersion& aBody, const bool aMustUseBeastHands)
{
  auto lHandsValue{QString()};

  switch (aBody)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
      if (aMustUseBeastHands)
        lHandsValue = QString("%1 - CBBE Beast Hands");
      else
        lHandsValue = QString("%1 - CBBE 3BBB Hands");
      break;
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
    case BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04:
    case BodyNameVersion::CBBE_3BBB_3BA_2_06:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
      if (aMustUseBeastHands)
        lHandsValue = QString("%1 - CBBE 3BBB Hands Beast");
      else
        lHandsValue = QString("%1 - CBBE 3BBB Hands");
      break;
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      if (aMustUseBeastHands)
        lHandsValue = QString("%1 - CBBE Hands Beast");
      else
        lHandsValue = QString("%1 - CBBE Hands");
      break;
    default: // UNP-based bodies
      lHandsValue = QString("%1 - BHUNP 3BBB Advanced Hands");
      break;
  }

  return (lHandsValue);
}

QString Utils::getFeetSliderValue(const BodyNameVersion& aBody, const int aFeetModIndex)
{
  auto lFeetValue{QString()};

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
          lFeetValue = QString("%1 - CBBE 3BBB Feet");
          break;
        case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
          lFeetValue = QString("%1 - CBBE Feet");
          break;
        case BodyNameVersion::BHUNP_3BBB_2_20:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
        case BodyNameVersion::BHUNP_BBP_2_20:
        case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
        case BodyNameVersion::BHUNP_TBBP_2_20:
        case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
        case BodyNameVersion::BHUNP_3BBB_2_25:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
        case BodyNameVersion::BHUNP_BBP_2_25:
        case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
        case BodyNameVersion::BHUNP_TBBP_2_25:
        case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
        case BodyNameVersion::BHUNP_3BBB_2_30:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
        case BodyNameVersion::BHUNP_BBP_2_30:
        case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
        case BodyNameVersion::BHUNP_TBBP_2_30:
        case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
        case BodyNameVersion::BHUNP_3BBB_2_31:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
        case BodyNameVersion::BHUNP_BBP_2_31:
        case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
        case BodyNameVersion::BHUNP_TBBP_2_31:
        case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
        case BodyNameVersion::BHUNP_3BBB_2_35:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35:
        case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35:
        case BodyNameVersion::BHUNP_BBP_2_35:
        case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35:
        case BodyNameVersion::BHUNP_TBBP_2_35:
        case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35:
          lFeetValue = QString("%1 - BHUNP 3BBB Advanced Feet");
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

  return (lFeetValue + "\n");
}

QString Utils::getBodySliderValue(const BodyNameVersion& aBody)
{
  auto lBodyValue{QString()};

  switch (aBody)
  {
    case BodyNameVersion::CBBE_3BBB_3BA_1_50:
    case BodyNameVersion::CBBE_3BBB_3BA_1_51_TO_1_55:
    case BodyNameVersion::CBBE_3BBB_3BA_2_02_TO_2_04:
    case BodyNameVersion::CBBE_3BBB_3BA_2_06:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
      lBodyValue = QString("%1 - CBBE 3BBB Body Amazing");
      break;
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      lBodyValue = QString("%1 - CBBE Body SMP (3BBB)");
      break;
    case BodyNameVersion::BHUNP_3BBB_2_20:
    case BodyNameVersion::BHUNP_3BBB_2_25:
    case BodyNameVersion::BHUNP_3BBB_2_30:
    case BodyNameVersion::BHUNP_3BBB_2_31:
    case BodyNameVersion::BHUNP_3BBB_2_35:
      lBodyValue = QString("%1 - BHUNP 3BBB");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35:
      lBodyValue = QString("%1 - BHUNP 3BBB Advanced");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35:
      lBodyValue = QString("%1 - BHUNP 3BBB Advanced Ver 2");
      break;
    case BodyNameVersion::BHUNP_BBP_2_20:
    case BodyNameVersion::BHUNP_BBP_2_25:
    case BodyNameVersion::BHUNP_BBP_2_30:
    case BodyNameVersion::BHUNP_BBP_2_31:
    case BodyNameVersion::BHUNP_BBP_2_35:
      lBodyValue = QString("%1 - BHUNP BBP");
      break;
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35:
      lBodyValue = QString("%1 - BHUNP BBP Advanced");
      break;
    case BodyNameVersion::BHUNP_TBBP_2_20:
    case BodyNameVersion::BHUNP_TBBP_2_25:
    case BodyNameVersion::BHUNP_TBBP_2_30:
    case BodyNameVersion::BHUNP_TBBP_2_31:
    case BodyNameVersion::BHUNP_TBBP_2_35:
      lBodyValue = QString("%1 - BHUNP TBBP");
      break;
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35:
      lBodyValue = QString("%1 - BHUNP TBBP Advanced");
      break;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
      lBodyValue = QString("%1 - BHUNP 3BBB Advanced Ver 2 Nevernude");
      break;
  }

  return (lBodyValue + "\n");
}

void Utils::saveAsJsonFile(const QJsonObject& aJsonToSave, const QString& aFilePath, QWidget* aParent, const QString& aIconFolder)
{
  // Open (or create and open) the file
  QFile lFile(aFilePath);
  if (lFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
  {
    lFile.write(QJsonDocument(aJsonToSave).toJson());

    // Project file save: success window
    if (aParent && aIconFolder.length() > 0)
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
  else if (aParent && aIconFolder.length() > 0)
  {
    // Project file save: fail window
    Utils::displayWarningMessage(tr("Could not save the project file to \"%1\".\nBe sure to not save the file in a OneDrive/DropBox space and that you executed the application with sufficient permissions.\nBe sure that you used characters authorized by your OS in the given paths.").arg(aFilePath));
  }

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

  // Font family
  if (lSettingsJSON.contains("font"))
  {
    auto lFontJSON{lSettingsJSON["font"].toObject()};

    if (lFontJSON.contains("family") && lFontJSON["family"].isString())
    {
      lSettings.font.family = lFontJSON["family"].toString();
    }

    if (lFontJSON.contains("italic") && lFontJSON["italic"].isBool())
    {
      lSettings.font.italic = lFontJSON["italic"].toBool();
    }

    if (lFontJSON.contains("size") && lFontJSON["size"].isDouble())
    {
      lSettings.font.size = lFontJSON["size"].toInt();
    }

    if (lFontJSON.contains("strikeOut") && lFontJSON["strikeOut"].isBool())
    {
      lSettings.font.strikeOut = lFontJSON["strikeOut"].toBool();
    }

    if (lFontJSON.contains("styleName") && lFontJSON["styleName"].isString())
    {
      lSettings.font.styleName = lFontJSON["styleName"].toString();
    }

    if (lFontJSON.contains("underline") && lFontJSON["underline"].isBool())
    {
      lSettings.font.underline = lFontJSON["underline"].toBool();
    }

    if (lFontJSON.contains("weight") && lFontJSON["weight"].isDouble())
    {
      lSettings.font.weight = lFontJSON["weight"].toInt();
    }
  }

  // Dark theme
  if (lSettingsJSON.contains("appTheme") && lSettingsJSON["appTheme"].isDouble())
  {
    auto lFoundAppTheme{lSettingsJSON["appTheme"].toInt()};
    lSettings.appTheme = static_cast<GUITheme>(lFoundAppTheme);
  }

  // Assisted Conversion: only scan the meshes subdir
  if (lSettingsJSON.contains("assistedConversionScanOnlyMeshesSubdir") && lSettingsJSON["assistedConversionScanOnlyMeshesSubdir"].isBool())
  {
    lSettings.assistedConversionScanOnlyMeshesSubdir = lSettingsJSON["assistedConversionScanOnlyMeshesSubdir"].toBool();
  }

  // Batch Conversion: output path
  if (lSettingsJSON.contains("batchConversionOutputPath") && lSettingsJSON["batchConversionOutputPath"].isString())
  {
    lSettings.batchConversionOutputPath = lSettingsJSON["batchConversionOutputPath"].toString();
  }

  // Check for updates at startup
  if (lSettingsJSON.contains("checkForUpdatesAtStartup") && lSettingsJSON["checkForUpdatesAtStartup"].isBool())
  {
    lSettings.checkForUpdatesAtStartup = lSettingsJSON["checkForUpdatesAtStartup"].toBool();
  }

  // Danger color
  if (lSettingsJSON.contains("dangerColor") && lSettingsJSON["dangerColor"].isString())
  {
    lSettings.dangerColor = lSettingsJSON["dangerColor"].toString();
  }

  // Batch Conversion: Default body name and version
  if (lSettingsJSON.contains("defaultBatchConversionBody") && lSettingsJSON["defaultBatchConversionBody"].isDouble())
  {
    auto lFoundBody{lSettingsJSON["defaultMainWindowBody"].toInt()};
    lSettings.defaultBatchConversionBody = static_cast<BodyNameVersion>(lFoundBody);
  }

  // Preset Creator: Default feet mod
  if (lSettingsJSON.contains("defaultMainFeetMod") && lSettingsJSON["defaultMainFeetMod"].isDouble())
  {
    lSettings.defaultMainFeetMod = lSettingsJSON["defaultMainFeetMod"].toInt();
  }

  // Preset Creator: Default body name and version
  if (lSettingsJSON.contains("defaultMainWindowBody") && lSettingsJSON["defaultMainWindowBody"].isDouble())
  {
    auto lFoundBody{lSettingsJSON["defaultMainWindowBody"].toInt()};
    lSettings.defaultMainWindowBody = static_cast<BodyNameVersion>(lFoundBody);
  }

  // Default BodySlide Presets' Retargeting body name and version
  if (lSettingsJSON.contains("defaultRetargetingToolBody") && lSettingsJSON["defaultRetargetingToolBody"].isDouble())
  {
    auto lFoundBody{lSettingsJSON["defaultRetargetingToolBody"].toInt()};
    lSettings.defaultRetargetingToolBody = static_cast<BodyNameVersion>(lFoundBody);
  }

  // Retargeting Tool: Default feet mod
  if (lSettingsJSON.contains("defaultRetargetingToolFeetMod") && lSettingsJSON["defaultRetargetingToolFeetMod"].isDouble())
  {
    lSettings.defaultRetargetingToolFeetMod = lSettingsJSON["defaultRetargetingToolFeetMod"].toInt();
  }

  // Each button stores the last opened path
  if (lSettingsJSON.contains("eachButtonSavesItsLastUsedPath") && lSettingsJSON["eachButtonSavesItsLastUsedPath"].isBool())
  {
    lSettings.eachButtonSavesItsLastUsedPath = lSettingsJSON["eachButtonSavesItsLastUsedPath"].toBool();
  }

  // Language
  if (lSettingsJSON.contains("language") && lSettingsJSON["language"].isDouble())
  {
    auto lFoundLanguage{lSettingsJSON["language"].toInt()};
    lSettings.language = static_cast<ApplicationLanguage>(lFoundLanguage);
  }

  // Main window auto open generated dir
  if (lSettingsJSON.contains("mainWindowAutomaticallyOpenGeneratedDirectory") && lSettingsJSON["mainWindowAutomaticallyOpenGeneratedDirectory"].isBool())
  {
    lSettings.mainWindowAutomaticallyOpenGeneratedDirectory = lSettingsJSON["mainWindowAutomaticallyOpenGeneratedDirectory"].toBool();
  }

  // Default window height
  if (lSettingsJSON.contains("mainWindowHeight") && lSettingsJSON["mainWindowHeight"].isDouble())
  {
    lSettings.mainWindowHeight = lSettingsJSON["mainWindowHeight"].toInt();
  }

  // Main window opening mode
  if (lSettingsJSON.contains("mainWindowOpeningMode") && lSettingsJSON["mainWindowOpeningMode"].isDouble())
  {
    auto lFoundWindowOpeningMode{lSettingsJSON["mainWindowOpeningMode"].toInt()};
    lSettings.mainWindowOpeningMode = static_cast<WindowOpeningMode>(lFoundWindowOpeningMode);
  }

  // Main window output path
  if (lSettingsJSON.contains("mainWindowOutputPath") && lSettingsJSON["mainWindowOutputPath"].isString())
  {
    lSettings.mainWindowOutputPath = lSettingsJSON["mainWindowOutputPath"].toString();
  }

  // Default window width
  if (lSettingsJSON.contains("mainWindowWidth") && lSettingsJSON["mainWindowWidth"].isDouble())
  {
    lSettings.mainWindowWidth = lSettingsJSON["mainWindowWidth"].toInt();
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

  // JSON format upgrade from v2.10.2.0 to 3.0.0.0
  if (!lSettingsJSON.contains("applicationVersion"))
  {
    // "windowWidth" -> "mainWindowWidth"
    if (lSettingsJSON.contains("windowWidth") && lSettingsJSON["windowWidth"].isDouble())
    {
      lSettings.mainWindowWidth = lSettingsJSON["windowWidth"].toInt();
    }

    // "windowHeight" -> "mainWindowHeight"
    if (lSettingsJSON.contains("windowHeight") && lSettingsJSON["windowHeight"].isDouble())
    {
      lSettings.mainWindowHeight = lSettingsJSON["windowHeight"].toInt();
    }

    // "retargetingToolDefaultBody" -> "defaultRetargetingToolBody"
    if (lSettingsJSON.contains("retargetingToolDefaultBody") && lSettingsJSON["retargetingToolDefaultBody"].isDouble())
    {
      auto lFoundBody{lSettingsJSON["retargetingToolDefaultBody"].toInt()};
      lSettings.defaultRetargetingToolBody = static_cast<BodyNameVersion>(lFoundBody);
    }

    // "defaultBody" -> "defaultMainWindowBody"
    if (lSettingsJSON.contains("defaultBody") && lSettingsJSON["defaultBody"].isDouble())
    {
      auto lFoundBody{lSettingsJSON["defaultBody"].toInt()};
      lSettings.defaultMainWindowBody = static_cast<BodyNameVersion>(lFoundBody);
    }

    Utils::saveSettingsToFile(lSettings);
    Utils::printMessageStdOut("User settings upgraded!");
  }
  // For any JSON format upgrade / settings values changed
  else
  {
    if (Utils::compareVersionNumbers(lSettingsJSON["applicationVersion"].toString(), "3.2.0.0") == ApplicationVersionRelative::OLDER)
    {
      lSettings.defaultMainWindowBody = BodyNameVersion::CBBE_3BBB_3BA_1_50;
      lSettings.defaultRetargetingToolBody = BodyNameVersion::CBBE_3BBB_3BA_1_50;
      lSettings.defaultBatchConversionBody = BodyNameVersion::CBBE_3BBB_3BA_1_50;
      lSettings.appTheme = GUITheme::MITSURIOU_DARK_THEME;

      Utils::saveSettingsToFile(lSettings);
      Utils::printMessageStdOut("User settings upgraded!");
    }
  }

  Utils::printMessageStdOut("User settings:");
  Utils::printMessageStdOut(QJsonDocument(Utils::settingsStructToJson(lSettings)).toJson(QJsonDocument::JsonFormat::Indented));

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
  lFontObj["italic"] = aSettings.font.italic;
  lFontObj["size"] = aSettings.font.size;
  lFontObj["strikeOut"] = aSettings.font.strikeOut;
  lFontObj["styleName"] = aSettings.font.styleName;
  lFontObj["underline"] = aSettings.font.underline;
  lFontObj["weight"] = aSettings.font.weight;

  // Construct the full settings object
  QJsonObject lSettings;
  lSettings["appTheme"] = static_cast<int>(aSettings.appTheme);
  lSettings["assistedConversionScanOnlyMeshesSubdir"] = aSettings.assistedConversionScanOnlyMeshesSubdir;
  lSettings["batchConversionOutputPath"] = aSettings.batchConversionOutputPath;
  lSettings["checkForUpdatesAtStartup"] = aSettings.checkForUpdatesAtStartup;
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
  lSettings["successColor"] = aSettings.successColor;
  lSettings["warningColor"] = aSettings.warningColor;

  // Version number string
  lSettings["applicationVersion"] = Utils::getApplicationVersion();

  return lSettings;
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
    lFiltersList.insert(std::make_pair(lKey, lVariantMap.value(lKey, "").toStringList()));
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

QString Utils::getAdditionalFeetFilter(const BodyNameVersion& aBody, const int aFeetModIndex)
{
  if (aFeetModIndex == 1 || aFeetModIndex == 2)
  {
    if (isCBBEBasedBody(aBody))
    {
      return QString("MSF CBBE Feet");
    }

    return QString("MSF BHUNP Feet");
  }

  return QString("");
}

std::vector<Struct::Filter> Utils::getFiltersForExport(const std::map<QString, QStringList>& aList, const QString& aKey, const BodyNameVersion& aBody, const int aFeetModIndex)
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
      lExportFilters.push_back(Struct::Filter(Utils::getAdditionalFeetFilter(aBody, aFeetModIndex), false, true, false));

      break;
    }
  }

  return lExportFilters;
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
      lMap.insert(std::make_pair(lKey, QString()));
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

QString Utils::getPathFromKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aFallbackPath, const bool aUseKeyPath)
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

void Utils::updatePathAtKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aPath, const bool aAuthorizeEmptyValue, const bool aMustSaveFile)
{
  if (aKey.length() == 0 || (!aAuthorizeEmptyValue && aPath.length() == 0))
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

  if (aMustSaveFile)
  {
    // Save the new list
    Utils::saveLastPathsToFile(*aMap);
  }
}

QString Utils::getShortLanguageNameFromEnum(const int aEnumValue)
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

QString Utils::getLongLanguageNameFromEnum(const int aEnumValue)
{
  auto lEnumLang{static_cast<ApplicationLanguage>(aEnumValue)};
  switch (lEnumLang)
  {
    case ApplicationLanguage::ENGLISH:
      return "English";
    case ApplicationLanguage::FRENCH:
      return "Français";
    default:
      return "English";
  }
}

ApplicationLanguage Utils::getStructLanguageFromName(const QString& aShortName)
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

void Utils::addLastPathLine(QWidget* aParent, QGridLayout* aLayout, const int aRow, const QString& aLabel, const QString& aValue, const QString& aIconFolder, const QString& aIconName)
{
  aLayout->addWidget(new QLabel(aLabel, aParent), aRow, 0);

  auto lGeneralValue{new QLineEdit(aValue, aParent)};
  lGeneralValue->setReadOnly(true);
  lGeneralValue->setCursor(Qt::CursorShape::IBeamCursor);
  lGeneralValue->setObjectName(QString("line_edit_path_%1").arg(aRow));
  aLayout->addWidget(lGeneralValue, aRow, 1);

  auto lGeneralEmptyButton{ComponentFactory::createButton(aParent, tr("Remove from history"), "", aIconName, aIconFolder, QString("clear_path_%1").arg(aRow), false, true)};
  aLayout->addWidget(lGeneralEmptyButton, aRow, 2);
}

void Utils::selectComboBoxAt(QComboBox* aComboBox, const int aIndex)
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

void Utils::setGroupBoxState(QGroupBox* aGroupBox, const bool aIsCollapsed)
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

void Utils::updateComboBoxBodyslideFiltersList(const std::map<QString, QStringList>& aFilterList, QComboBox* aComboBox, QLabel* aFiltersLabel)
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

void Utils::updateOutputPreview(QLineEdit* aMainDirTextEdit, const QString& aSubDirectory, const bool aUseOnlySubdir, const QString& aSuccessColor, const QString& aWarningColor, const QString& aDangerColor, QLabel* aOutputPathsPreview)
{
  auto lMainDirectory{aMainDirTextEdit->text().trimmed()};
  Utils::cleanPathString(lMainDirectory);
  Utils::cleanPathString(aSubDirectory);
  auto lIsValidPath{true};

  // Construct full path
  auto lFullPath{QString()};
  if (aUseOnlySubdir)
  {
    aMainDirTextEdit->setDisabled(true);

    if (aSubDirectory.length() > 0)
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
    if (lMainDirectory.length() > 0 && aSubDirectory.length() > 0)
    {
      lFullPath = lMainDirectory + "/" + aSubDirectory;
      aMainDirTextEdit->setDisabled(false);
    }
    else if (lMainDirectory.length() > 0 && aSubDirectory.length() == 0)
    {
      lFullPath = lMainDirectory;
      aMainDirTextEdit->setDisabled(false);
    }
    else if (lMainDirectory.length() == 0 && aSubDirectory.length() > 0)
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
  else if (aMessage.endsWith(".") || aMessage.endsWith(":") || aMessage.endsWith("!") || aMessage.endsWith("?") || aMessage.endsWith("\n"))
  {
    std::cout << aMessage.toStdString() << std::endl;
  }
  else
  {
    std::cout << aMessage.toStdString() << "." << std::endl;
  }
}
