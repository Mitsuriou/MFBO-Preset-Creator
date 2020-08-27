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
    QString language;
    QString fontFamily;
    QString fontSize;
    bool darkTheme = false;
    int defaulWindowHeight{ -1 };
    int defaulWindowWidth{ -1 };
    CBBE3BBBVersion defaultCBBEBBBVersion{ CBBE3BBBVersion::Version1_40 };
    CBBE3BBBVersion defaultUpgradingCBBEBBBVersion{ CBBE3BBBVersion::Version1_51 };
  };
}
