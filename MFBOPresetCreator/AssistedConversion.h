#pragma once

#include "DataLists.h"
#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"
#include <utility>

class AssistedConversion : public QDialog
{
  Q_OBJECT

public:
  explicit AssistedConversion(QWidget* parent, const Struct::Settings& aSettings);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  std::vector<int> mBoxSelectedIndexes;
  QString mScannedDirName;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
  void displayHintZone();
  void deleteAlreadyExistingWindowBottom();

  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension);
  void createSelectionBlock(QGridLayout& aLayout, const QString& aFileName, const QString& aPath, const int& aRowIndex);
  std::vector<Struct::AssistedConversionResult> getChosenValuesFromInterface();

signals:
  void valuesChosen(QString, std::vector<Struct::AssistedConversionResult>);

private slots:
  void chooseInputDirectory();
  void launchSearchProcess();
  void validateSelection();
  void modifyComboBoxLockState(int aIndex);
};
