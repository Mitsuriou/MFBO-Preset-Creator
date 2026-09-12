#pragma once
#include "Enum.h"
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

  // Lists of hands related stuff

  QStringList GetHandsNamesList(const BodyVariant& aBodyVariant);
  QStringList GetHandsVersionsList(const HandsName& aHandsName, const bool aIsCBBEBody);
  QStringList GetHandsVariantsList(const HandsName& aHandsName, const int aRelativeVersion, const bool aIsCBBEBody);
  QString GetQRCResourceName(const BodyVariant& aBodyVariant, const HandsNameVersion& aHandsNameVersion);

  // Lists of beast hands related stuff

  QStringList GetBeastHandsNamesList(const BodyVariant& aBodyVariant);
  QStringList GetBeastHandsVersionsList(const BeastHandsName& aBeastHandsName, const bool aIsCBBEBody);
  QStringList GetBeastHandsVariantsList(const BeastHandsName& aBeastHandsName, const int aRelativeVersion, const bool aIsCBBEBody);
  QString GetQRCResourceName(const BodyVariant& aBodyVariant, const BeastHandsNameVersion& aBeastHandsNameVersion);

  // Name from variant

  DLLEXP BodyName GetName(const BodyVariant& aBodyVariant);
  DLLEXP FeetName GetName(const FeetVariant& aFeetVariant);
  DLLEXP HandsName GetName(const HandsVariant& aHandsVariant);
  DLLEXP BeastHandsName GetName(const BeastHandsVariant& aBeastHandsVariant);

  // Name from NameVersion or from variant and version index

  DLLEXP BodyName GetName(const BodyNameVersion& aBodyNameVersion);
  DLLEXP FeetName GetName(const FeetNameVersion& aFeetNameVersion);
  DLLEXP FeetName GetNameFeet(const BodyVariant& aBodyVariant, const int aRelativeFeetName);
  DLLEXP HandsName GetName(const HandsNameVersion& aHandsNameVersion);
  DLLEXP HandsName GetNameHands(const BodyVariant& aBodyVariant, const int aRelativeHandsName);
  DLLEXP BeastHandsName GetName(const BeastHandsNameVersion& aBeastHandsNameVersion);
  DLLEXP BeastHandsName GetNameBeastHands(const BodyVariant& aBodyVariant, const int aRelativeBeastHandsName);

  // Variant from NameVersion or from name, version index and variant index

  DLLEXP BodyVariant GetVariant(const BodyNameVersion& aBodyNameVersion);
  DLLEXP BodyVariant GetVariant(const BodyName& aBodyName, const int aRelativeVersion, const int aRelativeVariant);
  DLLEXP FeetVariant GetVariant(const FeetNameVersion& aFeetNameVersion);
  DLLEXP FeetVariant GetVariant(const FeetName& aFeetName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);
  DLLEXP HandsVariant GetVariant(const HandsNameVersion& aHandsNameVersion);
  DLLEXP HandsVariant GetVariant(const HandsName& aHandsName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);
  DLLEXP BeastHandsVariant GetVariant(const BeastHandsNameVersion& aBeastHandsNameVersion);
  DLLEXP BeastHandsVariant GetVariant(const BeastHandsName& aBeastHandsName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);

  // NameVersion from variant and version index or from name, version index and variant index

  DLLEXP BodyNameVersion GetNameVersion(const BodyVariant& aBodyVariant, const int aRelativeVersion);
  DLLEXP BodyNameVersion GetNameVersion(const BodyName& aBodyName, const int aRelativeVersion, const int aRelativeVariant);
  DLLEXP FeetNameVersion GetNameVersion(const FeetVariant& aFeetVariant, const int aRelativeVersion, const bool aIsCBBEBody);
  DLLEXP FeetNameVersion GetNameVersion(const FeetName& aFeetName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);
  DLLEXP HandsNameVersion GetNameVersion(const HandsVariant& aHandsVariant, const int aRelativeVersion, const bool aIsCBBEBody);
  DLLEXP HandsNameVersion GetNameVersion(const HandsName& aHandsName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);
  DLLEXP BeastHandsNameVersion GetNameVersion(const BeastHandsVariant& aBeastHandsVariant, const int aRelativeVersion, const bool aIsCBBEBody);
  DLLEXP BeastHandsNameVersion GetNameVersion(const BeastHandsName& aBeastHandsName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);

  //

  DLLEXP int GetVersionIndex(const BodyNameVersion& aBodyNameVersion);
  DLLEXP int GetVariantIndex(const BodyNameVersion& aBodyNameVersion);

  DLLEXP int GetNameIndex(const BodyVariant& aBodyVariant, const FeetNameVersion& aFeetNameVersion);
  DLLEXP int GetVersionIndex(const FeetNameVersion& aFeetNameVersion);
  DLLEXP int GetVariantIndex(const FeetNameVersion& aFeetNameVersion);

  DLLEXP int GetNameIndex(const BodyVariant& aBodyVariant, const HandsNameVersion& aHandsNameVersion);
  DLLEXP int GetVersionIndex(const HandsNameVersion& aHandsNameVersion);
  DLLEXP int GetVariantIndex(const HandsNameVersion& aHandsNameVersion);

  DLLEXP int GetNameIndex(const BodyVariant& aBodyVariant, const BeastHandsNameVersion& aBeastHandsNameVersion);
  DLLEXP int GetVersionIndex(const BeastHandsNameVersion& aBeastHandsNameVersion);
  DLLEXP int GetVariantIndex(const BeastHandsNameVersion& aBeastHandsNameVersion);

  // Version string

  QString GetVersionString(const BodyNameVersion& aBodyNameVersion);
  QString GetVersionString(const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion);
  QString GetVersionString(const BodyNameVersion& aBodyNameVersion, const HandsNameVersion& aHandsNameVersion);
  QString GetVersionString(const BodyNameVersion& aBodyNameVersion, const BeastHandsNameVersion& aBeastHandsNameVersion);

  // Other operations with lists and enum values
  DLLEXP BodyNameVersion GetFirstKey(const BodyVariant& aBodyVariant);
  DLLEXP FeetNameVersion GetFirstKey(const FeetVariant& aFeetVariant);
  DLLEXP int GetVersionOffset(const BodyNameVersion& aBodyNameVersion);
  DLLEXP int GetVersionOffset(const BodyVariant& aBodyVariant, const int aRelativeVersion);
  DLLEXP int GetVersionOffset(const FeetVariant& aFeetVariant);
  DLLEXP int GetVersionOffset(const HandsVariant& aHandsVariant);
  DLLEXP int GetVersionOffset(const BeastHandsVariant& aBeastHandsVariant);

  DLLEXP QString GetQRCPathForResource(const BodyNameVersion aBodyNameVersion,
                                       const FeetNameVersion aFeetNameVersion,
                                       // const HandsNameVersion aHandsNameVersion,
                                       const MeshPartType aResourceType);

  // Pre 3.5.x.x data to 3.5.x.x+ data format
  DLLEXP std::pair<int, int> GetSplitNameVersionFromBodyVersionCompatibility(const int aBodyVersion);
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
