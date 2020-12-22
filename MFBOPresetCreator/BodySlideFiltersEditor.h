#pragma once

#include "TextInputDialog.h"
#include "Utils.h"
#include "stdafx.h"

class BodySlideFiltersEditor : public QDialog
{
  Q_OBJECT

public:
  explicit BodySlideFiltersEditor(QWidget* parent, Struct::Settings aSettings, const std::map<QString, QStringList>& aInitialList);

signals:
  void listEdited(const std::map<QString, QStringList>&);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

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
  void updateFiltersCombobox();

  // Utility function
  void displayFilterAt(const int& aIndex);

private slots:
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
};
