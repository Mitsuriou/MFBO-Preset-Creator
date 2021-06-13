#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>

class BatchConversionPicker final : public QDialog
{
  Q_OBJECT

public:
  explicit BatchConversionPicker(QWidget* aParent, const Struct::Settings& aSettings, const std::map<std::string, std::pair<QString, QString>, std::greater<std::string>>& aScannedData);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;

  struct GroupedData
  {
  public:
    std::map<std::string, std::vector<QString>> hands;
    std::map<std::string, std::vector<QString>> feet;
    std::map<std::string, std::vector<QString>> body;
  };

  void setWindowProperties();
  void initializeGUI();
  void deleteAlreadyExistingWindowBottom() const;

  void displayFoundFiles(QGridLayout* aLayout, const std::map<std::string, std::pair<QString, QString>, std::greater<std::string>>& aScannedData);
  void createRessourceBlock(const std::map<std::string, std::vector<QString>>& aMap, QGridLayout* aLayout);

  // GUI widgets events
  void scrollbarPressed();
  void scrollbarReleased();
  void groupBoxChecked(bool aIsChecked);
};
