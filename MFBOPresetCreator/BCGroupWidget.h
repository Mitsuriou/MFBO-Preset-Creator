#pragma once

#include <QWidget>

class BCGroupWidget : public QWidget
{
  Q_OBJECT

public:
  BCGroupWidget(QWidget* aParent, const QString& aSectionTitle);
};
