#pragma once

#include "Struct.h"

#include <QString>
#include <QMessageBox>
#include <QDirIterator>
#include <QStringList>
#include <QFile>
#include <QPair>
#include <QtXml/QDomDocument>

class Utils : public QObject
{
  Q_OBJECT

public:
  static void cleanPathString(QString& aPath);
  static QString getProgramVersion();
  static void displayWarningMessage(QString aMessage);
  static int getNumberFilesByExtension(QString aRootDir, QString aFileExtension);
  static bool copyRecursively(QString sourceFolder, QString destFolder);
  static QString getPresetNameFromXMLFile(QString aPath);
  static std::vector<Struct::SliderSet> getOutputPathsFromOSPFile(QString aPath);
  static bool isPresetUsingBeastHands(QString aPath);
};
