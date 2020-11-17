#include "Tab.h"

void Tab::updateSettings(Struct::Settings aSettings)
{
  this->mSettings = aSettings;
}

Tab::Tab(QWidget* aParent, const Struct::Settings& aSettings)
  : QWidget(aParent)
  , mSettings(aSettings)
{
  auto lBaseLayout{new QVBoxLayout(this)};

  auto lScrollArea{new QScrollArea(this)};
  lScrollArea->setObjectName("scrollable_zone");
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->setWidgetResizable(true);

  auto lMainWidget{new QFrame(this)};

  // Hacky color change for Windows Vista theme
  if (this->mSettings.appTheme == GUITheme::WindowsVista)
  {
    lScrollArea->setStyleSheet("QScrollArea{border:none;}");
    lMainWidget->setStyleSheet("QFrame{background-color:white;}");
  }

  mMainVertical = new QVBoxLayout(lMainWidget);
  mMainVertical->setContentsMargins(0, 0, 0, 0);

  lScrollArea->setWidget(lMainWidget);
  lBaseLayout->addWidget(lScrollArea);

  // Cursor change for the scroll bar
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderPressed, this, &Tab::mouseCursorPressed);
  this->connect(lScrollArea->verticalScrollBar(), &QAbstractSlider::sliderReleased, this, &Tab::mouseCursorReleased);

  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderPressed, this, &Tab::mouseCursorPressed);
  this->connect(lScrollArea->horizontalScrollBar(), &QAbstractSlider::sliderReleased, this, &Tab::mouseCursorReleased);
}

QVBoxLayout* Tab::getMainLayout()
{
  return this->mMainVertical;
}

void Tab::mouseCursorPressed()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::ClosedHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::ClosedHandCursor);
}

void Tab::mouseCursorReleased()
{
  auto lScrollArea{this->findChild<QScrollArea*>("scrollable_zone")};
  lScrollArea->verticalScrollBar()->setCursor(Qt::OpenHandCursor);
  lScrollArea->horizontalScrollBar()->setCursor(Qt::OpenHandCursor);
}
