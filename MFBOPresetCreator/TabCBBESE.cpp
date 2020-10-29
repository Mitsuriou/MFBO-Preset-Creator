#include "TabCBBESE.h"

TabCBBESE::TabCBBESE(QWidget* aParent, Struct::Settings aSettings)
  : Tab(aParent, aSettings)
  , mMinimumFirstColmunWidth(275)
{
  // Setup all the different GUI components
  this->setupBodyMeshesGUI(*mMainVertical);
  this->setupBodySlideGUI(*mMainVertical);
  this->setupOptionsGUI(*mMainVertical);
  this->setupOutputGUI(*mMainVertical);
  this->setupRemainingGUI(*mMainVertical);
}

void TabCBBESE::setupBodyMeshesGUI(QVBoxLayout& aLayout)
{
  // CBBE body meshes group box
  auto lMeshesGroupBox{new QGroupBox(tr("CBBE body meshes"), this)};
  aLayout.addWidget(lMeshesGroupBox);

  // Grid layout
  auto lMeshesGridLayout{new QGridLayout(lMeshesGroupBox)};
  lMeshesGridLayout->setColumnMinimumWidth(0, mMinimumFirstColmunWidth);

  // First line
  auto lCbbe3BBBVersionLabel{new QLabel(tr("CBBE 3BBB version:"), this)};
  lMeshesGridLayout->addWidget(lCbbe3BBBVersionLabel, 0, 0);

  auto lCbbe3BBBVersionSelector{new QComboBox(this)};
  lCbbe3BBBVersionSelector->addItems(DataLists::getCBBE3BBBVersions());
  lCbbe3BBBVersionSelector->setCurrentIndex(static_cast<int>(mSettings.defaultMainWindowCBBE3BBBVersion));
  lCbbe3BBBVersionSelector->setObjectName(QString("cbbe_3bbb_version"));
  lMeshesGridLayout->addWidget(lCbbe3BBBVersionSelector, 0, 1, 1, 2);

  // Second line
  auto lMeshesPathLabel{new QLabel(tr("Meshes path:"), this)};
  lMeshesGridLayout->addWidget(lMeshesPathLabel, 1, 0);

  auto lMeshesPathLineEdit{new QLineEdit("", this)};
  lMeshesPathLineEdit->setObjectName("meshes_path_input");
  lMeshesPathLineEdit->setPlaceholderText("meshes/");
  lMeshesGridLayout->addWidget(lMeshesPathLineEdit, 1, 1, 1, 2);

  // Third line
  auto lLabelBeastHands{new QLabel(tr("Use beast hands?"), this)};
  lMeshesGridLayout->addWidget(lLabelBeastHands, 2, 0);

  auto lNeedBeastHands{new QCheckBox(tr("Check this box if the follower or NPC uses beast hands."), this)};
  lNeedBeastHands->setObjectName("use_beast_hands");
  lMeshesGridLayout->addWidget(lNeedBeastHands, 2, 1, 1, 2);

  // Fourth line
  auto lMeshestitlePreview{new QLabel(tr("Meshes names:"), this)};
  lMeshesGridLayout->addWidget(lMeshestitlePreview, 3, 0, 3, 1);

  auto lBodyMeshNameInput{new QLineEdit("", this)};
  lBodyMeshNameInput->setObjectName("body_mesh_name_input");
  lMeshesGridLayout->addWidget(lBodyMeshNameInput, 3, 1);
  lBodyMeshNameInput->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_ -]+"), this));
  lBodyMeshNameInput->setText("femalebody");
  lBodyMeshNameInput->setPlaceholderText("femalebody");

  auto lBodyMeshNameLabel1{new QLabel(tr("_0.nif/_1.nif"), this)};
  lMeshesGridLayout->addWidget(lBodyMeshNameLabel1, 3, 2);

  auto lFeetMeshNameInput{new QLineEdit("", this)};
  lFeetMeshNameInput->setObjectName("feet_mesh_name_input");
  lMeshesGridLayout->addWidget(lFeetMeshNameInput, 4, 1);
  lFeetMeshNameInput->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_ -]+"), this));
  lFeetMeshNameInput->setText("femalefeet");
  lFeetMeshNameInput->setPlaceholderText("femalefeet");

  auto lBodyMeshNameLabel2{new QLabel(tr("_0.nif/_1.nif"), this)};
  lMeshesGridLayout->addWidget(lBodyMeshNameLabel2, 4, 2);

  auto lHandsMeshNameInput{new QLineEdit("", this)};
  lHandsMeshNameInput->setObjectName("hands_mesh_name_input");
  lMeshesGridLayout->addWidget(lHandsMeshNameInput, 5, 1);
  lHandsMeshNameInput->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_ -]+"), this));
  lHandsMeshNameInput->setText("femalehands");
  lHandsMeshNameInput->setPlaceholderText("femalehands");

  auto lBodyMeshNameLabel3{new QLabel(tr("_0.nif/_1.nif"), this)};
  lMeshesGridLayout->addWidget(lBodyMeshNameLabel3, 5, 2);

  // Preview
  auto lLabelPreview{new QLabel(this)};
  lLabelPreview->setText(tr("Preview:"));
  lMeshesGridLayout->addWidget(lLabelPreview, 6, 0);

  auto lMeshesPreview{new QLabel(this)};
  lMeshesPreview->setObjectName("meshes_preview");
  lMeshesGridLayout->addWidget(lMeshesPreview, 6, 1, 1, 2);

  // Initialization functions
  this->updateMeshesPreview();

  // Event binding
  connect(lMeshesPathLineEdit, &QLineEdit::textChanged, this, qOverload<>(&TabCBBESE::refreshAllPreviewFields));
  connect(lBodyMeshNameInput, &QLineEdit::textChanged, this, qOverload<>(&TabCBBESE::refreshAllPreviewFields));
  connect(lFeetMeshNameInput, &QLineEdit::textChanged, this, qOverload<>(&TabCBBESE::refreshAllPreviewFields));
  connect(lHandsMeshNameInput, &QLineEdit::textChanged, this, qOverload<>(&TabCBBESE::refreshAllPreviewFields));
  connect(lCbbe3BBBVersionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&TabCBBESE::refreshAllPreviewFields));
  connect(lNeedBeastHands, &QCheckBox::clicked, this, qOverload<>(&TabCBBESE::refreshAllPreviewFields));
}

