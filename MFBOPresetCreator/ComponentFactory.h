#pragma once

#include "GroupBox.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>

namespace ComponentFactory
{
  // Single component
  QPushButton* CreateButton(QWidget* aParent,
                            const QString& aText,
                            const QString& aTooltipText,
                            const QString& aIconName,
                            const QString& aIconFolder,
                            const QString& aObjectName = QString(),
                            const bool aIsDisabled = false,
                            const bool aMustDisableAutoDefaultBehavior = false);

  QCheckBox* CreateCheckBox(QWidget* aParent,
                            const QString& aText,
                            const QString& aTooltipText,
                            const QString& aObjectName = QString(),
                            const bool lIsChecked = false);

  GroupBox* CreateGroupBox(QWidget* aParent,
                           const QString& aTitle,
                           const QString& aIconName,
                           const QString& aIconFolder,
                           const int aFontSize,
                           const QString& aObjectName = QString(),
                           const bool aIsDisabled = false);

  // Constructed components
  QGridLayout* CreateScrollAreaWindowLayout(QWidget* aParent,
                                            const bool aMustForceLayoutTopAlignment = true,
                                            const bool aGenerateButtonsBottomLayout = true,
                                            QVBoxLayout* aParentLayout = nullptr,
                                            const QString& aScrollAreaObjectName = QString("scrollable_zone"),
                                            const QMargins& aNestedLayoutMargins = QMargins(10, 10, 10, 10),
                                            const QString& aNestedLayoutObjectName = QString("main_layout"));

  QGridLayout* CreateScrollAreaComponentLayout(QWidget* aParent,
                                               QGridLayout& aLayout,
                                               const int aRow,
                                               const int aColumn);

  // Full UI blocks
  QPushButton* CreateTargetMeshesPickerLine(QWidget* aParent,
                                            QGridLayout& aLayout,
                                            const bool aSingleLineForLabelAndActions,
                                            const int aLayoutRow,
                                            const QString& aIconFolder,
                                            const QString& aButtonObjectName,
                                            const QString& aLabelObjectName);

  void CreateOutputBox(QWidget* aParent,
                       QGridLayout& aLayout,
                       const int aLayoutRow,
                       const int aLayoutCol,
                       const QString& aIconFolder,
                       const int aMinimumFirstColumnWidth,
                       const int aFontSize,
                       const int aRowSpan = 1,
                       const int aColumnSpan = 1);
};
