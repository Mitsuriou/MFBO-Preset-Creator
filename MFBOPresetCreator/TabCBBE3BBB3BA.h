#pragma once

#include "BodySlideFiltersEditor.h"
#include "Tab.h"

class TabCBBE3BBB3BA : public Tab
{
  Q_OBJECT

public:
  explicit TabCBBE3BBB3BA(QWidget* aParent, const Struct::Settings& aSettings);
  void fillUIByAssistedConversionValues(QString aPresetName, std::vector<Struct::AssistedConversionResult> aResultsList);

private:
  int mMinimumFirstColumnWidth;

  void setupBodyMeshesGUI(QVBoxLayout& aLayout);
  void setupBodySlideGUI(QVBoxLayout& aLayout);
  void setupSkeletonGUI(QVBoxLayout& aLayout);
  void setupOutputGUI(QVBoxLayout& aLayout);
  void setupRemainingGUI(QVBoxLayout& aLayout);

  QStringList bodySlideFiltersStringToList();

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
};