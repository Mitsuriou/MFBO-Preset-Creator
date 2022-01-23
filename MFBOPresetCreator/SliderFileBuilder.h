#pragma once
#include "Struct.h"
#include <QString>
#include <vector>

class SliderFileBuilder final
{
public:
  // SliderSets
  static QString BuildOSPFileContent(const QString& aLineName, const BodyNameVersion& aBody, const FeetNameVersion& aFeet, const bool aMustUseBeastHands, const unsigned char& aTargetBlocks);

  // SliderGroups
  static QString BuildXMLFileContent(const QString& aLineName,
                                     const std::vector<Struct::Filter>& aFiltersList,
                                     const BodyNameVersion& aBodyNameVersion,
                                     const FeetNameVersion& aFeetNameVersion,
                                     const bool aMustUseBeastHands,
                                     const unsigned char& aTargetBlocks);
  static QString GetBodyLine(const BodyNameVersion& aBodyNameVersion);
  static QString GetFeetLine(const FeetNameVersion& aFeetNameVersion);
  static QString GetHandsLine(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands);
  static std::vector<Struct::Filter> GetXMLDefaultFiltersFromBody(const BodyNameVersion& aBody);

private:
  explicit SliderFileBuilder() {}
};
