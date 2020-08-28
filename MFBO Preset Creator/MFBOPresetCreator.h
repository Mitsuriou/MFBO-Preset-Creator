#pragma once

#include "Utils.h"
#include "UpgraderTool.h"
#include "Settings.h"

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <QString>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QDesktopServices>
#include <QComboBox>
#include <QStringList>
#include <QStatusBar>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
//#include <QPixmap>

#include "ui_MFBOPresetCreator.h"

class MFBOPresetCreator : public QMainWindow
{
  Q_OBJECT

public:
  MFBOPresetCreator(QWidget* parent = Q_NULLPTR);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  Ui::MFBOPresetCreatorClass ui;
  Struct::Settings mSettings;

  void initializeGUI();
  void setupMenuBar();
  void setupBodyMeshesGUI(QVBoxLayout& aLayout);
  void setupBodySlideGUI(QVBoxLayout& aLayout);
  void setupOptionsGUI(QVBoxLayout& aLayout);
  void setupOutputGUI(QVBoxLayout& aLayout);
  void setupRemainingGUI(QVBoxLayout& aLayout);

private slots:
  void updateOutputPreview();
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);
  void updateSkeletonPathState(int aState);
  void updateSkeletonPreview(QString aText);
  void chooseExportDirectory();
  void generateDirectoryStructure();
  void refreshAllPreviewFields(int);
  void refreshAllPreviewFields();

  // Menu dialogs
  void launchUpgraderTool();
  void showSettingsDialog();
  void showAboutDialog();
};
