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
  std::map<QString, QStringList> mFiltersList;
  QListWidget* mListWidget;
  int mPreviousChooserIndex = -1;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);
  void setupButtons(QGridLayout& aLayout);
  void initBodySlideFiltersList();
  void saveGUIListToFiltersMap(const QString& aMapKey);

private slots:
  void showFiltersList(int aIndex);
  void addSet();
  void addNewSetEntry(const QString& aSetName);
  void removeSet();
  void handleSetRenaming();
  void addRow();
  void deleteRow();
  void deleteAllRows();
};
