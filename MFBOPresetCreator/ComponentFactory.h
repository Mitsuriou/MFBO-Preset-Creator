#pragma once
#include <QGridLayout>
#include <QObject>
#include <QPushButton>

class ComponentFactory final : public QObject
{
  Q_OBJECT

public:
  // Single component
  static QPushButton* CreateButton(QWidget* aParent,
                                   const QString& aText,
                                   const QString& aTooltipText,
                                   const QString& aIconName,
                                   const QString& aIconFolder,
                                   const QString& aObjectName = "",
                                   const bool aIsDisabled = false,
                                   const bool aMustDisableAutoDefaultBehavior = false);

  // Constructed components
  static QGridLayout* CreateScrollAreaWindowLayout(QWidget* aParent, const bool aMustForceLayoutTopAlignment = true, const bool aGenerateButtonsBottomLayout = true);
  static QGridLayout* CreateScrollAreaComponentLayout(QWidget* aParent);

  // Full UI blocks
  static void CreateOutputBox(QWidget* aParent, QGridLayout& aLayout, const int aLayoutRow, const int aLayoutCol, const QString& aIconFolder, const QString& aInitialOutputPath, const int aMinimumFirstColumnWidth, const int aFontSize);

private:
  explicit ComponentFactory(){};
};
