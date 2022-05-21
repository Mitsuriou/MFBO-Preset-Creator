#pragma once
#include "Enum.h"
#include "LangManager.h"
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <map>
#include <vector>

namespace Struct
{
  struct SliderSet
  {
  public:
    explicit SliderSet() {}

    explicit SliderSet(const QString& aName,
                       const MeshPartType& aMeshPartType,
                       const QString& aOutputPath,
                       const QString& aOutputFile)
      : name(aName)
      , meshPartType(aMeshPartType)
      , outputPath(aOutputPath)
      , outputFile(aOutputFile)
    {}

    void setName(const QString& aName)
    {
      this->name = aName;
    }

    const QString& getName() const
    {
      return this->name;
    }

    void setMeshPart(const MeshPartType& aMeshPartType)
    {
      this->meshPartType = aMeshPartType;
    }

    const MeshPartType& getMeshPart() const
    {
      return this->meshPartType;
    }

    void setOutputPath(const QString& aOutputPath)
    {
      this->outputPath = aOutputPath;
    }

    const QString& getOutputPath() const
    {
      return this->outputPath;
    }

    void setOutputFile(const QString& aOutputFile)
    {
      this->outputFile = aOutputFile;
    }

    const QString& getOutputFile() const
    {
      return this->outputFile;
    }

    bool isValid()
    {
      return !this->name.isEmpty() && !this->getOutputPath().isEmpty() && !this->getOutputFile().isEmpty() && getMeshPart() != MeshPartType::UNKNOWN;
    }

    bool isValidNoMeshPartCheck()
    {
      return !this->name.isEmpty() && !this->getOutputPath().isEmpty() && !this->getOutputFile().isEmpty();
    }

  private:
    QString name{""};
    MeshPartType meshPartType{MeshPartType::UNKNOWN};
    QString outputPath{""};
    QString outputFile{""};
  };

  struct Font
  {
  public:
    QString family{"Roboto"};
    bool italic{false};
    int pointSize{10};
    bool strikeOut{false};
    QString styleName{"Regular"};
    bool underline{false};
    int weight{50};

    bool operator==(const Struct::Font& aSettings) const
    {
      return (
        this->family == aSettings.family
        && this->italic == aSettings.italic
        && this->pointSize == aSettings.pointSize
        && this->strikeOut == aSettings.strikeOut
        && this->styleName == aSettings.styleName
        && this->underline == aSettings.underline
        && this->weight == aSettings.weight);
    }

    bool operator!=(const Struct::Font& aSettings) const
    {
      return (this->family != aSettings.family
              || this->italic != aSettings.italic
              || this->pointSize != aSettings.pointSize
              || this->strikeOut != aSettings.strikeOut
              || this->styleName != aSettings.styleName
              || this->underline != aSettings.underline
              || this->weight != aSettings.weight);
    }
  };

  struct GeneralSettings
  {
  public:
    bool eachButtonSavesItsLastUsedPath{true};
    StartupAction startupAction{StartupAction::OPEN_WELCOME_SCREEN};

    QJsonObject toJson() const
    {
      QJsonObject lNode;
      lNode["eachButtonSavesItsLastUsedPath"] = this->eachButtonSavesItsLastUsedPath;
      lNode["startupAction"] = static_cast<int>(this->startupAction);
      return lNode;
    }

    bool operator==(const Struct::GeneralSettings& rhs) const
    {
      return (
        this->eachButtonSavesItsLastUsedPath == rhs.eachButtonSavesItsLastUsedPath
        && this->startupAction == rhs.startupAction);
    }

    bool operator!=(const Struct::GeneralSettings& rhs) const
    {
      return (
        this->eachButtonSavesItsLastUsedPath != rhs.eachButtonSavesItsLastUsedPath
        || this->startupAction != rhs.startupAction);
    }
  };

