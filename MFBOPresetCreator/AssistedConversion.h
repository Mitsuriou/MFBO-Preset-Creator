#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>

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

  void setWindowProperties();
  void initializeGUI();
  void displayHintZone();
  void deleteAlreadyExistingWindowBottom() const;

  std::map<std::string, std::pair<QString, QString>, std::greater<std::string>> scanForFilesByExtension(const QString& aRootDir, const QString& aFileExtension) const;
  void createSelectionBlock(QGridLayout& aLayout, const QString& aFileName, const QString& aPath, const int aRowIndex);
  std::vector<Struct::AssistedConversionResult> getChosenValuesFromInterface() const;

  // Utils functions
  bool hasUserSelectedAnything() const;

#pragma region PRIVATE_SLOTS
  void chooseInputDirectory();
  void launchSearchProcess();
  void validateSelection();
  void modifyComboBoxLockState(int aIndex);
#pragma endregion PRIVATE_SLOTS

  explicit AssistedConversion(const AssistedConversion&) = delete;
  AssistedConversion& operator=(const AssistedConversion&) = delete;

signals:
  void valuesChosen(QString, std::vector<Struct::AssistedConversionResult>);
};
