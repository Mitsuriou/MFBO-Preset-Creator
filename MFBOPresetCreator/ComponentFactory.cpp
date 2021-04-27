#include "ComponentFactory.h"

QPushButton* ComponentFactory::createButton(
  QWidget* aParent,
  const QString& aText,
  const QString& aTooltipText,
  const QString& aIconName,
  const QString& aIconFolder,
  const QString& aObjectName,
  const bool& aIsDisabled,
  const bool& aMustDisableAutoDefaultBehavior)
{
  // Declare and instanciate the button
  auto lButton{new QPushButton(aText, aParent)};
  // Icon
  if (aIconFolder.length() > 0 && aIconName.length() > 0)
  {
    lButton->setIcon(QIcon(QPixmap(QString(":/%1/%2").arg(aIconFolder).arg(aIconName)).scaledToHeight(48, Qt::SmoothTransformation)));
  }
  // Mouse cursor
  lButton->setCursor(Qt::PointingHandCursor);
  // Tooltip
  if (aTooltipText.length() > 0)
  {
    lButton->setToolTip(aTooltipText);
  }
  else
  {
    lButton->setToolTip(aText);
  }
  // Object name
  if (aObjectName.length() > 0)
  {
    lButton->setObjectName(aObjectName);
  }
  // Disable AutoDefault and Default behaviors
  if (aMustDisableAutoDefaultBehavior)
  {
    lButton->setAutoDefault(false);
    lButton->setDefault(false);
  }
  // Disable the button
  lButton->setDisabled(aIsDisabled);

  return lButton;
}

QGridLayout* ComponentFactory::createScrollAreaLayout(QWidget* aParent)
{
  // Very first container in which the scroll area will be added
  auto lBaseLayout{new QVBoxLayout(aParent)};
  lBaseLayout->setContentsMargins(0, 0, 0, 0);

  // Create a scroll area
  auto lScrollArea{new QScrollArea(aParent)};
  lScrollArea->setObjectName("scrollable_zone");
  lScrollArea->setContentsMargins(0, 0, 0, 0);
  lScrollArea->setWidgetResizable(true);
  // Remove the borders of the scroll area
  lScrollArea->setStyleSheet("QScrollArea{border: none;}");

  // Add a QFrame to permit automatic expanding of the content inside the scroll area
  auto lMainWidget{new QFrame(aParent)};

  // Main container
  auto lMainLayout{new QGridLayout(lMainWidget)};
  lMainLayout->setSpacing(10);
  lMainLayout->setContentsMargins(10, 10, 10, 10);
  lMainLayout->setAlignment(Qt::AlignTop);

  lScrollArea->setWidget(lMainWidget);
  lBaseLayout->addWidget(lScrollArea);

  return lMainLayout;
}
