#pragma once
#include "TitleDialog.h"
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QListWidget>

class BodySlideFiltersEditor final : public TitleDialog
{
  Q_OBJECT

public:
  explicit BodySlideFiltersEditor(QWidget* aParent,
                                  const Struct::Settings& aSettings,
                                  std::map<QString, QString>* aLastPaths,
                                  const std::map<QString, QStringList>& aInitialList);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  const std::map<QString, QStringList> mOriginalFiltersList;
  std::map<QString, QStringList> mFiltersList;
  QComboBox* mFiltersListChooser{nullptr};
  QListWidget* mListWidget{nullptr};

  // GUI construction
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
  void setupButtons(QGridLayout& aLayout);
  void updateFiltersCombobox() const;

  // Utility functions
  void displayFilterAt(const int aIndex) const;
  void shouldDisableFiltersControls();

  //#pragma region PRIVATE_SLOTS
  void showFiltersList(int aIndex);

  // Filter set
  void addSet();
  void addNewSetEntry(const QString& aSetName);
  void removeSet();
  void handleSetRenaming(const QString& aNewSetName);

  // Filter row
  void addRow();
  void deleteRow();
  void deleteAllRows();
  void handleRowRenaming();
  //#pragma endregion PRIVATE_SLOTS

signals:
  void listEdited(const std::map<QString, QStringList>&);
};
