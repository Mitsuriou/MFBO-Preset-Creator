#include "MFBOPresetCreator.h"
#include "stdafx.h"
#include "Utils.h"

MFBOPresetCreator::MFBOPresetCreator(QWidget* parent)
  : QMainWindow(parent)
{
  // Construct the GUI
  ui.setupUi(this);
  this->initializeGUI();
}

void MFBOPresetCreator::initializeGUI()
{
  // Main vertical container
  auto lMainVertical{ new QVBoxLayout(this->ui.mainContainer) };

  // Setup all the different GUI components
  this->setupBodyMeshesGUI(*lMainVertical);
  this->setupBodySlideGUI(*lMainVertical);
  this->setupOptionsGUI(*lMainVertical);
  this->setupOutputGUI(*lMainVertical);
  this->setupRemainingGUI(*lMainVertical);
}

void MFBOPresetCreator::setupBodyMeshesGUI(QVBoxLayout& aLayout)
{
  // CBBE body meshes group box
  auto lMeshesGroupBox{ new QGroupBox("CBBE body meshes") };
  aLayout.addWidget(lMeshesGroupBox);

  // Grid layout
  auto lMeshesGridLayout{ new QGridLayout(lMeshesGroupBox) };
  lMeshesGridLayout->setColumnMinimumWidth(0, 140);

  // First line
  auto lMeshesPathLabel{ new QLabel("Body meshes path:") };
  lMeshesGridLayout->addWidget(lMeshesPathLabel, 0, 0);

  auto lMeshesPathLineEdit{ new QLineEdit("") };
  lMeshesPathLineEdit->setObjectName("body_meshes_path_input");
  lMeshesGridLayout->addWidget(lMeshesPathLineEdit, 0, 1);

  // Second line
  auto lMeshestitlePreview{ new QLabel("Preview:") };
  lMeshesGridLayout->addWidget(lMeshestitlePreview, 1, 0);

  auto lMeshesPathsPreview{ new QLabel("") };
  lMeshesPathsPreview->setObjectName("body_meshes_path_preview");
  lMeshesGridLayout->addWidget(lMeshesPathsPreview, 1, 1);

  // Initialization functions
  this->updateBodyMeshesPreview(QString(""));

  // Event binding
  connect(lMeshesPathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBodyMeshesPreview(QString)));
}

void MFBOPresetCreator::setupBodySlideGUI(QVBoxLayout& aLayout)
{
  // Bodyslide defined names group box
  auto lBodyslideGroupBox{ new QGroupBox("BodySlide") };
  aLayout.addWidget(lBodyslideGroupBox);

  // Grid layout
  auto lBodyslideGridLayout{ new QGridLayout(lBodyslideGroupBox) };
  lBodyslideGridLayout->setColumnMinimumWidth(0, 140);

  // First line
  auto lOSPXMLNames{ new QLabel("Bodyslide files names:") };
  lBodyslideGridLayout->addWidget(lOSPXMLNames, 0, 0);

  auto lOSPXMLNamesLineEdit{ new QLineEdit("") };
  lOSPXMLNamesLineEdit->setObjectName("names_osp_xml_input");
  lBodyslideGridLayout->addWidget(lOSPXMLNamesLineEdit, 0, 1);

  // Second line
  auto lLabelOspXmlNames{ new QLabel("Preview:") };
  lBodyslideGridLayout->addWidget(lLabelOspXmlNames, 1, 0);

  auto lPathsNamesOspXmlNames{ new QLabel("") };
  lPathsNamesOspXmlNames->setObjectName("names_osp_xml_preview");
  lBodyslideGridLayout->addWidget(lPathsNamesOspXmlNames, 1, 1);

  // Third line
  auto lNamesInApp{ new QLabel("Name in bodyslide software:") };
  lBodyslideGridLayout->addWidget(lNamesInApp, 2, 0);

  auto lNamesInAppLineEdit{ new QLineEdit("") };
  lNamesInAppLineEdit->setObjectName("names_bodyslide_input");
  lBodyslideGridLayout->addWidget(lNamesInAppLineEdit, 2, 1);

  // Fourth line
  auto lLabelNamesInApp{ new QLabel("Preview:") };
  lBodyslideGridLayout->addWidget(lLabelNamesInApp, 3, 0);

  auto lResultNamesInApp{ new QLabel("") };
  lResultNamesInApp->setObjectName("names_bodyslide_preview");
  lBodyslideGridLayout->addWidget(lResultNamesInApp, 3, 1);

  // Initialization functions
  this->updateOSPXMLPreview(QString(""));
  this->updateBodyslideNamesPreview(QString(""));

  // Event binding
  connect(lOSPXMLNamesLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateOSPXMLPreview(QString)));
  connect(lNamesInAppLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBodyslideNamesPreview(QString)));
}

