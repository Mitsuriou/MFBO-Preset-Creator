#include "BodySlideFiltersEditor.h"

BodySlideFiltersEditor::BodySlideFiltersEditor(QWidget* parent, Struct::Settings aSettings, const std::map<QString, QStringList>& aInitialList)
  : QDialog(parent)
  , mSettings(aSettings)
  , mFiltersList(aInitialList)
  , mListWidget{new QListWidget(this)}
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void BodySlideFiltersEditor::closeEvent(QCloseEvent* aEvent)
{
  auto lEventButton{qobject_cast<QPushButton*>(sender())};
  auto lSaveButton{this->findChild<QPushButton*>("save_close")};

  if (lEventButton == lSaveButton)
  {
    // Save the list
    auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};
    this->saveGUIListToFiltersMap(lChooser->itemText(this->mPreviousChooserIndex));

    Utils::saveFiltersToFile(this->mFiltersList);
    emit listEdited(this->mFiltersList);
  }

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

void BodySlideFiltersEditor::initializeGUI()
{
  // Main window container
  auto lMainLayout{new QGridLayout(this)};
  this->setupInterface(*lMainLayout);
  this->setupButtons(*lMainLayout);

  // Focus the first line of the list
  this->mListWidget->setAlternatingRowColors(true);
  this->mListWidget->setCurrentRow(0);
  this->mListWidget->setFocus();
}

void BodySlideFiltersEditor::setupInterface(QGridLayout& aLayout)
{
  // Body filter set chooser
  auto lLabelFilters{new QLabel(tr("Edit set:"), this)};
  aLayout.addWidget(lLabelFilters, 0, 0);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  lFiltersListChooser->setDisabled(true);
  aLayout.addWidget(lFiltersListChooser, 0, 1);

  auto lAddSetBtn{new QPushButton(this)};
  lAddSetBtn->setText("+");
  lAddSetBtn->setObjectName("add-set");
  aLayout.addWidget(lAddSetBtn, 0, 2);

  auto lRemoveSetBtn{new QPushButton(this)};
  lRemoveSetBtn->setText("-");
  lAddSetBtn->setObjectName("remove-set");
  aLayout.addWidget(lRemoveSetBtn, 0, 3);

  // Global label
  auto lListLabel{new QLabel(tr("Current filter's name:"), this)};
  aLayout.addWidget(lListLabel, 1, 0);

  auto lSetNameEditor{new QLineEdit(this)};
  lSetNameEditor->setObjectName("current_filter_set_name");
  lSetNameEditor->setDisabled(true);
  aLayout.addWidget(lSetNameEditor, 1, 1);

  auto lLineCount{this->mListWidget->count()};
  for (int i = 0; i < lLineCount; i++)
  {
    auto lItem{this->mListWidget->item(i)};
    lItem->setSizeHint(QSize(-1, 30));
    lItem->setFlags(lItem->flags() | Qt::ItemFlag::ItemIsEditable);
  }

  aLayout.addWidget(this->mListWidget, 2, 0, 1, 2);

  // Right layout
  auto lButtonLayout{new QVBoxLayout(this)};
  aLayout.addLayout(lButtonLayout, 2, 2, 1, 2, Qt::AlignTop);

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

  // Event binding
  this->connect(lAddSetBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::addSet);
  this->connect(lRemoveSetBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::removeSet);
  this->connect(lSetNameEditor, &QLineEdit::editingFinished, this, &BodySlideFiltersEditor::handleSetRenaming);

  // Post-bind initialization functions
  this->mFiltersList = Utils::loadFiltersFromFile();
  this->initBodySlideFiltersList();
  lFiltersListChooser->setCurrentIndex(0);
}

void BodySlideFiltersEditor::setupButtons(QGridLayout& aLayout)
{
  // Vertical layout for the buttons
  auto lButtonsContainer{new QHBoxLayout()};
  lButtonsContainer->setSpacing(10);
  aLayout.addLayout(lButtonsContainer, 3, 0, 1, 4);

  // Create the buttons
  auto lSaveBtn{new QPushButton(tr("Save and close"), this)};
  lSaveBtn->setCursor(Qt::PointingHandCursor);
  lSaveBtn->setObjectName("save_close");
  lSaveBtn->setAutoDefault(false);
  lSaveBtn->setDefault(false);
  lButtonsContainer->addWidget(lSaveBtn);

  auto lCancelBtn{new QPushButton(tr("Cancel"), this)};
  lCancelBtn->setCursor(Qt::PointingHandCursor);
  lCancelBtn->setAutoDefault(false);
  lCancelBtn->setDefault(false);
  lButtonsContainer->addWidget(lCancelBtn);

  // Event binding
  this->connect(lSaveBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::close);
  this->connect(lCancelBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::close);
}