void TabCBBESE::setupBodySlideGUI(QVBoxLayout& aLayout)
{
  // BodySlide defined names group box
  auto lBodyslideGroupBox{new QGroupBox(tr("BodySlide"), this)};
  aLayout.addWidget(lBodyslideGroupBox);

  // Grid layout
  auto lBodyslideGridLayout{new QGridLayout(lBodyslideGroupBox)};
  lBodyslideGridLayout->setColumnMinimumWidth(0, mMinimumFirstColmunWidth);

  // First line
  auto lOSPXMLNames{new QLabel(tr("Bodyslide files names:"), this)};
  lBodyslideGridLayout->addWidget(lOSPXMLNames, 0, 0);

  auto lOSPXMLNamesLineEdit{new QLineEdit("", this)};
  lOSPXMLNamesLineEdit->setObjectName("names_osp_xml_input");
  lOSPXMLNamesLineEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_ -]+"), this));
  lBodyslideGridLayout->addWidget(lOSPXMLNamesLineEdit, 0, 1);

  // Second line
  auto lLabelOspXmlNames{new QLabel(tr("Preview:"), this)};
  lBodyslideGridLayout->addWidget(lLabelOspXmlNames, 1, 0);

  auto lPathsNamesOspXmlNames{new QLabel("", this)};
  lPathsNamesOspXmlNames->setObjectName("names_osp_xml_preview");
  lBodyslideGridLayout->addWidget(lPathsNamesOspXmlNames, 1, 1);

  // Third line
  auto lNamesInApp{new QLabel(this)};
  lNamesInApp->setTextFormat(Qt::RichText);
  lNamesInApp->setText(tr("Preset names: &#128712;"));
  lNamesInApp->setToolTip(QString(tr("This field represents the name under which the preset will be listed in the BodySlide software.")));
  lBodyslideGridLayout->addWidget(lNamesInApp, 2, 0);

  auto lNamesInAppLineEdit{new QLineEdit("", this)};
  lNamesInAppLineEdit->setObjectName("names_bodyslide_input");
  lBodyslideGridLayout->addWidget(lNamesInAppLineEdit, 2, 1);

  // Fourth line
  auto lLabelNamesInApp{new QLabel(tr("Preview:"), this)};
  lBodyslideGridLayout->addWidget(lLabelNamesInApp, 3, 0);

  auto lResultNamesInApp{new QLabel("", this)};
  lResultNamesInApp->setObjectName("names_bodyslide_preview");
  lBodyslideGridLayout->addWidget(lResultNamesInApp, 3, 1);

  // Initialization functions
  this->updateOSPXMLPreview(QString(""));
  this->updateBodyslideNamesPreview(QString(""));

  // Event binding
  connect(lOSPXMLNamesLineEdit, &QLineEdit::textChanged, this, &TabCBBESE::updateOSPXMLPreview);
  connect(lNamesInAppLineEdit, &QLineEdit::textChanged, this, &TabCBBESE::updateBodyslideNamesPreview);
}

