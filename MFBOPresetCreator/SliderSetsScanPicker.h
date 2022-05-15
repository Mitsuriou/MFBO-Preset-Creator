#pragma once
#include "TitleDialog.h"
#include <QDialog>
#include <QGridLayout>
#include <QNetworkAccessManager>
#include <QTabWidget>

class SliderSetsScanPicker final : public TitleDialog
{
  Q_OBJECT

public:
  explicit SliderSetsScanPicker(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  QNetworkAccessManager mManager;
  bool mHasUserDoneSomething{false};

  // GUI creation
  void setWindowProperties();
  void initializeGUI();
  void displayHintZone();
  void deleteAlreadyExistingWindowBottom() const;

  // From local folder
  void chooseInputDirectory();

  // Scan
  bool isFileNameRecognized(const QString& aFileName);
  void launchSearchProcess();

  void launchSearch();
  std::multimap<QString, std::vector<Struct::SliderSet>> scanForOspFiles(const QString& aRootDir) const;

  // Display, choose and accept values
  void displayObtainedData(const std::multimap<QString, std::vector<Struct::SliderSet>>& aFoundOspFiles);
  std::vector<Struct::AssistedConversionResult> getChosenValuesFromInterface() const;
  void validateSelection();

  // TODO
  // signals:
  //  void valuesChosen();
};
