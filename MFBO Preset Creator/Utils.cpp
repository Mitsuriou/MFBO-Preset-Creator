#include "Utils.h"

void Utils::cleanPathString(QString& aPath)
{
  aPath.replace("\\", "/");
}

QString Utils::getProgramVersion()
{
  return QString("1.5.0.0");
}

void Utils::displayWarningMessage(QString aMessage)
{
  QMessageBox lMessageBox(QMessageBox::Icon::Warning, tr("Warning"), aMessage);
  lMessageBox.exec();
}

int Utils::getNumberFilesByExtension(QString aRootDir, QString aFileExtension)
{
  auto lNumber{ 0 };

  QDirIterator it(aRootDir, QStringList() << QString("*." + aFileExtension), QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
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
