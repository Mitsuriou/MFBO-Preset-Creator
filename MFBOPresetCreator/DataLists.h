#pragma once
#include "Enum.h"
#include <QObject>
#include <QStringList>

#define DLLEXP __declspec(dllexport)

class DataLists final : public QObject
{
  Q_OBJECT

public:
  // Lists for bodies names and versions
  static DLLEXP QStringList GetBodiesNames();
  static DLLEXP QStringList GetVersionsFromBodyName(const BodyName& aBody);
  static DLLEXP BodyNameVersion GetBodyNameVersion(const BodyName& aBody, const int aRelativeVersion);
  static DLLEXP std::pair<int, int> GetSplittedNameVersionFromBodyVersion(BodyNameVersion aBodyVersion);
  static DLLEXP QString GetQRCPathFromBodyName(const BodyNameVersion& aBody, const int aFeetModIndex, const BodyPartType& aRessourceType);
  static DLLEXP QStringList GetFeetModsFromBodyName(const BodyName& aBody);

  // Other data lists
  static DLLEXP QStringList GetWindowOpeningModes();
  static DLLEXP QStringList GetDialogOpeningModes();
  static DLLEXP QStringList GetAppThemes();
  static DLLEXP QStringList GetLanguages();
  static DLLEXP QStringList GetAssistedConversionActions();
  static DLLEXP QStringList GetLastPathsKeys();

private:
  explicit DataLists(){};
  explicit DataLists(const DataLists&) = delete;
  DataLists& operator=(const DataLists&) = delete;
};
