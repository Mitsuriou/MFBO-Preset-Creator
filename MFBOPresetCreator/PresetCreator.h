#pragma once
#include "Struct.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QWidget>

class PresetCreator final : public QWidget
{
  Q_OBJECT

public:
  explicit PresetCreator(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

  // Function that will to be called from outside this class
  void loadProject(const QString& lFilePath = QString(""), const bool aSkipFileChooser = false);
  void saveProject(const bool aIsSaveAsContext);
  void setHasUserDoneSomething(const bool aHasUserDoneSomething);
  bool hasUserDoneSomething();
  void updateSettings(Struct::Settings aSettings);
  void updateBodySlideSets();
  void fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList);

private:
  Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths;
  std::map<QString, QStringList> mFiltersList;
  bool mHasUserDoneSomething;
  QString mLastUsedSavePath;
  int mMinimumFirstColumnWidth;

  // GUI creation
  void setupBodyMeshesGUI(QGridLayout& aLayout);
  void setupSkeletonGUI(QGridLayout& aLayout);
  void setupBodySlideGUI(QGridLayout& aLayout);
  void setupOutputGUI(QGridLayout& aLayout);
  void setupButtons(QHBoxLayout& aLayout);

  // Load and save project
  void loadValuesFromJsonObject(const QJsonObject& lFile);
  QJsonObject saveValuesToJsonObject();

  // GUI update scripts
  void updateGUIOnBodyChange();
  void updateAvailableBodyVersions();

  // Files generation
  bool generateXMLFile(const QString& aEntryDirectory, const bool aGenerateFilesInExistingMainDirectory, const QString& aOSPXMLNames, const bool aMustUseBeastHands, const int aBodySelected, const int aFeetModIndex, const QString& aBodyslideSlidersetsNames);
  bool generateOSPFile(const QString& aEntryDirectory, const bool aGenerateFilesInExistingMainDirectory, const QString& aOSPXMLNames, const bool aMustUseBeastHands, const int aBodySelected, const int aFeetModIndex, const QString& aBodyslideSlidersetsNames, QString aMeshesPathBody, QString aMeshesPathFeet, QString aMeshesPathHands, const QString& aBodyName, const QString& aFeetName, const QString& aHandsName);
  bool generateSkeletonFile(const QString& aEntryDirectory, const QString& aSkeletonPath);

#pragma region PRIVATE_SLOTS
  void populateSkeletonChooser();

  // Update the GUI preview
  void updateMeshesPreview();
  void useOnlySubdirStateChanged(int);
  void updateOutputPreview();
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);
  void updateSkeletonPathState(int aState);
  void updateSkeletonPreview();

  void chooseExportDirectory();

  void generateDirectoryStructure();

  void refreshAllPreviewFields(int);
  void refreshAllPreviewFields();

  // BodySlide Filters Editor
  void openBodySlideFiltersEditor();
  void initBodySlideFiltersList();
  void updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList);
  void updateBodySlideFiltersListPreview();

  // GUI widgets events
  void groupBoxChecked(bool aIsChecked);
#pragma endregion PRIVATE_SLOTS

  explicit PresetCreator(const PresetCreator&) = delete;
  PresetCreator& operator=(const PresetCreator&) = delete;
};