void TabCBBESE::setupOptionsGUI(QVBoxLayout& aLayout)
{
  // Custom skeleton and textures group box
  auto lOptionsGroupBox{new QGroupBox(tr("Additional options"), this)};
  aLayout.addWidget(lOptionsGroupBox);

  auto lOptionsGridLayout{new QGridLayout(lOptionsGroupBox)};
  lOptionsGridLayout->setColumnMinimumWidth(0, mMinimumFirstColmunWidth);

  // Skeleton
  auto lLabelSkeleton{new QLabel("", this)};
  lLabelSkeleton->setTextFormat(Qt::RichText);
  lLabelSkeleton->setText(tr("Use a custom skeleton? &#128712;"));
  lLabelSkeleton->setToolTip(QString(tr("Note: not overriding a custom skeleton would cause breasts collision and physics to be inaccurate.")));
  lOptionsGridLayout->addWidget(lLabelSkeleton, 0, 0);

  auto lNeedCustomSkeleton{new QCheckBox(tr("Check this box if the follower or NPC uses a custom skeleton."), this)};
  lNeedCustomSkeleton->setObjectName("use_custom_skeleton");
  lOptionsGridLayout->addWidget(lNeedCustomSkeleton, 0, 1);

  // Skeleton path
  auto lLabelSkeletonPath{new QLabel(tr("Skeleton path:"), this)};
  lOptionsGridLayout->addWidget(lLabelSkeletonPath, 1, 0);

  auto lSkeletonPathLineEdit{new QLineEdit("", this)};
  lSkeletonPathLineEdit->setDisabled(true);
  lSkeletonPathLineEdit->setObjectName("skeleton_path_directory");
  lOptionsGridLayout->addWidget(lSkeletonPathLineEdit, 1, 1);

  // Skeleton path preview
  auto lSkeletontitlePreview{new QLabel(tr("Preview:"), this)};
  lOptionsGridLayout->addWidget(lSkeletontitlePreview, 2, 0);

  auto lSkeletonPathsPreview{new QLabel("", this)};
  lSkeletonPathsPreview->setObjectName("skeleton_path_preview");
  lOptionsGridLayout->addWidget(lSkeletonPathsPreview, 2, 1);

  // Initialization functions
  this->updateSkeletonPreview(QString(""));

  // Event binding
  connect(lNeedCustomSkeleton, &QCheckBox::stateChanged, this, &TabCBBESE::updateSkeletonPathState);
  lNeedCustomSkeleton->setChecked(true);
  lNeedCustomSkeleton->setChecked(false);
  connect(lSkeletonPathLineEdit, &QLineEdit::textChanged, this, &TabCBBESE::updateSkeletonPreview);
}

void TabCBBESE::setupOutputGUI(QVBoxLayout& aLayout)
{
  // Output group box
  auto lOutputGroupBox{new QGroupBox(tr("Output"), this)};
  aLayout.addWidget(lOutputGroupBox);

  // Grid layout
  auto lOutputGridLayout{new QGridLayout(lOutputGroupBox)};
  lOutputGridLayout->setColumnMinimumWidth(0, mMinimumFirstColmunWidth);

  // First line
  auto lOutputPathLabel{new QLabel(tr("Output directory path:"), this)};
  lOutputGridLayout->addWidget(lOutputPathLabel, 0, 0);

  auto lOutputPathLineEdit{new QLineEdit("", this)};
  lOutputPathLineEdit->setReadOnly(true);
  lOutputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lOutputPathLineEdit->setObjectName("output_path_directory");
  lOutputPathLineEdit->setText(mSettings.mainWindowOutputPath);
  lOutputGridLayout->addWidget(lOutputPathLineEdit, 0, 1);

  auto lOutputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lOutputGridLayout->addWidget(lOutputPathChooser, 0, 2);

  // Second line
  auto lLabelSubDirectoryPath{new QLabel(tr("Output subdirectory name/path:"), this)};
  lOutputGridLayout->addWidget(lLabelSubDirectoryPath, 1, 0);

  auto lOutputSubpathLineEdit{new QLineEdit("", this)};
  lOutputSubpathLineEdit->setObjectName("output_path_subdirectory");
  lOutputGridLayout->addWidget(lOutputSubpathLineEdit, 1, 1);

  // Third line
  auto lOutputtitlePreview{new QLabel(tr("Preview:"), this)};
  lOutputGridLayout->addWidget(lOutputtitlePreview, 2, 0);

  auto lOutputPathsPreview{new QLabel("", this)};
  lOutputPathsPreview->setObjectName("output_path_preview");
  lOutputGridLayout->addWidget(lOutputPathsPreview, 2, 1);

  // Event binding
  connect(lOutputPathChooser, &QPushButton::clicked, this, &TabCBBESE::chooseExportDirectory);
  connect(lOutputSubpathLineEdit, &QLineEdit::textChanged, this, &TabCBBESE::updateOutputPreview);

  // Pre-filled data
  this->updateOutputPreview();
}

