#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>
#include <QTreeWidgetItem>

class BatchConversionPicker final : public QDialog
{
  Q_OBJECT

public:
  explicit BatchConversionPicker(QWidget* aParent, const Struct::Settings& aSettings, const int aBodyNameSelected, const int aBodyVersionSelected, const int aFeetModIndex, const std::map<std::string, std::pair<QString, QString>, std::greater<std::string>>& aScannedData);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  int mMinimumFirstColumnWidth;

  // Data defined in the previous step
  int mBodyNameSelected;
  int mBodyVersionSelected;
  int mFeetModIndex;

  // Data defined in the current step
  std::map<std::string, std::vector<QString>> mGroupedData;

  void setWindowProperties();
  void initializeGUI();
  void deleteAlreadyExistingWindowBottom() const;

  // Update the GUI preview
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);

  void displayFoundFiles(QGridLayout* aLayout, const std::map<std::string, std::pair<QString, QString>, std::greater<std::string>>& aScannedData);

  void validateSelection();
  void listRowSelectStateChanged(QTreeWidgetItem* aListItem);
  void listRowChanged();

  // GUI widgets events
  void scrollbarPressed();
  void scrollbarReleased();
  void groupBoxChecked(bool aIsChecked);
};
