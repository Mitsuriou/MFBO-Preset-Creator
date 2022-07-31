#include "ComponentFactory.h"
#include "Utils.h"
#include <QCheckBox>
#include <QFrame>
#include <QGridLayout>
#include <QIcon>
#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>

namespace ComponentFactory
{
  QPushButton* CreateButton(
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

  QCheckBox* CreateCheckBox(QWidget* aParent,
                            const QString& aText,
                            const QString& aTooltipText,
                            const QString& aObjectName,
                            const bool lIsChecked)
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

  GroupBox* CreateGroupBox(QWidget* aParent,
                           const QString& aTitle,
                           const QString& aIconName,
                           const QString& aIconFolder,
                           const int aFontSize,
                           const QString& aObjectName,
                           const bool aIsDisabled)
  {
    return (new GroupBox(aParent, aTitle, aIconName, aIconFolder, aFontSize, aObjectName, aIsDisabled));
  }

  QGridLayout* CreateScrollAreaWindowLayout(QWidget* aParent,
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
    const auto lScrollArea{new QScrollArea(aParent)};
    lScrollArea->setObjectName(aScrollAreaObjectName);
    lScrollArea->setContentsMargins(0, 0, 0, 0);
    lScrollArea->setWidgetResizable(true);
    lScrollArea->setStyleSheet("QScrollArea{border: none;}");

    // Add a QFrame to permit automatic expanding of the content inside the scroll area
    const auto lScrollAreaWidgetContent{new QFrame(lScrollArea)};

    // Main container
    const auto lNestedLayout{new QGridLayout(lScrollAreaWidgetContent)};
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
      const auto lButtonsLayout{new QHBoxLayout(aParent)};
      lButtonsLayout->setObjectName(QString("window_buttons_layout"));
      lButtonsLayout->setSpacing(10);
      lButtonsLayout->setContentsMargins(10, 0, 10, 10);
      lButtonsLayout->setAlignment(Qt::AlignTop);
      aParentLayout->addLayout(lButtonsLayout);
    }

    return lNestedLayout;
  }

  QGridLayout* CreateScrollAreaComponentLayout(QWidget* aParent,
                                               QGridLayout& aLayout,
                                               const int aRow,
                                               const int aColumn,
                                               const int aRowSpan,
                                               const int aColumnSpan)
  {
    auto lScrollArea{new QScrollArea(aParent)};
    lScrollArea->setObjectName(QString("scrollable_zone"));
    lScrollArea->setWidgetResizable(true);

    auto lMainWidget{new QFrame(aParent)};
    lScrollArea->setWidget(lMainWidget);

    auto lDataContainer{new QGridLayout(aParent)};
    lDataContainer->setObjectName(QString("data_container"));
    lDataContainer->setAlignment(Qt::AlignTop);
    lDataContainer->setContentsMargins(10, 10, 10, 10);

    lMainWidget->setLayout(lDataContainer);

    aLayout.addWidget(lScrollArea, aRow, aColumn, aRowSpan, aColumnSpan);

    return lDataContainer;
  }

  QPushButton* CreateTargetMeshesPickerLine(QWidget* aParent,
                                            QGridLayout& aLayout,
                                            const bool aSingleLineForLabelAndActions,
                                            const int aLayoutRow,
                                            const QString& aIconFolder,
                                            const QString& aButtonObjectName,
                                            const QString& aLabelObjectName)
  {
    // Targeted body and version
    aLayout.addWidget(new QLabel(QObject::tr("Targeted meshes mods:"), aParent), aLayoutRow, 0);

    QHBoxLayout* lBodyNameVersionWrapper{new QHBoxLayout(aParent)};
    lBodyNameVersionWrapper->setContentsMargins(0, 0, 0, 0);
    aLayout.addLayout(lBodyNameVersionWrapper,
                      aLayoutRow + (aSingleLineForLabelAndActions ? 0 : 1),
                      (aSingleLineForLabelAndActions ? 1 : 0));

    auto lTargetMeshesPicker{CreateButton(aParent, QObject::tr("Choose target meshes"), "", "mesh", aIconFolder, aButtonObjectName, false, true)};
    lBodyNameVersionWrapper->addWidget(lTargetMeshesPicker);

    auto lCurrentlyTargetedBody{new QLabel(QObject::tr("Targeted body: -\nTargeted feet: -"), aParent)};
    lCurrentlyTargetedBody->setObjectName(aLabelObjectName);
    lBodyNameVersionWrapper->addWidget(lCurrentlyTargetedBody);

    lBodyNameVersionWrapper->addStretch();

    return lTargetMeshesPicker;
  }

  void CreateOutputBox(QWidget* aParent,
                       QGridLayout& aLayout,
                       const int aLayoutRow,
                       const int aLayoutCol,
                       const QString& aIconFolder,
                       const int aMinimumFirstColumnWidth,
                       const int aFontSize,
                       const int aRowSpan,
                       const int aColumnSpan)
  {
    // Output group box
    auto lOutputGroupBox{CreateGroupBox(aParent, QObject::tr("Files generation's output location"), "file-tree", aIconFolder, aFontSize, "output_group_box")};
    aLayout.addWidget(lOutputGroupBox, aLayoutRow, aLayoutCol, aRowSpan, aColumnSpan);

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
    lOutputGridLayout->addWidget(new QLabel(QObject::tr("Output directory path:"), aParent), 0, 0);

    auto lOutputPathLineEdit{new LineEdit(aParent)};
    lOutputPathLineEdit->setReadOnly(true);
    lOutputPathLineEdit->setObjectName(QString("output_path_directory"));
    lOutputGridLayout->addWidget(lOutputPathLineEdit, 0, 1);

    // Main directory's file chooser button
    auto lOutputPathChooser{CreateButton(aParent, QObject::tr("Choose a directory..."), "", "folder", aIconFolder, "output_path_chooser")};
    lOutputGridLayout->addWidget(lOutputPathChooser, 0, 2);

    // Subdirectory
    lOutputGridLayout->addWidget(new QLabel(QObject::tr("Output subdirectory name/path:"), aParent), 1, 0);

    auto lOutputSubpathLineEdit{new LineEdit(aParent)};
    lOutputSubpathLineEdit->setObjectName(QString("output_path_subdirectory"));
    lOutputGridLayout->addWidget(lOutputSubpathLineEdit, 1, 1);

    // Use only subdirectory path
    lOutputGridLayout->addWidget(new QLabel(QObject::tr("Use only subdirectory path?"), aParent), 2, 0);

    auto lUseOnlySubdir{CreateCheckBox(aParent, QObject::tr("Check this box to define the export as only the subdirectory field (use at your own risk)"), "", "only_use_subdirectory")};
    lOutputGridLayout->addWidget(lUseOnlySubdir, 2, 1, 1, 2);

    // Preview
    lOutputGridLayout->addWidget(new QLabel(QObject::tr("Preview:"), aParent), 3, 0);

    auto lOutputPathsPreview{new QLabel("", aParent)};
    lOutputPathsPreview->setObjectName(QString("output_path_preview"));
    lOutputPathsPreview->setAutoFillBackground(true);
    lOutputGridLayout->addWidget(lOutputPathsPreview, 3, 1);
  }
}