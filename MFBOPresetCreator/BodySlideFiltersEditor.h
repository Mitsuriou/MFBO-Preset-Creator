#pragma once

#include "stdafx.h"

class BodySlideFiltersEditor : public QDialog
{
  Q_OBJECT

public:
  BodySlideFiltersEditor(QWidget* parent);

private:
  QStringListModel* mModel;
  QListView* mListView;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);

private slots:
  void addNewFilterRow();
};
