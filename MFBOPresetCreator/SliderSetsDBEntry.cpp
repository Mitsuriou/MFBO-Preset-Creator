#include "SliderSetsDBEntry.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Utils.h"
#include <QGridLayout>
#include <QStyledItemDelegate>

SliderSetsDBEntry::SliderSetsDBEntry(QWidget* aParent,
                                     const GUITheme aApplicationTheme,
                                     const int aPointSize,
                                     const int aDatabaseIndex,
                                     const Struct::DatabaseSliderSet& aDatabaseEntry)
  : GroupBox(aParent, aDatabaseEntry.getSliderSetName(), "file", Utils::GetIconResourceFolder(aApplicationTheme), aPointSize)
  , mDatabaseIndex(aDatabaseIndex)
{
  this->initializeGUI(aDatabaseIndex, aDatabaseEntry, aApplicationTheme);
}

void SliderSetsDBEntry::initializeGUI(const int aDatabaseIndex,
                                      const Struct::DatabaseSliderSet& aDatabaseEntry,
                                      const GUITheme aApplicationTheme)
{
  const auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setSpacing(10);
  lMainLayout->setContentsMargins(15, 20, 15, 15);
  lMainLayout->setColumnStretch(0, 0);
  lMainLayout->setColumnStretch(1, 1);
  lMainLayout->setColumnStretch(2, 1);
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  // SliderSet name
  const auto lSliderSetNameLabel{new QLabel(tr("Slider set name:"), this)};

  const auto lSliderSetNameValue{new LineEdit(aDatabaseEntry.getSliderSetName(), this)};
  lSliderSetNameValue->setObjectName("slider_set_name");

  lMainLayout->addWidget(lSliderSetNameLabel, 0, 0);
  lMainLayout->addWidget(lSliderSetNameValue, 0, 1, 1, 2);

  // Preview
  const auto lPreviewCheckbox{new QLabel(tr("Preview:"), this)};

  const auto lPreviewValue{new QLabel(this)};
  lPreviewValue->setObjectName("preview_value");

  lMainLayout->addWidget(lPreviewCheckbox, 1, 0);
  lMainLayout->addWidget(lPreviewValue, 1, 1, 1, 2);

  // Resource type
  const auto lResourceTypeLabel{new QLabel(tr("Resource type:"), this)};

  const auto lResourceTypeValue{new QComboBox(this)};
  lResourceTypeValue->setObjectName("slider_set_type");
  lResourceTypeValue->setItemDelegate(new QStyledItemDelegate());
  lResourceTypeValue->setCursor(Qt::PointingHandCursor);
  lResourceTypeValue->addItems(DataLists::GetSliderSetsImporterActions());
  lResourceTypeValue->setCurrentIndex(static_cast<int>(aDatabaseEntry.getMeshType()));

  lMainLayout->addWidget(lResourceTypeLabel, 2, 0);
  lMainLayout->addWidget(lResourceTypeValue, 2, 1, 1, 2);

  // ID
  const auto lIDLabel{new QLabel(tr("ID:") + " " + QString::number(aDatabaseIndex), this)};
  lMainLayout->addWidget(lIDLabel, 3, 0);

  // Delete slider set
  const auto lDeleteButton{ComponentFactory::CreateButton(this,
                                                          tr("Delete slider set"),
                                                          "",
                                                          "trash",
                                                          Utils::GetIconResourceFolder(aApplicationTheme))};

  lMainLayout->addWidget(lDeleteButton, 3, 1);

  // View content
  const auto lViewContentButton{ComponentFactory::CreateButton(this,
                                                               tr("View file content"),
                                                               "",
                                                               "preview",
                                                               Utils::GetIconResourceFolder(aApplicationTheme))};

  lMainLayout->addWidget(lViewContentButton, 3, 2);

  // Event binding
  QObject::connect(lSliderSetNameValue, &QLineEdit::textEdited, this, &SliderSetsDBEntry::sliderSetNameChanged);
  QObject::connect(lResourceTypeValue, &QComboBox::currentIndexChanged, this, [&](int aIndex) {
    emit meshTypeUpdated(this->mDatabaseIndex, static_cast<MeshPartType>(aIndex));
  });
  QObject::connect(lDeleteButton, &QPushButton::clicked, this, [&]() {
    emit deleteButtonClicked(this->mDatabaseIndex);
    this->deleteLater();
  });
  QObject::connect(lViewContentButton, &QPushButton::clicked, this, [&]() {
    emit viewContentButtonClicked(this->mDatabaseIndex);
  });

  // Post-bind initialization functions
  this->sliderSetNameChanged(lSliderSetNameValue->text());
}

void SliderSetsDBEntry::sliderSetNameChanged(const QString& aNewSliderSetName)
{
  this->setTitle(aNewSliderSetName);

  const auto lPreviewValue{this->findChild<QLabel*>("preview_value")};
  lPreviewValue->setText(tr("Follower Name - %1").arg(aNewSliderSetName));

  emit nameUpdated(this->mDatabaseIndex, aNewSliderSetName);
}
