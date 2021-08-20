#include "ComponentFactory.h"
#include "Utils.h"
#include <QCheckBox>
#include <QFrame>
#include <QGridLayout>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>

QPushButton* ComponentFactory::createButton(
  QWidget* aParent,
  const QString& aText,
  const QString& aTooltipText,
  const QString& aIconName,
  const QString& aIconFolder,
  const QString& aObjectName,
  const bool aIsDisabled,
  const bool aMustDisableAutoDefaultBehavior)
{
  // Declare and instanciate the button
  auto lButton{new QPushButton(aText, aParent)};
  // Icon
  if (aIconFolder.length() > 0 && aIconName.length() > 0)
  {
    lButton->setIcon(QIcon(QPixmap(QString(":/%1/%2").arg(aIconFolder).arg(aIconName)).scaledToHeight(48, Qt::SmoothTransformation)));
  }
  // Mouse cursor
  lButton->setCursor(Qt::PointingHandCursor);
  // Tooltip
  if (aTooltipText.length() > 0)
  {
    lButton->setToolTip(aTooltipText);
  }
  else
  {
    lButton->setToolTip(aText);
  }
  // Object name
  if (aObjectName.length() > 0)
  {
    lButton->setObjectName(aObjectName);
  }
  // Disable AutoDefault and Default behaviors
  if (aMustDisableAutoDefaultBehavior)
  {
    lButton->setAutoDefault(false);
    lButton->setDefault(false);
  }
  // Disable the button
  lButton->setDisabled(aIsDisabled);

  return lButton;
}

QGridLayout* ComponentFactory::createScrollAreaWindowLayout(QWidget* aParent, const bool aMustForceLayoutTopAlignment, const bool aGenerateButtonsBottomLayout)
{
  // Very first container in which the scroll area will be added
  auto lBaseLayout{new QVBoxLayout(aParent)};
  lBaseLayout->setContentsMargins(0, 0, 0, 0);

  // Create a scroll area
  auto lScrollArea{new QScrollArea(aParent)};
  lScrollArea->setObjectName(QString("scrollable_zone"));
  lScrollArea->setContentsMargins(0, 0, 0, 0);
  lScrollArea->setWidgetResizable(true);
  // Remove the borders of the scroll area
  lScrollArea->setStyleSheet("QScrollArea{border: none;}");

  // Add a QFrame to permit automatic expanding of the content inside the scroll area
  auto lMainWidget{new QFrame(aParent)};

  // Main container
  auto lMainLayout{new QGridLayout(lMainWidget)};
  lMainLayout->setObjectName(QString("main_layout"));
  lMainLayout->setSpacing(10);
  lMainLayout->setContentsMargins(10, 10, 10, 10);
  if (aMustForceLayoutTopAlignment)
  {
    lMainLayout->setAlignment(Qt::AlignTop);
  }

  lScrollArea->setWidget(lMainWidget);
  lBaseLayout->addWidget(lScrollArea);

  // Button layout
  if (aGenerateButtonsBottomLayout)
  {
    auto lButtonLayout{new QHBoxLayout(aParent)};
    lButtonLayout->setObjectName(QString("window_buttons_layout"));
    lButtonLayout->setSpacing(10);
    lButtonLayout->setContentsMargins(10, 0, 10, 10);
    lButtonLayout->setAlignment(Qt::AlignTop);
    lBaseLayout->addLayout(lButtonLayout);
  }

  return lMainLayout;
}

QGridLayout* ComponentFactory::createScrollAreaComponentLayout(QWidget* aParent)
{
  auto lScrollArea{new QScrollArea(aParent)};
  lScrollArea->setObjectName(QString("scrollable_zone"));
  lScrollArea->setWidgetResizable(true);

  auto lMainWidget{new QFrame(aParent)};
  lScrollArea->setWidget(lMainWidget);

  auto lDataContainer{new QGridLayout(aParent)};
  lDataContainer->setObjectName(QString("data_container"));
  lDataContainer->setAlignment(Qt::AlignTop);
  lDataContainer->setContentsMargins(0, 0, 0, 0);

  lMainWidget->setLayout(lDataContainer);

  auto lMainLayout{qobject_cast<QGridLayout*>(aParent->layout())};
  lMainLayout->addWidget(lScrollArea, 2, 0, 1, 3);

  return lDataContainer;
}