  struct DisplaySettings
  {
  public:
    GUITheme applicationTheme{GUITheme::MITSURIOU_BLACK_THEME};
    DialogOpeningMode assistedConversionDialogOpeningMode{DialogOpeningMode::WINDOWED};
    DialogOpeningMode batchConversionDialogOpeningMode{DialogOpeningMode::WINDOWED};
    DialogOpeningMode batchConversionPickerDialogOpeningMode{DialogOpeningMode::WINDOWED};
    DialogOpeningMode bodySlidePresetsRetargetingDialogOpeningMode{DialogOpeningMode::WINDOWED};
    QString dangerColor{"#f44034"};
    Struct::Font font;
    ApplicationLanguage language{LangManager::GetSystemLanguage()};
    int mainWindowHeight{620};
    WindowOpeningMode mainWindowOpeningMode{WindowOpeningMode::WINDOWED};
    int mainWindowWidth{1000};
    DialogOpeningMode sliderSetsDBManagerDialogOpeningMode{DialogOpeningMode::WINDOWED};
    DialogOpeningMode sliderSetsImporterDialogOpeningMode{DialogOpeningMode::WINDOWED};
    QString successColor{"#48c774"};
    DialogOpeningMode texturesAssistantDialogOpeningMode{DialogOpeningMode::WINDOWED};
    bool titleBarIconsBlack{true};
    QString warningColor{"#ffbc6b"};

    QJsonObject toJson() const
    {
      // Font
      QJsonObject lFontNode;
      lFontNode["family"] = this->font.family;
      lFontNode["italic"] = this->font.italic;
      lFontNode["size"] = this->font.pointSize;
      lFontNode["strikeOut"] = this->font.strikeOut;
      lFontNode["styleName"] = this->font.styleName;
      lFontNode["underline"] = this->font.underline;
      lFontNode["weight"] = this->font.weight;

      // Display
      QJsonObject lDisplayNode;
      lDisplayNode["applicationTheme"] = static_cast<int>(this->applicationTheme);
      lDisplayNode["assistedConversionDialogOpeningMode"] = static_cast<int>(this->assistedConversionDialogOpeningMode);
      lDisplayNode["batchConversionDialogOpeningMode"] = static_cast<int>(this->batchConversionDialogOpeningMode);
      lDisplayNode["batchConversionPickerDialogOpeningMode"] = static_cast<int>(this->batchConversionPickerDialogOpeningMode);
      lDisplayNode["bodySlidePresetsRetargetingDialogOpeningMode"] = static_cast<int>(this->bodySlidePresetsRetargetingDialogOpeningMode);
      lDisplayNode["dangerColor"] = this->dangerColor;
      lDisplayNode["font"] = lFontNode;
      lDisplayNode["language"] = static_cast<int>(this->language);
      lDisplayNode["mainWindowHeight"] = this->mainWindowHeight;
      lDisplayNode["mainWindowOpeningMode"] = static_cast<int>(this->mainWindowOpeningMode);
      lDisplayNode["mainWindowWidth"] = this->mainWindowWidth;
      lDisplayNode["sliderSetsDBManagerDialogOpeningMode"] = static_cast<int>(this->sliderSetsDBManagerDialogOpeningMode);
      lDisplayNode["sliderSetsImporterDialogOpeningMode"] = static_cast<int>(this->sliderSetsImporterDialogOpeningMode);
      lDisplayNode["successColor"] = this->successColor;
      lDisplayNode["texturesAssistantDialogOpeningMode"] = static_cast<int>(this->texturesAssistantDialogOpeningMode);
      lDisplayNode["titleBarIconsBlack"] = this->titleBarIconsBlack;
      lDisplayNode["warningColor"] = this->warningColor;

      return lDisplayNode;
    }

    bool operator==(const Struct::DisplaySettings& rhs) const
    {
      return (
        this->applicationTheme == rhs.applicationTheme
        && this->assistedConversionDialogOpeningMode == rhs.assistedConversionDialogOpeningMode
        && this->batchConversionDialogOpeningMode == rhs.batchConversionDialogOpeningMode
        && this->batchConversionPickerDialogOpeningMode == rhs.batchConversionPickerDialogOpeningMode
        && this->bodySlidePresetsRetargetingDialogOpeningMode == rhs.bodySlidePresetsRetargetingDialogOpeningMode
        && this->dangerColor == rhs.dangerColor
        && this->font == rhs.font
        && this->language == rhs.language
        && this->mainWindowHeight == rhs.mainWindowHeight
        && this->mainWindowOpeningMode == rhs.mainWindowOpeningMode
        && this->mainWindowWidth == rhs.mainWindowWidth
        && this->sliderSetsDBManagerDialogOpeningMode == rhs.sliderSetsDBManagerDialogOpeningMode
        && this->sliderSetsImporterDialogOpeningMode == rhs.sliderSetsImporterDialogOpeningMode
        && this->successColor == rhs.successColor
        && this->texturesAssistantDialogOpeningMode == rhs.texturesAssistantDialogOpeningMode
        && this->titleBarIconsBlack == rhs.titleBarIconsBlack
        && this->warningColor == rhs.warningColor);
    };

