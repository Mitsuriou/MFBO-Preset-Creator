#pragma once
#include <QGridLayout>
#include <QObject>
#include <QPushButton>

class ComponentFactory final : public QObject
{
  Q_OBJECT

public:
  // Single component
  static QPushButton* createButton(QWidget* aParent,
                                   const QString& aText,
                                   const QString& aTooltipText,
                                   const QString& aIconName,
                                   const QString& aIconFolder,
                                   const QString& aObjectName = "",
                                   const bool aIsDisabled = false,
                                   const bool aMustDisableAutoDefaultBehavior = false);

  // Constructed components
  static QGridLayout* createScrollAreaWindowLayout(QWidget* aParent, const bool aMustForceLayoutTopAlignment = true, const bool aGenerateButtonsBottomLayout = true);
  static QGridLayout* createScrollAreaComponentLayout(QWidget* aParent);

  // Full UI blocks
  static void createOutputBox(QWidget* aParent, QGridLayout& aLayout, const int aLayoutRow, const int aLayoutCol, const QString& aIconFolder, const QString& aInitialOutputPath, const int aMinimumFirstColumnWidth, const int aFontSize);

private:
  explicit ComponentFactory(){};
};
