#pragma once

#include "DataLists.h"
#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"
#include <utility>

class TexturesAssistant : public QDialog
{
  Q_OBJECT

public:
  TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths;
  bool mHasUserDoneSomething;
  QString mScannedDirName;

  struct ScannedData
  {
  public:
    // The map is storing <path+fileName, <path, fileName>>
    std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> groupedTextures;
    // The map is storing <path+fileName, <path, fileName>>
    std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> otherTextures;
  };

  struct GroupedData
  {
  public:
    std::map<std::string, std::vector<QString>> headTextures;
    std::map<std::string, std::vector<QString>> handsTextures;
    std::map<std::string, std::vector<QString>> bodyTextures;
  };

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout* aLayout);
  void displayHintZone();
  void deleteAlreadyExistingWindowBottom() const;

  TexturesAssistant::ScannedData scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const;
  void displayFoundTextures(QGridLayout* aLayout, const TexturesAssistant::ScannedData& aScannedData);
  void createRessourceBlock(const std::map<std::string, std::vector<QString>>& aMap, QGridLayout* aLayout);

signals:
  void valuesChosen(QString, std::vector<Struct::AssistedConversionResult>);

private slots:
  void chooseInputDirectory();
  void launchSearchProcess();

  // GUI widgets events
  void scrollbarPressed();
  void scrollbarReleased();
  void groupBoxChecked(bool aIsChecked);
};
