#pragma once
#include "SliderSetsDBDAO.h"
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
  std::map<int, Struct::DatabaseSliderSet> mDatabase{SliderSetsDBDAO::loadDatabase()};

  int mCurrentPreviewIndex{-1};

  // GUI creation
  void initializeGUI();
  void refreshList();

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
  int nextAvailableDatabaseIndex() const;
};
