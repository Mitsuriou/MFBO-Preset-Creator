#pragma once
#include "GroupBox.h"
#include "Struct.h"
#include <vector>

class SSSPSelectionBlock final : public GroupBox
{
  Q_OBJECT

public:
  SSSPSelectionBlock(QWidget* aParent,
                     const GUITheme& aApplicationTheme,
                     const int aPointSize,
                     const QString& aFullFilePath,
                     const Struct::SliderSet& aSliderSet);

  bool isCheckedForImport() const;
  MeshPartType getCurrentlySetMeshPartType() const;
  Struct::SliderSetResult getData() const;

private:
  const QString mFullFilePath;
  const QString mOriginalSliderSetName;

  void initializeGUI(const Struct::SliderSet& aSliderSet);

  // Events handlers
  void presetNameChanged(const QString& aNewPresetName);
};