void BodySlideFiltersEditor::initBodySlideFiltersList()
{
  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};
  this->disconnect(lChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&BodySlideFiltersEditor::showFiltersList));
  lChooser->clear();
  this->connect(lChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, qOverload<int>(&BodySlideFiltersEditor::showFiltersList));

  auto lSetNameEditor{this->findChild<QLineEdit*>("current_filter_set_name")};

  // Disable some GUI components if there is not any available filter
  if (this->mFiltersList.size() == 0)
  {
    lChooser->setDisabled(true);
    lSetNameEditor->setDisabled(true);
    return;
  }

  lChooser->setDisabled(false);
  lSetNameEditor->setDisabled(false);

  // Fill the combobox
  for (const auto& lPair : this->mFiltersList)
  {
    lChooser->addItem(lPair.first);
  }
}

void BodySlideFiltersEditor::saveGUIListToFiltersMap(const QString& aMapKey)
{
  if (aMapKey.length() == 0)
  {
    return;
  }

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

  auto lIterator{this->mFiltersList.find(aMapKey)};
  lIterator->second = lList;
}

void BodySlideFiltersEditor::showFiltersList(int aIndex)
{
  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};

  // Save the previously displayed list
  this->saveGUIListToFiltersMap(lChooser->itemText(this->mPreviousChooserIndex));

  // Update the selected index of the combobox
  this->mPreviousChooserIndex = aIndex;

  // Change the displayed name to be edited
  auto lSetNameEditor{this->findChild<QLineEdit*>("current_filter_set_name")};
  lSetNameEditor->setText(lChooser->currentText());

  // Clear the entries list
  for (int i = this->mListWidget->count(); i > 0; i--)
  {
    auto lItem{this->mListWidget->item(i - 1)};
    delete lItem;
    lItem = nullptr;
  }

  // Populate the list from the user data
  auto lList{this->mFiltersList.find(lChooser->currentText())->second};

  for (const auto& lFilter : lList)
  {
    this->mListWidget->addItem(lFilter);
    auto lItem{this->mListWidget->item(this->mListWidget->count() - 1)};
    lItem->setFlags(lItem->flags() | Qt::ItemFlag::ItemIsEditable);
  }
}

void BodySlideFiltersEditor::addSet()
{
}

void BodySlideFiltersEditor::removeSet()
{
  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};
  auto lIterator{this->mFiltersList.find(lChooser->currentText())};
  this->mFiltersList.erase(lIterator);
  this->initBodySlideFiltersList();
}

void BodySlideFiltersEditor::handleSetRenaming()
{
  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};
  auto lPreviousText{lChooser->currentText()};

  auto lSetNameEditor{this->findChild<QLineEdit*>("current_filter_set_name")};
  auto lNewText{lSetNameEditor->text()};

  // If the set name has not been changed but validated by the user, skip the renaming part
  if (lPreviousText.compare(lNewText, Qt::CaseSensitive) == 0)
  {
    return;
  }

  // Prevent two sets being named the same way
  if (this->mFiltersList.count(lNewText) > 0)
  {
    lSetNameEditor->setText(lNewText.append("(2)"));
    return;
  }

  // Save the list
  this->saveGUIListToFiltersMap(lChooser->itemText(this->mPreviousChooserIndex));

  // Rename the filters set
  auto lIterator{this->mFiltersList.find(lPreviousText)};
  std::swap(this->mFiltersList[lNewText], lIterator->second);
  this->mFiltersList.erase(lIterator);
  this->initBodySlideFiltersList();

  // Iterate the map to find the new key position
  auto lNewIndex{0};
  for (const auto& lPair : this->mFiltersList)
  {
    if (lPair.first.compare(lNewText, Qt::CaseSensitive) == 0)
    {
      break;
    }
    lNewIndex++;
  }

  if (lNewIndex == this->mFiltersList.size())
  {
    lNewIndex = 0;
  }

  lChooser->setCurrentIndex(lNewIndex);
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
