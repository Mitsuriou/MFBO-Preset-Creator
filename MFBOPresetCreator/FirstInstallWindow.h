#pragma once
#include "Struct.h"
#include <QHBoxLayout>
#include <QMainWindow>

class FirstInstallWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit FirstInstallWindow(QWidget* aParent = Q_NULLPTR);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  void initializeGUI();
  void setupButtons(QHBoxLayout& aLayout);

  void loadQSSTheme(const QString& aThemeName);
};