void MFBOPresetCreator::setupOptionsGUI(QVBoxLayout& aLayout)
{
  // Custom skeleton and textures group box
  auto lOptionsGroupBox{ new QGroupBox("Options") };
  aLayout.addWidget(lOptionsGroupBox);

  auto lOptionsGridLayout{ new QGridLayout(lOptionsGroupBox) };
  lOptionsGridLayout->setColumnMinimumWidth(0, 140);

  // Skeleton
  auto lLabelSkeleton{ new QLabel("Skeleton") };
  lOptionsGridLayout->addWidget(lLabelSkeleton, 0, 0);

  auto lNeedCustomSkeleton{
    new QCheckBox(
      QString("Check this box if the follower or NPC uses a custom skeleton. Not overriding a custom skeleton would cause breasts collision and physics to be inaccurate.")
    )
  };
  lNeedCustomSkeleton->setObjectName("use_custom_skeleton");
  lOptionsGridLayout->addWidget(lNeedCustomSkeleton, 0, 1);

  // Skeleton path
  auto lLabelSkeletonPath{ new QLabel("Skeleton path:") };
  lOptionsGridLayout->addWidget(lLabelSkeletonPath, 1, 0);

  auto lSkeletonPathLineEdit{ new QLineEdit("") };
  lSkeletonPathLineEdit->setDisabled(true);
  lSkeletonPathLineEdit->setObjectName("skeleton_path_directory");
  lOptionsGridLayout->addWidget(lSkeletonPathLineEdit, 1, 1);

  // Skeleton path preview
  auto lSkeletontitlePreview{ new QLabel("Preview:") };
  lOptionsGridLayout->addWidget(lSkeletontitlePreview, 2, 0);

  auto lSkeletonPathsPreview{ new QLabel("") };
  lSkeletonPathsPreview->setObjectName("skeleton_path_preview");
  lOptionsGridLayout->addWidget(lSkeletonPathsPreview, 2, 1);

  // TODO: Textures ?

  // Initialization functions
  this->updateSkeletonPreview(QString(""));

  // Event binding
  connect(lNeedCustomSkeleton, SIGNAL(stateChanged(int)), this, SLOT(updateSkeletonPathState(int)));
  connect(lSkeletonPathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateSkeletonPreview(QString)));

}

void MFBOPresetCreator::setupOutputGUI(QVBoxLayout& aLayout)
{
  // Output group box
  auto lOutputGroupBox{ new QGroupBox("Output") };
  aLayout.addWidget(lOutputGroupBox);

  // Grid layout
  auto lOutputGridLayout{ new QGridLayout(lOutputGroupBox) };
  lOutputGridLayout->setColumnMinimumWidth(0, 140);

  // First line
  auto lOutputPathLabel{ new QLabel("Output path:") };
  lOutputGridLayout->addWidget(lOutputPathLabel, 0, 0);

  auto lOutputPathLineEdit{ new QLineEdit("") };
  lOutputPathLineEdit->setReadOnly(true);
  lOutputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lOutputPathLineEdit->setObjectName("output_path_directory");
  lOutputGridLayout->addWidget(lOutputPathLineEdit, 0, 1);

  // Second line
  auto lOutputPathChooser{ new QPushButton("Choose a directory...") };
  lOutputGridLayout->addWidget(lOutputPathChooser, 0, 2);

  auto lLabelSubDirectoryPath{ new QLabel("Mod subdirectory:") };
  lOutputGridLayout->addWidget(lLabelSubDirectoryPath, 1, 0);

  auto lOutputSubpathLineEdit{ new QLineEdit("") };
  lOutputSubpathLineEdit->setObjectName("output_path_subdirectory");
  lOutputGridLayout->addWidget(lOutputSubpathLineEdit, 1, 1);

  // Third line
  auto lOutputtitlePreview{ new QLabel("Preview:") };
  lOutputGridLayout->addWidget(lOutputtitlePreview, 2, 0);

  auto lOutputPathsPreview{ new QLabel("") };
  lOutputPathsPreview->setObjectName("output_path_preview");
  lOutputGridLayout->addWidget(lOutputPathsPreview, 2, 1);

  // Initialization functions
  this->updateOutputPreview();

  // Event binding
  connect(lOutputPathChooser, SIGNAL(clicked()), this, SLOT(chooseExportDirectory()));
  connect(lOutputSubpathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateOutputPreview()));
}