void ComponentFactory::createOutputBox(QWidget* aParent, QGridLayout& aLayout, const int aLayoutRow, const int aLayoutCol, const QString& aIconFolder, const QString& aInitialOutputPath, const int aMinimumFirstColumnWidth, const int aFontSize)
{
  // Output group box
  auto lOutputGroupBox{new QGroupBox(tr("Files generation's output location").append("  "), aParent)};
  Utils::addIconToGroupBox(lOutputGroupBox, aIconFolder, "file-tree", aFontSize);
  Utils::setGroupBoxState(lOutputGroupBox, false);
  lOutputGroupBox->setObjectName(QString("output_group_box"));
  aLayout.addWidget(lOutputGroupBox, aLayoutRow, aLayoutCol);

  // Grid layout
  auto lOutputGridLayout{new QGridLayout(lOutputGroupBox)};
  lOutputGridLayout->setSpacing(10);
  lOutputGridLayout->setContentsMargins(15, 20, 15, 15);
  lOutputGridLayout->setAlignment(Qt::AlignTop);

  if (aMinimumFirstColumnWidth > 0)
  {
    lOutputGridLayout->setColumnMinimumWidth(0, aMinimumFirstColumnWidth);
  }

  // Main directory
  lOutputGridLayout->addWidget(new QLabel(tr("Output directory path:"), aParent), 0, 0);

  auto lOutputPathLineEdit{new QLineEdit(aParent)};
  lOutputPathLineEdit->setReadOnly(true);
  lOutputPathLineEdit->setObjectName(QString("output_path_directory"));
  if (aInitialOutputPath.length() > 0)
  {
    lOutputPathLineEdit->setText(aInitialOutputPath);
  }
  lOutputGridLayout->addWidget(lOutputPathLineEdit, 0, 1);

  // Main directory's file chooser button
  auto lOutputPathChooser{ComponentFactory::createButton(aParent, tr("Choose a directory..."), "", "folder", aIconFolder, "output_path_chooser")};
  lOutputGridLayout->addWidget(lOutputPathChooser, 0, 2);

  // Subdirectory
  lOutputGridLayout->addWidget(new QLabel(tr("Output subdirectory name/path:"), aParent), 1, 0);

  auto lOutputSubpathLineEdit{new QLineEdit(aParent)};
  lOutputSubpathLineEdit->setObjectName(QString("output_path_subdirectory"));
  lOutputGridLayout->addWidget(lOutputSubpathLineEdit, 1, 1);

  // Use only subdirectory path
  lOutputGridLayout->addWidget(new QLabel(tr("Use only subdirectory path?"), aParent), 2, 0);

  auto lUseOnlySubdir{new QCheckBox(tr("Check this box to define the export as only the subdirectory field (use at your own risk)."))};
  lUseOnlySubdir->setCursor(Qt::PointingHandCursor);
  lUseOnlySubdir->setObjectName(QString("only_use_subdirectory"));
  lOutputGridLayout->addWidget(lUseOnlySubdir, 2, 1, 1, 2);

  // Preview
  lOutputGridLayout->addWidget(new QLabel(tr("Preview:"), aParent), 3, 0);

  auto lOutputPathsPreview{new QLabel("", aParent)};
  lOutputPathsPreview->setObjectName(QString("output_path_preview"));
  lOutputPathsPreview->setAutoFillBackground(true);
  lOutputGridLayout->addWidget(lOutputPathsPreview, 3, 1);
}
