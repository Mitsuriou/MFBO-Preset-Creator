#pragma once
#include "Enum.h"
#include <QObject>
#include <QStringList>

class DataLists final : public QObject
{
  Q_OBJECT

public:
  // Lists for bodies names and versions
  static QStringList getBodiesNames();
  static QStringList getVersionsFromBodyName(const BodyName& aBody);
  static BodyNameVersion getBodyNameVersion(const BodyName& aBody, const int aRelativeVersion);
  static std::pair<int, int> getSplittedNameVersionFromBodyVersion(BodyNameVersion aBodyVersion);
  static QString getQRCPathFromBodyName(const BodyNameVersion& aBody, const QString& aRessourceType);
  static QStringList getFeetModsEntries();

  // Other data lists
  static QStringList getWindowOpeningModes();
  static QStringList getAppThemes();
  static QStringList getLanguages();
  static QStringList getAssistedConversionActions();
  static QStringList getLastPathsKeys();

private:
  explicit DataLists(){};
};
