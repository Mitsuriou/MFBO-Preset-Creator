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

    bool operator==(const Struct::GeneralSettings& rhs)
    {
      return (
        this->eachButtonSavesItsLastUsedPath == rhs.eachButtonSavesItsLastUsedPath
        && this->startupAction == rhs.startupAction);
    }

    bool operator!=(const Struct::GeneralSettings& rhs)
    {
      return (
        this->eachButtonSavesItsLastUsedPath != rhs.eachButtonSavesItsLastUsedPath
        || this->startupAction != rhs.startupAction);
    }
  };

  struct DisplaySettings
  {
  public:
    GUITheme applicationTheme{GUITheme::MITSURIOU_DARK_THEME};
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
    QString successColor{"#48c774"};
    DialogOpeningMode texturesAssistantDialogOpeningMode{DialogOpeningMode::WINDOWED};
    QString warningColor{"#ffbc6b"};

    QJsonObject toJson() const
    {
      // Font
      QJsonObject lFontNode;
      lFontNode["family"] = this->font.family;
      lFontNode["italic"] = this->font.italic;
      lFontNode["size"] = this->font.size;
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
      lDisplayNode["successColor"] = this->successColor;
      lDisplayNode["texturesAssistantDialogOpeningMode"] = static_cast<int>(this->texturesAssistantDialogOpeningMode);
      lDisplayNode["warningColor"] = this->warningColor;

      return lDisplayNode;
    }

    bool operator==(const Struct::DisplaySettings& rhs)
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
        && this->successColor == rhs.successColor
        && this->texturesAssistantDialogOpeningMode == rhs.texturesAssistantDialogOpeningMode
        && this->warningColor == rhs.warningColor);
    };

    bool operator!=(const Struct::DisplaySettings& rhs)
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
        || this->successColor != rhs.successColor
        || this->texturesAssistantDialogOpeningMode != rhs.texturesAssistantDialogOpeningMode
        || this->warningColor != rhs.warningColor);
    };
  };

  struct BodyFeetSettings
  {
  public:
    BodyNameVersion bodyMod{BodyNameVersion::CBBE_3BBB_3BA_1_50};
    int feetMod{0};

    QJsonObject toJson() const
    {
      QJsonObject lNode;
      lNode["bodyMod"] = static_cast<int>(this->bodyMod);
      lNode["feetMod"] = this->feetMod;
      return lNode;
    }

    bool operator==(const Struct::BodyFeetSettings& rhs)
    {
      return (
        this->bodyMod == rhs.bodyMod
        && this->feetMod == rhs.feetMod);
    }

    bool operator!=(const Struct::BodyFeetSettings& rhs)
    {
      return (
        this->bodyMod != rhs.bodyMod
        || this->feetMod != rhs.feetMod);
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

    bool operator==(const Struct::GenericDialogSettings& rhs)
    {
      return (
        this->automaticallyOpenFinalDirectory == rhs.automaticallyOpenFinalDirectory
        && this->defaultBodyFeet == rhs.defaultBodyFeet);
    }

    bool operator!=(const Struct::GenericDialogSettings& rhs)
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

    bool operator==(const Struct::Settings& rhs)
    {
      return (
        this->batchConversion == rhs.batchConversion
        && this->display == rhs.display
        && this->general == rhs.general
        && this->presetCreator == rhs.presetCreator
        && this->presetsRetargeting == rhs.presetsRetargeting);
    }

    bool operator!=(const Struct::Settings& rhs)
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

    explicit AssistedConversionResult() = delete;
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

    explicit Filter() = delete;
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

    const std::pair<QString, QString>& getNames() const
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

    const std::pair<QString, QString>& getBodyData() const
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

    const std::pair<QString, QString>& getFeetData() const
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
    std::pair<QString, QString> body;  // <origin mod name, mesh path/name (aka full ressource path)>
    std::pair<QString, QString> feet;  // <origin mod name, mesh path/name (aka full ressource path)>
    BatchConversionEntry hands;        // <origin mod name, mesh path/name (aka full ressource path), use alternate model>
    BatchConversionEntry skeleton;     // <origin mod name, mesh path/name (aka full ressource path), mash name, use alternate model>
  };

  struct BatchConversionData
  {
    QString humanSkeletonPath;
    QString beastSkeletonPath;
    BodyNameVersion bodyMod{BodyNameVersion::INVALID_VALUE};
    int feetModIndex{-1};
    std::vector<Struct::Filter> filters;
    QString fullOutputPath;
    bool mustGenerateFilesInExistingDirectory{false};
    std::map<QString, std::vector<QString>> scannedData;
    std::vector<BatchConversionPresetData> presets;

    // TODO: Make this structure cleaner (with a constructor and private attributes)
  };
}
