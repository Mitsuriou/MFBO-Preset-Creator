#include "BodySlideFiltersEditor.h"

BodySlideFiltersEditor::BodySlideFiltersEditor(QWidget* parent, Struct::Settings aSettings, const std::map<QString, QStringList>& aInitialList)
  : QDialog(parent)
  , mSettings(aSettings)
  , mOriginalFiltersList(aInitialList)
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
    // Remove every empty filter
    for (auto& lPair : this->mFiltersList)
    {
      lPair.second.removeAll(QString(""));
    }

    Utils::saveFiltersToFile(this->mFiltersList);
    emit listEdited(this->mFiltersList);
    aEvent->accept();
    return;
  }

  if (this->mOriginalFiltersList.size() != this->mFiltersList.size()
      || !(std::equal(this->mOriginalFiltersList.begin(), this->mOriginalFiltersList.end(), this->mFiltersList.begin())))
  {
    // User theme accent
    const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

    QMessageBox lConfirmationBox(QMessageBox::Icon::Question, tr("Closing"), tr("Do you want to close the window?"), QMessageBox::StandardButton::NoButton, this);
    lConfirmationBox.setIconPixmap(QPixmap(QString(":/%1/help-circle").arg(lIconFolder)).scaledToHeight(48, Qt::SmoothTransformation));

    auto lCloseButton{lConfirmationBox.addButton(tr("Close the editor window without saving"), QMessageBox::ButtonRole::YesRole)};
    lCloseButton->setCursor(Qt::PointingHandCursor);
    lCloseButton->setStyleSheet("color: hsl(4, 90%, 58%);");

    auto lStayButton{lConfirmationBox.addButton(tr("Go back to the editor window"), QMessageBox::ButtonRole::NoRole)};
    lStayButton->setCursor(Qt::PointingHandCursor);
    lStayButton->setStyleSheet("color: hsl(141, 53%, 53%)");

    lConfirmationBox.setDefaultButton(lStayButton);
    lConfirmationBox.exec();

    if (lConfirmationBox.clickedButton() != lCloseButton)
    {
      aEvent->ignore();
      return;
    }
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

  this->mFiltersListChooser = new QComboBox(this);
  this->mFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  this->mFiltersListChooser->setCursor(Qt::PointingHandCursor);
  this->mFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  this->mFiltersListChooser->setDisabled(true);
  this->mFiltersListChooser->setEditable(true);
  this->mFiltersListChooser->setCompleter(nullptr);
  aLayout.addWidget(this->mFiltersListChooser, 0, 1);

  // Add filters set
  auto lNewSetBtn{new QPushButton(this)};
  lNewSetBtn->setCursor(Qt::PointingHandCursor);
  lNewSetBtn->setToolTip(tr("Add a new BodySlide filters set"));
  lNewSetBtn->setText(tr("New set"));
  lNewSetBtn->setIcon(QIcon(QPixmap(QString(":/%1/plus").arg(lIconFolder))));
  lNewSetBtn->setObjectName("add-set");
  lNewSetBtn->setAutoDefault(false);
  lNewSetBtn->setDefault(false);
  aLayout.addWidget(lNewSetBtn, 0, 2);

  // Remove filters set
  auto lDelSetBtn{new QPushButton(this)};
  lDelSetBtn->setCursor(Qt::PointingHandCursor);
  lDelSetBtn->setToolTip(tr("Remove the selected BodySlide filters set"));
  lDelSetBtn->setText(tr("Remove set"));
  lDelSetBtn->setIcon(QIcon(QPixmap(QString(":/%1/minus").arg(lIconFolder))));
  lDelSetBtn->setAutoDefault(false);
  lDelSetBtn->setDefault(false);
  aLayout.addWidget(lDelSetBtn, 0, 3);

  // Filters list
  aLayout.addWidget(this->mListWidget, 1, 0, 1, 3);

  // Right buttons layout
  auto lButtonLayout{new QVBoxLayout(this)};
  aLayout.addLayout(lButtonLayout, 1, 3, Qt::AlignTop);

  // New filter
  auto lAddNewRow{new QPushButton(this)};
  lAddNewRow->setStyleSheet("text-align:left;");
  lAddNewRow->setCursor(Qt::PointingHandCursor);
  lAddNewRow->setToolTip(tr("Add a new BodySlide filter"));
  lAddNewRow->setText(tr("New filter"));
  lAddNewRow->setIcon(QIcon(QPixmap(QString(":/%1/playlist-plus").arg(lIconFolder))));
  lAddNewRow->setAutoDefault(false);
  lAddNewRow->setDefault(false);
  lButtonLayout->addWidget(lAddNewRow);

  // Remove filter
  auto lDeleteRow{new QPushButton(this)};
  lDeleteRow->setStyleSheet("text-align:left;");
  lDeleteRow->setCursor(Qt::PointingHandCursor);
  lDeleteRow->setToolTip(tr("Remove the selected BodySlide filter"));
  lDeleteRow->setText(tr("Remove filter"));
  lDeleteRow->setIcon(QIcon(QPixmap(QString(":/%1/trash").arg(lIconFolder))));
  lDeleteRow->setAutoDefault(false);
  lDeleteRow->setDefault(false);
  lButtonLayout->addWidget(lDeleteRow);

  // Remove all filters
  auto lDeleteAllRows{new QPushButton(this)};
  lDeleteAllRows->setStyleSheet("text-align:left;");
  lDeleteAllRows->setCursor(Qt::PointingHandCursor);
  lDeleteAllRows->setToolTip(tr("Remove all the BodySlide filters"));
  lDeleteAllRows->setText(tr("Remove all"));
  lDeleteAllRows->setIcon(QIcon(QPixmap(QString(":/%1/trash-lines").arg(lIconFolder))));
  lDeleteAllRows->setAutoDefault(false);
  lDeleteAllRows->setDefault(false);
  lButtonLayout->addWidget(lDeleteAllRows);

  // Delete row action
  auto lDelAction{new QAction(this->mListWidget)};
  lDelAction->setShortcut(Qt::Key_Delete);
  lDelAction->setShortcutContext(Qt::WidgetShortcut);
  this->mListWidget->addAction(lDelAction);

  // Event binding
  this->connect(lNewSetBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::addSet);
  this->connect(lDelSetBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::removeSet);

  this->connect(lAddNewRow, &QPushButton::clicked, this, &BodySlideFiltersEditor::addRow);
  this->connect(lDeleteRow, &QPushButton::clicked, this, &BodySlideFiltersEditor::deleteRow);
  this->connect(lDeleteAllRows, &QPushButton::clicked, this, &BodySlideFiltersEditor::deleteAllRows);
  this->connect(lDelAction, &QAction::triggered, this, &BodySlideFiltersEditor::deleteRow);

  // Post-bind initialization functions
  this->mFiltersList = Utils::loadFiltersFromFile();
  this->updateFiltersCombobox();
  this->displayFilterAt(0);
}

