#pragma once

#include "Struct.h"
#include <QWidget>

class BCGroupWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BCGroupWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aSectionTitle, const QString& aSectionIconName);

signals:
  void removePressed(const QString& aOriginFolder, const QString& aRessourcePath);
  void dropEventTriggered(const QString& aOriginFolder, const QString& aRessourcePath);

private:
  void removeButtonClicked();
  void dropEventTrigerredReceiver(const QString& aOldOriginFolder, const QString& aOldRessourcePath, const QString& aNewOriginFolder, const QString& aNewRessourcePath);

  // GUI widgets events
  void groupBoxChecked(bool aIsChecked);
};
