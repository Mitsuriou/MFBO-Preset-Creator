#include "ComponentFactory.h"
#include "Utils.h"
#include <QCheckBox>
#include <QFrame>
#include <QGridLayout>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>

QPushButton* ComponentFactory::CreateButton(
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
  if (!aIconFolder.isEmpty() && !aIconName.isEmpty())
  {
    lButton->setIcon(QIcon(QPixmap(QString(":/%1/%2").arg(aIconFolder, aIconName))));
    lButton->setIconSize(QSize(17, 17)); // TODO: Multiply the size by the DPI scale
  }
  // Mouse cursor
  lButton->setCursor(Qt::PointingHandCursor);
  // Tooltip
  if (!aTooltipText.isEmpty())
  {
    lButton->setToolTip(aTooltipText);
  }
  else
  {
    lButton->setToolTip(aText);
  }
  // Object name
  if (!aObjectName.isEmpty())
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

QCheckBox* ComponentFactory::CreateCheckBox(QWidget* aParent, const QString& aText, const QString& aTooltipText, const QString& aObjectName, const bool lIsChecked)
{
  // Declare and instanciate the checkbox
  auto lCheckBox{new QCheckBox(aText, aParent)};
  // Mouse cursor
  lCheckBox->setCursor(Qt::PointingHandCursor);
  // Tooltip
  if (!aTooltipText.isEmpty())
  {
    lCheckBox->setToolTip(aTooltipText);
  }
  else
  {
    lCheckBox->setToolTip(aText);
  }
  // Object name
  lCheckBox->setObjectName(aObjectName);
  // Default check state
  lCheckBox->setChecked(lIsChecked);

  return lCheckBox;
}

QGridLayout* ComponentFactory::CreateScrollAreaWindowLayout(QWidget* aParent,
                                                            const bool aMustForceLayoutTopAlignment,
                                                            const bool aGenerateButtonsBottomLayout,
                                                            QVBoxLayout* aParentLayout,
                                                            const QString& aScrollAreaObjectName,
                                                            const QMargins& aNestedLayoutMargins,
                                                            const QString& aNestedLayoutObjectName)
{
  if (aParentLayout == nullptr)
  {
    // Very first container in which the scroll area will be added
    aParentLayout = new QVBoxLayout(aParent);
    aParentLayout->setContentsMargins(0, 0, 0, 0);
  }

  // Create a scroll area
  auto lScrollArea{new QScrollArea(aParent)};
  lScrollArea->setObjectName(aScrollAreaObjectName);
  lScrollArea->setContentsMargins(0, 0, 0, 0);
  lScrollArea->setWidgetResizable(true);
  lScrollArea->setStyleSheet("QScrollArea{border: none;}");

  // Add a QFrame to permit automatic expanding of the content inside the scroll area
  auto lScrollAreaWidgetContent{new QFrame(lScrollArea)};

  // Main container
  auto lNestedLayout{new QGridLayout(lScrollAreaWidgetContent)};
  lNestedLayout->setObjectName(aNestedLayoutObjectName);
  lNestedLayout->setSpacing(10);
  lNestedLayout->setContentsMargins(aNestedLayoutMargins);
  if (aMustForceLayoutTopAlignment)
  {
    lNestedLayout->setAlignment(Qt::AlignTop);
  }

  lScrollAreaWidgetContent->setLayout(lNestedLayout);
  lScrollArea->setWidget(lScrollAreaWidgetContent);
  aParentLayout->addWidget(lScrollArea);

  // Bottom layout for buttons
  if (aGenerateButtonsBottomLayout)
  {
    auto lButtonLayout{new QHBoxLayout(aParent)};
    lButtonLayout->setObjectName(QString("window_buttons_layout"));
    lButtonLayout->setSpacing(10);
    lButtonLayout->setContentsMargins(10, 0, 10, 10);
    lButtonLayout->setAlignment(Qt::AlignTop);
    aParentLayout->addLayout(lButtonLayout);
  }

  return lNestedLayout;
}

QGridLayout* ComponentFactory::CreateScrollAreaComponentLayout(QWidget* aParent)
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

QPushButton* ComponentFactory::CreateTargetMeshesPickerLine(QWidget* aParent,
                                                            QGridLayout& aLayout,
                                                            const bool aSingleLineForLabelAndActions,
                                                            const int aLayoutRow,
                                                            const QString& aIconFolder,
                                                            const QString& aButtonObjectName,
                                                            const QString& aLabelObjectName)
{
  // Targeted body and version
  aLayout.addWidget(new QLabel(tr("Targeted meshes mods:"), aParent), aLayoutRow, 0);

  QHBoxLayout* lBodyNameVersionWrapper{new QHBoxLayout(aParent)};
  lBodyNameVersionWrapper->setMargin(0);
  aLayout.addLayout(lBodyNameVersionWrapper,
                    aLayoutRow + aSingleLineForLabelAndActions ? 0 : 1,
                    aSingleLineForLabelAndActions ? 1 : 0);

  auto lTargetMeshesPicker{ComponentFactory::CreateButton(aParent, tr("Choose target meshes"), "", "mesh", aIconFolder, aButtonObjectName, false, true)};
  lBodyNameVersionWrapper->addWidget(lTargetMeshesPicker);

  auto lCurrentlyTargetedBody{new QLabel(tr("Targeted body: -\nTargeted feet: -"), aParent)};
  lCurrentlyTargetedBody->setObjectName(aLabelObjectName);
  lBodyNameVersionWrapper->addWidget(lCurrentlyTargetedBody);

  lBodyNameVersionWrapper->addStretch();

  return lTargetMeshesPicker;
}

void ComponentFactory::CreateOutputBox(QWidget* aParent, QGridLayout& aLayout, const int aLayoutRow, const int aLayoutCol, const QString& aIconFolder, const int aMinimumFirstColumnWidth, const int aFontSize)
{
  // Output group box
  auto lOutputGroupBox{new QGroupBox(tr("Files generation's output location").append("  "), aParent)};
  Utils::AddIconToGroupBox(lOutputGroupBox, aIconFolder, "file-tree", aFontSize);
  Utils::SetGroupBoxState(lOutputGroupBox, false);
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
  lOutputGridLayout->addWidget(lOutputPathLineEdit, 0, 1);

  // Main directory's file chooser button
  auto lOutputPathChooser{ComponentFactory::CreateButton(aParent, tr("Choose a directory..."), "", "folder", aIconFolder, "output_path_chooser")};
  lOutputGridLayout->addWidget(lOutputPathChooser, 0, 2);

  // Subdirectory
  lOutputGridLayout->addWidget(new QLabel(tr("Output subdirectory name/path:"), aParent), 1, 0);

  auto lOutputSubpathLineEdit{new QLineEdit(aParent)};
  lOutputSubpathLineEdit->setObjectName(QString("output_path_subdirectory"));
  lOutputGridLayout->addWidget(lOutputSubpathLineEdit, 1, 1);

  // Use only subdirectory path
  lOutputGridLayout->addWidget(new QLabel(tr("Use only subdirectory path?"), aParent), 2, 0);

  auto lUseOnlySubdir{ComponentFactory::CreateCheckBox(aParent, tr("Check this box to define the export as only the subdirectory field (use at your own risk)"), "", "only_use_subdirectory")};
  lOutputGridLayout->addWidget(lUseOnlySubdir, 2, 1, 1, 2);

  // Preview
  lOutputGridLayout->addWidget(new QLabel(tr("Preview:"), aParent), 3, 0);

  auto lOutputPathsPreview{new QLabel("", aParent)};
  lOutputPathsPreview->setObjectName(QString("output_path_preview"));
  lOutputPathsPreview->setAutoFillBackground(true);
  lOutputGridLayout->addWidget(lOutputPathsPreview, 3, 1);
}
