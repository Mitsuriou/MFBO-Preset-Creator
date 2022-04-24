#pragma once
#include "Struct.h"
#include <QString>
#include <vector>

namespace SliderFileBuilder
{
  // SliderSets
  QString BuildOSPFileContent(const QString& aLineName,
                              const BodyNameVersion& aBody,
                              const FeetNameVersion& aFeet,
                              const bool aMustUseBeastHands,
                              const unsigned char& aTargetBlocks);

  // SliderGroups
  QString BuildXMLFileContent(const QString& aLineName,
                              const std::vector<Struct::Filter>& aFiltersList,
                              const BodyNameVersion& aBodyNameVersion,
                              const FeetNameVersion& aFeetNameVersion,
                              const bool aMustUseBeastHands,
                              const unsigned char& aTargetBlocks);
  QString GetBodyLine(const BodyNameVersion& aBodyNameVersion);
  QString GetFeetLine(const FeetNameVersion& aFeetNameVersion);
  QString GetHandsLine(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands);
  std::vector<Struct::Filter> GetXMLDefaultFiltersFromBody(const BodyNameVersion& aBody);
};
