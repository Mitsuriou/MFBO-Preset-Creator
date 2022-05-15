#pragma once
#include "Struct.h"
#include "VersionsInformation.h"
#include <QComboBox>
#include <QFileSystemWatcher>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <set>

#if defined(_MSC_VER) && !defined(DLLEXP)
#define DLLEXP __declspec(dllexport)
#elif !defined(DLLEXP)
#define DLLEXP
#endif

class Utils final : public QObject
{
  Q_OBJECT

public:
  static int const EXIT_CODE_REBOOT{-123456789};
  static bool RESTART_PENDING;

  // String
  static void CleanPathString(QString& aPath);
  [[nodiscard]] static QString CleanPathString(const QString& aPath);
  static void CleanBreaksString(QString& aString);
  [[nodiscard]] static QString CleanBreaksString(const QString& aString);

  // Convert types
  static std::map<QString, std::vector<QString>> ToMapQsVecQs(const std::map<QString, std::set<QString>>& aMap);

  // Version
  static QString GetApplicationVersion();
  static ApplicationVersionRelative CompareVersionNumbers(const QString& aVersionNumber);
  static ApplicationVersionRelative CompareVersionNumbers(const QString& aVersionNumber1, const QString& aVersionNumber2);

  // Message boxes
  static void DisplayInfoMessage(QWidget* aParent, const QString& aTitle, const QString& aMessage, const QString& aIconFolder, const QString& aIconName, const QString& aButtonText);
  static void DisplayWarningMessage(const QString& aMessage);
  static ButtonClicked DisplayQuestionMessage(QWidget* aParent, const QString& aTitle, const QString& aMessage, const QString& aIconFolder, const QString& aIconName, const QString& aTextBtnYes, const QString& aTextBtnNo, const QString& aTextBtnOther, const QString& aColorYesBtn, const QString& aColorNoBtn, const QString& aColorOtherBtn, const bool aIsYesBtnDefault);
  static void DisplayErrorMessage(const QString& aMessage);

  // GitHub API
  static VersionsInformation ParseGitHubReleasesRequestResult(const QString& aResult);