void MFBOPresetCreator::setupRemainingGUI(QVBoxLayout& aLayout)
{
  // Generate button
  auto lGenerateButton{ new QPushButton("Generate") };
  aLayout.addWidget(lGenerateButton);

  // Event binding
  connect(lGenerateButton, SIGNAL(clicked()), this, SLOT(generateDirectoryStructure()));
}

void MFBOPresetCreator::displayWarningMessage(QString aMessage)
{
  QMessageBox lMessageBox(QMessageBox::Icon::Warning, "Warning", aMessage);
  lMessageBox.exec();
  lMessageBox.deleteLater();
}

void MFBOPresetCreator::updateBodyMeshesPreview(QString aText)
{
  auto lMeshesPathsPreview{ this->ui.mainContainer->findChild<QLabel*>("body_meshes_path_preview") };
  Utils::cleanPathString(aText);

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
  }

  QString lConstructedPreviewText(
    QStringLiteral(
      "%1/CUSTOM CBBE SMP.xml\n"
      "%1/femalebody_0.nif\n"
      "%1/femalebody_1.nif\n"
      "%1/femalefeet_0.nif\n"
      "%1/femalefeet_1.nif\n"
      "%1/femalehands_0.nif\n"
      "%1/femalehands_1.nif"
    ).arg(aText)
  );

  lMeshesPathsPreview->setText(lConstructedPreviewText);
}

void MFBOPresetCreator::updateOutputPreview()
{
  // Get main directory
  auto lMainDirTextEdit{ this->ui.mainContainer->findChild<QLineEdit*>("output_path_directory") };
  auto lMainDirectory{ lMainDirTextEdit->text().trimmed() };
  Utils::cleanPathString(lMainDirectory);

  // Get subdirectory
  auto lSubDirectory{ this->ui.mainContainer->findChild<QLineEdit*>("output_path_subdirectory")->text().trimmed() };
  Utils::cleanPathString(lSubDirectory);

  // Construct full path
  QString lFullPath("");
  if (lMainDirectory.length() > 0 && lSubDirectory.length() > 0)
  {
    lFullPath = lMainDirectory + "/" + lSubDirectory;
    lMainDirTextEdit->setDisabled(false);
  }
  else if (lMainDirectory.length() > 0 && lSubDirectory.length() == 0)
  {
    lFullPath = lMainDirectory;
    lMainDirTextEdit->setDisabled(false);
  }
  else
  {
    lMainDirTextEdit->setDisabled(true);
  }

  // Set the full path value in the preview label
  auto lOutputPathsPreview{ this->ui.mainContainer->findChild<QLabel*>("output_path_preview") };
  lOutputPathsPreview->setText(lFullPath);
}

void MFBOPresetCreator::updateOSPXMLPreview(QString aText)
{
  auto lOutputPathsPreview{ this->ui.mainContainer->findChild<QLabel*>("names_osp_xml_preview") };

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
  }

  QString lConstructedPreviewText(
    QStringLiteral(
      "CalienteTools/BodySlide/SliderGroups/%1.xml\n"
      "CalienteTools/BodySlide/SliderSets/%1.osp"
    ).arg(aText)
  );

  lOutputPathsPreview->setText(lConstructedPreviewText);
}