    bool operator!=(const Struct::DisplaySettings& rhs) const
    {
      return (
        this->applicationTheme != rhs.applicationTheme
        || this->assistedConversionDialogOpeningMode != rhs.assistedConversionDialogOpeningMode
        || this->batchConversionDialogOpeningMode != rhs.batchConversionDialogOpeningMode
        || this->batchConversionPickerDialogOpeningMode != rhs.batchConversionPickerDialogOpeningMode
        || this->bodySlidePresetsRetargetingDialogOpeningMode != rhs.bodySlidePresetsRetargetingDialogOpeningMode
        || this->dangerColor != rhs.dangerColor
        || this->font != rhs.font
        || this->language != rhs.language
        || this->mainWindowHeight != rhs.mainWindowHeight
        || this->mainWindowOpeningMode != rhs.mainWindowOpeningMode
        || this->mainWindowWidth != rhs.mainWindowWidth
        || this->sliderSetsDBManagerDialogOpeningMode != rhs.sliderSetsDBManagerDialogOpeningMode
        || this->sliderSetsImporterDialogOpeningMode != rhs.sliderSetsImporterDialogOpeningMode
        || this->successColor != rhs.successColor
        || this->texturesAssistantDialogOpeningMode != rhs.texturesAssistantDialogOpeningMode
        || this->titleBarIconsBlack != rhs.titleBarIconsBlack
        || this->warningColor != rhs.warningColor);
    };
  };

  struct BodyFeetSettings
  {
  public:
    BodyNameVersion bodyMesh{BodyNameVersion::CBBE_3BA_3BBB_1_50};
    FeetNameVersion feetMesh{FeetNameVersion::CBBE_3BA_3BBB_1_50};

    QJsonObject toJson() const
    {
      QJsonObject lNode;
      lNode["bodyMesh"] = static_cast<int>(this->bodyMesh);
      lNode["feetMesh"] = static_cast<int>(this->feetMesh);
      return lNode;
    }

    bool operator==(const Struct::BodyFeetSettings& rhs) const
    {
      return (this->bodyMesh == rhs.bodyMesh && this->feetMesh == rhs.feetMesh);
    }

    bool operator!=(const Struct::BodyFeetSettings& rhs) const
    {
      return (this->bodyMesh != rhs.bodyMesh || this->feetMesh != rhs.feetMesh);
    }
  };

  struct GenericDialogSettings
  {
  public:
    bool automaticallyOpenFinalDirectory{true};
    BodyFeetSettings defaultBodyFeet;

    QJsonObject toJson() const
    {
      QJsonObject lNode;
      lNode["automaticallyOpenFinalDirectory"] = this->automaticallyOpenFinalDirectory;
      lNode["defaultBodyFeet"] = this->defaultBodyFeet.toJson();
      return lNode;
    }

    bool operator==(const Struct::GenericDialogSettings& rhs) const
    {
      return (
        this->automaticallyOpenFinalDirectory == rhs.automaticallyOpenFinalDirectory
        && this->defaultBodyFeet == rhs.defaultBodyFeet);
    }

    bool operator!=(const Struct::GenericDialogSettings& rhs) const
    {
      return (
        this->automaticallyOpenFinalDirectory != rhs.automaticallyOpenFinalDirectory
        || this->defaultBodyFeet != rhs.defaultBodyFeet);
    }
  };

  struct Settings
  {
    GenericDialogSettings batchConversion;
    DisplaySettings display;
    GeneralSettings general;
    GenericDialogSettings presetCreator;
    GenericDialogSettings presetsRetargeting;

    QJsonObject toJson() const
    {
      QJsonObject lRootNode;
      lRootNode["batchConversion"] = this->batchConversion.toJson();
      lRootNode["display"] = this->display.toJson();
      lRootNode["general"] = this->general.toJson();
      lRootNode["presetCreator"] = this->presetCreator.toJson();
      lRootNode["presetsRetargeting"] = this->presetsRetargeting.toJson();

      return lRootNode;
    }

