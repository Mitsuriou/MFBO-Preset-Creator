#pragma once

#include "stdafx.h"

class TextInputDialog : public QDialog
{
  Q_OBJECT

public:
  TextInputDialog(const QString& aTitle, const QString& aLabel, QWidget* aParent);

signals:
  void getTextValue(const QString&);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  void setWindowProperties(const QString& aTitle);
  void initializeGUI(const QString& aLabel);
};
