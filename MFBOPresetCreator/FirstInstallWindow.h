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
  GUITheme mLastLoadedTheme{GUITheme::DEFAULT_OS_THEME};

  void initializeGUI();
  void setupButtons();

  void loadQSSTheme(const QString& aThemeName);
};
