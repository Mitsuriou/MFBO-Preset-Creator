#pragma once
#include "BCDragWidget.h"
#include "BCGroupWidget.h"
#include "Struct.h"
#include <QDialog>
#include <map>
#include <set>

class BatchConversionPicker final : public QDialog
{
  Q_OBJECT

public:
  explicit BatchConversionPicker(QWidget* aParent, const Struct::Settings& aSettings, const Struct::BatchConversionData& aData);

signals:
  void presetsCreationValidated(const Struct::BatchConversionData& aPresetsData);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  int mMinimumFirstColumnWidth;
  Struct::BatchConversionData mData;
  std::vector<BCDragWidget*> mMiddleListButtons;
  bool mPreventPresetSave{true};

  // General
  void setWindowProperties();
  void initializeGUI();
  void connectGroupWidgetEvents(BCGroupWidget* lGroupWidget);
  void displayLeftList();

  // Update the GUI
  void refreshLeftListFont();
  void refreshMiddleList();
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);

  // Through-layer communications
  void removeDataFromActiveMiddleList(const QString& aOriginFolder, const QString& aResourcePath, const bool isCheckBoxChecked = false);
  void addDataToActiveMiddleList(const QString& aOriginFolder, const QString& aResourcePath);
  void handsCheckBoxStateChanged(const bool aIsActive);
  void skeletonCheckBoxStateChanged(const bool aIsActive);

  // Data update
  void saveBodySlideDataToPreset();

  // Presets controls
  void goToPreviousPreset() const;
  void goToNextPreset() const;
  void removeActivePreset();
  void addNewEmptyPreset();
  void updatePresetInterfaceState(const int aNextIndex);
  void updateActivePresetNumberSpinBox();

  // Presets manipulation
  void fullQuickCreatePreset();
  void simpleQuickCreatePreset();
  std::map<QString, std::set<QString>> findNewPresets(const QString& aOriginFolder);
  void generateNewPresets(const std::multimap<QString, std::map<QString, std::set<QString>>>& aPresets); // multimap<origin_folder, map<path, set<mesh_file_name>>>
  void validateSelection();

  explicit BatchConversionPicker(const BatchConversionPicker&) = delete;
  BatchConversionPicker& operator=(const BatchConversionPicker&) = delete;
};
