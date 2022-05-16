#pragma once
#include "Struct.h"
#include <QFileSystemWatcher>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QWidget>

class PresetCreator final : public QWidget
{
  Q_OBJECT

public:
  explicit PresetCreator(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);
  ~PresetCreator();

  // Function that will to be called from outside this class
  void loadProject(const QString& lFilePath = QString(), const bool aSkipFileChooser = false);
  void saveProject(const bool aIsSaveAsContext);
  void injectDataFromOSPFile();
  void setHasUserDoneSomething(const bool aHasUserDoneSomething);
  bool hasUserDoneSomething();
  void updateSettings(Struct::Settings aSettings);
  void updateBodySlideSets();
  void fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList);

private:
  QFileSystemWatcher* mFileWatcher{nullptr};
  Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths{nullptr};
  std::map<QString, QStringList> mFiltersList;
  bool mHasUserDoneSomething;
  QString mLastUsedSavePath;
  int mMinimumFirstColumnWidth;
  BodyNameVersion mTargetBodyMesh;
  FeetNameVersion mTargetFeetMesh;

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
  void updateBeastHandsCheckboxState();

  //#pragma region PRIVATE_SLOTS
  void populateSkeletonChooser();
  void openSkeletonsAssetsDirectory();

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

  // Target meshes pikcker
  void openTargetMeshesPicker();
  void targetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);

  // BodySlide Filters Editor
  void openBodySlideFiltersEditor();
  void initBodySlideFiltersList();
  void updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList);
  void updateBodySlideFiltersListPreview();
  //#pragma endregion PRIVATE_SLOTS

  void testtt();
};