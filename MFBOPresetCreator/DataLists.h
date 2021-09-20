#pragma once
#include "Enum.h"
#include <QObject>
#include <QStringList>

class DataLists final : public QObject
{
  Q_OBJECT

public:
  // Lists for bodies names and versions
  static QStringList GetBodiesNames();
  static QStringList GetVersionsFromBodyName(const BodyName& aBody);
  static BodyNameVersion GetBodyNameVersion(const BodyName& aBody, const int aRelativeVersion);
  static std::pair<int, int> GetSplittedNameVersionFromBodyVersion(BodyNameVersion aBodyVersion);
  static QString GetQRCPathFromBodyName(const BodyNameVersion& aBody, const int aFeetModIndex, const BodyPartType& aRessourceType);
  static QStringList GetFeetModsFromBodyName(const BodyName& aBody);

  // Other data lists
  static QStringList GetWindowOpeningModes();
  static QStringList GetDialogOpeningModes();
  static QStringList GetAppThemes();
  static QStringList GetLanguages();
  static QStringList GetAssistedConversionActions();
  static QStringList GetLastPathsKeys();

private:
  explicit DataLists(){};
  explicit DataLists(const DataLists&) = delete;
  DataLists& operator=(const DataLists&) = delete;
};
