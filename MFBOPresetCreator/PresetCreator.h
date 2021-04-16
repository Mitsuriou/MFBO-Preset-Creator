#pragma once

#include "BodySlideFiltersEditor.h"
#include "DataLists.h"
#include "Settings.h"
#include "Utils.h"
#include "stdafx.h"

class PresetCreator : public QWidget
{
  Q_OBJECT

public:
  explicit PresetCreator(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

  // Function that will to be called from outside this class
  void loadProject(const QString& lFilePath = "", const bool& aSkipFileChooser = false);
  void saveProject(const bool& aIsSaveAsContext);
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
  void setupBodyMeshesGUI(QVBoxLayout& aLayout);
  void setupBodySlideGUI(QVBoxLayout& aLayout);
  void setupSkeletonGUI(QVBoxLayout& aLayout);
  void setupOutputGUI(QVBoxLayout& aLayout);
  void setupRemainingGUI(QVBoxLayout& aLayout);

  // Load and save project
  void loadValuesFromJsonObject(const QJsonObject& lFile);
  QJsonObject saveValuesToJsonObject();

  // GUI update scripts
  void updateGUIOnBodyChange();
  void updateAvailableBodyVersions();

  // Files generation
  bool generateXMLFile(const QString& aEntryDirectory, const bool& aGenerateFilesInExistingMainDirectory, const QString& aOSPXMLNames, const bool& aMustUseBeastHands, const QString& aRessourcesFolder, const int& aBodySelected, const QString& aBodyslideSlidersetsNames);
  bool generateOSPFile(const QString& aEntryDirectory, const bool& aGenerateFilesInExistingMainDirectory, const QString& aOSPXMLNames, const bool& aMustUseBeastHands, const QString& aRessourcesFolder, const QString& aBodyslideSlidersetsNames, QString aMeshesPathBody, QString aMeshesPathFeet, QString aMeshesPathHands, const QString& aBodyName, const QString& aFeetName, const QString& aHandsName);
  bool generateSkeletonFile(const QString& aEntryDirectory, const QString& aSkeletonPath);

private slots:
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
  void updateBodySlideFiltersListPreview(int aIndex);

  // GUI widgets events
  void scrollbarPressed();
  void scrollbarReleased();
  void preventGroupBoxCheckEvent(bool aIsChecked);
};