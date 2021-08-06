#pragma once

#include "Struct.h"
#include <QWidget>

class BCGroupWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCGroupWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aSectionTitle);

signals:
  void removePressed(const QString& aOriginFolder, const QString& aRessourcePath);
  void dropEventTriggered(const QString& aOriginFolder, const QString& aRessourcePath);

private:
  void removeButtonClicked();
  void dropEventTrigerredReceiver(const QString& aOriginFolder, const QString& aRessourcePath);
};
