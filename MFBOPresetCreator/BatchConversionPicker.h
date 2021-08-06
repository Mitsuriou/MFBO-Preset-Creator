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
  void refreshMiddleList();
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);

  // Presets construction related fuctions
  void removeDataFromActiveMiddleList(const QString& aOriginFolder, const QString& aRessourcePath);
  void addDataToActiveMiddleList(const QString& aOriginFolder, const QString& aRessourcePath);

  //
  void validateSelection();

  // GUI widgets events
  void groupBoxChecked(bool aIsChecked);
};
