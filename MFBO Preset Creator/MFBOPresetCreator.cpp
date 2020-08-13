#include "MFBOPresetCreator.h"

MFBOPresetCreator::MFBOPresetCreator(QWidget* parent)
  : QMainWindow(parent)
{
  // Construct the GUI
  ui.setupUi(this);
  this->initializeGUI();
}

void MFBOPresetCreator::initializeGUI()
{
  // Menu bar
  this->setupMenuBar();

  // Status bar
  //this->setupStatusBar();

  // Main window container
  auto lMainVertical{ new QVBoxLayout(this->ui.mainContainer) };

  // Setup all the different GUI components
  this->setupBodyMeshesGUI(*lMainVertical);
  this->setupBodySlideGUI(*lMainVertical);
  this->setupOptionsGUI(*lMainVertical);
  this->setupOutputGUI(*lMainVertical);
  this->setupRemainingGUI(*lMainVertical);
}

void MFBOPresetCreator::setupMenuBar()
{
  // Construct the menu bar
  auto lMenuBar{ new QMenuBar(this) };
  this->setMenuBar(lMenuBar);

  // File
  auto lFileMenu{ new QMenu(QString("File"), this) };
  lMenuBar->addMenu(lFileMenu);

  // Submenu: Exit
  auto lExitAction = new QAction();
  lExitAction->setText(tr("Exit"));
  lExitAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
  lExitAction->setIcon(QIcon(":/black/exit"));
  lFileMenu->addAction(lExitAction);

  // Tools
  auto lToolsMenu{ new QMenu(QString("Tools"), this) };
  lMenuBar->addMenu(lToolsMenu);

  // Submenu: Upgrader
  auto lUpgraderToolAction = new QAction();
  lUpgraderToolAction->setText(tr("Upgrade CBBE 3BBB version"));
  lUpgraderToolAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
  lUpgraderToolAction->setIcon(QIcon(":/black/arrow_up"));
  lToolsMenu->addAction(lUpgraderToolAction);

  // Submenu: Settings
  auto lSettingsAction = new QAction();
  lSettingsAction->setText(tr("Settings"));
  lSettingsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
  lSettingsAction->setIcon(QIcon(":/black/cog"));
  lToolsMenu->addAction(lSettingsAction);

  // Help
  auto lHelpMenu{ new QMenu(QString("Help"), this) };
  lMenuBar->addMenu(lHelpMenu);

  // Submenu: About
  auto lAboutAction = new QAction();
  lAboutAction->setText(tr("About"));
  lAboutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  lAboutAction->setIcon(QIcon(":/black/information"));
  lHelpMenu->addAction(lAboutAction);

  // Connect all action signals
  connect(lExitAction, SIGNAL(triggered()), this, SLOT(close()));
  connect(lUpgraderToolAction, SIGNAL(triggered()), this, SLOT(launchUpgraderTool()));
  connect(lSettingsAction, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
  connect(lAboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
}

void MFBOPresetCreator::setupStatusBar()
{
  // Divide the status bar in multiple parts
  this->statusBar()->addWidget(new QLabel(""), 1);
  this->statusBar()->addPermanentWidget(new QLabel("1"), 1);
  this->statusBar()->addPermanentWidget(new QLabel("2"), 1);
  this->statusBar()->addPermanentWidget(new QLabel("3"), 1);
  this->statusBar()->addPermanentWidget(new QLabel("4"), 0);
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
  auto lCbbe3BBBVersionLabel{ new QLabel("CBBE 3BBB version:") };
  lMeshesGridLayout->addWidget(lCbbe3BBBVersionLabel, 0, 0);

  QStringList lVersions;
  lVersions.append(QString("1.40"));
  lVersions.append(QString("1.50"));
  lVersions.append(QString("1.51"));

  auto lCbbe3BBBVersionSelector{ new QComboBox() };
  lCbbe3BBBVersionSelector->addItems(lVersions);
  lCbbe3BBBVersionSelector->setObjectName(QString("cbbe_3bbb_version"));
  lMeshesGridLayout->addWidget(lCbbe3BBBVersionSelector, 0, 1);

  // Second line
  auto lMeshesPathLabel{ new QLabel("Body meshes path:") };
  lMeshesGridLayout->addWidget(lMeshesPathLabel, 1, 0);

  auto lMeshesPathLineEdit{ new QLineEdit("") };
  lMeshesPathLineEdit->setObjectName("body_meshes_path_input");
  lMeshesGridLayout->addWidget(lMeshesPathLineEdit, 1, 1);

  // Beast hands
  auto lLabelBeastHands{ new QLabel("Beast hands?") };
  lMeshesGridLayout->addWidget(lLabelBeastHands, 2, 0);

  auto lNeedBeastHands{ new QCheckBox(QString("Check this box if the follower or NPC uses beast hands.")) };
  lNeedBeastHands->setObjectName("use_beast_hands");
  lMeshesGridLayout->addWidget(lNeedBeastHands, 2, 1);

  // Third line
  auto lMeshestitlePreview{ new QLabel("Preview:") };
  lMeshesGridLayout->addWidget(lMeshestitlePreview, 3, 0);

  auto lMeshesPathsPreview{ new QLabel("") };
  lMeshesPathsPreview->setObjectName("body_meshes_path_preview");
  lMeshesGridLayout->addWidget(lMeshesPathsPreview, 3, 1);

  // Initialization functions
  this->updateBodyMeshesPreview(QString(""));

  // Event binding
  connect(lCbbe3BBBVersionSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshAllPreviewInputs(int)));
  connect(lNeedBeastHands, SIGNAL(clicked()), this, SLOT(refreshAllPreviewInputs()));
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
  // Selected CBBE 3BBB version
  auto lCBBE3BBBVersionSelected{ this->ui.mainContainer->findChild<QComboBox*>(QString("cbbe_3bbb_version"))->currentIndex() };

  // Beast hands
  auto lMustUseBeastHands{ this->ui.mainContainer->findChild<QCheckBox*>("use_beast_hands")->isChecked() };

  // Path
  auto lOutputPathsPreview{ this->ui.mainContainer->findChild<QLabel*>("names_bodyslide_preview") };

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
  }

  QString lConstructedPreviewText{ "" };

  switch (lCBBE3BBBVersionSelected)
  {
  case Version1_40:
    if (lMustUseBeastHands)
    {
      lConstructedPreviewText =
        QStringLiteral(
          "%1 - 3BBB Body Amazing\n"
          "%1 - Feet\n"
          "%1 - Beast Hands"
        ).arg(aText);
    }
    else
    {
      lConstructedPreviewText =
        QStringLiteral(
          "%1 - 3BBB Body Amazing\n"
          "%1 - Feet\n"
          "%1 - Hands "
        ).arg(aText);
    }
    break;
  case Version1_50:
    if (lMustUseBeastHands)
    {
      lConstructedPreviewText =
        QStringLiteral(
          "%1 - CBBE 3BBB Body Amazing\n"
          "%1 - CBBE 3BBB Feet\n"
          "%1 - CBBE Beast Hands"
        ).arg(aText);
    }
    else
    {
      lConstructedPreviewText =
        QStringLiteral(
          "%1 - CBBE 3BBB Body Amazing\n"
          "%1 - CBBE 3BBB Feet\n"
          "%1 - CBBE 3BBB Hands"
        ).arg(aText);
    }
    break;
  case Version1_51:
    if (lMustUseBeastHands)
    {
      lConstructedPreviewText =
        QStringLiteral(
          "%1 - CBBE 3BBB Body Amazing\n"
          "%1 - CBBE 3BBB Feet\n"
          "%1 - CBBE 3BBB Hands Beast"
        ).arg(aText);
    }
    else
    {
      lConstructedPreviewText =
        QStringLiteral(
          "%1 - CBBE 3BBB Body Amazing\n"
          "%1 - CBBE 3BBB Feet\n"
          "%1 - CBBE 3BBB Hands"
        ).arg(aText);
    }
    break;
  default:
    lConstructedPreviewText = "Error while evaluating the data.";
    break;
  }

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
  // Selected CBBE 3BBB version
  auto lCBBE3BBBVersionSelected{ this->ui.mainContainer->findChild<QComboBox*>(QString("cbbe_3bbb_version"))->currentIndex() };

  // Beast hands
  auto lMustUseBeastHands{ this->ui.mainContainer->findChild<QCheckBox*>("use_beast_hands")->isChecked() };

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

  // Check if the path could be valid
  if (lEntryDirectory.startsWith("/"))
  {
    this->displayWarningMessage("Error: the path given to export the files seems to be invalid.");
    return;
  }

  // Create main directory
  if (!QDir(lEntryDirectory).exists())
  {
    // Wait to know the result of the mkdir()
    if (!QDir().mkdir(lEntryDirectory))
    {
      this->displayWarningMessage("Error while creating the main directory: \"" + lEntryDirectory + "\" could not be created on your computer. Did you execute the program with limited permissions?");
      return;
    }
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
  auto lRessourcesFolder{ QString("") };

  switch (lCBBE3BBBVersionSelected)
  {
  case Version1_40:
    lRessourcesFolder = "cbbe_3bbb_1.40";
    break;
  case Version1_50:
    lRessourcesFolder = "cbbe_3bbb_1.50";
    break;
  case Version1_51:
    lRessourcesFolder = "cbbe_3bbb_1.51";
    break;
  default:
    this->displayWarningMessage("Error while searching for the CBBE 3BBB version. If it happens, try restarting the program. If the error is still here after restarting the program, contact the developer team.");
    return;
  }

  // Copy the XML file
  if (lMustUseBeastHands)
  {
    if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_beast_hands_xml", lXMLPathName))
    {
      this->displayWarningMessage("The XML file could not be created. Did you execute the program with limited permissions?");
      return;
    }
  }
  else
  {
    if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_xml", lXMLPathName))
    {
      this->displayWarningMessage("The XML file could not be created. Did you execute the program with limited permissions?");
      return;
    }
  }

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

  // Copy the OSP file
  if (lMustUseBeastHands)
  {
    if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_beast_hands_osp", lOSPPathName))
    {
      this->displayWarningMessage("The OSP file could not be created. Did you execute the program with limited permissions?");
      return;
    }
  }
  else
  {
    if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_osp", lOSPPathName))
    {
      this->displayWarningMessage("The OSP file could not be created. Did you execute the program with limited permissions?");
      return;
    }
  }

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

      if (!QFile::copy(":/ressources/skeleton_female", lSkeletonDirectory + "/" + "skeleton_female.nif"))
      {
        this->displayWarningMessage("The skeleton file could not be created. Did you execute the program with limited permissions?");
        return;
      }
    }
    else
    {
      this->displayWarningMessage("Error: no path given for the custom skeleton.");
      return;
    }
  }

  // Message when the generation has completed successfully
  QString lSuccessText{ "" };

  switch (lCBBE3BBBVersionSelected)
  {
  case Version1_40:
    lSuccessText = "Every file has been correctly generated, for the version 1.40 and lower of CBBE 3BBB. You can now exit the program or create another conversion! :)";
    break;
  case Version1_50:
    lSuccessText = "Every file has been correctly generated, for the version 1.50 of CBBE 3BBB. You can now exit the program or create another conversion! :)";
    break;
  case Version1_51:
    lSuccessText = "Every file has been correctly generated, for the version 1.51 of CBBE 3BBB. You can now exit the program or create another conversion! :)";
    break;
  default:
    lSuccessText = "Every file has been correctly generated. You can now exit the program or create another conversion! :)";
    break;
  }

  QMessageBox lMessageBox(QMessageBox::Icon::Information, "Generation successful", lSuccessText);
  lMessageBox.exec();

  // Open the folder where the file structure has been created
  QDesktopServices::openUrl(lEntryDirectory);
}

