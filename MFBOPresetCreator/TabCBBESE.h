#pragma once

#include "BodySlideFiltersEditor.h"
#include "Tab.h"

class TabCBBESE : public Tab
{
  Q_OBJECT

public:
  explicit TabCBBESE(QWidget* aParent, const Struct::Settings& aSettings);

private:
  int mMinimumFirstColumnWidth;

  void setupBodyMeshesGUI(QVBoxLayout& aLayout);
  void setupBodySlideGUI(QVBoxLayout& aLayout);
  void setupSkeletonGUI(QVBoxLayout& aLayout);
  void setupOutputGUI(QVBoxLayout& aLayout);
  void setupRemainingGUI(QVBoxLayout& aLayout);

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
};