void TabCBBESE::setupRemainingGUI(QVBoxLayout& aLayout)
{
  // Generate button
  auto lGenerateButton{new QPushButton(tr("Generate the files on my computer"), this)};
  aLayout.addWidget(lGenerateButton);

  // Event binding
  connect(lGenerateButton, &QPushButton::clicked, this, &TabCBBESE::generateDirectoryStructure);
}

void TabCBBESE::updateMeshesPreview()
{
  // Get all input fields
  auto lMeshesPath{this->findChild<QLineEdit*>("meshes_path_input")->text().trimmed()};
  Utils::cleanPathString(lMeshesPath);

  auto lBodyName{this->findChild<QLineEdit*>("body_mesh_name_input")->text().trimmed()};
  auto lFeetName{this->findChild<QLineEdit*>("feet_mesh_name_input")->text().trimmed()};
  auto lHandsName{this->findChild<QLineEdit*>("hands_mesh_name_input")->text().trimmed()};

  // Get preview label
  auto lPreviewLabel{this->findChild<QLabel*>("meshes_preview")};
  auto lFullPreview(QString(""));
  auto lIsValidPath{true};

  if (lMeshesPath == "")
  {
    lMeshesPath = "*";
    lIsValidPath = false;
  }

  if (lBodyName == "")
  {
    lBodyName = "femalebody";
  }

  if (lFeetName == "")
  {
    lFeetName = "femalefeet";
  }

  if (lHandsName == "")
  {
    lHandsName = "femalehands";
  }

  lFullPreview += QStringLiteral("[...]/Skyrim Special Edition/Data/%1/%2_0.nif\n").arg(lMeshesPath).arg(lBodyName);
  lFullPreview += QStringLiteral("[...]/Skyrim Special Edition/Data/%1/%2_1.nif\n").arg(lMeshesPath).arg(lBodyName);
  lFullPreview += QStringLiteral("[...]/Skyrim Special Edition/Data/%1/%2_0.nif\n").arg(lMeshesPath).arg(lFeetName);
  lFullPreview += QStringLiteral("[...]/Skyrim Special Edition/Data/%1/%2_1.nif\n").arg(lMeshesPath).arg(lFeetName);
  lFullPreview += QStringLiteral("[...]/Skyrim Special Edition/Data/%1/%2_0.nif\n").arg(lMeshesPath).arg(lHandsName);
  lFullPreview += QStringLiteral("[...]/Skyrim Special Edition/Data/%1/%2_0.nif").arg(lMeshesPath).arg(lHandsName);

  if (lIsValidPath)
  {
    if (lMeshesPath.startsWith("meshes/") && lMeshesPath.length() > 7)
    {
      lPreviewLabel->setStyleSheet("");
    }
    else
    {
      lPreviewLabel->setStyleSheet("QLabel {color: #FF9800;}");
    }
  }
  else
  {
    lPreviewLabel->setStyleSheet("QLabel {color: #F44336;}");
  }

  lPreviewLabel->setText(lFullPreview);
}