    bool operator==(const Struct::Settings& rhs) const
    {
      return (
        this->batchConversion == rhs.batchConversion
        && this->display == rhs.display
        && this->general == rhs.general
        && this->presetCreator == rhs.presetCreator
        && this->presetsRetargeting == rhs.presetsRetargeting);
    }

    bool operator!=(const Struct::Settings& rhs) const
    {
      return (
        this->batchConversion != rhs.batchConversion
        || this->display != rhs.display
        || this->general != rhs.general
        || this->presetCreator != rhs.presetCreator
        || this->presetsRetargeting != rhs.presetsRetargeting);
    }
  };

  struct AssistedConversionResult
  {
  public:
    explicit AssistedConversionResult(const QString& aPath, const QString& aName, const int aRole)
      : path(aPath)
      , name(aName)
      , role(aRole)
    {}

    const QString& getPath() const
    {
      return this->path;
    }

    const QString& getName() const
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

    AssistedConversionResult() = delete;
  };

  struct DatabaseSliderSet
  {
  public:
    explicit DatabaseSliderSet(const bool aIsActive,
                               const QString& aSliderSetName,
                               const MeshPartType aMeshType)
      : active(aIsActive)
      , sliderSetName(aSliderSetName)
      , meshType(aMeshType)
    {}

    // Active
    void setActive(const bool aIsActive)
    {
      this->active = aIsActive;
    }

    const bool& isActive() const
    {
      return this->active;
    }

    // Slider set name
    void setSliderSetName(const QString& aSliderSetName)
    {
      this->sliderSetName = aSliderSetName;
    }

    const QString& getSliderSetName() const
    {
      return this->sliderSetName;
    }

    // Mesh type
    void setMeshType(const MeshPartType aMeshType)
    {
      this->meshType = aMeshType;
    }

    const MeshPartType& getMeshType() const
    {
      return this->meshType;
    }

    // Operators overload
    bool operator==(const Struct::DatabaseSliderSet& rhs) const
    {
      return (this->active == rhs.active
              && this->sliderSetName == rhs.sliderSetName
              && this->meshType == rhs.meshType);
    }

    bool operator!=(const Struct::DatabaseSliderSet& rhs) const
    {
      return (this->active != rhs.active
              || this->sliderSetName != rhs.sliderSetName
              || this->meshType != rhs.meshType);
    }

  private:
    bool active;
    QString sliderSetName;
    MeshPartType meshType{MeshPartType::UNKNOWN};

    DatabaseSliderSet() = delete;
  };

  struct SliderSetResult
  {
  public:
    explicit SliderSetResult(const QString& aFilePath,
                             const QString& aOriginalSliderSetName,
                             const QString& aNewSliderSetName,
                             const MeshPartType aMeshType)
      : filePath(aFilePath)
      , originalSliderSetName(aOriginalSliderSetName)
      , newSliderSetName(aNewSliderSetName)
      , meshType(aMeshType)
    {}

    const QString& getFilePath() const
    {
      return this->filePath;
    }

    const QString& getOriginalSliderSetName() const
    {
      return this->originalSliderSetName;
    }

    const QString& getNewSliderSetName() const
    {
      return this->newSliderSetName;
    }

    const MeshPartType& getMeshType() const
    {
      return this->meshType;
    }

  private:
    QString filePath;
    QString originalSliderSetName;
    QString newSliderSetName;
    MeshPartType meshType{MeshPartType::UNKNOWN};

    SliderSetResult() = delete;
  };

  struct Filter
  {
  public:
    explicit Filter(const QString& aFilter, const bool aBody, const bool aFeet, const bool aHands)
      : filter(aFilter)
      , body(aBody)
      , feet(aFeet)
      , hands(aHands)
    {}

    const QString& getFilter() const
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
  };

  struct BatchConversionEntry
  {
  public:
    explicit BatchConversionEntry() {}

    explicit BatchConversionEntry(const QString& aOriginFolder, const QString& aResourcePath, const bool aUseAlternativeModel)
    {
      setOriginFolder(aOriginFolder);
      setResourcePath(aResourcePath);
      setUseAlternativeModel(aUseAlternativeModel);
    }

