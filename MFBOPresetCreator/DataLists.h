#pragma once
#include "Enum.h"
#include <QObject>
#include <QStringList>

#if defined(_MSC_VER) && !defined(DLLEXP)
#define DLLEXP __declspec(dllexport)
#elif !defined(DLLEXP)
#define DLLEXP
#endif

namespace DataLists
{
  // Lists of body related stuff
  QStringList GetBodyNamesList();
  QStringList GetBodyVersionsList(const BodyName& aBodyName);
  QStringList GetBodyVariantsList(const BodyName& aBodyName, const int aVersionNumber);
  QString GetQRCResourceName(const BodyVariant& aBodyVariant);

  // Lists of feet related stuff
  QStringList GetFeetNamesList(const BodyVariant& aBodyVariant);
  QStringList GetFeetVersionsList(const FeetName& aFeetName, const bool aIsCBBEBody);
  QStringList GetFeetVariantsList(const FeetName& aFeetName, const int aRelativeVersion, const bool aIsCBBEBody);
  QString GetQRCResourceName(const BodyVariant& aBodyVariant, const FeetNameVersion& aFeetNameVersion);

  // Body lists and enum values type conversions
  DLLEXP BodyName GetName(const BodyVariant& aBodyVariant);
  DLLEXP FeetName GetName(const FeetVariant& aFeetVariant);

  DLLEXP BodyName GetName(const BodyNameVersion& aBodyNameVersion);
  DLLEXP FeetName GetName(const FeetNameVersion& aFeetNameVersion);
  DLLEXP FeetName GetName(const BodyVariant& aBodyVariant, const int aRelativeFeetName);

  DLLEXP BodyVariant GetVariant(const BodyNameVersion& aBodyNameVersion);
  DLLEXP BodyVariant GetVariant(const BodyName& aBodyName, const int aRelativeVersion, const int aRelativeVariant);
  DLLEXP FeetVariant GetVariant(const FeetNameVersion& aFeetNameVersion);
  DLLEXP FeetVariant GetVariant(const FeetName& aFeetName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);

  DLLEXP BodyNameVersion GetBodyNameVersion(const BodyVariant& aBodyVariant, const int aRelativeVersion);
  DLLEXP BodyNameVersion GetBodyNameVersion(const BodyName& aBodyName, const int aRelativeVersion, const int aRelativeVariant);
  DLLEXP FeetNameVersion GetFeetNameVersion(const FeetVariant& aFeetVariant, const int aRelativeVersion, const bool aIsCBBEBody);
  DLLEXP FeetNameVersion GetFeetNameVersion(const FeetName& aFeetName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);

  DLLEXP int GetVersionIndex(const BodyNameVersion& aBodyNameVersion);
  DLLEXP int GetVariantIndex(const BodyNameVersion& aBodyNameVersion);
  DLLEXP int GetNameIndex(const BodyVariant& aBodyVariant, const FeetNameVersion& aFeetNameVersion);
  DLLEXP int GetVersionIndex(const FeetNameVersion& aFeetNameVersion);
  DLLEXP int GetVariantIndex(const FeetNameVersion& aFeetNameVersion);

  QString GetVersionString(const BodyNameVersion& aBodyNameVersion);
  QString GetVersionString(const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion);

  // Other operations with lists and enum values
  DLLEXP BodyNameVersion GetFirstKey(const BodyVariant& aBodyVariant);
  DLLEXP FeetNameVersion GetFirstKey(const FeetVariant& aFeetVariant);
  DLLEXP int GetVersionOffset(const BodyNameVersion& aBodyNameVersion);
  DLLEXP int GetVersionOffset(const BodyVariant& aBodyVariant, const int aRelativeVersion);
  DLLEXP int GetVersionOffset(const FeetVariant& aFeetVariant);

  DLLEXP QString GetQRCPathForResource(const BodyNameVersion aBodyNameVersion, const FeetNameVersion aFeetNameVersion, const MeshPartType aResourceType);

  // Pre 3.5.x.x data to 3.5.x.x+ data format
  DLLEXP std::pair<int, int> GetSplittedNameVersionFromBodyVersionCompatibility(const int aBodyVersion);
  DLLEXP std::pair<BodyNameVersion, FeetNameVersion> ReadBodyFeetModsCompatibility(const int aBodyName, const int aBodyVersion, const int aFeetIndex);

  //
  QStringList GetKnownTexturesFilesNames();

  // Slider files data
  QString GetBodySliderValue(const BodyNameVersion& aBodyNameVersion);
  QString GetFeetSliderValue(const FeetNameVersion& aFeetNameVersion);
  QString GetHandsSliderValue(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands);

  // Other data lists
  QStringList GetWindowOpeningModes();
  QStringList GetDialogOpeningModes();
  QStringList GetAppThemes();
  QStringList GetLanguages();
  QStringList GetAssistedConversionActions();
  QStringList GetSliderSetsImporterActions();
  QStringList GetLastPathsKeys();
};
