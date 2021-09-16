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
    DialogOpeningMode assistedConversionDialogOpeningMode{DialogOpeningMode::WINDOWED};
    bool assistedConversionScanOnlyMeshesSubdir{false};
    DialogOpeningMode batchConversionDialogOpeningMode{DialogOpeningMode::WINDOWED};
    QString batchConversionOutputPath{""};
    DialogOpeningMode batchConversionPickerDialogOpeningMode{DialogOpeningMode::WINDOWED};
    DialogOpeningMode bodySlidePresetsRetargetingDialogOpeningMode{DialogOpeningMode::WINDOWED};
    QString dangerColor{"#f44034"};
    BodyNameVersion defaultBatchConversionBody{BodyNameVersion::CBBE_3BBB_3BA_1_50};
    int defaultMainFeetMod{0};
    BodyNameVersion defaultMainWindowBody{BodyNameVersion::CBBE_3BBB_3BA_1_50};
    BodyNameVersion defaultRetargetingToolBody{BodyNameVersion::CBBE_3BBB_3BA_1_50};
    int defaultRetargetingToolFeetMod{0};
    bool eachButtonSavesItsLastUsedPath{true};
    Struct::Font font;
    ApplicationLanguage language{LangManager::GetSystemLanguage()};
    bool mainWindowAutomaticallyOpenGeneratedDirectory{true};
    int mainWindowHeight{620};
    WindowOpeningMode mainWindowOpeningMode{WindowOpeningMode::WINDOWED};
    QString mainWindowOutputPath{""};
    int mainWindowWidth{1000};
    bool retargetingToolAutomaticallyOpenGeneratedDirectory{true};
    StartupAction startupAction{StartupAction::OPEN_WELCOME_SCREEN};
    QString successColor{"#48c774"};
    DialogOpeningMode texturesAssistantDialogOpeningMode{DialogOpeningMode::WINDOWED};
    QString warningColor{"#ffbc6b"};

    bool operator==(const Struct::Settings& aSettings)
    {
      return (
        this->appTheme == aSettings.appTheme
        && this->assistedConversionDialogOpeningMode == aSettings.assistedConversionDialogOpeningMode
        && this->assistedConversionScanOnlyMeshesSubdir == aSettings.assistedConversionScanOnlyMeshesSubdir
        && this->batchConversionDialogOpeningMode == aSettings.batchConversionDialogOpeningMode
        && this->batchConversionOutputPath == aSettings.batchConversionOutputPath
        && this->batchConversionPickerDialogOpeningMode == aSettings.batchConversionPickerDialogOpeningMode
        && this->bodySlidePresetsRetargetingDialogOpeningMode == aSettings.bodySlidePresetsRetargetingDialogOpeningMode
        && this->dangerColor == aSettings.dangerColor
        && this->defaultBatchConversionBody == aSettings.defaultBatchConversionBody
        && this->defaultMainFeetMod == aSettings.defaultMainFeetMod
        && this->defaultMainWindowBody == aSettings.defaultMainWindowBody
        && this->defaultRetargetingToolBody == aSettings.defaultRetargetingToolBody
        && this->defaultRetargetingToolFeetMod == aSettings.defaultRetargetingToolFeetMod
        && this->eachButtonSavesItsLastUsedPath == aSettings.eachButtonSavesItsLastUsedPath
        && this->font == aSettings.font
        && this->language == aSettings.language
        && this->mainWindowAutomaticallyOpenGeneratedDirectory == aSettings.mainWindowAutomaticallyOpenGeneratedDirectory
        && this->mainWindowHeight == aSettings.mainWindowHeight
        && this->mainWindowOpeningMode == aSettings.mainWindowOpeningMode
        && this->mainWindowOutputPath == aSettings.mainWindowOutputPath
        && this->mainWindowWidth == aSettings.mainWindowWidth
        && this->retargetingToolAutomaticallyOpenGeneratedDirectory == aSettings.retargetingToolAutomaticallyOpenGeneratedDirectory
        && this->startupAction == aSettings.startupAction
        && this->successColor == aSettings.successColor
        && this->texturesAssistantDialogOpeningMode == aSettings.texturesAssistantDialogOpeningMode
        && this->warningColor == aSettings.warningColor);
    }

    bool operator!=(const Struct::Settings& aSettings)
    {
      return (
        this->appTheme != aSettings.appTheme
        || this->assistedConversionDialogOpeningMode != aSettings.assistedConversionDialogOpeningMode
        || this->assistedConversionScanOnlyMeshesSubdir != aSettings.assistedConversionScanOnlyMeshesSubdir
        || this->batchConversionDialogOpeningMode != aSettings.batchConversionDialogOpeningMode
        || this->batchConversionOutputPath != aSettings.batchConversionOutputPath
        || this->batchConversionPickerDialogOpeningMode != aSettings.batchConversionPickerDialogOpeningMode
        || this->bodySlidePresetsRetargetingDialogOpeningMode != aSettings.bodySlidePresetsRetargetingDialogOpeningMode
        || this->dangerColor != aSettings.dangerColor
        || this->defaultBatchConversionBody != aSettings.defaultBatchConversionBody
        || this->defaultMainFeetMod != aSettings.defaultMainFeetMod
        || this->defaultMainWindowBody != aSettings.defaultMainWindowBody
        || this->defaultRetargetingToolBody != aSettings.defaultRetargetingToolBody
        || this->defaultRetargetingToolFeetMod != aSettings.defaultRetargetingToolFeetMod
        || this->eachButtonSavesItsLastUsedPath != aSettings.eachButtonSavesItsLastUsedPath
        || this->font != aSettings.font
        || this->language != aSettings.language
        || this->mainWindowAutomaticallyOpenGeneratedDirectory != aSettings.mainWindowAutomaticallyOpenGeneratedDirectory
        || this->mainWindowHeight != aSettings.mainWindowHeight
        || this->mainWindowOpeningMode != aSettings.mainWindowOpeningMode
        || this->mainWindowOutputPath != aSettings.mainWindowOutputPath
        || this->mainWindowWidth != aSettings.mainWindowWidth
        || this->retargetingToolAutomaticallyOpenGeneratedDirectory != aSettings.retargetingToolAutomaticallyOpenGeneratedDirectory
        || this->startupAction != aSettings.startupAction
        || this->successColor != aSettings.successColor
        || this->texturesAssistantDialogOpeningMode != aSettings.texturesAssistantDialogOpeningMode
        || this->warningColor != aSettings.warningColor);
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
    explicit BatchConversionEntry(){};

    explicit BatchConversionEntry(const QString& aOriginFolder, const QString& aRessourcePath, const bool aUseAlternativeModel)
    {
      setOriginFolder(aOriginFolder);
      setRessourcePath(aRessourcePath);
      setUseAlternativeModel(aUseAlternativeModel);
    }

    // Origin directory
    void setOriginFolder(const QString& aOriginFolder)
    {
      this->originFolder = aOriginFolder;
    }

    QString getOriginFolder() const
    {
      return this->originFolder;
    }

    // Ressource path
    void setRessourcePath(const QString& aRessourcePath)
    {
      this->ressourcePath = aRessourcePath;
    }

    QString getRessourcePath() const
    {
      return this->ressourcePath;
    }

    // Alternative model
    void setUseAlternativeModel(const bool aUseAlternativeModel)
    {
      this->useAlternativeModel = aUseAlternativeModel;
    }

    bool mustUseAlternativeModel() const
    {
      return this->useAlternativeModel;
    }

  private:
    QString originFolder;
    QString ressourcePath;
    bool useAlternativeModel{false}; // Use beasts variants model
  };

  struct BatchConversionPresetData
  {
  public:
    explicit BatchConversionPresetData(){};

    explicit BatchConversionPresetData(const QString& aFilesNames,
                                       const QString& aPresetsNames,
                                       const QString& aBodyPath,
                                       const QString& aBodyName,
                                       const QString& aFeetPath,
                                       const QString& aFeetName,
                                       const QString& aHandsPath,
                                       const QString& aHandsName,
                                       const bool aHandsUseAlternativeModel,
                                       const QString& aSkeletonPath,
                                       const QString& aSkeletonName,
                                       const bool aSkeletonUseAlternativeModel)
    {
      this->names = std::make_pair(aFilesNames, aPresetsNames);
      this->body = std::make_pair(aBodyPath, aBodyName);
      this->feet = std::make_pair(aFeetPath, aFeetName);
      this->hands = Struct::BatchConversionEntry(aHandsPath, aHandsName, aHandsUseAlternativeModel);
      this->skeleton = Struct::BatchConversionEntry(aSkeletonPath, aSkeletonName, aSkeletonUseAlternativeModel);
    };

    bool isValid() const
    {
      // Conditions to ba considered as valid:
      // The files names and presets names must be defined.
      // A body, a feet or a hands mesh must be defined, at least
      // The skeleton is facultative
      return (!this->names.first.isEmpty()
              && !this->names.second.isEmpty()
              && (!this->body.second.isEmpty() || !this->feet.second.isEmpty() || !this->hands.getRessourcePath().isEmpty()));
    };

    // Names
    void setNames(const QString& aFilesNames, const QString& aPresetsNames)
    {
      this->names = std::make_pair(aFilesNames, aPresetsNames);
    }

    std::pair<QString, QString> getNames() const
    {
      return this->names;
    }

    // Body
    void setBodyData(const QString& aOriginFolder, const QString& aRessourcePath)
    {
      this->body = std::make_pair(aOriginFolder, aRessourcePath);
    }

    void resetBodyData()
    {
      this->setBodyData("", "");
    }

    std::pair<QString, QString> getBodyData() const
    {
      return this->body;
    }

    // Feet
    void setFeetData(const QString& aOriginFolder, const QString& aRessourcePath)
    {
      this->feet = std::make_pair(aOriginFolder, aRessourcePath);
    }

    void resetFeetData()
    {
      this->setFeetData("", "");
    }

    std::pair<QString, QString> getFeetData() const
    {
      return this->feet;
    }

    // Hands
    void setHandsData(const QString& aOriginFolder, const QString& aRessourcePath, const bool aUseAlternativeModel)
    {
      this->hands.setOriginFolder(aOriginFolder);
      this->hands.setRessourcePath(aRessourcePath);
      this->hands.setUseAlternativeModel(aUseAlternativeModel);
    };

    void resetHandsData()
    {
      this->setHandsData("", "", false);
    }

    BatchConversionEntry getHandsData() const
    {
      return this->hands;
    };

    void setHandsUseAlternativeModel(const bool aUseAlternativeModel)
    {
      this->hands.setUseAlternativeModel(aUseAlternativeModel);
    }

    bool mustHandsUseAlternativeModel() const
    {
      return this->hands.mustUseAlternativeModel();
    }

    // Skeleton
    void setSkeletonData(const QString& aOriginFolder, const QString& aRessourcePath, const bool aUseAlternativeModel)
    {
      this->skeleton.setOriginFolder(aOriginFolder);
      this->skeleton.setRessourcePath(aRessourcePath);
      this->skeleton.setUseAlternativeModel(aUseAlternativeModel);
    };

    void resetSkeletonData()
    {
      this->setSkeletonData("", "", false);
    }

    BatchConversionEntry getSkeletonData() const
    {
      return this->skeleton;
    };

    void setSkeletonUseAlternativeModel(const bool aUseAlternativeModel)
    {
      this->skeleton.setUseAlternativeModel(aUseAlternativeModel);
    }

    bool mustSkeletonUseAlternativeModel() const
    {
      return this->skeleton.mustUseAlternativeModel();
    }

  private:
    std::pair<QString, QString> names; // <OSP and XML files names, Presets names in BodySlide>
    std::pair<QString, QString> body;  // <origin mod name, mesh path/name (aka full ressource path)>
    std::pair<QString, QString> feet;  // <origin mod name, mesh path/name (aka full ressource path)>
    BatchConversionEntry hands;        // <origin mod name, mesh path/name (aka full ressource path), use alternate model>
    BatchConversionEntry skeleton;     // <origin mod name, mesh path/name (aka full ressource path), mash name, use alternate model>
  };

  struct BatchConversionData
  {
    QString humanSkeletonPath;
    QString beastSkeletonPath;
    BodyNameVersion bodyMod;
    int feetModIndex{-1};
    std::vector<Struct::Filter> filters;
    QString fullOutputPath;
    std::map<QString, std::set<QString>> scannedData;
    std::vector<BatchConversionPresetData> presets;

    // TODO: Make this structure cleaner (with a constructor and private attributes)
  };
}
