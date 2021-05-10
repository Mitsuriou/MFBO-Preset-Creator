#pragma once

#include "Struct.h"
#include <QString>

class SliderSetsHelper
{
public:
  static QString getSliderSetFile(const BodyNameVersion& aBody, const bool& aMustUseBeastHands, const int& aFeetModIndex);
  static QString getHandsBlock(const BodyNameVersion& aBody, const bool& aMustUseBeastHands);
  static QString getFeetBlock(const BodyNameVersion& aBody, const int& aFeetModIndex);
  static QString getBodyBlock(const BodyNameVersion& aBody);

private:
  explicit SliderSetsHelper(){};
};