void MFBOPresetCreator::refreshAllPreviewInputs(int)
{
  this->refreshAllPreviewInputs();
}

void MFBOPresetCreator::refreshAllPreviewInputs()
{
  // Refresh the names in the bodyslide software
  auto lBodyslideSlidersetsNames{ this->ui.mainContainer->findChild<QLineEdit*>("names_bodyslide_input")->text().trimmed() };
  this->updateBodyslideNamesPreview(lBodyslideSlidersetsNames);
}


void MFBOPresetCreator::launchUpgraderTool()
{

}

void MFBOPresetCreator::showSettingsDialog()
{
  // Construct the modal
  auto lDialog{ new QDialog(this) };
  lDialog->setWindowTitle(QString("Settings"));
  lDialog->setFixedSize(400, 300);

  // Construct and display the content of the modal
  QString lText("Settings will be available soon...");
  auto lModalContent{ new QLabel(lDialog) };
  lModalContent->setText(lText);
  lModalContent->adjustSize();

  // Display and delete the modal
  lDialog->exec();
  lDialog->deleteLater();
}

void MFBOPresetCreator::showAboutDialog()
{
  // Build the description
  auto lDescription{ QStringLiteral(
    "Mitsuriou's Follower Bodies Overhaul Preset Creator (MFBOPC) is a software "
    "created by Dylan Jacquemin (also known under the nickname \"Mitsuriou\").<br />"
    "This software has been developed to be provided for free to any user that wants to use the software. <br />"
    "The totality of the source code is available on GitHub.com, under "
    "<a href='https://github.com/Mitsuriou/MFBO-Preset-Creator'>this</a> link."
    "<br /><br />"
    "Ressources used to make this software:<br />"
    "&bull; <a href='https://www.qt.io'>Qt</a> (free version) is used for the Graphical User Iterface (GUI)."
    "<br /><br />"
    "Ressources bundled in this software:<br />"
    "&bull; The BodySlide files that are generated were taken from the "
    "<a href='https://www.nexusmods.com/skyrimspecialedition/mods/30174'>CBBE 3BBB</a> mod on NexusMod "
    "and slightly modified by Dylan Jacquemin.<br />"
    "&bull; The \"female_skeleton.nif\" file has been taken from the "
    "<a href='https://www.nexusmods.com/skyrimspecialedition/mods/1988'>XP32 Maximum Skeleton Special Extended - XPMSSE</a> "
    "mod on NexusMod. The file has not been modified."
  ) };

  // Construct the message box
  QMessageBox lDialog(QMessageBox::Icon::Information, "About", lDescription, QMessageBox::StandardButton::Close);
  lDialog.setIconPixmap(QPixmap(":/software/icon"));
  lDialog.setTextFormat(Qt::RichText);
  lDialog.adjustSize();

  // Display the message box
  lDialog.exec();
}
