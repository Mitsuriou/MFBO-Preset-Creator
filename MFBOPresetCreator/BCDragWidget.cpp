#include "BCDragWidget.h"
#include "Utils.h"
#include <QApplication>
#include <QDrag>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>
#include <QVBoxLayout>

BCDragWidget::BCDragWidget(QWidget* aParent, const Struct::Settings& aSettings, const QString& aOriginFolder, const QString& aPath)
  : QWidget(aParent)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(aSettings.appTheme)};

  // Main layout
  auto lMainLayout{new QVBoxLayout(this)};
  lMainLayout->setMargin(0);
  this->setLayout(lMainLayout);

  auto lSection{new QGroupBox(tr("Preset data"), this)};
  // TODO: Add an icon
  // TODO: Add the collapse button

  auto lSectionLayout{new QGridLayout(this)};
  lSectionLayout->setSpacing(10);
  lSectionLayout->setContentsMargins(15, 20, 15, 15);
  lSectionLayout->setAlignment(Qt::AlignTop);
  lSectionLayout->setColumnStretch(0, 0);
  lSectionLayout->setColumnStretch(1,1);
  lSection->setLayout(lSectionLayout);

  lMainLayout->addWidget(lSection);

  // Path
  auto lPath{new QLabel(tr("Mesh path:"))};
  lSectionLayout->addWidget(lPath, 0, 0);

  auto lPathLabel{new QLabel(aPath, this)};
  lPathLabel->setObjectName("path");
  lSectionLayout->addWidget(lPathLabel, 0, 1);

  // Origin folder
  auto lOrigin{new QLabel(tr("Origin directory:"))};
  lSectionLayout->addWidget(lOrigin, 1, 0);

  auto lOriginFolderLabel{new QLabel(aOriginFolder, this)};
  lOriginFolderLabel->setObjectName("origin");
  lSectionLayout->addWidget(lOriginFolderLabel, 1, 1);
}

void BCDragWidget::mousePressEvent(QMouseEvent* aEvent)
{
  if (aEvent->button() == Qt::LeftButton)
  {
    this->mDragStartPosition = aEvent->pos();
  }
}

void BCDragWidget::mouseMoveEvent(QMouseEvent* aEvent)
{
  if (!(aEvent->buttons() & Qt::LeftButton))
  {
    return;
  }

  if ((aEvent->pos() - this->mDragStartPosition).manhattanLength() < QApplication::startDragDistance())
  {
    return;
  }

  auto lPathLabel{this->findChild<QLabel*>(QString("path"))};

  QMimeData* lMimeData = new QMimeData;
  lMimeData->setData("text/plain", lPathLabel->text().toLocal8Bit());

  QDrag* lDragObject{new QDrag(this)};
  lDragObject->setMimeData(lMimeData);
  lDragObject->exec(Qt::MoveAction);
}
