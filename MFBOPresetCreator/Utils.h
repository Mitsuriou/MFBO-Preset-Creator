#pragma once
#include "Struct.h"
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>

class Utils final : public QObject
{
  Q_OBJECT

public:
  static int const EXIT_CODE_REBOOT{-123456789};
  static bool RESTART_PENDING;

  // General functions
  static void cleanPathString(QString& aPath);
  static QString cleanPathString(const QString& aPath);
  static void cleanBreaksString(QString& aPath);
  static QString cleanBreaksString(const QString& aPath);
  static QStringList splitString(QString aString, const QString& aSeparator);
  static QString getApplicationVersion();
  static ApplicationVersionRelative compareVersionNumbers(const QString& aVersionNumber);
  static ApplicationVersionRelative compareVersionNumbers(const QString& aVersionNumber1, const QString& aVersionNumber2);
  static void displayWarningMessage(const QString& aMessage);
  static ButtonClicked displayQuestionMessage(QWidget* aParent, const QString& aTitle, const QString& aMessage, const QString& aIconFolder, const QString& aIconName, const QString& aTextBtnYes, const QString& aTextBtnNo, const QString& aTextBtnOther, const QString& aColorYesBtn, const QString& aColorNoBtn, const QString& aColorOtherBtn, const bool aIsYesBtnDefault);
  static int getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension);
  static int getNumberFilesByExtensionRecursive(const QString& aRootDir, const QString& aFileExtension);
  static int getNumberFilesByExtensionRecursiveIgnoringFOMOD(const QString& aRootDir, const QString& aFileExtension);
  static int getNumberFilesByExtensions(const QString& aRootDir, const QStringList& aFileExtensions);
  static int getNumberFilesByExtensionsRecursive(const QString& aRootDir, const QStringList& aFileExtensions);
  static bool copyRecursively(const QString& aSourcePath, const QString& aDestinationPath);
  static bool removeDirectoryAndSubDirs(const QString& aPath);
  static bool isThemeDark(const GUITheme& aTheme);
  static QString getIconRessourceFolder(const GUITheme& aTheme);
  static bool isCBBEBasedBody(const BodyNameVersion& aBody);
  static bool isBodySupportingBeastHands(const BodyNameVersion& aBody);
  static QString getAppDataPathFolder();
  static QString readQRCFileContent(const QString& aFilePath);

  // XML and OSP file parse
  static QString getPresetNameFromXMLFile(const QString& aPath);
  static std::vector<Struct::SliderSet> getOutputPathsFromOSPFile(const QString& aPath);
  static bool isPresetUsingBeastHands(const QString& aPath);

  // Preset values
  static QString getHandsSliderValue(const BodyNameVersion& aBody, const bool aMustUseBeastHands);
  static QString getFeetSliderValue(const BodyNameVersion& aBody, const int aFeetModIndex);
  static QString getBodySliderValue(const BodyNameVersion& aBody);

  // Load and save
  static void saveAsJsonFile(const QJsonObject& aJsonToSave, const QString& aFilePath, QWidget* aParent = nullptr, const QString& aIconFolder = "");
  static QJsonObject loadFromJsonFile(const QString& aFilePath);

  // Settings
  static void checkSettingsFileExistence();
  static Struct::Settings loadSettingsFromFile();
  static void saveSettingsToFile(const Struct::Settings& aSettings);
  static QJsonObject settingsStructToJson(const Struct::Settings& aSettings);

  // Filters
  static void checkFiltersFileExistence();
  static std::map<QString, QStringList> loadFiltersFromFile();
  static void saveFiltersToFile(const std::map<QString, QStringList>& aList);
  static QJsonObject filtersMapToJson(const std::map<QString, QStringList>& aList);
  static QString getAdditionalFeetFilter(const BodyNameVersion& aBody, const int aFeetModIndex);
  static std::vector<Struct::Filter> getFiltersForExport(const std::map<QString, QStringList>& aList, const QString& aKey, const BodyNameVersion& aBody, const int aFeetModIndex);

  // Paths
  static void checkLastPathsFileExistence();
  static std::map<QString, QString> loadLastPathsFromFile();
  static void saveLastPathsToFile(const std::map<QString, QString>& aLastPaths);
  static QJsonObject lastPathsStructToJson(const std::map<QString, QString>& aLastPaths);
  static QString getPathFromKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aFallbackPath, const bool aUseKeyPath);
  static void updatePathAtKey(std::map<QString, QString>* aMap, const QString& aKey, const QString& aPath, const bool aAuthorizeEmptyValue = false, const bool aMustSaveFile = true);

  // Language
  static QString getShortLanguageNameFromEnum(const int aEnumValue);
  static QString getLongLanguageNameFromEnum(const int aEnumValue);
  static ApplicationLanguage getStructLanguageFromName(const QString& aShortName);

  // Widgets builder
  static QAction* buildQAction(QWidget* aParent, const QString& aText, const QKeySequence& aKeysCombination, const QString& aIconName, const QString& aIconFolder);
  static void addIconToGroupBox(QGroupBox* aGroupBox, const QString& aIconFolder, const QString& aIconName);
  static void addLastPathLine(QWidget* aParent, QGridLayout* aLayout, const int aRow, const QString& aLabel, const QString& aValue, const QString& aIconFolder, const QString& aIconName);

  // GUI modifiers
  static void setGroupBoxState(QGroupBox* aGroupBox, const bool aIsCollapsed);
  static void updateComboBoxBodyslideFiltersList(const std::map<QString, QStringList>& aFilterList, QComboBox* aComboBox, QLabel* aFiltersLabel);
  static void updateOutputPreview(QLineEdit* aMainDirTextEdit, const QString& aSubDirectory, const bool aUseOnlySubdir, const QString& aSuccessColor, const QString& aWarningColor, const QString& aDangerColor, QLabel* aOutputPathsPreview);

  // Windows console
  static void bindConsoleToStdOut();
  static void printMessageStdOut(const QString& aMessage);

private:
  explicit Utils(){};
};
