#pragma once
#include "Enum.h"
#include "LangManager.h"
#include <QString>

namespace Struct
{
  struct SliderSet
  {
  public:
    explicit SliderSet() {}

    explicit SliderSet(const QString& aName, const QString& aMeshPart, const QString& aOutputPath, const QString& aOutputFile)
    {
      this->name = aName;
      this->meshPart = aMeshPart;
      this->outputPath = aOutputPath;
      this->outputFile = aOutputFile;
    }

    void setName(const QString& name)
    {
      this->name = name;
    }

    QString getName() const
    {
      return this->name;
    }

    void setMeshPart(const QString& meshPart)
    {
      this->meshPart = meshPart;
    }

    QString getMeshPart() const
    {
      return this->meshPart;
    }

    void setOutputPath(const QString& outputPath)
    {
      this->outputPath = outputPath;
    }

    QString getOutputPath() const
    {
      return this->outputPath;
    }

    void setOutputFile(const QString& outputFile)
    {
      this->outputFile = outputFile;
    }

    QString getOutputFile() const
    {
      return this->outputFile;
    }

  private:
    QString name{};
    QString meshPart{};
    QString outputPath{};
    QString outputFile{};
  };

  struct Font
  {
  public:
    QString family{"Roboto"};
    bool italic{false};
    int size{10};
    bool strikeOut{false};
    QString styleName{"Regular"};
    bool underline{false};
    int weight{50};

    bool operator==(const Struct::Font& aSettings)
    {
      return (
        this->family == aSettings.family
        && this->italic == aSettings.italic
        && this->size == aSettings.size
        && this->strikeOut == aSettings.strikeOut
        && this->styleName == aSettings.styleName
        && this->underline == aSettings.underline
        && this->weight == aSettings.weight);
    };

    bool operator!=(const Struct::Font& aSettings)
    {
      return (this->family != aSettings.family
              || this->italic != aSettings.italic
              || this->size != aSettings.size
              || this->strikeOut != aSettings.strikeOut
              || this->styleName != aSettings.styleName
              || this->underline != aSettings.underline
              || this->weight != aSettings.weight);
    };
  };

  struct Settings
  {
  public:
    GUITheme appTheme{GUITheme::MitsuriouDarkTheme};
    bool assistedConversionScanOnlyMeshesSubdir{false};
    QString batchConversionOutputPath{""};
    bool checkForUpdatesAtStartup{true};
    QString dangerColor{"#f44034"};
    BodyNameVersion defaultBatchConversionBody{BodyNameVersion::CBBE_3BBB_3BA_1_40};
    int defaultMainFeetMod{0};
    BodyNameVersion defaultMainWindowBody{BodyNameVersion::CBBE_3BBB_3BA_1_40};
    BodyNameVersion defaultRetargetingToolBody{BodyNameVersion::CBBE_3BBB_3BA_1_40};
    int defaultRetargetingToolFeetMod{0};
    bool eachButtonSavesItsLastUsedPath{true};
    Struct::Font font;
    ApplicationLanguage language{LangManager::getSystemLanguage()};
    bool mainWindowAutomaticallyOpenGeneratedDirectory{true};
    int mainWindowHeight{620};
    WindowOpeningMode mainWindowOpeningMode{WindowOpeningMode::Windowed};
    QString mainWindowOutputPath{""};
    int mainWindowWidth{1000};
    QString successColor{"#48c774"};
    QString warningColor{"#ffbc6b"};