void TabCBBESE::updateOutputPreview()
{
  // Get main directory
  auto lMainDirTextEdit{this->findChild<QLineEdit*>("output_path_directory")};
  auto lMainDirectory{lMainDirTextEdit->text().trimmed()};
  Utils::cleanPathString(lMainDirectory);

  // Get subdirectory
  auto lSubDirectory{this->findChild<QLineEdit*>("output_path_subdirectory")->text().trimmed()};
  Utils::cleanPathString(lSubDirectory);
  auto lIsValidPath{true};

  // Construct full path
  auto lFullPath(QString(""));
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
    lFullPath = tr("No path given or invalid path given.");
    lMainDirTextEdit->setDisabled(true);
    lIsValidPath = false;
  }

  // Set the full path value in the preview label
  auto lOutputPathsPreview{this->findChild<QLabel*>("output_path_preview")};

  if (lIsValidPath)
  {
    if (QDir(lFullPath).exists())
    {
      lOutputPathsPreview->setStyleSheet("QLabel {color: #FF9800;}");
    }
    else
    {
      lOutputPathsPreview->setStyleSheet("");
    }
  }
  else
  {
    lOutputPathsPreview->setStyleSheet("QLabel {color: #F44336;}");
  }

  lOutputPathsPreview->setText(lFullPath);
}

void TabCBBESE::updateOSPXMLPreview(QString aText)
{
  auto lOutputPathsPreview{this->findChild<QLabel*>("names_osp_xml_preview")};

  auto lIsValidPath{true};

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
    lIsValidPath = false;
  }

  auto lConstructedPreviewText(
    QStringLiteral(
      "[...]/Skyrim Special Edition/Data/CalienteTools/BodySlide/SliderGroups/%1.xml\n"
      "[...]/Skyrim Special Edition/Data/CalienteTools/BodySlide/SliderSets/%1.osp")
      .arg(aText));

  if (lIsValidPath)
  {
    lOutputPathsPreview->setStyleSheet("");
  }
  else
  {
    lOutputPathsPreview->setStyleSheet("QLabel {color: #F44336;}");
  }

  lOutputPathsPreview->setText(lConstructedPreviewText);
}

void TabCBBESE::updateBodyslideNamesPreview(QString aText)
{
  // Selected CBBE 3BBB version
  auto lCBBE3BBBVersionSelected{this->findChild<QComboBox*>(QString("cbbe_3bbb_version"))->currentIndex()};

  // Beast hands
  auto lMustUseBeastHands{this->findChild<QCheckBox*>("use_beast_hands")->isChecked()};

  // Path
  auto lOutputPathsPreview{this->findChild<QLabel*>("names_bodyslide_preview")};

  auto lIsValidPath{true};

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
    lIsValidPath = false;
  }

  auto lConstructedPreviewText{QString("")};

  switch (lCBBE3BBBVersionSelected)
  {
    case static_cast<int>(CBBE3BBBVersion::Version1_40):
      if (lMustUseBeastHands)
      {
        lConstructedPreviewText = QStringLiteral(
                                    "%1 - 3BBB Body Amazing\n"
                                    "%1 - Feet\n"
                                    "%1 - Beast Hands")
                                    .arg(aText);
      }
      else
      {
        lConstructedPreviewText = QStringLiteral(
                                    "%1 - 3BBB Body Amazing\n"
                                    "%1 - Feet\n"
                                    "%1 - Hands ")
                                    .arg(aText);
      }
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_50):
      if (lMustUseBeastHands)
      {
        lConstructedPreviewText = QStringLiteral(
                                    "%1 - CBBE 3BBB Body Amazing\n"
                                    "%1 - CBBE 3BBB Feet\n"
                                    "%1 - CBBE Beast Hands")
                                    .arg(aText);
      }
      else
      {
        lConstructedPreviewText = QStringLiteral(
                                    "%1 - CBBE 3BBB Body Amazing\n"
                                    "%1 - CBBE 3BBB Feet\n"
                                    "%1 - CBBE 3BBB Hands")
                                    .arg(aText);
      }
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
      if (lMustUseBeastHands)
      {
        lConstructedPreviewText = QStringLiteral(
                                    "%1 - CBBE 3BBB Body Amazing\n"
                                    "%1 - CBBE 3BBB Feet\n"
                                    "%1 - CBBE 3BBB Hands Beast")
                                    .arg(aText);
      }
      else
      {
        lConstructedPreviewText = QStringLiteral(
                                    "%1 - CBBE 3BBB Body Amazing\n"
                                    "%1 - CBBE 3BBB Feet\n"
                                    "%1 - CBBE 3BBB Hands")
                                    .arg(aText);
      }
      break;
    default:
      lConstructedPreviewText = tr("Error while evaluating the data.");
      break;
  }

  if (lIsValidPath)
  {
    lOutputPathsPreview->setStyleSheet("");
  }
  else
  {
    lOutputPathsPreview->setStyleSheet("QLabel {color: #F44336;}");
  }

  lOutputPathsPreview->setText(lConstructedPreviewText);
}

