#pragma once

#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QString>

class ComponentFactory final
{
public:
  static QPushButton* createButton(
    QWidget* aParent,
    const QString& aText,
    const QString& aTooltipText,
    const QString& aIconName,
    const QString& aIconFolder,
    const QString& aObjectName = "",
    const bool& aIsDisabled = false,
    const bool& aMustDisableAutoDefaultBehavior = false);

private:
  ~ComponentFactory() = delete;
  explicit ComponentFactory(){};
};
