#pragma once
#include "TitleDialog.h"
#include <QDialog>
#include <QGridLayout>
#include <QNetworkAccessManager>
#include <QTabWidget>

using mapSpSS = std::map<QString, std::pair<QString, QString>, std::greater<QString>>;

class AssistedConversion final : public TitleDialog
{
  Q_OBJECT

public:
  explicit AssistedConversion(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  QNetworkAccessManager mManager;
  bool mHasUserDoneSomething{false};
  std::vector<int> mBoxSelectedIndexes;
  QString mScannedDirName;

  // GUI creation
  void initializeGUI();
  void setupFromLocalFolderTab(QTabWidget& aTabWidget);
  void setupFromURLTab(QTabWidget& aTabWidget);
  void displayHintZone();
  void deleteAlreadyExistingWindowBottom() const;

  // From local folder
  void chooseInputDirectory();

  // From URL
  void saveAPIKey();
  void updateSaveAPIKeyButtonState(const QString&);
  void updateSaveAPIKeyButtonState(const bool aMustBeDisabled);

  // Scan
  void updateLaunchSearchButtonState(const QString&);
  void updateLaunchSearchButtonState(int aCurrentTabIndex);

  bool isFileNameRecognized(const QString& aFileName);
  void launchSearchProcess();

  void launchSearchFromLocalFolder();
  mapSpSS scanForNifFiles(const QString& aRootDir) const;

  void launchSearchNexusModsURL();
  int getModIDFromUserInput();
  bool isModIDValid();

  void requestModInformation(const int aModID);
  void requestModInformationFinished();
  std::vector<Struct::NexusModsFileInformation> parseFilesListFromModInformation(const bool aSucceeded, const QByteArray& aResult);
  void displayFileIDPicker(const std::vector<Struct::NexusModsFileInformation>& aFilesInformation);
  void requestModFileContent(const QString& aFileName, const QString& aContentPreviewLink);
  void requestModFileContentFinished();
  mapSpSS parseFileContent(const bool aSucceeded, const QByteArray& aResult, bool& aHasFoundBSAFile) const;
  void parseNode(const QJsonArray& aArray, mapSpSS& aNifFilesList, const QString& aRootNodeName, const bool aScanMeshesSubdirsOnly, bool& aHasFoundBSAFile) const;
  std::pair<QString, QString> parseNode(const QJsonObject& aNode, const QString& aRootNodeName, const bool aScanMeshesSubdirsOnly, bool& aHasFoundBSAFile) const;

  // Display, choose and accept values
  void displayObtainedData(const mapSpSS& aFoundNifFiles);
  void createSelectionBlock(QGridLayout& aLayout, const QString& aFileName, const QString& aPath, const int aRowIndex);
  void modifyComboBoxLockState(int aIndex);
  bool hasUserSelectedAnything() const;
  std::vector<Struct::AssistedConversionResult> getChosenValuesFromInterface() const;
  void validateSelection();

  // GUI widgets events
  void openAPIKeysManagementPage();

signals:
  void valuesChosen(QString, std::vector<Struct::AssistedConversionResult>);
};
