#include "AssistedConversion.h"

AssistedConversion::AssistedConversion(QWidget* parent, const Struct::Settings& aSettings)
  : QDialog(parent)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void AssistedConversion::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("Assisted Conversion"));
  this->setWindowIcon(QIcon(QPixmap(":/black/arrow_up")));
}

void AssistedConversion::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QGridLayout(this)};

  this->setupInterface(*lMainVertical);
}

void AssistedConversion::setupInterface(QGridLayout& aLayout)
{
  // First line
  auto lInputPathLabel{new QLabel(tr("Input path:"), this)};
  aLayout.addWidget(lInputPathLabel, 0, 0);

  auto lInputPathLineEdit{new QLineEdit("", this)};
  lInputPathLineEdit->setReadOnly(true);
  lInputPathLineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  lInputPathLineEdit->setObjectName("input_path_directory");
  lInputPathLineEdit->setDisabled(true);
  aLayout.addWidget(lInputPathLineEdit, 0, 1);

  auto lInputPathChooser{new QPushButton(tr("Choose a directory..."), this)};
  lInputPathChooser->setCursor(Qt::PointingHandCursor);
  lInputPathChooser->setAutoDefault(false);
  lInputPathChooser->setDefault(false);
  aLayout.addWidget(lInputPathChooser, 0, 2);

  // Generate button
  auto lGenerateButton{new QPushButton(tr("[WIP] Launch the files search"), this)};
  lGenerateButton->setCursor(Qt::PointingHandCursor);
  lGenerateButton->setAutoDefault(false);
  lGenerateButton->setDefault(false);
  aLayout.addWidget(lGenerateButton, 1, 0, 1, 3, Qt::AlignBottom);

  // Event binding
  this->connect(lInputPathChooser, &QPushButton::clicked, this, &AssistedConversion::chooseInputDirectory);
  this->connect(lGenerateButton, &QPushButton::clicked, this, &AssistedConversion::launchUpDownGradeProcess);
}

void AssistedConversion::chooseInputDirectory()
{
  auto lLineEdit{this->findChild<QLineEdit*>("input_path_directory")};
  const auto& lPath{QFileDialog::getExistingDirectory(this, "", lLineEdit->text().size() > 0 ? lLineEdit->text() : QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))};
  lLineEdit->setText(lPath);
  lLineEdit->setDisabled(lPath.compare("", Qt::CaseInsensitive) == 0);
}

void AssistedConversion::launchUpDownGradeProcess()
{
}
