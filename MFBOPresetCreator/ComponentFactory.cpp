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
