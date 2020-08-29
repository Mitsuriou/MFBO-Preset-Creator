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
    ApplicationLanguage language{ApplicationLanguage::English};
    QString fontFamily{"MS Shell Dlg 2"};
    int fontSize{11};
    GUITheme appTheme{GUITheme::WindowsVista};
    int mainWindowWidth{800};
    int mainWindowHeight{600};
    CBBE3BBBVersion defaultMainWindowCBBE3BBBVersion{CBBE3BBBVersion::Version1_40};
    CBBE3BBBVersion defaultUpgradeToolCBBE3BBBVersion{CBBE3BBBVersion::Version1_51_and_1_52};
  };
}
