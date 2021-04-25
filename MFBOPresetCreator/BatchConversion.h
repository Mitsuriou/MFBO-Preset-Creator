#pragma once

#include "BodySlideFiltersEditor.h"
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
  void setupInterface(QGridLayout& aLayout);
  void userHasDoneAnAction();

  int getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const;

private slots:
  void userHasDoneAnAction(int);
  void updateAvailableBodyVersions();
  void chooseInputDirectory();
  void chooseBackupDirectory();
  void updateBackupState(int aState);
  void updateBackupPreview();
  void launchUpDownGradeProcess();

  // BodySlide Filters Editor
  void openBodySlideFiltersEditor();
  void initBodySlideFiltersList();
  void updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList);
  void updateBodySlideFiltersListPreview(int aIndex);
};
