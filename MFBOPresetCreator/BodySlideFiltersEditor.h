#pragma once
#include "Struct.h"
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QListWidget>

class BodySlideFiltersEditor final : public QDialog
{
  Q_OBJECT

public:
  explicit BodySlideFiltersEditor(QWidget* aParent, Struct::Settings aSettings, const std::map<QString, QStringList>& aInitialList);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  std::map<QString, QStringList> mOriginalFiltersList;
  std::map<QString, QStringList> mFiltersList;
  QComboBox* mFiltersListChooser{nullptr};
  QListWidget* mListWidget{nullptr};

  // GUI construction
  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
  void setupButtons(QGridLayout& aLayout);
  void updateFiltersCombobox() const;

  // Utility functions
  void displayFilterAt(const int aIndex) const;
  void shouldDisableFiltersControls();

#pragma region PRIVATE_SLOTS
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
#pragma endregion PRIVATE_SLOTS

signals:
  void listEdited(const std::map<QString, QStringList>&);
};
