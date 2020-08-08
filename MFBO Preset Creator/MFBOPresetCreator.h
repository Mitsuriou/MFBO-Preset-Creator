#pragma once

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <QString>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QDesktopServices>
#include <QRadioButton>
#include <QButtonGroup>

#include "ui_MFBOPresetCreator.h"

class MFBOPresetCreator : public QMainWindow
{
  Q_OBJECT

public:
  MFBOPresetCreator(QWidget* parent = Q_NULLPTR);

private:
  Ui::MFBOPresetCreatorClass ui;
  void initializeGUI();
  void setupMenuBar();
  void setupBodyMeshesGUI(QVBoxLayout& aLayout);
  void setupBodySlideGUI(QVBoxLayout& aLayout);
  void setupOptionsGUI(QVBoxLayout& aLayout);
  void setupOutputGUI(QVBoxLayout& aLayout);
  void setupRemainingGUI(QVBoxLayout& aLayout);
  void displayWarningMessage(QString aMessage);

private slots:
  void updateBodyMeshesPreview(QString aText);
  void updateOutputPreview();
  void updateOSPXMLPreview(QString aText);
  void updateBodyslideNamesPreview(QString aText);
  void updateSkeletonPathState(int aState);
  void updateSkeletonPreview(QString aText);
  void chooseExportDirectory();
  void generateDirectoryStructure();

  // Menu dialogs
  void showAboutDialog();
  void showSettingsDialog();
};
