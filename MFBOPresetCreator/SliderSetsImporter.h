#pragma once
#include "TitleDialog.h"

class SliderSetsImporter final : public TitleDialog
{
  Q_OBJECT

public:
  explicit SliderSetsImporter(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  bool mHasUserDoneSomething{false};

  // GUI creation
  void initializeGUI();
  void displayHintZone();
  void deleteAlreadyExistingWindowBottom() const;

  // From local folder
  void chooseInputDirectory();

  // Scan
  bool isFileNameRecognized(const QString& aFileName);
  void launchSearchProcess();

  void launchSearch();
  std::multimap<QString, std::vector<Struct::SliderSet>> scanForOspFilesData(const QString& aRootDir) const;

  // Display, choose and accept values
  void displayObtainedData(const std::multimap<QString, std::vector<Struct::SliderSet>>& aFoundOspFiles);
  std::vector<Struct::SliderSetResult> getChosenValuesFromInterface() const;
  void validateSelection();

signals:
  void valuesChosen(std::vector<Struct::SliderSetResult>);
};
