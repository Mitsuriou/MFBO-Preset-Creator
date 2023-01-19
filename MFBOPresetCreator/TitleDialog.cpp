#include "TitleDialog.h"
#include "Utils.h"
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
  , mApplicationIconResourceFolder{Utils::GetIconResourceFolder(aSettings.display.applicationTheme)}
{
  this->setWindowProperties(aTitle, aIconName, aMinimumWidth, aMinimumHeight);
  this->constructGUI(aTitle);
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
  this->findChild<QLabel*>(QStringLiteral("td_main_title"))->setText(aTitle);
}

void TitleDialog::hideTitle()
{
  this->findChild<QLabel*>(QStringLiteral("td_main_title"))->hide();
}

void TitleDialog::showTitle()
{
  this->findChild<QLabel*>(QStringLiteral("td_main_title"))->show();
}

QWidget* TitleDialog::getCentralWidget()
{
  return this->findChild<QWidget*>(QStringLiteral("td_central_widget"));
}

QLayout* TitleDialog::getCentralLayout()
{
  if (this->getCentralWidget() != nullptr && this->getCentralWidget()->layout() != nullptr)
    return this->getCentralWidget()->layout();

  return nullptr;
}

QHBoxLayout* TitleDialog::getButtonsLayout()
{
  return this->findChild<QHBoxLayout*>(QStringLiteral("td_buttons_layout"));
}

const QString& TitleDialog::getThemedResourcePath() const
{
  return mApplicationIconResourceFolder;
}

void TitleDialog::setWindowProperties(const QString& aTitle,
                                      const QString& aIconName,
                                      const int aMinimumWidth,
                                      const int aMinimumHeight)
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(aTitle);
  this->setWindowIcon(QIcon(QPixmap(
    QString(":/%1/%2").arg(Utils::GetTitleBarResourceFolder(this->settings().display.titleBarIconsBlack),
                           aIconName))));

  if (aMinimumWidth > -1)
    this->setMinimumWidth(aMinimumWidth);

  if (aMinimumHeight > -1)
    this->setMinimumHeight(aMinimumHeight);
}

void TitleDialog::constructGUI(const QString& aTitle)
{
  // Global layout
  const auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setContentsMargins(0, 10, 0, 0);
  lMainLayout->setSpacing(10);
  this->setLayout(lMainLayout);

  // Main title
  const auto lMainTitle{new QLabel(aTitle, this)};
  lMainTitle->setObjectName(QStringLiteral("td_main_title"));
  lMainTitle->setAlignment(Qt::AlignCenter);
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(this->settings().display.font.pointSize * 2));
  lMainLayout->addWidget(lMainTitle, 0);

  // Central widget
  const auto lCentralWidget{new QWidget(this)};
  lCentralWidget->setObjectName(QStringLiteral("td_central_widget"));
  lMainLayout->addWidget(lCentralWidget, 1);
}
