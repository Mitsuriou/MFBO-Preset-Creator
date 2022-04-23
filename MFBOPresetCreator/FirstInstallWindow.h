#pragma once
#include "Enum.h"
#include <QMainWindow>

class FirstInstallWindow final : public QMainWindow
{
  Q_OBJECT

public:
  explicit FirstInstallWindow(QWidget* aParent = nullptr);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  GUITheme mLastLoadedTheme{GUITheme::DEFAULT_OS_THEME};

  void initializeGUI();
  void setupButtons();

  void loadQSSTheme(const QString& aThemeName);
};
