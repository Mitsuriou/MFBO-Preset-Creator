#include "BodySlideFiltersEditor.h"
#include "ComponentFactory.h"
#include "TextInputDialog.h"
#include "Utils.h"
#include <QAction>
#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStyledItemDelegate>

BodySlideFiltersEditor::BodySlideFiltersEditor(QWidget* aParent, Struct::Settings aSettings, const std::map<QString, QStringList>& aInitialList)
  : QDialog(aParent)
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
  auto lEventButton{qobject_cast<QPushButton*>(this->sender())};
  auto lSaveButton{this->findChild<QPushButton*>(QString("save_close"))};

  if (lEventButton == lSaveButton)
  {
    // Remove every empty filter
    for (auto& lPair : this->mFiltersList)
    {
      lPair.second.removeAll(QString());
    }

    Utils::SaveFiltersToFile(this->mFiltersList);
    emit listEdited(this->mFiltersList);
    aEvent->accept();
    return;
  }

  if (this->mOriginalFiltersList.size() != this->mFiltersList.size()
      || !(std::equal(this->mOriginalFiltersList.begin(), this->mOriginalFiltersList.end(), this->mFiltersList.begin())))
  {
    // User theme accent
    const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

    if (Utils::DisplayQuestionMessage(this,
                                      tr("Closing"),
                                      tr("Do you want to close the window?"),
                                      lIconFolder,
                                      "help-circle",
                                      tr("Close the editor window without saving"),
                                      tr("Go back to the editor window"),
                                      "",
                                      this->mSettings.display.dangerColor,
                                      this->mSettings.display.successColor,
                                      "",
                                      false)
        != ButtonClicked::YES)
    {
      aEvent->ignore();
      return;
    }
  }

  aEvent->accept();
}

void BodySlideFiltersEditor::reject()
{
  this->close();
}

void BodySlideFiltersEditor::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
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

  // Post-GUI construction initialization functions
  this->shouldDisableFiltersControls();
}

void BodySlideFiltersEditor::setupInterface(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Body filter set chooser
  aLayout.addWidget(new QLabel(tr("Edit set:"), this), 0, 0);

  this->mFiltersListChooser = new QComboBox(this);
  this->mFiltersListChooser->setItemDelegate(new QStyledItemDelegate());
  this->mFiltersListChooser->setCursor(Qt::PointingHandCursor);
  this->mFiltersListChooser->setObjectName(QString("bodyslide_filters_chooser"));
  this->mFiltersListChooser->setDisabled(true);
  this->mFiltersListChooser->setEditable(true);
  this->mFiltersListChooser->setCompleter(nullptr);
  aLayout.addWidget(this->mFiltersListChooser, 0, 1);

  // Add filters set
  auto lNewSetBtn{ComponentFactory::CreateButton(this, tr("New set"), tr("Add a new BodySlide filters set"), "plus", lIconFolder, "", false, true)};
  aLayout.addWidget(lNewSetBtn, 0, 2);

  // Remove filters set
  auto lDelSetBtn{ComponentFactory::CreateButton(this, tr("Remove set"), tr("Remove the selected BodySlide filters set"), "minus", lIconFolder, "remove_set", false, true)};
  aLayout.addWidget(lDelSetBtn, 0, 3);

  // Filters list
  aLayout.addWidget(this->mListWidget, 1, 0, 1, 3);

  // Right buttons layout
  auto lButtonLayout{new QVBoxLayout(this)};
  aLayout.addLayout(lButtonLayout, 1, 3, Qt::AlignTop);

  // New filter
  auto lAddNewRow{ComponentFactory::CreateButton(this, tr("New filter"), tr("Add a new BodySlide filter"), "playlist-plus", lIconFolder, "add_filter", false, true)};
  lAddNewRow->setStyleSheet("text-align:left;");
  lButtonLayout->addWidget(lAddNewRow);

  // Remove filter
  auto lDeleteRow{ComponentFactory::CreateButton(this, tr("Remove filter"), tr("Remove the selected BodySlide filter"), "trash", lIconFolder, "remove_filter", false, true)};
  lDeleteRow->setStyleSheet("text-align:left;");
  lButtonLayout->addWidget(lDeleteRow);

  // Remove all filters
  auto lDeleteAllRows{ComponentFactory::CreateButton(this, tr("Remove all"), tr("Remove all the BodySlide filters"), "trash-lines", lIconFolder, "remove_all_filters", false, true)};
  lDeleteAllRows->setStyleSheet("text-align:left;");
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
  this->mFiltersList = Utils::LoadFiltersFromFile();
  this->updateFiltersCombobox();
  this->displayFilterAt(0);
}

