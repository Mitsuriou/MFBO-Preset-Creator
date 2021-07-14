#pragma once
#include "Struct.h"
#include <QString>
#include <vector>

class SliderFileBuilder final
{
public:
  // SliderSets
  static QString buildOSPFileContent(const QString& aLineName, const BodyNameVersion& aBody, const bool aMustUseBeastHands, const int aFeetModIndex, const unsigned char& aTargetBlocks = 111);
  static QString getFeetBlock(const QString& aLineName, const BodyNameVersion& aBody, const int aFeetModIndex);

  // SliderGroups
  static QString buildXMLFileContent(const QString& aLineName, const std::vector<Struct::Filter>& aFiltersList, const BodyNameVersion& aBody, const bool aMustUseBeastHands, const int aFeetModIndex, const unsigned char& aTargetBlocks = 111);
  static QString getFeetLineForBHUNP(const int aFeetModIndex);
  static std::vector<Struct::Filter> getXMLDefaultFiltersFromBody(const BodyNameVersion& aBody);

private:
  explicit SliderFileBuilder(){};
};
