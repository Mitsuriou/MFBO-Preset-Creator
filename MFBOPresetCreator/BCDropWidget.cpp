#include "BCDropWidget.h"
#include "BCDragWidget.h"
#include "ComponentFactory.h"
#include "Utils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeData>
#include <QMouseEvent>

BCDropWidget::BCDropWidget(QWidget* aParent, const BCGroupWidgetCallContext& aCallContext)
  : QWidget(aParent)
  , mCallContext(aCallContext)
{
  // Accept the drops since this is the purprose of this widget
  this->setAcceptDrops(true);

  // Main layout
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setMargin(0);
  lMainLayout->setSpacing(10);
  lMainLayout->setColumnStretch(0, 0);
  lMainLayout->setColumnStretch(1, 1);
  this->setLayout(lMainLayout);

  // Path
  auto lPathLabel{new QLabel(this)};
  lPathLabel->setObjectName(QString("path_label"));
  lMainLayout->addWidget(lPathLabel, 0, 0);

  auto lPathContent{new QLabel(this)};
  lPathContent->setObjectName(QString("path_content"));
  lMainLayout->addWidget(lPathContent, 0, 1);

  // Origin directory
  auto lOriginLabel{new QLabel(this)};
  lOriginLabel->setObjectName(QString("origin_label"));
  lMainLayout->addWidget(lOriginLabel, 1, 0);

  auto lOriginContent{new QLabel(this)};
  lOriginContent->setObjectName(QString("origin_content"));
  lMainLayout->addWidget(lOriginContent, 1, 1);

  // Alternative model
  if (this->mCallContext == BCGroupWidgetCallContext::HANDS)
  {
    auto lUseAlternativeModel{this->createCheckBox(tr("Check this box if the follower or NPC uses beast hands"), *lMainLayout)};
    this->connect(lUseAlternativeModel, &QCheckBox::stateChanged, this, &BCDropWidget::checkBoxStateChanged);
  }
  else if (this->mCallContext == BCGroupWidgetCallContext::SKELETON)
  {
    auto lUseAlternativeModel{this->createCheckBox(tr("Check this box if the follower or NPC uses beast skeleton"), *lMainLayout)};
    this->connect(lUseAlternativeModel, &QCheckBox::stateChanged, this, &BCDropWidget::checkBoxStateChanged);
  }

  this->resetData();
}

QString BCDropWidget::getOriginFolder() const
{
  return this->mOriginFolder;
}

QString BCDropWidget::getRessourcePath() const
{
  return this->mRessourcePath;
}

void BCDropWidget::resetData()
{
  // Clear the two strings
  this->mOriginFolder.clear();
  this->mRessourcePath.clear();

  this->tweakWidgetsVisibility(true);
}

void BCDropWidget::setData(const QString& aOriginFolder, const QString& aRessourcePath, const bool aUseAlternativeModel)
{
  this->mOriginFolder = aOriginFolder;
  this->mRessourcePath = aRessourcePath;
  this->tweakWidgetsVisibility(aRessourcePath.isEmpty(), aOriginFolder, aRessourcePath, aUseAlternativeModel);
}

void BCDropWidget::dragEnterEvent(QDragEnterEvent* aEvent)
{
  // Only accept drag events from a BCDragWidget widget
  if (static_cast<BCDragWidget*>(aEvent->source()))
  {
    aEvent->acceptProposedAction();
  }
}

void BCDropWidget::dragMoveEvent(QDragMoveEvent* aEvent)
{
  // Only accept drag events from a BCDragWidget widget
  if (static_cast<BCDragWidget*>(aEvent->source()))
  {
    aEvent->acceptProposedAction();
  }
}

