#pragma once
#include "Enum.h"
#include "LangManager.h"
#include <QGridLayout>
#include <QMainWindow>
#include <QStackedWidget>
#include <QTranslator>

class FirstInstallWindow final : public QMainWindow
{
  Q_OBJECT

public:
  explicit FirstInstallWindow(QTranslator* aTranslator,
                              QTranslator* aQtBaseTranslator,
                              QWidget* aParent = nullptr);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void changeEvent(QEvent* event) override;

private:
  QTranslator* mTranslator{nullptr};
  QTranslator* mQtBaseTranslator{nullptr};

  GUITheme mChosenTheme{GUITheme::MITSURIOU_BLACK_THEME};
  ApplicationLanguage mChosenLanguage{LangManager::GetSystemLanguage()};

  // GUI
  void initializeGUI();
  void constructLanguagePage(QStackedWidget& aParent);
  void constructApplicationThemePage(QStackedWidget& aParent);
  void setupButtons(QGridLayout& aMainLayout);

  void swapTranslators(const QString& aLanguageToSet);

  // Navigation
  void goToPreviousStep();
  void goToNextStep();
  void goToStep(int aStep);

  // Callbacks
  void loadLanguage(const int aLanguage);
  void retranslateUi();
  void loadQSSTheme(const QString& aThemeName, const int aCardIndex);
};