void MFBOPresetCreator::updateBodyslideNamesPreview(QString aText)
{
  auto lOutputPathsPreview{ this->ui.mainContainer->findChild<QLabel*>("names_bodyslide_preview") };

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
  }

  QString lConstructedPreviewText(
    QStringLiteral(
      "%1 - Feet\n"
      "%1 - Hands\n"
      "%1 - 3BBB Body Amazing"
    ).arg(aText)
  );

  lOutputPathsPreview->setText(lConstructedPreviewText);
}

void MFBOPresetCreator::updateSkeletonPathState(int aState)
{
  auto lSkeletonPathLineEdit{ this->ui.mainContainer->findChild<QLineEdit*>("skeleton_path_directory") };

  switch (aState)
  {
  case Qt::Unchecked:
    lSkeletonPathLineEdit->setDisabled(true);
    break;
  case Qt::Checked:
    lSkeletonPathLineEdit->setDisabled(false);
    break;
  default:
    break;
  }
}

void MFBOPresetCreator::updateSkeletonPreview(QString aText)
{
  auto lPathsPreview{ this->ui.mainContainer->findChild<QLabel*>("skeleton_path_preview") };
  Utils::cleanPathString(aText);

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
  }

  aText = aText + "/skeleton_female.nif";
  lPathsPreview->setText(aText);
}

void MFBOPresetCreator::chooseExportDirectory()
{
  auto lLineEdit{ this->ui.mainContainer->findChild<QLineEdit*>("output_path_directory") };
  auto lPath{ QFileDialog::getExistingDirectory() };
  lLineEdit->setText(lPath);
  this->updateOutputPreview();
}

