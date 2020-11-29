#pragma once

#include "DataLists.h"
#include "Enum.h"
#include "Struct.h"
#include "Utils.h"
#include "stdafx.h"

class RetargetingTool : public QDialog
{
  Q_OBJECT

public:
  explicit RetargetingTool(QWidget* parent, const Struct::Settings& aSettings);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  const Struct::Settings mSettings;

  void setWindowProperties();
  void initializeGUI();
  void setupInterface(QGridLayout& aLayout);

  int getNumberFilesByExtension(const QString& aRootDir, const QString& aFileExtension);

private slots:
  void chooseInputDirectory();
  void chooseBackupDirectory();
  void updateBackupState(int aState);
  void updateBackupPreview();
  void launchUpDownGradeProcess();
};