void BodySlideFiltersEditor::setupButtons(QGridLayout& aLayout)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Vertical layout for the buttons
  auto lButtonsContainer{new QHBoxLayout()};
  lButtonsContainer->setSpacing(10);
  aLayout.addLayout(lButtonsContainer, 3, 0, 1, 4);

  // Create the buttons
  auto lSaveBtn{ComponentFactory::CreateButton(this, tr("Save and close"), "", "save", lIconFolder, "save_close", false, true)};
  lButtonsContainer->addWidget(lSaveBtn);

  auto lCancelBtn{ComponentFactory::CreateButton(this, tr("Cancel"), "", "undo", lIconFolder, "", false, true)};
  lButtonsContainer->addWidget(lCancelBtn);

  // Event binding
  this->connect(lSaveBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::close);
  this->connect(lCancelBtn, &QPushButton::clicked, this, &BodySlideFiltersEditor::close);
}

void BodySlideFiltersEditor::updateFiltersCombobox() const
{
  this->disconnect(this->mFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &BodySlideFiltersEditor::showFiltersList);
  this->disconnect(this->mFiltersListChooser, &QComboBox::currentTextChanged, this, &BodySlideFiltersEditor::handleSetRenaming);
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

  this->connect(this->mFiltersListChooser, qOverload<int>(&QComboBox::currentIndexChanged), this, &BodySlideFiltersEditor::showFiltersList);
  this->connect(this->mFiltersListChooser, &QComboBox::currentTextChanged, this, &BodySlideFiltersEditor::handleSetRenaming);
  this->connect(this->mListWidget, &QListWidget::itemChanged, this, &BodySlideFiltersEditor::handleRowRenaming);
}

void BodySlideFiltersEditor::displayFilterAt(const int aIndex) const
{
  this->mFiltersListChooser->setCurrentIndex(-1);
  this->mFiltersListChooser->setCurrentIndex(aIndex);
}

void BodySlideFiltersEditor::shouldDisableFiltersControls()
{
  // Fetch the button in the GUI
  auto lDelSetBtn{this->findChild<QPushButton*>(QString("remove_set"))};
  auto lAddNewRow{this->findChild<QPushButton*>(QString("add_filter"))};
  auto lDeleteRow{this->findChild<QPushButton*>(QString("remove_filter"))};
  auto lDeleteAllRows{this->findChild<QPushButton*>(QString("remove_all_filters"))};

  if (this->mFiltersList.size() > 0)
  {
    lDelSetBtn->setDisabled(false);
    lAddNewRow->setDisabled(false);
    lDeleteRow->setDisabled(false);
    lDeleteAllRows->setDisabled(false);
  }
  else
  {
    lDelSetBtn->setDisabled(true);
    lAddNewRow->setDisabled(true);
    lDeleteRow->setDisabled(true);
    lDeleteAllRows->setDisabled(true);
  }
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
  auto lDialog{new TextInputDialog(tr("Add a new BodySlide filters set"), tr("Filters set name:"), this->mSettings.display.applicationTheme, this)};
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
    Utils::DisplayWarningMessage(tr("Error: there is already one set with the name \"%1\".").arg(aSetName));
    return;
  }

  this->mFiltersList.insert(std::make_pair(aSetName, QStringList()));
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

  this->shouldDisableFiltersControls();
}

void BodySlideFiltersEditor::removeSet()
{
  auto lIterator{this->mFiltersList.find(this->mFiltersListChooser->currentText())};

  this->mFiltersList.erase(lIterator);
  this->updateFiltersCombobox();

  // Select the first available element
  this->displayFilterAt(0);

  this->shouldDisableFiltersControls();
}

void BodySlideFiltersEditor::handleSetRenaming(const QString& aNewSetName)
{
  auto lNewSetName{QString(aNewSetName)};
  const auto lPreviousText{this->mFiltersListChooser->itemText(this->mFiltersListChooser->currentIndex())};

  // Save the curent cursor position to restore it later
  auto lCursorPos{this->mFiltersListChooser->lineEdit()->cursorPosition()};

  // If the set name has not been changed but validated by the user, skip the renaming part
  if (lPreviousText.compare(lNewSetName, Qt::CaseSensitive) == 0)
  {
    return;
  }

  // Prevent two sets being named the same way
  auto lOpenIndex{0};
  auto lCloseIndex{-1};
  auto lNextValue{-1};
  auto lLength{-1};
  auto lSuccessfullyCasted{false};

  while (this->mFiltersList.count(lNewSetName) > 0)
  {
    // Find last opening and closing parenthesis
    lOpenIndex = lNewSetName.lastIndexOf('(');
    lCloseIndex = lNewSetName.lastIndexOf(')');

    // Calculate the number of characters between the two parenthesis
    lLength = lCloseIndex - lOpenIndex - 1;

    // If two parenthesis have been found
    if (lOpenIndex != -1 && lCloseIndex != -1 && lLength > 0)
    {
      lNextValue = lNewSetName.midRef(lOpenIndex + 1, lLength).toInt(&lSuccessfullyCasted);
      if (!lSuccessfullyCasted)
      {
        lNextValue = 2;
      }
      lNewSetName.replace(lOpenIndex + 1, lLength, QString::number(lNextValue + 1));
    }
    else
    {
      lNewSetName.append(" (2)");
    }
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

  if (lNewIndex == static_cast<int>(this->mFiltersList.size()))
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
