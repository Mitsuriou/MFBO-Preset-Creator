#include "SSSPSelectionBlock.h"
#include "ComponentFactory.h"
#include "DataLists.h"
#include "Utils.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFileInfo>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStyledItemDelegate>

SSSPSelectionBlock::SSSPSelectionBlock(QWidget* aParent,
                                       const GUITheme& aApplicationTheme,
                                       const int aPointSize,
                                       const QString& aFullFilePath,
                                       const Struct::SliderSet& aSliderSet)
  : GroupBox(aParent, aSliderSet.getName(), "file", Utils::GetIconResourceFolder(aApplicationTheme), aPointSize)
{
  this->initializeGUI(aFullFilePath, aSliderSet);
}

void SSSPSelectionBlock::initializeGUI(const QString& aFullFilePath, const Struct::SliderSet& aSliderSet)
{
  QFormLayout* lMainLayout = new QFormLayout(this);
  lMainLayout->setSpacing(10);
  lMainLayout->setContentsMargins(15, 20, 15, 15);
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  // Import checkbox
  const auto lImportPresetCheckbox{ComponentFactory::CreateCheckBox(this, tr("Import this preset"), "")};
  lMainLayout->addRow(lImportPresetCheckbox);

  // Preset name
  const auto lPresetNameLabel{new QLabel(tr("Preset name:"), this)};
  const auto lPresetNameValue{new QLineEdit(aSliderSet.getName(), this)};
  lMainLayout->addRow(lPresetNameLabel, lPresetNameValue);

  // Preview
  const auto lPreviewCheckbox{new QLabel(tr("Preview:"), this)};
  const auto lPreviewValue{new QLabel(this)};
  lPreviewValue->setObjectName("preview_value");
  lMainLayout->addRow(lPreviewCheckbox, lPreviewValue);

  // Ressource type
  const auto lRessourceTypeLabel{new QLabel(tr("Ressource type:"), this)};
  const auto lRessourceTypeValue{new QComboBox(this)};
  lRessourceTypeValue->setItemDelegate(new QStyledItemDelegate());
  lRessourceTypeValue->setCursor(Qt::PointingHandCursor);
  lRessourceTypeValue->addItems(DataLists::GetSliderSetsScanPickerActions());
  lMainLayout->addRow(lRessourceTypeLabel, lRessourceTypeValue);

  //// Source directory
  // const auto lSourceDirectoryNameLabel{new QLabel(tr("Source directory:"), this)};
  // const auto lSourceDirectoryNameValue{new QLabel(QFileInfo(aFullFilePath).absolutePath(), this)};
  // lMainLayout->addRow(lSourceDirectoryNameLabel, lSourceDirectoryNameValue);

  // Source file
  const auto lSourceFileNameLabel{new QLabel(tr("Source file:"), this)};
  const auto lSourceFileNameValue{new QLabel(QFileInfo(aFullFilePath).fileName(), this)};
  lMainLayout->addRow(lSourceFileNameLabel, lSourceFileNameValue);

  // Event binding
  QObject::connect(lPresetNameValue, &QLineEdit::textEdited, this, &SSSPSelectionBlock::presetNameChanged);

  // Post-bind initialization functions
  presetNameChanged(lPresetNameValue->text());
}

void SSSPSelectionBlock::presetNameChanged(const QString& aNewPresetName)
{
  this->setTitle(aNewPresetName);

  const auto lPreviewValue{this->findChild<QLabel*>("preview_value")};
  lPreviewValue->setText(QString("A Follower Name - %1").arg(aNewPresetName));
}
