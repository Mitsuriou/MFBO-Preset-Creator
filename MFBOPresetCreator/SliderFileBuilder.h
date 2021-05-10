#pragma once

#include "Struct.h"
#include <QString>
#include <QStringList>

class SliderFileBuilder
{
public:
  // SliderSets
  static QString buildOSPFileContent(const QString& aLineName, const BodyNameVersion& aBody, const bool& aMustUseBeastHands, const int& aFeetModIndex);
  static QString getHandsBlock(const QString& aLineName, const BodyNameVersion& aBody, const bool& aMustUseBeastHands);
  static QString getFeetBlock(const QString& aLineName, const BodyNameVersion& aBody, const int& aFeetModIndex);
  static QString getBodyBlock(const QString& aLineName, const BodyNameVersion& aBody);

  // SliderGroups
  static QString buildXMLFileContent(const QString& aLineName, const QStringList& aFiltersList, const BodyNameVersion& aBody, const bool& aMustUseBeastHands, const int& aFeetModIndex);
  static QString getFeetLineForBHUNP(const int& aFeetModIndex);
  static QStringList getXMLDefaultFiltersFromBody(const BodyNameVersion& aBody);

private:
  explicit SliderFileBuilder(){};
};