void MFBOPresetCreator::generateDirectoryStructure()
{
  // Body meshes path
  auto lBodyMeshesPath{ this->ui.mainContainer->findChild<QLineEdit*>("body_meshes_path_input")->text().trimmed() };
  Utils::cleanPathString(lBodyMeshesPath);

  // BodySlide names
  auto lOSPXMLNames{ this->ui.mainContainer->findChild<QLineEdit*>("names_osp_xml_input")->text().trimmed() };
  auto lBodyslideSlidersetsNames{ this->ui.mainContainer->findChild<QLineEdit*>("names_bodyslide_input")->text().trimmed() };

  // Options
  auto lMustCopySkeleton{ this->ui.mainContainer->findChild<QCheckBox*>("use_custom_skeleton")->isChecked() };
  auto lSkeletonPath{ this->ui.mainContainer->findChild<QLineEdit*>("skeleton_path_directory")->text().trimmed() };
  Utils::cleanPathString(lSkeletonPath);

  // Output paths
  auto lMainDirectory{ this->ui.mainContainer->findChild<QLineEdit*>("output_path_directory")->text().trimmed() };
  auto lSubDirectory{ this->ui.mainContainer->findChild<QLineEdit*>("output_path_subdirectory")->text().trimmed() };
  Utils::cleanPathString(lSubDirectory);

  // Full extract path
  auto lEntryDirectory{ (lSubDirectory.length() == 0 ? lMainDirectory : (lMainDirectory + "/" + lSubDirectory)) };

  // Check if the full extract path has been given by the user
  if (lEntryDirectory.length() == 0)
  {
    this->displayWarningMessage("Error: no path given to export the files.");
    return;
  }

  // Create main directory
  if (!QDir(lEntryDirectory).exists())
  {
    QDir().mkdir(lEntryDirectory);
  }
  else
  {
    this->displayWarningMessage("Error while creating the main directory: \"" + lEntryDirectory + "\" already exists on your computer.");
    return;
  }

  // Export the meshes
  if (lBodyMeshesPath.length() == 0)
  {
    this->displayWarningMessage("Error: no path has been given for the meshes.");
    return;
  }

  // Check if a name has been given for the OSP and XML files
  if (lOSPXMLNames.length() == 0)
  {
    this->displayWarningMessage("Error: no name given for the bodyslide files.");
    return;
  }

  // Check if a name has been given for the presets
  if (lBodyslideSlidersetsNames.length() == 0)
  {
    this->displayWarningMessage("Error: no name given for the slider sets (names that appear in the Bodyslide software).");
    return;
  }

  // Create the SliderGroups directory
  auto lSliderGroupsDirectory{ lEntryDirectory + "/CalienteTools/BodySlide/SliderGroups" };
  if (!QDir(lSliderGroupsDirectory).exists())
  {
    QDir().mkpath(lSliderGroupsDirectory);
  }
  else
  {
    this->displayWarningMessage("Error while creating the meshes directory: \"" + lSliderGroupsDirectory + "\" already exists.");
    return;
  }

  // Copy the QRC file and change the slidergroups names in the XML file
  QString lXMLPathName{ lSliderGroupsDirectory + "/" + lOSPXMLNames + ".xml" };
  QFile::copy(":/ressources/bodyslide_xml", lXMLPathName);

  QFile lXMLFile(lXMLPathName);
  lXMLFile.setPermissions(QFile::WriteUser);

  QByteArray lTempXMLContent;

  if (lXMLFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    lTempXMLContent = lXMLFile.readAll();
    lXMLFile.close();
  }

  if (lTempXMLContent.length() > 0)
  {
    if (lXMLFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
      QString lTextToParse(lTempXMLContent);
      lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), lBodyslideSlidersetsNames);

      QTextStream lTextStream(&lXMLFile);
      lTextStream << lTextToParse;
      lTextStream.flush();

      lXMLFile.close();
    }
  }
  else
  {
    this->displayWarningMessage("Error while trying to parse the XML Bodyslide file.");
    return;
  }

  // Create the SliderSets directory
  auto lSliderSetsDirectory{ lEntryDirectory + "/CalienteTools/BodySlide/SliderSets" };

  if (!QDir(lSliderSetsDirectory).exists())
  {
    QDir().mkpath(lSliderSetsDirectory);
  }
  else
  {
    this->displayWarningMessage("Error while creating the meshes directory: \"" + lSliderSetsDirectory + "\" already exists.");
    return;
  }

  // Copy the QRC file and change the slidergroups names in the OSP file
  QString lOSPPathName(lSliderSetsDirectory + "/" + lOSPXMLNames + ".osp");
  QFile::copy(":/ressources/bodyslide_osp", lOSPPathName);

  QFile lOSPFile(lOSPPathName);
  lOSPFile.setPermissions(QFile::WriteUser);

  QByteArray lTempOSPContent;

  if (lOSPFile.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    lTempOSPContent = lOSPFile.readAll();
    lOSPFile.close();
  }

  // Replace the slider sets names
  if (lTempOSPContent.length() > 0)
  {
    if (lOSPFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
      QString lTextToParse(lTempOSPContent);
      lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), lBodyslideSlidersetsNames);
      lTextToParse.replace(QString("{%%body_meshes_path%%}"), lBodyMeshesPath.replace("/", "\\"));

      QTextStream lTextStream(&lOSPFile);
      lTextStream << lTextToParse;
      lTextStream.flush();

      lOSPFile.close();
    }
  }
  else
  {
    this->displayWarningMessage("Error while trying to parse the OSP Bodyslide file.");
    return;
  }

  // If the user checked the custom skeleton
  if (lMustCopySkeleton)
  {
    if (lSkeletonPath.length() > 0)
    {
      auto lSkeletonDirectory{ lEntryDirectory + "/" + lSkeletonPath };
      QDir().mkpath(lSkeletonDirectory);

      QFile::copy(":/ressources/skeleton_female", lSkeletonDirectory + "/" + "skeleton_female.nif");
    }
    else
    {
      this->displayWarningMessage("Error: no path given for the custom skeleton.");
      return;
    }
  }

  // Message when the generation has completed successfully
  QString lSuccessText("Every file has been correctly generated. You can now exit the program or create another conversion! :)");
  QMessageBox lMessageBox(QMessageBox::Icon::Information, "Generation successful", lSuccessText);
  lMessageBox.exec();
  lMessageBox.deleteLater();
}
