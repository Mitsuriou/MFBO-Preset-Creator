#include "BodySlideFiltersEditor.h"

BodySlideFiltersEditor::BodySlideFiltersEditor(QWidget* parent, Struct::Settings aSettings, const QStringList& aInitialList)
  : QDialog(parent)
  , mSettings(aSettings)
  , mListWidget{new QListWidget(this)}
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI(aInitialList);

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void BodySlideFiltersEditor::closeEvent(QCloseEvent* aEvent)
{
  // Construct the filters list from GUI
  QStringList lList;

  auto lRowCount{this->mListWidget->count()};
  for (int i = 0; i < lRowCount; i++)
  {
    auto lText{this->mListWidget->item(i)->text()};
    if (lText.trimmed().size() > 0)
    {
      lList.append(lText);
    }
  }

  // Save the filters in a file
  Utils::saveFiltersToFile(lList);

  emit listEdited(Struct::FilterList());
  aEvent->accept();
}

void BodySlideFiltersEditor::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("BodySlide filters editor"));
  this->setWindowIcon(QIcon(QPixmap(":/black/filter")));
}

void BodySlideFiltersEditor::initializeGUI(const QStringList& aInitialList)
{
  // Main window container
  auto lMainLayout{new QGridLayout(this)};
  this->setupInterface(*lMainLayout);

  // Populate the list from the user data
  for (const auto& lFilter : aInitialList)
  {
    this->mListWidget->addItem(lFilter);
    auto lItem{this->mListWidget->item(this->mListWidget->count() - 1)};
    lItem->setFlags(lItem->flags() | Qt::ItemFlag::ItemIsEditable);
  }

  // Focus the first line of the list
  this->mListWidget->setAlternatingRowColors(true);
  this->mListWidget->setCurrentRow(0);
  this->mListWidget->setFocus();
}

void BodySlideFiltersEditor::setupInterface(QGridLayout& aLayout)
{
  // Global label
  auto lListLabel{new QLabel(tr("BodySlide filters:"), this)};
  aLayout.addWidget(lListLabel, 0, 0, 1, 2);

  auto lLineCount{this->mListWidget->count()};
  for (int i = 0; i < lLineCount; i++)
  {
    auto lItem{this->mListWidget->item(i)};
    lItem->setSizeHint(QSize(-1, 30));
    lItem->setFlags(lItem->flags() | Qt::ItemFlag::ItemIsEditable);
  }

  aLayout.addWidget(this->mListWidget, 1, 0);

  // Right layout
  auto lButtonLayout{new QVBoxLayout(this)};
  aLayout.addLayout(lButtonLayout, 1, 1, Qt::AlignTop);

  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // Add row button
  auto lAddNewRow{new QPushButton(this)};
  lAddNewRow->setToolTip(tr("Add a new empty filter"));
  lAddNewRow->setCursor(Qt::PointingHandCursor);
  lAddNewRow->setIcon(QIcon(QPixmap(QString(":/%1/new_line").arg(lIconFolder))));
  lButtonLayout->addWidget(lAddNewRow);
  this->connect(lAddNewRow, &QPushButton::clicked, this, &BodySlideFiltersEditor::addRow);

  // Delete row button
  auto lDeleteRow{new QPushButton(this)};
  lDeleteRow->setToolTip(tr("Delete the selected filter"));
  lDeleteRow->setCursor(Qt::PointingHandCursor);
  lDeleteRow->setIcon(QIcon(QPixmap(QString(":/%1/bin").arg(lIconFolder))));
  lButtonLayout->addWidget(lDeleteRow);
  this->connect(lDeleteRow, &QPushButton::clicked, this, &BodySlideFiltersEditor::deleteRow);

  // Delete all rows button
  auto lDeleteAllRows{new QPushButton(this)};
  lDeleteAllRows->setToolTip(tr("Delete all filters"));
  lDeleteAllRows->setCursor(Qt::PointingHandCursor);
  lDeleteAllRows->setIcon(QIcon(QPixmap(QString(":/%1/delete_all").arg(lIconFolder))));
  lButtonLayout->addWidget(lDeleteAllRows);
  this->connect(lDeleteAllRows, &QPushButton::clicked, this, &BodySlideFiltersEditor::deleteAllRows);

  // Delete row action
  auto lDelAction{new QAction(this->mListWidget)};
  lDelAction->setShortcut(Qt::Key_Delete);
  lDelAction->setShortcutContext(Qt::WidgetShortcut);
  this->mListWidget->addAction(lDelAction);
  this->connect(lDelAction, &QAction::triggered, this, &BodySlideFiltersEditor::deleteRow);
}

void BodySlideFiltersEditor::addRow()
{
  this->mListWidget->addItem("");

  auto lLastLineIndex{this->mListWidget->count() - 1};

  auto lItem{this->mListWidget->item(lLastLineIndex)};
  lItem->setFlags(lItem->flags() | Qt::ItemFlag::ItemIsEditable);

  // TODO: lock the data to QRegExp("[A-Za-z0-9_ -]+")

  this->mListWidget->setCurrentRow(lLastLineIndex);
  this->mListWidget->editItem(lItem);
}

void BodySlideFiltersEditor::deleteRow()
{
  auto lItem{this->mListWidget->currentItem()};

  delete lItem;
  lItem = nullptr;

  this->mListWidget->setFocus();
}

void BodySlideFiltersEditor::deleteAllRows()
{
  for (int i = this->mListWidget->count(); i > 0; i--)
  {
    auto lItem{this->mListWidget->item(i - 1)};
    delete lItem;
    lItem = nullptr;
  }

  this->mListWidget->setFocus();
}