void BodySlideFiltersEditor::setupButtons(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(mSettings.appTheme)};

  // Vertical layout for the buttons
  auto lButtonsContainer{new QHBoxLayout()};
  lButtonsContainer->setSpacing(10);
  aLayout.addLayout(lButtonsContainer, 3, 0, 1, 4);

  // Create the buttons
  auto lSaveBtn{new QPushButton(tr("Save and close"), this)};
  lSaveBtn->setCursor(Qt::PointingHandCursor);
  lSaveBtn->setIcon(QIcon(QPixmap(QString(":/%1/save").arg(lIconFolder))));
  lSaveBtn->setObjectName("save_close");
  lSaveBtn->setAutoDefault(false);
  lSaveBtn->setDefault(false);
  lButtonsContainer->addWidget(lSaveBtn);

  auto lCancelBtn{new QPushButton(tr("Cancel"), this)};
  lCancelBtn->setCursor(Qt::PointingHandCursor);
  lCancelBtn->setIcon(QIcon(QPixmap(QString(":/%1/undo").arg(lIconFolder))));
  lCancelBtn->setAutoDefault(false);
  lCancelBtn->setDefault(false);
  lButtonsContainer->addWidget(lCancelBtn);

  // Event binding
  this->connect(lSaveBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::close);
  this->connect(lCancelBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::close);
}

void BodySlideFiltersEditor::updateFiltersCombobox()
{
  this->disconnect(this->mFiltersListChooser, &QComboBox::currentTextChanged, this, &BodySlideFiltersEditor::handleSetRenaming);
  this->disconnect(this->mFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &BodySlideFiltersEditor::showFiltersList);
  this->disconnect(this->mListWidget, &QListWidget::itemChanged, this, &BodySlideFiltersEditor::handleRowRenaming);

  this->mFiltersListChooser->clear();

  // Disable some GUI components if there is not any available filter
  if (this->mFiltersList.size() == 0)
  {
    this->mFiltersListChooser->setDisabled(true);
    this->mListWidget->clear();
    return;
  }

  this->mFiltersListChooser->setDisabled(false);

  // Fill the combobox
  for (const auto& lPair : this->mFiltersList)
  {
    this->mFiltersListChooser->addItem(lPair.first);
  }

  this->connect(this->mListWidget, &QListWidget::itemChanged, this, &BodySlideFiltersEditor::handleRowRenaming);
  this->connect(this->mFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &BodySlideFiltersEditor::showFiltersList);
  this->connect(this->mFiltersListChooser, &QComboBox::currentTextChanged, this, &BodySlideFiltersEditor::handleSetRenaming);
}

void BodySlideFiltersEditor::displayFilterAt(const int& aIndex)
{
  this->mFiltersListChooser->setCurrentIndex(-1);
  this->mFiltersListChooser->setCurrentIndex(aIndex);
}

