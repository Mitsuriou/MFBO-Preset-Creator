#pragma once

#include "Utils.h"
#include "stdafx.h"

class BodySlideFiltersEditor : public QDialog
{
  Q_OBJECT

public:
  BodySlideFiltersEditor(QWidget* parent, Struct::Settings aSettings, const QStringList& aInitialList);

signals:
  void listEdited(QStringList);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  const Struct::Settings mSettings;
  QListWidget* mListWidget;

  void setWindowProperties();
  void initializeGUI(const QStringList& aInitialList);
  void setupInterface(QGridLayout& aLayout);

private slots:
  void addRow();
  void deleteRow();
  void deleteAllRows();
};
