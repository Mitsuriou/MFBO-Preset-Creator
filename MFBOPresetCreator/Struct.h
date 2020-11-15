#pragma once

#include "Enum.h"
#include "LangManager.h"
#include <QString>

namespace Struct
{
  struct SliderSet
  {
  public:
    QString name{""};
    QString meshpart{""};
    QString outputpath{""};
    QString outputfile{""};
  };

  struct Font
  {
  public:
    QString family{"MS Shell Dlg 2"};
    QString styleName{""};
    int size{10};
    int weight{-1};
    bool italic{false};
    bool underline{false};
    bool strikeOut{false};

    bool operator==(const Struct::Font& aSettings)
    {
      return (
        this->family == aSettings.family
        && this->styleName == aSettings.styleName
        && this->size == aSettings.size
        && this->weight == aSettings.weight
        && this->italic == aSettings.italic
        && this->underline == aSettings.underline
        && this->strikeOut == aSettings.strikeOut);
    };

    bool operator!=(const Struct::Font& aSettings)
    {
      return (this->family != aSettings.family
              || this->styleName != aSettings.styleName
              || this->size != aSettings.size
              || this->weight != aSettings.weight
              || this->italic != aSettings.italic
              || this->underline != aSettings.underline
              || this->strikeOut != aSettings.strikeOut);
    };
  };

  struct Settings
  {
  public:
    GUITheme appTheme{GUITheme::WindowsVista};
    CBBE3BBBVersion defaultMainWindowCBBE3BBBVersion{CBBE3BBBVersion::Version1_40};
    Struct::Font font;
    ApplicationLanguage language{LangManager::getSystemLanguage()};
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
        && this->font == aSettings.font
        && this->language == aSettings.language
        && this->mainWindowOpeningMode == aSettings.mainWindowOpeningMode
        && this->defaultRetargetingToolCBBE3BBBVersion == aSettings.defaultRetargetingToolCBBE3BBBVersion
        && this->mainWindowHeight == aSettings.mainWindowHeight
        && this->mainWindowWidth == aSettings.mainWindowWidth
        && this->mainWindowOutputPath == aSettings.mainWindowOutputPath
        && this->mainWindowAutomaticallyOpenGeneratedDirectory == aSettings.mainWindowAutomaticallyOpenGeneratedDirectory);
    };

    bool operator!=(const Struct::Settings& aSettings)
    {
      return (
        this->appTheme != aSettings.appTheme
        || this->defaultMainWindowCBBE3BBBVersion != aSettings.defaultMainWindowCBBE3BBBVersion
        || this->font != aSettings.font
        || this->language != aSettings.language
        || this->mainWindowOpeningMode != aSettings.mainWindowOpeningMode
        || this->defaultRetargetingToolCBBE3BBBVersion != aSettings.defaultRetargetingToolCBBE3BBBVersion
        || this->mainWindowHeight != aSettings.mainWindowHeight
        || this->mainWindowWidth != aSettings.mainWindowWidth
        || this->mainWindowOutputPath != aSettings.mainWindowOutputPath
        || this->mainWindowAutomaticallyOpenGeneratedDirectory != aSettings.mainWindowAutomaticallyOpenGeneratedDirectory);
    };
  };

  struct AssistedConversionResult
  {
  public:
    QString path;
    QString name;
    int role;
  };
}
