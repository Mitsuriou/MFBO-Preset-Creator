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

private:
  const Struct::Settings mSettings;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);

  // TODO: Optimize the code to avoid the multimap below:
  std::multimap<std::string, QString, std::greater<std::string>> scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension);
  void createSelectionBlock(QGridLayout& aLayout, const QString& aFileName, const QString& aPath, const int& aRowIndex);
  std::vector<Struct::AssistedConversionResult> getChosenValuesFromInterface();

private slots:
  void chooseInputDirectory();
  void launchSearchProcess();
  void validateSelection();
};