  // Get disk files by extension
  static int GetNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension);
  static int GetNumberFilesByExtensionRecursive(const QString& aRootDir, const QString& aFileExtension);
  static int GetNumberFilesByExtensionRecursiveIgnoringFOMOD(const QString& aRootDir, const QString& aFileExtension);
  static int GetNumberFilesByExtensions(const QString& aRootDir, const QStringList& aFileExtensions);
  static int GetNumberFilesByExtensionsRecursive(const QString& aRootDir, const QStringList& aFileExtensions);

  // Manipulates files and directories
  static bool CopyRecursively(const QString& aSourcePath, const QString& aDestinationPath);
  static bool RemoveDirectoryAndSubDirs(const QString& aPath);

  // Theming
  static void ApplyApplicationStyleSheet(const QString& aQSSFileName);
  static bool IsThemeDark(const GUITheme& aTheme);
  static QString GetIconResourceFolder(const GUITheme& aTheme);
  static void OverrideHTMLLinksColor(QString& aHTMLString, const GUITheme& aTheme);

  // Skyrim related stuff
  static bool CreateXMLFile(const QString& aEntryDirectory, const bool aGenerateFilesInExistingMainDirectory, const QString& aOSPXMLNames, const bool aMustUseBeastHands, const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion, const QString& aBodyslideSlidersetsNames, const std::vector<Struct::Filter>& aBodySlideFilters, const bool aIsBatchConversionPreset, const unsigned char& aTargetBlocks);
  static bool CreateOSPFile(const QString& aEntryDirectory, const bool aGenerateFilesInExistingMainDirectory, const QString& aOSPXMLNames, const bool aMustUseBeastHands, const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion, const QString& aBodyslideSlidersetsNames, QString aMeshesPathBody, QString aMeshesPathFeet, QString aMeshesPathHands, const QString& aBodyName, const QString& aFeetName, const QString& aHandsName, const bool aIsBatchConversionPreset, const unsigned char& aTargetBlocks);
  static void CreateTextureFile(const QString& aSourcePath, const QString& aDestinationEntryDirectory, const QString& aDestinationRelativePath, const QString& aDestinationFileName);
  static bool CreateSkeletonFile(const QString& aSourcePath, const QString& aDestinationEntryDirectory, const QString& aDestinationRelativePath, const QString& aDestinationFileName);

  static BCGroupWidgetCallContext GetMeshTypeFromFileName(const QString& aFileName);
  template<typename T>
  static bool ContainsBodyOrHandsOrFeetMesh(const T& aList);
  static void ClearUselessEntries(std::map<QString, std::set<QString>>& aScannedData);

  static DLLEXP bool IsCBBEBasedBody(const BodyName& aBodyName);
  static DLLEXP bool IsCBBEBasedBody(const BodyVariant& aBodyVariant);
  static DLLEXP bool IsCBBEBasedBody(const BodyNameVersion& aBodyNameVersion);
  static bool IsBodySupportingBeastHands(const BodyNameVersion& aBodyNameVersion);

  static DLLEXP bool IsVersionOffsetValid(const BodyVariant& aBodyVariant, const int aRelativeVersion);
  static DLLEXP bool IsVersionOffsetValid(const FeetVariant& aFeetVariant, const int aRelativeVersion);

  static DLLEXP bool QRCResourceExists(const QString& aRessourcePath);

  // App data behavior settings
  static bool IsRunningStandaloneVersion();
  static QString GetAppDataPathFolder();

  // Read virtual file from QRC
  static QString ReadQRCFileContent(const QString& aFilePath);

  // XML and OSP file parse
  static QString GetPresetNameFromXMLFile(const QString& aPath);
  static std::vector<Struct::SliderSet> ReadOSPFileInformation(const QString& aPath, const bool aSkipTypeAnalysis = false);
  static bool AlreadyContainsMeshPartType(const std::vector<Struct::SliderSet>& aSliderSetsList, const MeshPartType& aMeshPartType);
  static std::pair<bool, Struct::SliderSet> GetSliderSetByMeshPartType(const std::vector<Struct::SliderSet>& aSliderSetsList, const MeshPartType& aMeshPartType);
  static bool IsPresetUsingBeastHands(const QString& aPath);

  // Preset values
  static QString GetBodySliderValue(const BodyNameVersion& aBodyNameVersion);
  static QString GetFeetSliderValue(const FeetNameVersion& aFeetNameVersion);
  static QString GetHandsSliderValue(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands);

  // Load and save
  static void SaveAsJsonFile(const QJsonObject& aJsonToSave, const QString& aFilePath, QWidget* aParent = nullptr, const QString& aIconFolder = QString());
  static QJsonObject LoadFromJsonFile(const QString& aFilePath);

  // Settings
  static QString SettingsFullFilePath();
  static bool SettingsFileExists();

  static Struct::Settings LoadSettingsFromFile();
  static void ParseSettingsCompatibility(Struct::Settings& aSettings, const QJsonObject& aJSONObject);
  static void ParseSettings(Struct::Settings& aSettings, const QJsonObject& aJSONObject, const QString& aSettingsFileVersion);
  static void ParseDisplaySettings(Struct::DisplaySettings& aSettings, const QJsonObject& aJSONObject);
  static void ParseFontSettings(Struct::Font& aSettings, const QJsonObject& aJSONObject);
  static void ParseGeneralSettings(Struct::GeneralSettings& aSettings, const QJsonObject& aJSONObject);
  static void ParseGenericDialogSettings(Struct::GenericDialogSettings& aSettings, const QJsonObject& aJSONObject, const QString& aSettingsFileVersion);
  static void ParseBodyFeetSettings(Struct::BodyFeetSettings& aSettings, const QJsonObject& aJSONObject, const QString& aSettingsFileVersion);

  static void SaveSettingsToFile(const Struct::Settings& aSettings);
  static QJsonObject SettingsStructToJson(const Struct::Settings& aSettings);

  // Filters
  static void CheckFiltersFileExistence();
  static std::map<QString, QStringList> LoadFiltersFromFile();
  static void SaveFiltersToFile(const std::map<QString, QStringList>& aList);
  static QJsonObject FiltersMapToJson(const std::map<QString, QStringList>& aList);
  static QString GetAdditionalFeetFilter(const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion);
  static std::vector<Struct::Filter> GetFiltersForExport(const std::map<QString, QStringList>& aList, const QString& aKey, const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion);

  // Paths
  static void CheckLastPathsFileExistence();
  static std::map<QString, QString> LoadLastPathsFromFile();
  static void SaveLastPathsToFile(const std::map<QString, QString>& aLastPaths);
  static QJsonObject LastPathsStructToJson(const std::map<QString, QString>& aLastPaths);
  static QString GetPathFromKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aFallbackPath, const bool aUseKeyPath);
  static bool UpdatePathAtKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aPath, const bool aMustUpdateGeneralKey = true, const bool aAuthorizeEmptyValue = false, const bool aMustSaveFile = true);

  // API
  static bool SaveAPIKeyToFile(const QString& aAPIKey, QWidget* aParent = nullptr, const QString& aIconFolder = QString());
  static QString ReadAPIKeyFromFile();

  // Language
  static QString GetShortLanguageNameFromEnum(const int aEnumValue);
  static QString GetShortLanguageNameFromEnum(const ApplicationLanguage& aLanguage);
  static ApplicationLanguage GetStructLanguageFromName(const QString& aShortName);

  // Widgets builder
  static QAction* BuildQAction(QWidget* aParent, const QString& aText, const QKeySequence& aKeysCombination, const QString& aIconName, const QString& aIconFolder);
  static void AddLastPathLine(QWidget* aParent, QGridLayout* aLayout, const int aRow, const QString& aLabel, const QString& aValue, const QString& aIconFolder, const QString& aIconName);

  // GUI modifiers
  static void SelectComboBoxAt(QComboBox* aComboBox, const int aIndex);
  static void UpdateComboBoxBodyslideFiltersList(const std::map<QString, QStringList>& aFilterList, QComboBox* aComboBox, QLabel* aFiltersLabel);
  static void UpdateOutputPreview(QFileSystemWatcher* aFileWatcher,
                                  QLineEdit* aMainDirTextEdit,
                                  const QString& aSubDirectory,
                                  const bool aUseOnlySubdir,
                                  const QString& aSuccessColor,
                                  const QString& aWarningColor,
                                  const QString& aDangerColor,
                                  QLabel* aOutputPathsPreview);

  // Windows console
  static void BindConsoleToStdOut();
  static void PrintMessageStdOut(const QString& aMessage);

private:
  explicit Utils() {}
  explicit Utils(const Utils&) = delete;
  Utils& operator=(const Utils&) = delete;
};

template<typename T>
inline bool Utils::ContainsBodyOrHandsOrFeetMesh(const T& aList)
{
  static_assert(std::is_same<typename T::value_type, QString>::value, "T must be a standard container of QString");

  auto lResourceType{BCGroupWidgetCallContext::UNDEFINED};

  // Iterate through the list of meshes paths
  for (const auto& lValue : aList)
  {
    lResourceType = Utils::GetMeshTypeFromFileName(lValue);

    // Check if the resource if of any useful type
    if (lResourceType == BCGroupWidgetCallContext::BODY || lResourceType == BCGroupWidgetCallContext::FEET || lResourceType == BCGroupWidgetCallContext::HANDS)
    {
      return true;
    }
  }

  return false;
}
