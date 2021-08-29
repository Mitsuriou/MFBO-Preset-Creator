#pragma once
#include "Struct.h"
#include <QString>
#include <vector>

class SliderFileBuilder final
{
public:
  // SliderSets
  static QString BuildOSPFileContent(const QString& aLineName, const BodyNameVersion& aBody, const bool aMustUseBeastHands, const int aFeetModIndex, const unsigned char& aTargetBlocks = 111);

  // SliderGroups
  static QString BuildXMLFileContent(const QString& aLineName, const std::vector<Struct::Filter>& aFiltersList, const BodyNameVersion& aBody, const bool aMustUseBeastHands, const int aFeetModIndex, const unsigned char& aTargetBlocks = 111);
  static QString GetFeetLineForBHUNP(const int aFeetModIndex);
  static std::vector<Struct::Filter> GetXMLDefaultFiltersFromBody(const BodyNameVersion& aBody);

private:
  explicit SliderFileBuilder(){};
};
