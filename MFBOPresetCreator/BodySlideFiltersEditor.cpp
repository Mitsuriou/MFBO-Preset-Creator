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
  this->setLayout(lMainLayout);

  // Focus the first line of the list
  this->mListWidget->setAlternatingRowColors(true);
  this->mListWidget->setCurrentRow(0);
  this->mListWidget->setFocus();
}

void BodySlideFiltersEditor::setupInterface(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // Body filter set chooser
  auto lLabelFilters{new QLabel(tr("Edit set:"), this)};
  aLayout.addWidget(lLabelFilters, 0, 0);

  auto lFiltersListChooser{new QComboBox(this)};
  lFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  lFiltersListChooser->setCursor(Qt::PointingHandCursor);
  lFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  lFiltersListChooser->setDisabled(true);
  aLayout.addWidget(lFiltersListChooser, 0, 1);

  // Add filters set
  auto lNewSetBtn{new QPushButton(this)};
  lNewSetBtn->setCursor(Qt::PointingHandCursor);
  lNewSetBtn->setToolTip(tr("Add a new BodySlide filters set"));
  lNewSetBtn->setText("New set");
  lNewSetBtn->setIcon(QIcon(QPixmap(QString(":/%1/plus").arg(lIconFolder))));
  lNewSetBtn->setObjectName("add-set");
  aLayout.addWidget(lNewSetBtn, 0, 2);

  // Remove filters set
  auto lDelSetBtn{new QPushButton(this)};
  lDelSetBtn->setCursor(Qt::PointingHandCursor);
  lDelSetBtn->setToolTip(tr("Remove the selected BodySlide filters set"));
  lDelSetBtn->setText("Remove set");
  lDelSetBtn->setIcon(QIcon(QPixmap(QString(":/%1/minus").arg(lIconFolder))));
  lNewSetBtn->setObjectName("remove-set");
  aLayout.addWidget(lDelSetBtn, 0, 3);

  // Filter's name
  auto lListLabel{new QLabel(tr("Current filter's name:"), this)};
  aLayout.addWidget(lListLabel, 1, 0);

  auto lSetNameEditor{new QLineEdit(this)};
  lSetNameEditor->setObjectName("current_filter_set_name");
  lSetNameEditor->setDisabled(true);
  aLayout.addWidget(lSetNameEditor, 1, 1);

  // Filters list
  auto lLineCount{this->mListWidget->count()};
  for (int i = 0; i < lLineCount; i++)
  {
    auto lItem{this->mListWidget->item(i)};
    lItem->setSizeHint(QSize(-1, 30));
    lItem->setFlags(lItem->flags() | Qt::ItemFlag::ItemIsEditable);
  }

  aLayout.addWidget(this->mListWidget, 2, 0, 1, 3);

  // Right buttons layout
  auto lButtonLayout{new QVBoxLayout(this)};
  aLayout.addLayout(lButtonLayout, 2, 3, Qt::AlignTop);

  // New filter
  auto lAddNewRow{new QPushButton(this)};
  lAddNewRow->setStyleSheet("text-align:left;");
  lAddNewRow->setCursor(Qt::PointingHandCursor);
  lAddNewRow->setToolTip(tr("Add a new BodySlide filter"));
  lAddNewRow->setText("New filter");
  lAddNewRow->setIcon(QIcon(QPixmap(QString(":/%1/new_line").arg(lIconFolder))));
  lButtonLayout->addWidget(lAddNewRow);

  // Remove filter
  auto lDeleteRow{new QPushButton(this)};
  lDeleteRow->setStyleSheet("text-align:left;");
  lDeleteRow->setCursor(Qt::PointingHandCursor);
  lDeleteRow->setToolTip(tr("Remove the selected BodySlide filter"));
  lDeleteRow->setText("Remove filter");
  lDeleteRow->setIcon(QIcon(QPixmap(QString(":/%1/bin").arg(lIconFolder))));
  lButtonLayout->addWidget(lDeleteRow);

  // Remove all filters
  auto lDeleteAllRows{new QPushButton(this)};
  lDeleteAllRows->setStyleSheet("text-align:left;");
  lDeleteAllRows->setCursor(Qt::PointingHandCursor);
  lDeleteAllRows->setToolTip(tr("Remove all the BodySlide filters"));
  lDeleteAllRows->setText("Clear");
  lDeleteAllRows->setIcon(QIcon(QPixmap(QString(":/%1/delete_all").arg(lIconFolder))));
  lButtonLayout->addWidget(lDeleteAllRows);

  // Delete row action
  auto lDelAction{new QAction(this->mListWidget)};
  lDelAction->setShortcut(Qt::Key_Delete);
  lDelAction->setShortcutContext(Qt::WidgetShortcut);
  this->mListWidget->addAction(lDelAction);

  // Event binding
  this->connect(lNewSetBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::addSet);
  this->connect(lDelSetBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::removeSet);
  this->connect(lSetNameEditor, &QLineEdit::editingFinished, this, &BodySlideFiltersEditor::handleSetRenaming);

  this->connect(lAddNewRow, &QPushButton::clicked, this, &BodySlideFiltersEditor::addRow);
  this->connect(lDeleteRow, &QPushButton::clicked, this, &BodySlideFiltersEditor::deleteRow);
  this->connect(lDeleteAllRows, &QPushButton::clicked, this, &BodySlideFiltersEditor::deleteAllRows);
  this->connect(lDelAction, &QAction::triggered, this, &BodySlideFiltersEditor::deleteRow);

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
  aLayout.addLayout(lButtonsContainer, 6, 0, 1, 4);

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
  auto lDialog{new TextInputDialog(tr("Add a new BodySlide filters set"), tr("Filters set name:"), this)};
  this->connect(lDialog, &TextInputDialog::getTextValue, this, &BodySlideFiltersEditor::addNewSetEntry);
  lDialog->exec();

  delete lDialog;
  lDialog = nullptr;
}

void BodySlideFiltersEditor::addNewSetEntry(const QString& aSetName)
{
  // Check if the value already exist
  if (aSetName.size() == 0)
  {
    return;
  }

  if (this->mFiltersList.count(aSetName) > 0)
  {
    Utils::displayWarningMessage(tr("Error: there is already one set with the name \"%1\".").arg(aSetName));
    return;
  }

  this->mFiltersList.insert({aSetName, QStringList()});
  this->initBodySlideFiltersList();

  // Iterate the map to find the created key
  auto lNewIndex{0};
  for (const auto& lPair : this->mFiltersList)
  {
    if (lPair.first.compare(aSetName, Qt::CaseSensitive) == 0)
    {
      break;
    }
    lNewIndex++;
  }

  // Select the new key
  auto lChooser{this->findChild<QComboBox*>("bodyslide_filters_chooser")};
  lChooser->setCurrentIndex(lNewIndex);
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
