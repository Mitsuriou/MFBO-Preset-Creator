#pragma once
#include "Struct.h"
#include <QDialog>
#include <QFileSystemWatcher>
#include <QGridLayout>
#include <set>

class BatchConversion final : public QDialog
{
  Q_OBJECT

public:
  explicit BatchConversion(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);
  ~BatchConversion();

signals:
  void modalClosed();

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  QFileSystemWatcher* mFileWatcher{nullptr};
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths;
  std::map<QString, QStringList> mFiltersList;
  bool mHasUserDoneSomething;
  int mMinimumFirstColumnWidth;
  BodyNameVersion mTargetBodyMesh;
  FeetNameVersion mTargetFeetMesh;

  void setWindowProperties();
  void setupGeneralGUI(QGridLayout& aLayout);
  void setupSkeletonGUI(QGridLayout& aLayout);
  void setupBodySlideGUI(QGridLayout& aLayout);
  void setupOutputGUI(QGridLayout& aLayout);
  void setupScanTweaksGUI(QGridLayout& aLayout);
  void setupGenerationAdjustmentGUI(QGridLayout& aLayout);
  void setupButtons(QHBoxLayout& aLayout);

  void launchPicker(const std::map<QString, std::set<QString>>& aScannedData, const bool aMustGenerateFilesInExistingDirectory);

  void userHasDoneAnAction();
  void userHasDoneAnAction(int);

  void chooseInputDirectory();
  void launchSearchProcess();
  void batchCreatePresets(const Struct::BatchConversionData& aPresetsData);
  void populateSkeletonChoosers();
  void openSkeletonsAssetsDirectory();

  // Update the GUI preview
  void useOnlySubdirStateChanged(int);
  void updateOutputPreview();

  void chooseExportDirectory();

  // Target meshes pikcker
  void openTargetMeshesPicker();
  void targetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);

  // BodySlide Filters Editor
  void openBodySlideFiltersEditor();
  void initBodySlideFiltersList();
  void updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList);
  void updateBodySlideFiltersListPreview();

  explicit BatchConversion(const BatchConversion&) = delete;
  BatchConversion& operator=(const BatchConversion&) = delete;
};
