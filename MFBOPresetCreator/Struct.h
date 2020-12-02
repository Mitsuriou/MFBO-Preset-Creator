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
    QString family{"Roboto"};
    QString styleName{"Regular"};
    int size{10};
    int weight{50};
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
    BodyNameVersion defaultMainWindowBody{BodyNameVersion::CBBE_3BBB_3BA_1_40};
    Struct::Font font;
    ApplicationLanguage language{LangManager::getSystemLanguage()};
    WindowOpeningMode mainWindowOpeningMode{WindowOpeningMode::Windowed};
    BodyNameVersion defaultRetargetingToolBody{BodyNameVersion::CBBE_3BBB_3BA_1_40};
    int mainWindowHeight{600};
    int mainWindowWidth{900};
    QString mainWindowOutputPath{""};
    bool mainWindowAutomaticallyOpenGeneratedDirectory{true};
    bool checkForUpdatesAtStartup{true};

    bool operator==(const Struct::Settings& aSettings)
    {
      return (
        this->appTheme == aSettings.appTheme
        && this->defaultMainWindowBody == aSettings.defaultMainWindowBody
        && this->font == aSettings.font
        && this->language == aSettings.language
        && this->mainWindowOpeningMode == aSettings.mainWindowOpeningMode
        && this->defaultRetargetingToolBody == aSettings.defaultRetargetingToolBody
        && this->mainWindowHeight == aSettings.mainWindowHeight
        && this->mainWindowWidth == aSettings.mainWindowWidth
        && this->mainWindowOutputPath == aSettings.mainWindowOutputPath
        && this->mainWindowAutomaticallyOpenGeneratedDirectory == aSettings.mainWindowAutomaticallyOpenGeneratedDirectory);
    };

    bool operator!=(const Struct::Settings& aSettings)
    {
      return (
        this->appTheme != aSettings.appTheme
        || this->defaultMainWindowBody != aSettings.defaultMainWindowBody
        || this->font != aSettings.font
        || this->language != aSettings.language
        || this->mainWindowOpeningMode != aSettings.mainWindowOpeningMode
        || this->defaultRetargetingToolBody != aSettings.defaultRetargetingToolBody
        || this->mainWindowHeight != aSettings.mainWindowHeight
        || this->mainWindowWidth != aSettings.mainWindowWidth
        || this->mainWindowOutputPath != aSettings.mainWindowOutputPath
        || this->mainWindowAutomaticallyOpenGeneratedDirectory != aSettings.mainWindowAutomaticallyOpenGeneratedDirectory);
    };
  };

  struct AssistedConversionResult
  {
  public:
    QString path{""};
    QString name{""};
    int role{0};
  };
}
