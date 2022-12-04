#include "SSSPSelectionBlock.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "LineEdit.h"
#include "Utils.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFileInfo>
#include <QFormLayout>
#include <QLabel>
#include <QStyledItemDelegate>

SSSPSelectionBlock::SSSPSelectionBlock(QWidget* aParent,
                                       const GUITheme aApplicationTheme,
                                       const int aPointSize,
                                       const QString& aFullFilePath,
                                       const Struct::SliderSet& aSliderSet)
  : GroupBox(aParent, aSliderSet.getName(), "file", Utils::GetIconResourceFolder(aApplicationTheme), aPointSize)
  , mFullFilePath(aFullFilePath)
  , mOriginalSliderSetName(aSliderSet.getName())
{
  this->initializeGUI(aSliderSet);
}

bool SSSPSelectionBlock::isCheckedForImport() const
{
  return this->findChild<QComboBox*>("slider_set_type")->currentIndex() != 0;
}

MeshPartType SSSPSelectionBlock::getCurrentlySetMeshPartType() const
{
  return static_cast<MeshPartType>(this->findChild<QComboBox*>("slider_set_type")->currentIndex());
}

Struct::SliderSetResult SSSPSelectionBlock::getData() const
{
  return Struct::SliderSetResult(this->mFullFilePath,
                                 this->mOriginalSliderSetName,
                                 this->findChild<QLineEdit*>("slider_set_name")->text(),
                                 this->getCurrentlySetMeshPartType());
}

void SSSPSelectionBlock::initializeGUI(const Struct::SliderSet& aSliderSet)
{
  const auto lMainLayout = new QFormLayout(this);
  lMainLayout->setSpacing(10);
  lMainLayout->setContentsMargins(15, 20, 15, 15);
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  // SliderSet name
  const auto lSliderSetNameLabel{new QLabel(tr("Slider set name:"), this)};

  const auto lSliderSetNameValue{new LineEdit(aSliderSet.getName(), this)};
  lSliderSetNameValue->setObjectName("slider_set_name");

  lMainLayout->addRow(lSliderSetNameLabel, lSliderSetNameValue);

  // Preview
  const auto lPreviewCheckbox{new QLabel(tr("Preview:"), this)};

  const auto lPreviewValue{new QLabel(this)};
  lPreviewValue->setObjectName("preview_value");

  lMainLayout->addRow(lPreviewCheckbox, lPreviewValue);

  // Resource type
  const auto lResourceTypeLabel{new QLabel(tr("Resource type:"), this)};

  const auto lResourceTypeValue{new QComboBox(this)};
  lResourceTypeValue->setObjectName("slider_set_type");
  lResourceTypeValue->setItemDelegate(new QStyledItemDelegate());
  lResourceTypeValue->setCursor(Qt::PointingHandCursor);
  lResourceTypeValue->addItems(DataLists::GetSliderSetsImporterActions());

  lMainLayout->addRow(lResourceTypeLabel, lResourceTypeValue);

  // Source file
  const auto lSourceFileNameLabel{new QLabel(tr("Source file:"), this)};

  const auto lSourceFileNameValue{new QLabel(QFileInfo(this->mFullFilePath).fileName(), this)};

  lMainLayout->addRow(lSourceFileNameLabel, lSourceFileNameValue);

  // Event binding
  QObject::connect(lSliderSetNameValue, &QLineEdit::textEdited, this, &SSSPSelectionBlock::sliderSetNameChanged);

  // Post-bind initialization functions
  this->sliderSetNameChanged(lSliderSetNameValue->text());
}

void SSSPSelectionBlock::sliderSetNameChanged(const QString& aNewSliderSetName)
{
  this->setTitle(aNewSliderSetName);

  const auto lPreviewValue{this->findChild<QLabel*>("preview_value")};
  lPreviewValue->setText(tr("Follower Name - %1").arg(aNewSliderSetName));
}
