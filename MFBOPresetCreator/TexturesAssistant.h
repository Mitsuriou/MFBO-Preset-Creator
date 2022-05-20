#pragma once
#include "TitleDialog.h"
#include <QFileSystemWatcher>
#include <QGridLayout>
#include <QNetworkAccessManager>
#include <QTabWidget>

class TexturesAssistant final : public TitleDialog
{
  Q_OBJECT

public:
  explicit TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);
  ~TexturesAssistant();

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  struct ScannedData
  {
    // std::vector<relative_path, file_name_with_extension>
    std::vector<std::pair<QString, QString>> groupedTextures;
    // std::vector<relative_path, file_name_with_extension>
    std::vector<std::pair<QString, QString>> otherTextures;
  };

  struct GroupedData
  {
    std::map<QString, std::vector<QString>> headTextures;
    std::map<QString, std::vector<QString>> handsTextures;
    std::map<QString, std::vector<QString>> bodyTextures;
    std::map<QString, std::vector<QString>> extraBodyTextures;
    std::map<QString, std::vector<QString>> mouthTextures;
  };

  QFileSystemWatcher* mFileWatcher{nullptr};
  QNetworkAccessManager mManager;
  bool mHasUserDoneSomething{false};
  int mMinimumFirstColumnWidth{300};
  TexturesAssistant::ScannedData mScannedFiles;

  // GUI creation
  void initializeGUI();
  void setupFromLocalFolderTab(QTabWidget& aTabWidget);
  void setupFromURLTab(QTabWidget& aTabWidget);
  void displayHintZone();
  void setupTexturesSetGUI(QGridLayout& aLayout);
  void setupOutputBox(QGridLayout& aLayout);
  void setupButtons(QGridLayout& aLayout);
  void deleteAlreadyExistingWindowBottom() const;

  // From local folder
  void chooseInputDirectory();

  // From URL
  void saveAPIKey();
  void updateSaveAPIKeyButtonState(const QString&);
  void updateSaveAPIKeyButtonState(const bool aMustBeDisabled);

  // Toggle GUI widgets' state
  void toggleGroupBoxesState(const bool aMustBeDisabled) const;
  void updateLaunchSearchButtonState(const QString&) const;
  void updateLaunchSearchButtonState(int aCurrentTabIndex) const;

  // Scan
  void launchSearchProcess();

  void launchSearchFromLocalFolder();
  void scanForTexturesFiles(const QString& aRootDir, const QString& aFileExtension);

  void launchSearchNexusModsURL();
  int getModIDFromUserInput() const;
  bool isModIDValid() const;

  void requestModInformation(const int aModID);
  void requestModInformationFinished();
  std::vector<Struct::NexusModsFileInformation> parseFilesListFromModInformation(const bool aSucceeded, const QByteArray& aResult);
  void displayFileIDPicker(const std::vector<Struct::NexusModsFileInformation>& aFilesInformation);
  void requestModFileContent(const QString& aContentPreviewLink);
  void requestModFileContentFinished();
  void parseFileContent(const bool aSucceeded, const QByteArray& aResult, bool& aHasFoundBSAFile);
  void parseNode(const QJsonArray& aArray, std::map<QString, std::vector<QString>>& aNifFilesList, const QString& aRootNodeName, bool& aHasFoundBSAFile);
  void parseNode(const QJsonObject& aNode, const QString& aRootNodeName, bool& aHasFoundBSAFile);

  // Display, choose and accept values
  void displayObtainedData();
  void createResourceBlock(const std::map<QString, std::vector<QString>>& aMap, QGridLayout* aLayout);
  void generateTexturesStructure();

  // GUI widgets events
  void openAPIKeysManagementPage();
  void updateOutputPreview();
  void populateTexturesSetChooser();
  void openTexturesSetsAssetsDirectory();
  void useOnlySubdirStateChanged(int);
  void chooseExportDirectory();
};