void TabCBBESE::updateSkeletonPathState(int aState)
{
  auto lSkeletonPathLineEdit{this->findChild<QLineEdit*>("skeleton_path_directory")};
  auto lSkeletonPreview{this->findChild<QLabel*>("skeleton_path_preview")};

  switch (aState)
  {
    case Qt::Unchecked:
      lSkeletonPathLineEdit->setDisabled(true);
      lSkeletonPreview->setDisabled(true);
      lSkeletonPreview->setStyleSheet("");
      break;
    case Qt::Checked:
      lSkeletonPathLineEdit->setDisabled(false);
      lSkeletonPreview->setDisabled(false);

      if (this->findChild<QLineEdit*>("skeleton_path_directory")->text().trimmed().length() == 0)
      {
        lSkeletonPreview->setStyleSheet("QLabel {color: #F44336;}");
      }

      break;
    default:
      break;
  }
}

void TabCBBESE::updateSkeletonPreview(QString aText)
{
  Utils::cleanPathString(aText);
  auto lIsValidPath{true};

  if (aText.trimmed().length() == 0)
  {
    aText = QString::fromStdString("*");
    lIsValidPath = false;
  }

  auto lConstructedPath(QStringLiteral("[...]/Skyrim Special Edition/Data/%1/skeleton_female.nif").arg(aText));
  auto lOutputPathPreview{this->findChild<QLabel*>("skeleton_path_preview")};

  if (lIsValidPath)
  {
    lOutputPathPreview->setStyleSheet("");
  }
  else if (this->findChild<QCheckBox*>("use_custom_skeleton")->isChecked())
  {
    lOutputPathPreview->setStyleSheet("QLabel {color: #F44336;}");
  }

  lOutputPathPreview->setText(lConstructedPath);
}

void TabCBBESE::chooseExportDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("output_path_directory")};

  auto lSetGUIPath{lLineEdit->text()};
  auto lPreSelectedDirectory{QString("")};
  if (lSetGUIPath == "")
  {
    lPreSelectedDirectory = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
  }
  else
  {
    lPreSelectedDirectory = lSetGUIPath;
  }

  auto lPath{QFileDialog::getExistingDirectory(this, "", lPreSelectedDirectory)};
  lLineEdit->setText(lPath);
  this->updateOutputPreview();
}

