#pragma once
#include "GroupBox.h"
#include "Struct.h"

class SliderSetsDBEntry final : public GroupBox
{
  Q_OBJECT

public:
  SliderSetsDBEntry(QWidget* aParent,
                    const GUITheme aApplicationTheme,
                    const int aPointSize,
                    const int aDatabaseIndex,
                    const Struct::DatabaseSliderSet& aDatabaseEntry);

signals:
  void nameUpdated(const int aIndex, const QString& aNewSliderSetName);
  void meshTypeUpdated(const int aIndex, const MeshPartType& aNewMeshType);
  void deleteButtonClicked(const int aIndex);
  void viewContentButtonClicked(const int aIndex);

private:
  const int mDatabaseIndex;

  void initializeGUI(const Struct::DatabaseSliderSet& aDatabaseEntry, const GUITheme aApplicationTheme);

  // Events handlers
  void sliderSetNameChanged(const QString& aNewSliderSetName);
};
