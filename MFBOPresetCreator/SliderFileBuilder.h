#pragma once

#include "Struct.h"
#include <QString>
#include <QStringList>

class SliderFileBuilder
{
public:
  // SliderSets
  static QString buildOSPFileContent(const BodyNameVersion& aBody, const bool& aMustUseBeastHands, const int& aFeetModIndex);
  static QString getHandsBlock(const BodyNameVersion& aBody, const bool& aMustUseBeastHands);
  static QString getFeetBlock(const BodyNameVersion& aBody, const int& aFeetModIndex);
  static QString getBodyBlock(const BodyNameVersion& aBody);
  // SliderGroups
  static QString buildXMLFileContent(const QString& aLineName, const QStringList& aFiltersList, const BodyNameVersion& aBody, const bool& aMustUseBeastHands, const int& aFeetModIndex);
  static QString getFeetLineForBHUNP(const int& aFeetModIndex);
  static QStringList getXMLDefaultFiltersFromBody(const BodyNameVersion& aBody);

private:
  explicit SliderFileBuilder(){};
};
