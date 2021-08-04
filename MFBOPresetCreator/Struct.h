#pragma once
#include "Enum.h"
#include "LangManager.h"
#include <QString>
#include <QStringList>
#include <map>
#include <set>
#include <vector>

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

    void setName(const QString& aName)
    {
      this->name = aName;
    }

    QString getName() const
    {
      return this->name;
    }

    void setMeshPart(const QString& aMeshPart)
    {
      this->meshPart = aMeshPart;
    }

    QString getMeshPart() const
    {
      return this->meshPart;
    }

    void setOutputPath(const QString& aOutputPath)
    {
      this->outputPath = aOutputPath;
    }

    QString getOutputPath() const
    {
      return this->outputPath;
    }

    void setOutputFile(const QString& aOutputFile)
    {
      this->outputFile = aOutputFile;
    }

    QString getOutputFile() const
    {
      return this->outputFile;
    }

  private:
    QString name{""};
    QString meshPart{""};
    QString outputPath{""};
    QString outputFile{""};
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
    }

    bool operator!=(const Struct::Font& aSettings)
    {
      return (this->family != aSettings.family
              || this->italic != aSettings.italic
              || this->size != aSettings.size
              || this->strikeOut != aSettings.strikeOut
              || this->styleName != aSettings.styleName
              || this->underline != aSettings.underline
              || this->weight != aSettings.weight);
    }
  };

  struct Settings
  {
  public:
    GUITheme appTheme{GUITheme::MITSURIOU_DARK_THEME};
    bool assistedConversionScanOnlyMeshesSubdir{false};
    QString batchConversionOutputPath{""};
    QString dangerColor{"#f44034"};
    BodyNameVersion defaultBatchConversionBody{BodyNameVersion::CBBE_3BBB_3BA_1_50};
    int defaultMainFeetMod{0};
    BodyNameVersion defaultMainWindowBody{BodyNameVersion::CBBE_3BBB_3BA_1_50};
    BodyNameVersion defaultRetargetingToolBody{BodyNameVersion::CBBE_3BBB_3BA_1_50};
    int defaultRetargetingToolFeetMod{0};
    bool eachButtonSavesItsLastUsedPath{true};
    Struct::Font font;
    ApplicationLanguage language{LangManager::getSystemLanguage()};
    bool mainWindowAutomaticallyOpenGeneratedDirectory{true};
    int mainWindowHeight{620};
    WindowOpeningMode mainWindowOpeningMode{WindowOpeningMode::WINDOWED};
    QString mainWindowOutputPath{""};
    int mainWindowWidth{1000};
    StartupAction startupAction{StartupAction::OPEN_WELCOME_SCREEN};
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
        && this->assistedConversionScanOnlyMeshesSubdir == aSettings.assistedConversionScanOnlyMeshesSubdir
        && this->eachButtonSavesItsLastUsedPath == aSettings.eachButtonSavesItsLastUsedPath
        && this->startupAction == aSettings.startupAction
        && this->successColor == aSettings.successColor
        && this->warningColor == aSettings.warningColor
        && this->dangerColor == aSettings.dangerColor);
    }

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
        || this->assistedConversionScanOnlyMeshesSubdir != aSettings.assistedConversionScanOnlyMeshesSubdir
        || this->eachButtonSavesItsLastUsedPath != aSettings.eachButtonSavesItsLastUsedPath
        || this->startupAction != aSettings.startupAction
        || this->successColor != aSettings.successColor
        || this->warningColor != aSettings.warningColor
        || this->dangerColor != aSettings.dangerColor);
    }
  };

  struct AssistedConversionResult
  {
  public:
    explicit AssistedConversionResult(const QString& aPath, const QString& aName, const int aRole)
    {
      this->path = aPath;
      this->name = aName;
      this->role = aRole;
    }

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
    QString filter{""};
    bool body{false};
    bool feet{false};
    bool hands{false};

    explicit Filter() {}
  };

  struct BatchConversionEntry
  {
  public:
    explicit BatchConversionEntry(const QString& aPath, const QString& aName, const bool aUseAlternativeModel)
    {
      this->path = aPath;
      this->name = aName;
      this->useAlternativeModel = aUseAlternativeModel;
    }

    void setPath(const QString& aPath)
    {
      this->path = aPath;
    }

    QString getPath() const
    {
      return this->path;
    }

    void setName(const QString& aName)
    {
      this->name = aName;
    }

    QString getName() const
    {
      return this->name;
    }

    void setUseAlternativeModel(const bool aUseAlternativeModel)
    {
      this->useAlternativeModel = aUseAlternativeModel;
    }

    bool mustUseAlternativeModel() const
    {
      return this->useAlternativeModel;
    }

  private:
    QString path{""};
    QString name{""};
    bool useAlternativeModel{false}; // Use beasts variants model
  };

  struct BatchConversionPresetData
  {
  public:
    std::map<int, std::pair<QString, QString>> names;       // <OSP and XML files names, Name in BS>
    std::multimap<int, std::pair<QString, QString>> bodies; // <sub preset index, <path, name>>
    std::multimap<int, std::pair<QString, QString>> feet;   // <sub preset index, <path, name>>
    std::multimap<int, BatchConversionEntry> hands;
    std::multimap<int, BatchConversionEntry> skeletons;

    // TODO: Make this structure cleaner (with a constructor and private attributes)
  };

  struct BatchConversionData
  {
    QString humanSkeletonPath;
    QString beastSkeletonPath;
    BodyNameVersion bodyMod;
    int feetModIndex{-1};
    std::vector<Struct::Filter> filters;
    QString fullOutputPath{""};
    std::map<QString, std::set<QString>> scannedData;
    std::map<QString, BatchConversionPresetData> presets;

    // TODO: Make this structure cleaner (with a constructor and private attributes)
  };
}
