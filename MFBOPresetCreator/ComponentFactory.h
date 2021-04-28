#pragma once

#include "Utils.h"
#include <QFrame>
#include <QGridLayout>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QString>

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
                                   const bool& aIsDisabled = false,
                                   const bool& aMustDisableAutoDefaultBehavior = false);

  // Constructed components
  static QGridLayout* createScrollAreaWindowLayout(QWidget* aParent);
  static QGridLayout* createScrollAreaComponentLayout(QWidget* aParent);

  // Full UI blocks
  static void createOutputBox(QWidget* aParent, QGridLayout* aLayout, const QString& aIconFolder, const QString& aInitialOutputPath = "", const int& aMinimumFirstColumnWidth = -1);

private:
  explicit ComponentFactory(){};
};
