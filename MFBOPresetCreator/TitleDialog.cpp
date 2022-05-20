#include "TitleDialog.h"
#include <QCloseEvent>
#include <QIcon>
#include <QLabel>
#include <QVBoxLayout>

TitleDialog::TitleDialog(QWidget* aParent,
                         const QString& aTitle,
                         const QString& aIconName,
                         const Struct::Settings& aSettings,
                         std::map<QString, QString>* aLastPaths,
                         const int aMinimumWidth,
                         const int aMinimumHeight)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings{aSettings}
  , mLastPaths{aLastPaths}
{
  this->setWindowProperties(aTitle, aIconName, aMinimumWidth, aMinimumHeight);
  // this->constructGUI(aTitle, aGenerateButtonsLayout);
  this->constructGUI(aTitle, false);
}

const Struct::Settings& TitleDialog::settings()
{
  return this->mSettings;
}

std::map<QString, QString>* TitleDialog::lastPaths()
{
  return this->mLastPaths;
}

void TitleDialog::reject()
{
  this->close();
}

void TitleDialog::overrideContentTitle(const QString& aTitle)
{
  this->findChild<QLabel*>("td_main_title")->setText(aTitle);
}

void TitleDialog::hideTitle()
{
  this->findChild<QLabel*>("td_main_title")->hide();
}

void TitleDialog::showTitle()
{
  this->findChild<QLabel*>("td_main_title")->show();
}

QWidget* TitleDialog::getCentralWidget()
{
  return this->findChild<QWidget*>("td_central_widget");
}

QLayout* TitleDialog::getCentralLayout()
{
  if (this->getCentralWidget() != nullptr && this->getCentralWidget()->layout() != nullptr)
    return this->getCentralWidget()->layout();

  return nullptr;
}

QHBoxLayout* TitleDialog::getButtonsLayout()
{
  return this->findChild<QHBoxLayout*>("td_buttons_layout");
}

void TitleDialog::setWindowProperties(const QString& aTitle,
                                      const QString& aIconName,
                                      const int aMinimumWidth,
                                      const int aMinimumHeight)
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(aTitle);
  this->setWindowIcon(QIcon(QPixmap(QString(":/black/%1").arg(aIconName))));

  if (aMinimumWidth > -1)
    this->setMinimumWidth(aMinimumWidth);

  if (aMinimumHeight > -1)
    this->setMinimumHeight(aMinimumHeight);
}

void TitleDialog::constructGUI(const QString& aTitle, const bool aGenerateButtonsLayout)
{
  // Global layout
  const auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setContentsMargins(0, 10, 0, 0);
  lMainLayout->setSpacing(10);
  this->setLayout(lMainLayout);

  // Main title
  const auto lMainTitle{new QLabel(aTitle, this)};
  lMainTitle->setObjectName("td_main_title");
  lMainTitle->setAlignment(Qt::AlignCenter);
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(this->settings().display.font.pointSize * 2));
  lMainLayout->addWidget(lMainTitle, 0);

  // Central widget
  const auto lCentralWidget{new QWidget(this)};
  lCentralWidget->setObjectName("td_central_widget");
  lMainLayout->addWidget(lCentralWidget, 1);

  // Buttons layouts
  if (aGenerateButtonsLayout)
  {
    const auto lButtonsLayout{new QHBoxLayout(lCentralWidget)};
    lButtonsLayout->setContentsMargins(0, 0, 0, 0);
    lButtonsLayout->setSpacing(10);
    lButtonsLayout->setObjectName("td_buttons_layout");
    lMainLayout->addLayout(lButtonsLayout);
  }
}
