#pragma once
#include "Struct.h"
#include "TitleDialog.h"
#include <QDomElement>

class SliderSetsDBManager final : public TitleDialog
{
  Q_OBJECT

public:
  explicit SliderSetsDBManager(QWidget* aParent, const Struct::Settings& aSettings, std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  std::map<int, Struct::DatabaseSliderSet> mInitialDatabase;
  std::map<int, Struct::DatabaseSliderSet> mRunningDatabase;

  int mCurrentPreviewIndex{-1};

  // GUI creation
  void initializeGUI();
  void refreshList();

  // Database I/O streams
  void loadDatabase();
  void saveDatabase();

  void detectRemovedFiles();

  // Create
  void openSliderSetsImporter();
  void importNewSliderSets(const std::vector<Struct::SliderSetResult>& aChosenSliderSets);
  bool saveSliderSetToDatabase(QDomElement& aSliderSetNode, const Struct::DatabaseSliderSet& aSliderSetData, const int aIndex = -1);

  void addDatabaseLine(const int aIndex, const Struct::DatabaseSliderSet& aSliderSetData);

  // Update
  void updateSliderSetName(const int aIndex, const QString& aNewSliderSetName);
  void updateSliderSetMeshType(const int aIndex, const MeshPartType aMeshType);

  // Delete
  void removeFromDatabase(const int aIndex);

  // Other utils functions
  void displaySliderSetContent(const int aIndex);
  void clearPreviewContent();
  int nextAvailableDatabaseIndex();

  QString databaseToString();
  std::pair<int, Struct::DatabaseSliderSet> parseDatabaseLine(const QString& aLine);
  QString stringifyDatabaseEntry(const std::pair<int, Struct::DatabaseSliderSet>& lEntry);
};