    bool operator==(const Struct::Settings& aSettings)
    {
      return (
        this->appTheme == aSettings.appTheme
        && this->defaultMainWindowBody == aSettings.defaultMainWindowBody
        && this->defaultRetargetingToolBody == aSettings.defaultRetargetingToolBody
        && this->defaultBatchConversionBody == aSettings.defaultBatchConversionBody
        && this->defaultMainFeetMod == aSettings.defaultMainFeetMod
        && this->defaultRetargetingToolFeetMod == aSettings.defaultRetargetingToolFeetMod
        && this->font == aSettings.font
        && this->language == aSettings.language
        && this->mainWindowOpeningMode == aSettings.mainWindowOpeningMode
        && this->mainWindowHeight == aSettings.mainWindowHeight
        && this->mainWindowWidth == aSettings.mainWindowWidth
        && this->mainWindowOutputPath == aSettings.mainWindowOutputPath
        && this->batchConversionOutputPath == aSettings.batchConversionOutputPath
        && this->mainWindowAutomaticallyOpenGeneratedDirectory == aSettings.mainWindowAutomaticallyOpenGeneratedDirectory
        && this->checkForUpdatesAtStartup == aSettings.checkForUpdatesAtStartup
        && this->assistedConversionScanOnlyMeshesSubdir == aSettings.assistedConversionScanOnlyMeshesSubdir
        && this->eachButtonSavesItsLastUsedPath == aSettings.eachButtonSavesItsLastUsedPath
        && this->successColor == aSettings.successColor
        && this->warningColor == aSettings.warningColor
        && this->dangerColor == aSettings.dangerColor);
    };

    bool operator!=(const Struct::Settings& aSettings)
    {
      return (
        this->appTheme != aSettings.appTheme
        || this->defaultMainWindowBody != aSettings.defaultMainWindowBody
        || this->defaultRetargetingToolBody != aSettings.defaultRetargetingToolBody
        || this->defaultBatchConversionBody != aSettings.defaultBatchConversionBody
        || this->defaultMainFeetMod != aSettings.defaultMainFeetMod
        || this->defaultRetargetingToolFeetMod != aSettings.defaultRetargetingToolFeetMod
        || this->font != aSettings.font
        || this->language != aSettings.language
        || this->mainWindowOpeningMode != aSettings.mainWindowOpeningMode
        || this->mainWindowHeight != aSettings.mainWindowHeight
        || this->mainWindowWidth != aSettings.mainWindowWidth
        || this->mainWindowOutputPath != aSettings.mainWindowOutputPath
        || this->batchConversionOutputPath != aSettings.batchConversionOutputPath
        || this->mainWindowAutomaticallyOpenGeneratedDirectory != aSettings.mainWindowAutomaticallyOpenGeneratedDirectory
        || this->checkForUpdatesAtStartup != aSettings.checkForUpdatesAtStartup
        || this->assistedConversionScanOnlyMeshesSubdir != aSettings.assistedConversionScanOnlyMeshesSubdir
        || this->eachButtonSavesItsLastUsedPath != aSettings.eachButtonSavesItsLastUsedPath
        || this->successColor != aSettings.successColor
        || this->warningColor != aSettings.warningColor
        || this->dangerColor != aSettings.dangerColor);
    };
  };

  struct AssistedConversionResult
  {
  public:
    explicit AssistedConversionResult(const QString& aPath, const QString& aName, const int aRole)
    {
      this->path = aPath;
      this->name = aName;
      this->role = aRole;
    };

    QString getPath() const
    {
      return this->path;
    }

    QString getName() const
    {
      return this->name;
    }

    int getRole() const
    {
      return this->role;
    }

  private:
    QString path{""};
    QString name{""};
    int role{0};

    explicit AssistedConversionResult() {}
  };

  struct Filter
  {
  public:
    explicit Filter(const QString& aFilters, const bool aBody, const bool aFeet, const bool aHands)
    {
      this->filter = aFilters;
      this->body = aBody;
      this->feet = aFeet;
      this->hands = aHands;
    }

    QString getFilter() const
    {
      return this->filter;
    }

    bool isBodyCompatible() const
    {
      return this->body;
    }

    bool isFeetCompatible() const
    {
      return this->feet;
    }

    bool isHandsCompatible() const
    {
      return this->hands;
    }

  private:
    QString filter{};
    bool body{false};
    bool feet{false};
    bool hands{false};

    explicit Filter() {}
  };
}