void TabCBBESE::generateDirectoryStructure()
{
  // Selected CBBE 3BBB version
  auto lCBBE3BBBVersionSelected{this->findChild<QComboBox*>(QString("cbbe_3bbb_version"))->currentIndex()};

  // Beast hands
  auto lMustUseBeastHands{this->findChild<QCheckBox*>("use_beast_hands")->isChecked()};

  // Body meshes path
  auto lBodyMeshesPath{this->findChild<QLineEdit*>("meshes_path_input")->text().trimmed()};
  Utils::cleanPathString(lBodyMeshesPath);

  // Nif files names
  auto lFemaleBodyNifName{this->findChild<QLineEdit*>("body_mesh_name_input")->text().trimmed()};
  auto lFemaleFeetNifName{this->findChild<QLineEdit*>("feet_mesh_name_input")->text().trimmed()};
  auto lFemaleHandsNifName{this->findChild<QLineEdit*>("hands_mesh_name_input")->text().trimmed()};

  // BodySlide names
  auto lOSPXMLNames{this->findChild<QLineEdit*>("names_osp_xml_input")->text().trimmed()};
  auto lBodyslideSlidersetsNames{this->findChild<QLineEdit*>("names_bodyslide_input")->text().trimmed()};

  // Options
  auto lMustCopySkeleton{this->findChild<QCheckBox*>("use_custom_skeleton")->isChecked()};
  auto lSkeletonPath{this->findChild<QLineEdit*>("skeleton_path_directory")->text().trimmed()};
  Utils::cleanPathString(lSkeletonPath);

  // Output paths
  auto lMainDirectory{this->findChild<QLineEdit*>("output_path_directory")->text().trimmed()};
  auto lSubDirectory{this->findChild<QLineEdit*>("output_path_subdirectory")->text().trimmed()};
  Utils::cleanPathString(lSubDirectory);

  // Full extract path
  auto lEntryDirectory{(lSubDirectory.length() == 0 ? lMainDirectory : (lMainDirectory + QDir::separator() + lSubDirectory))};

  // Check if the full extract path has been given by the user
  if (lEntryDirectory.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no path given to export the files."));
    return;
  }

  // Check if the path could be valid
  if (lEntryDirectory.startsWith(QDir::separator()))
  {
    Utils::displayWarningMessage(tr("Error: the path given to export the files seems to be invalid."));
    return;
  }

  // Create main directory
  if (!QDir(lEntryDirectory).exists())
  {
    // Wait to know the result of the mkdir()
    if (!QDir().mkdir(lEntryDirectory))
    {
      Utils::displayWarningMessage(tr("Error while creating the main directory: \"") + lEntryDirectory + tr("\" could not be created on your computer. Did you execute the program with limited permissions?"));
      return;
    }
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while creating the main directory: \"") + lEntryDirectory + tr("\" already exists on your computer."));
    return;
  }

  // Export the meshes
  if (lBodyMeshesPath.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no path has been given for the meshes."));
    return;
  }

  // Check if a name has been given for the OSP and XML files
  if (lOSPXMLNames.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no name given for the BodySlide files."));
    return;
  }

  // Check if a name has been given for the presets
  if (lBodyslideSlidersetsNames.length() == 0)
  {
    Utils::displayWarningMessage(tr("Error: no name given for the slider sets (names that appear in the BodySlide software)."));
    return;
  }

  // Create the SliderGroups directory
  auto lSliderGroupsDirectory{lEntryDirectory + QDir::separator() + "CalienteTools" + QDir::separator() + "BodySlide" + QDir::separator() + "SliderGroups"};
  if (!QDir(lSliderGroupsDirectory).exists())
  {
    QDir().mkpath(lSliderGroupsDirectory);
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while creating the meshes directory: \"") + lSliderGroupsDirectory + tr("\" already exists."));
    return;
  }

  // Copy the QRC file and change the slidergroups names in the XML file
  auto lXMLPathName{lSliderGroupsDirectory + QDir::separator() + lOSPXMLNames + ".xml"};
  auto lRessourcesFolder{QString("")};

  switch (lCBBE3BBBVersionSelected)
  {
    case static_cast<int>(CBBE3BBBVersion::Version1_40):
      lRessourcesFolder = "cbbe_3bbb_1.40";
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_50):
      lRessourcesFolder = "cbbe_3bbb_1.50";
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
      lRessourcesFolder = "cbbe_3bbb_1.51";
      break;
    default:
      Utils::displayWarningMessage(tr("Error while searching for the CBBE 3BBB version. If it happens, try restarting the program. If the error is still here after restarting the program, contact the developer."));
      return;
  }

  // Copy the XML file
  if (lMustUseBeastHands)
  {
    if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_beast_hands_xml", lXMLPathName))
    {
      Utils::displayWarningMessage(tr("The XML file could not be created. Did you execute the program with limited permissions?"));
      return;
    }
  }
  else
  {
    if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_xml", lXMLPathName))
    {
      Utils::displayWarningMessage(tr("The XML file could not be created. Did you execute the program with limited permissions?"));
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
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lXMLPathName + tr("\"."));
    return;
  }

  if (lTempXMLContent.length() > 0)
  {
    if (lXMLFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
      auto lTextToParse{static_cast<QString>(lTempXMLContent)};
      lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), lBodyslideSlidersetsNames);

      QTextStream lTextStream(&lXMLFile);
      lTextStream << lTextToParse;
      lTextStream.flush();

      lXMLFile.close();
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lXMLPathName + tr("\"."));
      return;
    }
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to parse the XML BodySlide file."));
    return;
  }

  // Create the SliderSets directory
  auto lSliderSetsDirectory{lEntryDirectory + QDir::separator() + "CalienteTools" + QDir::separator() + "BodySlide" + QDir::separator() + "SliderSets"};

  if (!QDir(lSliderSetsDirectory).exists())
  {
    QDir().mkpath(lSliderSetsDirectory);
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while creating the meshes directory: \"") + lSliderSetsDirectory + tr("\" already exists."));
    return;
  }

  // Copy the QRC file and change the slidergroups names in the OSP file
  auto lOSPPathName(lSliderSetsDirectory + QDir::separator() + lOSPXMLNames + ".osp");

  // Copy the OSP file
  if (lMustUseBeastHands)
  {
    if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_beast_hands_osp", lOSPPathName))
    {
      Utils::displayWarningMessage(tr("The OSP file could not be created. Did you execute the program with limited permissions?"));
      return;
    }
  }
  else
  {
    if (!QFile::copy(":/" + lRessourcesFolder + "/bodyslide_osp", lOSPPathName))
    {
      Utils::displayWarningMessage(tr("The OSP file could not be created. Did you execute the program with limited permissions?"));
      return;
    }
  }

  QFile lOSPFile(lOSPPathName);
  lOSPFile.setPermissions(QFile::WriteUser);

  QByteArray lTempOSPContent;

  if (lOSPFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    lTempOSPContent = lOSPFile.readAll();
    lOSPFile.close();
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lOSPPathName + tr("\"."));
    return;
  }

  // Replace the slider sets names
  if (lTempOSPContent.length() > 0)
  {
    if (lOSPFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
      auto lTextToParse{static_cast<QString>(lTempOSPContent)};
      lTextToParse.replace(QString("{%%bodyslide_set_name%%}"), lBodyslideSlidersetsNames);
      lTextToParse.replace(QString("{%%body_output_path%%}"), lBodyMeshesPath.replace("/", "\\"));
      lTextToParse.replace(QString("{%%feet_output_path%%}"), lBodyMeshesPath.replace("/", "\\"));
      lTextToParse.replace(QString("{%%hands_output_path%%}"), lBodyMeshesPath.replace("/", "\\"));

      lTextToParse.replace(QString("{%%body_output_file%%}"), lFemaleBodyNifName.size() > 0 ? lFemaleBodyNifName : "femalebody");
      lTextToParse.replace(QString("{%%feet_output_file%%}"), lFemaleFeetNifName.size() > 0 ? lFemaleFeetNifName : "femalefeet");
      lTextToParse.replace(QString("{%%hands_output_file%%}"), lFemaleHandsNifName.size() > 0 ? lFemaleHandsNifName : "femalehands");

      QTextStream lTextStream(&lOSPFile);
      lTextStream << lTextToParse;
      lTextStream.flush();

      lOSPFile.close();
    }
    else
    {
      Utils::displayWarningMessage(tr("Error while trying to open the file \"") + lOSPPathName + tr("\"."));
      return;
    }
  }
  else
  {
    Utils::displayWarningMessage(tr("Error while trying to parse the OSP BodySlide file."));
    return;
  }

  // If the user checked the custom skeleton
  if (lMustCopySkeleton)
  {
    if (lSkeletonPath.length() > 0)
    {
      auto lSkeletonDirectory{lEntryDirectory + QDir::separator() + lSkeletonPath};
      QDir().mkpath(lSkeletonDirectory);

      if (!QFile::copy(":/ressources/skeleton_female", lSkeletonDirectory + QDir::separator() + "skeleton_female.nif"))
      {
        Utils::displayWarningMessage(tr("The skeleton file could not be created. Did you execute the program with limited permissions?"));
        return;
      }
    }
    else
    {
      Utils::displayWarningMessage(tr("Error: no path given for the custom skeleton."));
      return;
    }
  }

  // Message when the generation has completed successfully
  auto lSuccessText{QString("")};

  switch (lCBBE3BBBVersionSelected)
  {
    case static_cast<int>(CBBE3BBBVersion::Version1_40):
      lSuccessText = tr("Every file has been correctly generated, for the version 1.40 and lower of CBBE 3BBB. You can now exit the program or create another conversion! :)");
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_50):
      lSuccessText = tr("Every file has been correctly generated, for the version 1.50 of CBBE 3BBB. You can now exit the program or create another conversion! :)");
      break;
    case static_cast<int>(CBBE3BBBVersion::Version1_51_and_1_52):
      lSuccessText = tr("Every file has been correctly generated, for the version 1.51 and 1.52 of CBBE 3BBB. You can now exit the program or create another conversion! :)");
      break;
    default:
      lSuccessText = tr("Every file has been correctly generated. You can now exit the program or create another conversion! :)");
      break;
  }

  auto lReply{QMessageBox::information(this, tr("Generation successful"), lSuccessText, QMessageBox::Ok, QMessageBox::Ok)};

  if (lReply == QMessageBox::Ok)
  {
    if (mSettings.mainWindowAutomaticallyOpenGeneratedDirectory)
    {
      // Open the folder where the file structure has been created
      QDesktopServices::openUrl(QUrl::fromLocalFile(lEntryDirectory));
    }
  }
}

void TabCBBESE::refreshAllPreviewFields(int)
{
  this->refreshAllPreviewFields();
}

void TabCBBESE::refreshAllPreviewFields()
{
  // Refresh the preview of the body meshes parts
  this->updateMeshesPreview();

  // Refresh the names in the bodyslide software
  auto lBodyslideSlidersetsNames{this->findChild<QLineEdit*>("names_bodyslide_input")->text().trimmed()};
  this->updateBodyslideNamesPreview(lBodyslideSlidersetsNames);
}