    // Origin directory
    void setOriginFolder(const QString& aOriginFolder)
    {
      this->originFolder = aOriginFolder;
    }

    const QString& getOriginFolder() const
    {
      return this->originFolder;
    }

    // Resource path
    void setResourcePath(const QString& aResourcePath)
    {
      this->resourcesPath = aResourcePath;
    }

    const QString& getResourcePath() const
    {
      return this->resourcesPath;
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
    QString resourcesPath;
    bool useAlternativeModel{false}; // Use beasts variants model
  };

  struct BatchConversionPresetData
  {
  public:
    explicit BatchConversionPresetData() {}

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
              && (!this->body.second.isEmpty() || !this->feet.second.isEmpty() || !this->hands.getResourcePath().isEmpty()));
    };

    // Names
    void setNames(const QString& aFilesNames, const QString& aPresetsNames)
    {
      this->names = std::make_pair(aFilesNames, aPresetsNames);
    }

    const std::pair<QString, QString>& getNames() const
    {
      return this->names;
    }

    // Body
    void setBodyData(const QString& aOriginFolder, const QString& aResourcePath)
    {
      this->body = std::make_pair(aOriginFolder, aResourcePath);
    }

    void resetBodyData()
    {
      this->setBodyData(QString(), QString());
    }

    const std::pair<QString, QString>& getBodyData() const
    {
      return this->body;
    }

    // Feet
    void setFeetData(const QString& aOriginFolder, const QString& aResourcePath)
    {
      this->feet = std::make_pair(aOriginFolder, aResourcePath);
    }

    void resetFeetData()
    {
      this->setFeetData(QString(), QString());
    }

    const std::pair<QString, QString>& getFeetData() const
    {
      return this->feet;
    }

    // Hands
    void setHandsData(const QString& aOriginFolder, const QString& aResourcePath, const bool aUseAlternativeModel)
    {
      this->hands.setOriginFolder(aOriginFolder);
      this->hands.setResourcePath(aResourcePath);
      this->hands.setUseAlternativeModel(aUseAlternativeModel);
    };

    void resetHandsData()
    {
      this->setHandsData(QString(), QString(), false);
    }

    const BatchConversionEntry& getHandsData() const
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
    void setSkeletonData(const QString& aOriginFolder, const QString& aResourcePath, const bool aUseAlternativeModel)
    {
      this->skeleton.setOriginFolder(aOriginFolder);
      this->skeleton.setResourcePath(aResourcePath);
      this->skeleton.setUseAlternativeModel(aUseAlternativeModel);
    };

    void resetSkeletonData()
    {
      this->setSkeletonData(QString(), QString(), false);
    }

