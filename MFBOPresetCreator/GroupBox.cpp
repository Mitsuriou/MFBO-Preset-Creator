#include "GroupBox.h"
#include "Utils.h"
#include <QApplication>
#include <math.h>

GroupBox::GroupBox(QWidget* aParent,
                   const QString& aTitle,
                   const QString& aIconName,
                   const QString& aIconFolder,
                   const int aFontSize,
                   const QString& aObjectName,
                   const bool aIsDisabled)
  : QGroupBox(aTitle + "  ", aParent)
  , mFontSize(aFontSize)
{
  // Object name
  if (aObjectName.trimmed().length() > 0)
  {
    this->setObjectName(aObjectName);
  }

  // Icon
  this->setCheckable(true);
  this->setStyleSheet(QString("QGroupBox{font-size: %1pt;}"
                              "QGroupBox::indicator{width: 16px; height: 16px; image: url(:/%2/%3);}")
                        .arg(static_cast<int>(floor(aFontSize * 1.25)))
                        .arg(aIconFolder, aIconName));

  // Collapse state (displayed icon + widget's height)
  this->setExpandState(true);

  // Disabled state
  this->setDisabled(aIsDisabled);

  // Redirect the toggled event to a custom function
  QObject::connect(this, &QGroupBox::toggled, this, &GroupBox::setExpandState);
}

void GroupBox::setExpandState(const bool aMustExpand)
{
  // Store the title to then modify it
  auto lTitle{this->title()};

  if (aMustExpand)
  {
    this->setMaximumHeight(this->mExpandedHeight == -1 ? QWIDGETSIZE_MAX : this->mExpandedHeight);
    lTitle.replace(lTitle.length() - 1, 1, QChar(0x25BC));

    this->mExpandedHeight = -1;
  }
  else
  {
    this->mExpandedHeight = this->geometry().height();

    const auto lFontMetrics{QFontMetricsF(qApp->font())};
    this->setMaximumHeight(lFontMetrics.height() * 1.5);
    lTitle.replace(lTitle.length() - 1, 1, QChar(0x25B6));
  }

  // Update the title with the updated chevron state
  this->setTitle(lTitle);
}