void BCDropWidget::dropEvent(QDropEvent* aEvent)
{
  if (aEvent->source() == this && aEvent->possibleActions() & Qt::MoveAction)
  {
    return;
  }

  // BCDropWidget is only compatibl with the Qt::MoveAction
  if (aEvent->proposedAction() == Qt::MoveAction)
  {
    // Accept the drop event
    aEvent->acceptProposedAction();

    // Parse the JSON string
    auto lDataObject{QJsonDocument::fromJson(aEvent->mimeData()->data("application/json")).object()};

    // Upper treatment (before updating the data)
    emit BCDropWidget::dropEventTriggered(this->mOriginFolder, this->mRessourcePath, lDataObject["originFolder"].toString(), lDataObject["ressourcePath"].toString());

    // Save the new data
    this->mOriginFolder = lDataObject["originFolder"].toString();
    this->mRessourcePath = lDataObject["ressourcePath"].toString();

    // Update the displayed information
    this->tweakWidgetsVisibility(false, this->mOriginFolder, this->mRessourcePath);
  }
}

void BCDropWidget::checkBoxStateChanged(int aNewState)
{
  emit BCDropWidget::checkBoxStateChangedTriggered(aNewState == Qt::CheckState::Checked);
}

void BCDropWidget::tweakWidgetsVisibility(const bool aShouldViewDropZoneOnly, const QString& aNewOriginText, const QString& aNewRessourceText, const bool aUseAlternativeModel)
{
  auto lPathLabel{this->findChild<QLabel*>(QString("path_label"))};
  auto lPathContent{this->findChild<QLabel*>(QString("path_content"))};
  auto lOriginLabel{this->findChild<QLabel*>(QString("origin_label"))};
  auto lOriginContent{this->findChild<QLabel*>(QString("origin_content"))};
  auto lUseAlternativeModel{this->findChild<QCheckBox*>(QString("use_alternative_model"))};

  if (aShouldViewDropZoneOnly)
  {
    lPathContent->setText("");
    lOriginContent->setText("");

    // If there is the checkbox
    if (lUseAlternativeModel != nullptr)
    {
      // Uncheck the box
      this->disconnect(lUseAlternativeModel, &QCheckBox::stateChanged, this, &BCDropWidget::checkBoxStateChanged);
      lUseAlternativeModel->setChecked(false);
      lUseAlternativeModel->hide();
      this->connect(lUseAlternativeModel, &QCheckBox::stateChanged, this, &BCDropWidget::checkBoxStateChanged);

      // Update the texts to have the placeholder centered
      lPathLabel->setText("");
      lOriginLabel->setText(tr("Drop some data here..."));
    }
    // If there is not the checkbox
    else
    {
      lPathLabel->setText(tr("Drop some data here..."));
      lOriginLabel->setText("");
    }
  }
  else
  {
    lPathLabel->setText(tr("Mesh path:"));
    lPathContent->setText(aNewRessourceText);
    lOriginLabel->setText(tr("Origin mod:"));
    lOriginContent->setText(aNewOriginText);

    // If there is the checkbox
    if (lUseAlternativeModel != nullptr)
    {
      // Check the box if needed
      this->disconnect(lUseAlternativeModel, &QCheckBox::stateChanged, this, &BCDropWidget::checkBoxStateChanged);
      lUseAlternativeModel->setChecked(aUseAlternativeModel);
      lUseAlternativeModel->show();
      this->connect(lUseAlternativeModel, &QCheckBox::stateChanged, this, &BCDropWidget::checkBoxStateChanged);
    }
  }
}

QCheckBox* BCDropWidget::createCheckBox(const QString& lText, QGridLayout& aLayout)
{
  QCheckBox* lUseAlternativeModel{ComponentFactory::CreateCheckBox(this, lText, "", "use_alternative_model", false)};

  QSizePolicy lKeepHeightWhenHidden = lUseAlternativeModel->sizePolicy();
  lKeepHeightWhenHidden.setRetainSizeWhenHidden(true);
  lUseAlternativeModel->setSizePolicy(lKeepHeightWhenHidden);

  aLayout.addWidget(lUseAlternativeModel, 2, 0, 1, 2);

  return lUseAlternativeModel;
}
