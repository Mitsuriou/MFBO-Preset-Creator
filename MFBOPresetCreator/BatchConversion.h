#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>

class BatchConversion final : public QDialog
{
  Q_OBJECT

public:
  explicit BatchConversion(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths;
  std::map<QString, QStringList> mFiltersList;
  bool mHasUserDoneSomething;
  int mMinimumFirstColumnWidth;

  void setWindowProperties();
  void setupGeneralGUI(QGridLayout& aLayout);
  void setupSkeletonGUI(QGridLayout& aLayout);
  void setupBodySlideGUI(QGridLayout& aLayout);
  void setupOutputGUI(QGridLayout& aLayout);
  void setupRemainingGUI(QHBoxLayout& aLayout);

  void launchPicker(const std::map<QString, std::set<QString>>& aScannedData);

  void userHasDoneAnAction();

#pragma region PRIVATE_SLOTS
  void userHasDoneAnAction(int);
  void updateAvailableBodyVersions();
  void chooseInputDirectory();
  void launchBatchGenerationProcess();
  void populateSkeletonChoosers();

  // Update the GUI preview
  void useOnlySubdirStateChanged(int);
  void updateOutputPreview();

  void chooseExportDirectory();

  // BodySlide Filters Editor
  void openBodySlideFiltersEditor();
  void initBodySlideFiltersList();
  void updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList);
  void updateBodySlideFiltersListPreview(int aIndex);

  // GUI widgets events
  void scrollbarPressed();
  void scrollbarReleased();
  void groupBoxChecked(bool aIsChecked);
#pragma endregion PRIVATE_SLOTS
};