void BodySlideFiltersEditor::showFiltersList(int aIndex)
{
  // If there is nothing to display, return
  if (aIndex == -1)
  {
    return;
  }

  // Clear the entries list
  for (int i = this->mListWidget->count(); i > 0; i--)
  {
    auto lItem{this->mListWidget->item(i - 1)};
    delete lItem;
    lItem = nullptr;
  }

  // Populate the list from the user data
  auto lList{this->mFiltersList.find(this->mFiltersListChooser->itemText(aIndex))->second};

  for (const auto& lFilter : lList)
  {
    this->mListWidget->addItem(lFilter);
    auto lItem{this->mListWidget->item(this->mListWidget->count() - 1)};
    lItem->setFlags(lItem->flags() | Qt::ItemFlag::ItemIsEditable);
  }
}

void BodySlideFiltersEditor::addSet()
{
  auto lDialog{new TextInputDialog(tr("Add a new BodySlide filters set"), tr("Filters set name:"), this->mSettings.appTheme, this)};
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
  this->updateFiltersCombobox();

  // Iterate the map to find the created key
  auto lNewIndex{0};
  for (const auto& lPair : this->mFiltersList)
  {
    if (lPair.first.compare(aSetName, Qt::CaseSensitive) == 0)
      break;
    lNewIndex++;
  }

  // Select the new key
  this->displayFilterAt(lNewIndex);
}

void BodySlideFiltersEditor::removeSet()
{
  auto lIterator{this->mFiltersList.find(this->mFiltersListChooser->currentText())};
  this->mFiltersList.erase(lIterator);
  this->updateFiltersCombobox();

  // Select the first available element
  this->displayFilterAt(0);
}

void BodySlideFiltersEditor::handleSetRenaming(const QString& aNewSetName)
{
  QString lNewSetName = aNewSetName;
  auto lPreviousText{this->mFiltersListChooser->itemText(this->mFiltersListChooser->currentIndex())};

  // Save the curent cursor position to restore it later
  auto lCursorPos{this->mFiltersListChooser->lineEdit()->cursorPosition()};

  // If the set name has not been changed but validated by the user, skip the renaming part
  if (lPreviousText.compare(lNewSetName, Qt::CaseSensitive) == 0)
  {
    return;
  }

  // Prevent two sets being named the same way
  if (this->mFiltersList.count(lNewSetName) > 0)
  {
    lNewSetName.append("(2)");
  }

  // Rename the filters set
  auto lIterator{this->mFiltersList.find(lPreviousText)};
  std::swap(this->mFiltersList[lNewSetName], lIterator->second);
  this->mFiltersList.erase(lIterator);
  this->updateFiltersCombobox();

  // Iterate the map to find the new key position
  auto lNewIndex{0};
  for (const auto& lPair : this->mFiltersList)
  {
    if (lPair.first.compare(lNewSetName, Qt::CaseSensitive) == 0)
    {
      break;
    }
    lNewIndex++;
  }

  if (lNewIndex == this->mFiltersList.size())
  {
    lNewIndex = 0;
  }

  // Select the wanted index
  this->displayFilterAt(lNewIndex);

  // Restore the cursor position
  this->mFiltersListChooser->lineEdit()->setCursorPosition(lCursorPos);
}

void BodySlideFiltersEditor::addRow()
{
  // Add the element to the filters list
  this->mFiltersList.find(this->mFiltersListChooser->currentText())->second.append("");

  // Add the element to the list widget
  this->mListWidget->addItem("");

  auto lLastLineIndex{this->mListWidget->count() - 1};

  auto lItem{this->mListWidget->item(lLastLineIndex)};
  lItem->setFlags(lItem->flags() | Qt::ItemFlag::ItemIsEditable);

  this->mListWidget->setCurrentRow(lLastLineIndex);
  this->mListWidget->editItem(lItem);
}

void BodySlideFiltersEditor::deleteRow()
{
  // Delete the element from the filters list
  auto lIterator{this->mFiltersList.find(this->mFiltersListChooser->currentText())};
  lIterator->second.removeAt(this->mListWidget->currentRow());

  // Delete the element from the list widget
  auto lItem{this->mListWidget->currentItem()};
  delete lItem;
  lItem = nullptr;

  this->mListWidget->setFocus();
}

void BodySlideFiltersEditor::deleteAllRows()
{
  this->mFiltersList.find(this->mFiltersListChooser->currentText())->second.clear();
  this->mListWidget->clear();
  this->mListWidget->setFocus();
}

void BodySlideFiltersEditor::handleRowRenaming()
{
  if (this->mListWidget->currentRow() < 0)
  {
    return;
  }

  // Delete the element from the filters list
  auto lIterator{this->mFiltersList.find(this->mFiltersListChooser->currentText())};

  // Save the filters list to the selected set
  auto lEntries{this->mListWidget->count()};
  for (int i = 0; i < lEntries; i++)
  {
    lIterator->second.replace(i, this->mListWidget->item(i)->text());
  }
}
