#pragma once

#include <QFrame>
#include <QGridLayout>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QString>

class ComponentFactory final
{
public:
  static QPushButton* createButton(QWidget* aParent,
                                   const QString& aText,
                                   const QString& aTooltipText,
                                   const QString& aIconName,
                                   const QString& aIconFolder,
                                   const QString& aObjectName = "",
                                   const bool& aIsDisabled = false,
                                   const bool& aMustDisableAutoDefaultBehavior = false);

  static QGridLayout* createScrollAreaLayout(QWidget* aParent);

private:
  ~ComponentFactory() = delete;
  explicit ComponentFactory(){};
};
