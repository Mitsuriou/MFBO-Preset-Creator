#pragma once

#include "Enum.h"

#include <QString>

namespace Struct
{
  struct SliderSet
  {
  public:
    QString name;
    QString meshpart;
    QString outputpath;
    QString outputfile;
  };

  struct Settings
  {
  public:
    QString language{""};
    QString fontFamily{""};
    int fontSize{9};
    bool darkTheme{false};
    int defaulWindowWidth{-1};
    int defaulWindowHeight{-1};
    CBBE3BBBVersion defaultCBBE3BBBVersion{CBBE3BBBVersion::Version1_40};
    CBBE3BBBVersion defaultUpgradingCBBE3BBBVersion{CBBE3BBBVersion::Version1_51_and_1_52};
  };
}
