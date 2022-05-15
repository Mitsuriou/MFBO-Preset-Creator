#pragma once
#include "Struct.h"
#include <QGroupBox>
#include <vector>

class SSSPSelectionBlock final : public QGroupBox
{
public:
  SSSPSelectionBlock(QWidget* aParent, const Struct::SliderSet& aSliderSet);
};