    const BatchConversionEntry& getSkeletonData() const
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
    std::pair<QString, QString> body;  // <origin mod name, mesh path/name (aka full resource path)>
    std::pair<QString, QString> feet;  // <origin mod name, mesh path/name (aka full resource path)>
    BatchConversionEntry hands;        // <origin mod name, mesh path/name (aka full resource path), use alternate model>
    BatchConversionEntry skeleton;     // <origin mod name, mesh path/name (aka full resource path), mash name, use alternate model>
  };

  struct BatchConversionData
  {
  public:
    explicit BatchConversionData(const QString& aHumanSkeletonPath,
                                 const QString& aBeastSkeletonPath,
                                 const BodyNameVersion& aBodyMod,
                                 const FeetNameVersion& aFeetNameVersion,
                                 const std::vector<Struct::Filter>& aFilters,
                                 const QString& aFullOutputPath,
                                 const bool aMustGenerateFilesInExistingDirectory,
                                 const bool aMustGenerateEachPresetInADedicatedDirectory,
                                 const std::map<QString, std::vector<QString>>& aScannedData)
    {
      this->humanSkeletonPath = aHumanSkeletonPath;
      this->beastSkeletonPath = aBeastSkeletonPath;
      this->bodyMod = aBodyMod;
      this->feetMod = aFeetNameVersion;
      this->filters = aFilters;
      this->fullOutputPath = aFullOutputPath;
      this->mustGenerateFilesInExistingDirectory = aMustGenerateFilesInExistingDirectory;
      this->mustGenerateEachPresetInADedicatedDirectory = aMustGenerateEachPresetInADedicatedDirectory;
      this->scannedData = aScannedData;
    }

    // Human skeleton path
    void setHumanSkeletonPath(const QString& aHumanSkeletonPath)
    {
      this->humanSkeletonPath = aHumanSkeletonPath;
    }

    const QString& getHumanSkeletonPath() const
    {
      return this->humanSkeletonPath;
    }

    // Beast skeleton path
    void setBeastSkeletonPath(const QString& aBeastSkeletonPath)
    {
      this->beastSkeletonPath = aBeastSkeletonPath;
    }

    const QString& getBeastSkeletonPath() const
    {
      return this->beastSkeletonPath;
    }

    // Body mod
    void setBodyMod(const BodyNameVersion& aBodyMod)
    {
      this->bodyMod = aBodyMod;
    }

    BodyNameVersion getBodyMod() const
    {
      return this->bodyMod;
    }

    // Body mod
    void setFeetModIndex(const FeetNameVersion& aFeetNameVersion)
    {
      this->feetMod = aFeetNameVersion;
    }

    FeetNameVersion getFeetModIndex() const
    {
      return this->feetMod;
    }

    // Filters list
    void setFiltersList(const std::vector<Struct::Filter>& aFilters)
    {
      this->filters = aFilters;
    }

    const std::vector<Struct::Filter>& getFiltersList() const
    {
      return this->filters;
    }

    // Output path
    void setFullOutputPath(const QString& aFullOutputPath)
    {
      this->fullOutputPath = aFullOutputPath;
    }

    const QString& getFullOutputPath() const
    {
      return this->fullOutputPath;
    }

    // Must generate files in existing directory
    void setMustGenerateFilesInExistingDirectory(const bool aMustGenerateFilesInExistingDirectory)
    {
      this->mustGenerateFilesInExistingDirectory = aMustGenerateFilesInExistingDirectory;
    }

    bool getMustGenerateFilesInExistingDirectory() const
    {
      return this->mustGenerateFilesInExistingDirectory;
    }

    // Must generate each preset in a dedicated directory
    void setMustGenerateEachPresetInADedicatedDirectory(const bool aMustGenerateEachPresetInADedicatedDirectory)
    {
      this->mustGenerateEachPresetInADedicatedDirectory = aMustGenerateEachPresetInADedicatedDirectory;
    }

    bool getMustGenerateEachPresetInADedicatedDirectory() const
    {
      return this->mustGenerateEachPresetInADedicatedDirectory;
    }

    // Public attributes, because getters cannot return references to these objects
    std::map<QString, std::vector<QString>> scannedData;
    std::vector<BatchConversionPresetData> presets;

  private:
    QString humanSkeletonPath;
    QString beastSkeletonPath;
    BodyNameVersion bodyMod{BodyNameVersion::_INVALID_VALUE};
    FeetNameVersion feetMod{FeetNameVersion::_INVALID_VALUE};
    std::vector<Struct::Filter> filters;
    QString fullOutputPath;
    bool mustGenerateFilesInExistingDirectory{false};
    bool mustGenerateEachPresetInADedicatedDirectory{false};
  };

  struct NexusModsFileInformation
  {
  public:
    NexusModsFileInformation(const int aFileID,
                             const QString& aName,
                             const qint64 aDateTime,
                             const QString& aVersion,
                             const QString& aContentPreviewURL,
                             const QString& aCategoryName)
      : fileID{aFileID}
      , name{aName}
      , version{aVersion}
      , contentPreviewURL{aContentPreviewURL}
      , categoryName{aCategoryName}
    {
      dateTime.setSecsSinceEpoch(aDateTime);
    }

    const int& getFileID() const
    {
      return this->fileID;
    }

    const QString& getName() const
    {
      return this->name;
    }

    const QDateTime& getDate() const
    {
      return this->dateTime;
    }

    const QString& getVersion() const
    {
      return this->version;
    }

    const QString& getContentPreviewURL() const
    {
      return this->contentPreviewURL;
    }

    const QString& getCategoryName() const
    {
      return this->categoryName;
    }

  private:
    int fileID;
    QString name;
    QDateTime dateTime;
    QString version;
    QString contentPreviewURL;
    QString categoryName;
  };
}
