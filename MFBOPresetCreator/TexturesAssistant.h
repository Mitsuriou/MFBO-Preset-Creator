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

  struct GroupedData
  {
  public:
    std::map<std::string, std::vector<QString>> headTextures;
    std::map<std::string, std::vector<QString>> handsTextures;
    std::map<std::string, std::vector<QString>> bodyTextures;
  };

  void setWindowProperties();
  void initializeGUI();
  void setupInterface();
  void displayHintZone();
  void deleteAlreadyExistingWindowBottom() const;

  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const;
  void createFoundRessourcesBlocks(QGridLayout* aLayout, const TexturesAssistant::GroupedData& aGroupedPaths);

signals:
  void valuesChosen(QString, std::vector<Struct::AssistedConversionResult>);

private slots:
  void chooseInputDirectory();
  void launchSearchProcess();

  // Scrollbar events
  void scrollbarPressed();
  void scrollbarReleased();
};
