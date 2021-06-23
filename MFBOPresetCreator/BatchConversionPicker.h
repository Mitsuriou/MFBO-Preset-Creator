#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>
#include <QTreeWidgetItem>

class BatchConversionPicker final : public QDialog
{
  Q_OBJECT

public:
  explicit BatchConversionPicker(QWidget* aParent, const Struct::Settings& aSettings, Struct::BatchConversionData* aData);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  int mMinimumFirstColumnWidth;
  Struct::BatchConversionData* mData;

  void setWindowProperties();
  void initializeGUI();
  void deleteAlreadyExistingWindowBottom() const;

  // Update the GUI preview
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);

  void validateSelection();
  void listRowSelectStateChanged(QTreeWidgetItem* aListItem);
  void listRowChanged();

  // GUI widgets events
  void scrollbarPressed();
  void scrollbarReleased();
  void groupBoxChecked(bool aIsChecked);
};
