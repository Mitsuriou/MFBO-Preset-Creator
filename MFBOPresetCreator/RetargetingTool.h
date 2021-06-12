#pragma once
#include "Struct.h"
#include <QDialog>
#include <QGridLayout>

class RetargetingTool final : public QDialog
{
  Q_OBJECT

public:
  explicit RetargetingTool(QWidget* parent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  std::map<QString, QString>* mLastPaths;
  bool mHasUserDoneSomething;
  std::map<QString, QStringList> mFiltersList;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
  void userHasDoneAnAction();

#pragma region PRIVATE_SLOTS
  void userHasDoneAnAction(int);
  void updateAvailableBodyVersions();
  void chooseInputDirectory();
  void chooseBackupDirectory();
  void updateBackupState(int aState);
  void updateBackupPreview();
  void launchUpDownGradeProcess();

  // BodySlide Filters Editor
  void openBodySlideFiltersEditor();
  void initBodySlideFiltersList();
  void updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList);
  void updateBodySlideFiltersListPreview(int aIndex);

  // GUI widgets events
  void groupBoxChecked(bool aIsChecked);
#pragma endregion PRIVATE_SLOTS
};
