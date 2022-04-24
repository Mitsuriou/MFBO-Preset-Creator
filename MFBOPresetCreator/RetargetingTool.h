#pragma once
#include "TitleDialog.h"
#include <QFileSystemWatcher>
#include <QGridLayout>

class RetargetingTool final : public TitleDialog
{
  Q_OBJECT

public:
  explicit RetargetingTool(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);
  ~RetargetingTool();

signals:
  void modalClosed();

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  QFileSystemWatcher* mFileWatcher{nullptr};
  std::map<QString, QString>* mLastPaths{nullptr};
  bool mHasUserDoneSomething;
  std::map<QString, QStringList> mFiltersList;
  BodyNameVersion mTargetBodyMesh;
  FeetNameVersion mTargetFeetMesh;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
  void userHasDoneAnAction();

  //#pragma region PRIVATE_SLOTS
  void userHasDoneAnAction(int);
  void chooseInputDirectory();
  void chooseBackupDirectory();
  void updateBackupState(int aState);
  void updateBackupPreview();
  void launchUpDownGradeProcess();

  // Target meshes pikcker
  void openTargetMeshesPicker();
  void targetMeshesChanged(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);

  // BodySlide Filters Editor
  void openBodySlideFiltersEditor();
  void initBodySlideFiltersList();
  void updateBodySlideFiltersList(const std::map<QString, QStringList>& aFilterList);
  void updateBodySlideFiltersListPreview();
  //#pragma endregion PRIVATE_SLOTS
};
