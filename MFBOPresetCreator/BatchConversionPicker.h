#pragma once
#include "BCDragWidget.h"
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>

class BatchConversionPicker final : public QDialog
{
  Q_OBJECT

public:
  explicit BatchConversionPicker(QWidget* aParent, const Struct::Settings& aSettings, const Struct::BatchConversionData& aData);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  int mMinimumFirstColumnWidth;
  Struct::BatchConversionData mData;
  std::vector<BCDragWidget*> mMiddleListButtons;

  void setWindowProperties();
  void initializeGUI();
  void displayLeftList();

  // Update the GUI
  void leftListIndexChanged();
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);

  void validateSelection();

  // GUI widgets events
  void scrollbarPressed();
  void scrollbarReleased();
  void groupBoxChecked(bool aIsChecked);
};
