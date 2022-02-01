#pragma once
#include "Enum.h"
#include <QObject>
#include <QStringList>

#if defined(_MSC_VER) && !defined(DLLEXP)
#define DLLEXP __declspec(dllexport)
#elif !defined(DLLEXP)
#define DLLEXP
#endif

class DataLists final : public QObject
{
  Q_OBJECT

public:
  // Lists of body related stuff
  static QStringList GetBodyNamesList();
  static QStringList GetBodyVersionsList(const BodyName& aBodyName);
  static QStringList GetBodyVariantsList(const BodyName& aBodyName, const int aVersionNumber);
  static QString GetQRCResourceName(const BodyVariant& aBodyVariant);

  // Lists of feet related stuff
  static QStringList GetFeetNamesList(const BodyVariant& aBodyVariant);
  static QStringList GetFeetVersionsList(const FeetName& aFeetName, const bool aIsCBBEBody);
  static QStringList GetFeetVariantsList(const FeetName& aFeetName, const int aRelativeVersion, const bool aIsCBBEBody);
  static QString GetQRCResourceName(const BodyVariant& aBodyVariant, const FeetNameVersion& aFeetNameVersion);

  // Body lists and enum values type conversions
  static DLLEXP BodyName GetName(const BodyVariant& aBodyVariant);
  static DLLEXP FeetName GetName(const FeetVariant& aFeetVariant);

  static DLLEXP BodyName GetName(const BodyNameVersion& aBodyNameVersion);
  static DLLEXP FeetName GetName(const FeetNameVersion& aFeetNameVersion);
  static DLLEXP FeetName GetName(const BodyVariant& aBodyVariant, const int aRelativeFeetName);

  static DLLEXP BodyVariant GetVariant(const BodyNameVersion& aBodyNameVersion);
  static DLLEXP BodyVariant GetVariant(const BodyName& aBodyName, const int aRelativeVersion, const int aRelativeVariant);
  static DLLEXP FeetVariant GetVariant(const FeetNameVersion& aFeetNameVersion);
  static DLLEXP FeetVariant GetVariant(const FeetName& aFeetName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);

  static DLLEXP BodyNameVersion GetBodyNameVersion(const BodyVariant& aBodyVariant, const int aRelativeVersion);
  static DLLEXP BodyNameVersion GetBodyNameVersion(const BodyName& aBodyName, const int aRelativeVersion, const int aRelativeVariant);
  static DLLEXP FeetNameVersion GetFeetNameVersion(const FeetVariant& aFeetVariant, const int aRelativeVersion, const bool aIsCBBEBody);
  static DLLEXP FeetNameVersion GetFeetNameVersion(const FeetName& aFeetName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody);

  static DLLEXP int GetVersionIndex(const BodyNameVersion& aBodyNameVersion);
  static DLLEXP int GetVariantIndex(const BodyNameVersion& aBodyNameVersion);
  static DLLEXP int GetNameIndex(const BodyVariant& aBodyVariant, const FeetNameVersion& aFeetNameVersion);
  static DLLEXP int GetVersionIndex(const FeetNameVersion& aFeetNameVersion);
  static DLLEXP int GetVariantIndex(const FeetNameVersion& aFeetNameVersion);

  static QString GetVersionString(const BodyNameVersion& aBodyNameVersion);
  static QString GetVersionString(const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion);

  // Other operations with lists and enum values
  static DLLEXP BodyNameVersion GetFirstKey(const BodyVariant& aBodyVariant);
  static DLLEXP FeetNameVersion GetFirstKey(const FeetVariant& aFeetVariant);
  static DLLEXP int GetVersionOffset(const BodyNameVersion& aBodyNameVersion);
  static DLLEXP int GetVersionOffset(const BodyVariant& aBodyVariant, const int aRelativeVersion);
  static DLLEXP int GetVersionOffset(const FeetVariant& aFeetVariant);

  static QString GetQRCPathForResource(const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion, const BodyPartType& aResourceType);

  // Pre 3.5.x.x data to 3.5.x.x+ data format
  static DLLEXP std::pair<int, int> GetSplittedNameVersionFromBodyVersionCompatibility(const int aBodyVersion);
  static DLLEXP std::pair<BodyNameVersion, FeetNameVersion> ReadBodyFeetModsCompatibility(const int aBodyName, const int aBodyVersion, const int aFeetIndex);

  //
  static QStringList GetKnownTexturesFilesNames();

  // Slider files data
  static QString GetBodySliderValue(const BodyNameVersion& aBodyNameVersion);
  static QString GetFeetSliderValue(const FeetNameVersion& aFeetNameVersion);
  static QString GetHandsSliderValue(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands);

  // Other data lists
  static QStringList GetWindowOpeningModes();
  static QStringList GetDialogOpeningModes();
  static QStringList GetAppThemes();
  static QStringList GetLanguages();
  static QStringList GetAssistedConversionActions();
  static QStringList GetLastPathsKeys();

private:
  explicit DataLists() {}
  explicit DataLists(const DataLists&) = delete;
  DataLists& operator=(const DataLists&) = delete;
};
