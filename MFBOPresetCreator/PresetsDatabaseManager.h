#pragma once
#include "TitleDialog.h"
#include <QDomElement>

class PresetsDatabaseManager final : public TitleDialog
{
  Q_OBJECT

public:
  explicit PresetsDatabaseManager(QWidget* aParent,
                                  const Struct::Settings& aSettings,
                                  std::map<QString, QString>* aLastPaths);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  std::map<int, bool> mInitialDatabase;
  std::map<int, bool> mRunningDatabase;

  // Database I/O streams
  void loadDatabase();
  void saveDatabase();

  void detectRemovedFiles();

  // Create
  void openSliderSetsImporter();
  void importNewSliderSets(const std::vector<Struct::SliderSetResult>& aChosenPresets);
  bool saveSliderSetToDatabase(const QDomElement& aSliderSetNode);

  void addDatabaseLine(const int& aIndex);

  // Update
  void updateDatabaseLine(const int aIndex, const bool aValue);

  // Delete
  void removeFromDatabase(const int& aIndex);

  // Other utils functions
  int nextAvailableDatabaseIndex();

  QString databaseToString();
  std::pair<int, bool> parseDatabaseLine(const QString& aLine);
  QString stringifyDatabaseEntry(const std::pair<int, bool>& lEntry);
};
