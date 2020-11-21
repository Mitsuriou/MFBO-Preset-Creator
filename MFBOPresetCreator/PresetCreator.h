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
  explicit PresetCreator(QWidget* aParent, const Struct::Settings& aSettings);
  void updateSettings(Struct::Settings aSettings);
  void fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList);

private:
  Struct::Settings mSettings;
  int mMinimumFirstColumnWidth;

  void setupBodyMeshesGUI(QVBoxLayout& aLayout);
  void setupBodySlideGUI(QVBoxLayout& aLayout);
  void setupSkeletonGUI(QVBoxLayout& aLayout);
  void setupOutputGUI(QVBoxLayout& aLayout);
  void setupRemainingGUI(QVBoxLayout& aLayout);

  QStringList bodySlideFiltersStringToList();

  // Files generation
  bool generateXMLFile(const QString& aEntryDirectory,
                       const bool& aGenerateFilesInExistingMainDirectory,
                       const QString& aOSPXMLNames,
                       const bool& aMustUseBeastHands,
                       const QString& aRessourcesFolder,
                       const int& aBodySelected,
                       const QString& aBodyslideSlidersetsNames,
                       QString aMeshesPathBody,
                       QString aMeshesPathFeet,
                       QString aMeshesPathHands);
  bool generateOSPFile(const QString& aEntryDirectory,
                       const bool& aGenerateFilesInExistingMainDirectory,
                       const QString& aOSPXMLNames,
                       const bool& aMustUseBeastHands,
                       const QString& aRessourcesFolder,
                       const QString& aBodyslideSlidersetsNames,
                       QString aMeshesPathBody,
                       QString aMeshesPathFeet,
                       QString aMeshesPathHands,
                       const QString& aBodyName,
                       const QString& aFeetName,
                       const QString& aHandsName);
  bool generateSkeletonFile(const QString& aEntryDirectory, const QString& aSkeletonPath);

private slots:
  void populateSkeletonChooser();
  void updateMeshesPreview();
  void updateOutputPreview();
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);
  void updateSkeletonPathState(int aState);
  void updateSkeletonPreview();
  void chooseExportDirectory();
  void generateDirectoryStructure();
  void refreshAllPreviewFields(int);
  void refreshAllPreviewFields();
  void openBodySlideFiltersEditor();
  void updateBodySlideFiltersList(QStringList aList);
  void mouseCursorPressed();
  void mouseCursorReleased();
};