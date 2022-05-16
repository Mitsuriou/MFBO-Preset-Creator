#pragma once
#include "GroupBox.h"
#include "Struct.h"
#include <vector>

class SSSPSelectionBlock final : public GroupBox
{
public:
  SSSPSelectionBlock(QWidget* aParent,
                     const GUITheme& aApplicationTheme,
                     const int aPointSize,
                     const QString& aFullFilePath,
                     const Struct::SliderSet& aSliderSet);

private:
  void initializeGUI(const QString& aFullFilePath, const Struct::SliderSet& aSliderSet);

  // Events handlers
  void presetNameChanged(const QString& aNewPresetName);
};
