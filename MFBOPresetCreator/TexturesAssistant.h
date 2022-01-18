#pragma once
#include "Struct.h"
#include <QDialog>
#include <QFileSystemWatcher>
#include <QGridLayout>

class TexturesAssistant final : public QDialog
{
  Q_OBJECT

public:
  explicit TexturesAssistant(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

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
    std::map<std::string, std::vector<QString>> headTextures;
    std::map<std::string, std::vector<QString>> handsTextures;
    std::map<std::string, std::vector<QString>> bodyTextures;
    std::map<std::string, std::vector<QString>> extraBodyTextures;
    std::map<std::string, std::vector<QString>> mouthTextures;
  };

  QFileSystemWatcher* mFileWatcher;
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths;
  bool mHasUserDoneSomething;
  int mMinimumFirstColumnWidth;
  TexturesAssistant::ScannedData mScannedFiles;

  // GUI creation
  void setWindowProperties();
  void initializeGUI();
  void displayHintZone();
  void setupTexturesSetGUI(QGridLayout& aLayout);
  void setupOutputBox(QGridLayout& aLayout);
  void setupButtons(QGridLayout& aLayout);
  void deleteAlreadyExistingWindowBottom() const;

  // Files scan and results display in the GUI
  void scanForTexturesFiles(const QString& aRootDir, const QString& aFileExtension);
  void displayTexturesFilesList();
  void createResourceBlock(const std::map<std::string, std::vector<QString>>& aMap, QGridLayout* aLayout);

  void updateOutputPreview();

  void generateTexturesStructure();

  // GUI widgets events
  void chooseInputDirectory();
  void launchSearchProcess();
  void populateTexturesSetChooser();
  void openTexturesSetsAssetsDirectory();
  void useOnlySubdirStateChanged(int);
  void chooseExportDirectory();

  explicit TexturesAssistant(const TexturesAssistant&) = delete;
  TexturesAssistant& operator=(const TexturesAssistant&) = delete;

signals:
  void valuesChosen(QString, std::vector<Struct::AssistedConversionResult>);
};
