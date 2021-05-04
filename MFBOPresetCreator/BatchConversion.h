#pragma once

#include "BodySlideFiltersEditor.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Enum.h"
#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"

class BatchConversion : public QDialog
{
  Q_OBJECT

public:
  explicit BatchConversion(QWidget* parent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

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
  void setupGeneralGUI(QGridLayout* aLayout);
  void setupSkeletonGUI(QGridLayout* aLayout);
  void setupBodySlideGUI(QGridLayout* aLayout);
  void setupOutputGUI(QGridLayout* aLayout);
  void setupRemainingGUI(QGridLayout* aLayout);

  void userHasDoneAnAction();

private slots:
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
};
