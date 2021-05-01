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
  bool mHasUserDoneSomething;
  std::map<QString, QStringList> mFiltersList;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout* aLayout);
  void setupOutputGUI(QGridLayout* aLayout);
  void userHasDoneAnAction();

private slots:
  void userHasDoneAnAction(int);
  void updateAvailableBodyVersions();
  void chooseInputDirectory();
  void launchSearchProcess();
  void populateSkeletonChooser();

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
  void groupBoxChecked(bool aIsChecked);
};
