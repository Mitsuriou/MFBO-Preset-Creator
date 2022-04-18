#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>
#include <QTabWidget>

class AssistedConversion final : public QDialog
{
  Q_OBJECT

public:
  explicit AssistedConversion(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths;
  bool mHasUserDoneSomething;
  std::vector<int> mBoxSelectedIndexes;
  QString mScannedDirName;

  // GUI
  void setWindowProperties();
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
  void updateLaunchSearchButtonState(int aCurrentTabIndex);

  bool isFileNameRecognized(const QString& aFileName);
  void launchSearchProcess();
  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> launchSearchFromTabContext();

  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> launchSearchFromLocalFolder();
  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const;

  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> launchSearchNexusModsURL();

  // Choose and accept values
  void createSelectionBlock(QGridLayout& aLayout, const QString& aFileName, const QString& aPath, const int aRowIndex);
  void modifyComboBoxLockState(int aIndex);
  bool hasUserSelectedAnything() const;
  std::vector<Struct::AssistedConversionResult> getChosenValuesFromInterface() const;
  void validateSelection();

  explicit AssistedConversion(const AssistedConversion&) = delete;
  AssistedConversion& operator=(const AssistedConversion&) = delete;

signals:
  void valuesChosen(QString, std::vector<Struct::AssistedConversionResult>);
};
