#pragma once
#include "TitleDialog.h"

class PresetsDatabaseManager final : public TitleDialog
{
  Q_OBJECT

public:
  explicit PresetsDatabaseManager(QWidget* aParent,
                                  const Struct::Settings& aSettings,
                                  std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  void openSliderSetsImporter();
  void importNewPresets(const std::vector<Struct::SliderSetResult>& aChosenPresets);
};
