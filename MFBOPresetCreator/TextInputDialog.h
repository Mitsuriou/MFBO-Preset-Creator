#pragma once

#include "Utils.h"
#include "stdafx.h"

class TextInputDialog : public QDialog
{
  Q_OBJECT

public:
  TextInputDialog(const QString& aTitle, const QString& aLabel, const GUITheme& aAppTheme, QWidget* aParent);

signals:
  void getTextValue(const QString&);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  GUITheme mAppTheme;

  void setWindowProperties(const QString& aTitle);
  void initializeGUI(const QString& aLabel);
};
