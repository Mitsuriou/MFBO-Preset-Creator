#pragma once

#include "Utils.h"
#include "stdafx.h"

class BodySlideFiltersEditor : public QDialog
{
  Q_OBJECT

public:
  explicit BodySlideFiltersEditor(QWidget* parent, Struct::Settings aSettings, const std::map<QString, QStringList>& aInitialList, const QString& aKey);

signals:
  void listEdited(const std::map<QString, QStringList>&);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  const Struct::Settings mSettings;
  std::map<QString, QStringList> mFiltersList;
  QListWidget* mListWidget;

  int prevIndex = -1;
  int newIndex = -1;

  void setWindowProperties();
  void initializeGUI(const QString& aKey);
  void setupInterface(QGridLayout& aLayout);
  void initBodySlideFiltersList();

private slots:
  void showFiltersList(int aIndex);
  void handleSetRenaming();
  void addRow();
  void deleteRow();
  void deleteAllRows();
};
