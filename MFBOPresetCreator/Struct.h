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
    GUITheme appTheme{GUITheme::WindowsVista};
    CBBE3BBBVersion defaultMainWindowCBBE3BBBVersion{CBBE3BBBVersion::Version1_40};
    QString fontFamily{"MS Shell Dlg 2"};
    int fontSize{11};
    ApplicationLanguage language{ApplicationLanguage::English};
    WindowOpeningMode mainWindowOpeningMode{WindowOpeningMode::Windowed};
    CBBE3BBBVersion defaultRetargetingToolCBBE3BBBVersion{CBBE3BBBVersion::Version1_51_and_1_52};
    int mainWindowHeight{600};
    int mainWindowWidth{800};
    QString mainWindowOutputPath{""};
    bool mainWindowAutomaticallyOpenGeneratedDirectory{true};

    bool operator==(const Struct::Settings& aSettings)
    {
      return (
        this->appTheme == aSettings.appTheme
        && this->defaultMainWindowCBBE3BBBVersion == aSettings.defaultMainWindowCBBE3BBBVersion
        && this->fontFamily == aSettings.fontFamily
        && this->fontSize == aSettings.fontSize
        && this->language == aSettings.language
        && this->mainWindowOpeningMode == aSettings.mainWindowOpeningMode
        && this->defaultRetargetingToolCBBE3BBBVersion == aSettings.defaultRetargetingToolCBBE3BBBVersion
        && this->mainWindowHeight == aSettings.mainWindowHeight && this->mainWindowWidth == aSettings.mainWindowWidth
        && this->mainWindowOutputPath == aSettings.mainWindowOutputPath
        && this->mainWindowAutomaticallyOpenGeneratedDirectory == aSettings.mainWindowAutomaticallyOpenGeneratedDirectory);
    };

    bool operator!=(const Struct::Settings& aSettings)
    {
      return (
        this->appTheme != aSettings.appTheme
        || this->defaultMainWindowCBBE3BBBVersion != aSettings.defaultMainWindowCBBE3BBBVersion
        || this->fontFamily != aSettings.fontFamily
        || this->fontSize != aSettings.fontSize
        || this->language != aSettings.language
        || this->mainWindowOpeningMode != aSettings.mainWindowOpeningMode
        || this->defaultRetargetingToolCBBE3BBBVersion != aSettings.defaultRetargetingToolCBBE3BBBVersion
        || this->mainWindowHeight != aSettings.mainWindowHeight && this->mainWindowWidth == aSettings.mainWindowWidth
        || this->mainWindowOutputPath != aSettings.mainWindowOutputPath
        || this->mainWindowAutomaticallyOpenGeneratedDirectory != aSettings.mainWindowAutomaticallyOpenGeneratedDirectory);
    };
  };
}
