#pragma once

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <QString>

#include "ui_MFBOPresetCreator.h"

class MFBOPresetCreator : public QMainWindow
{
  Q_OBJECT

public:
  MFBOPresetCreator(QWidget* parent = Q_NULLPTR);

private:
  Ui::MFBOPresetCreatorClass ui;
  void initializeGUI();
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
};
