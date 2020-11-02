#include "BodySlideFiltersEditor.h"

BodySlideFiltersEditor::BodySlideFiltersEditor(QWidget* parent)
  : QDialog(parent)
  , mModel{nullptr}
  , mListView{new QListView(this)}
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();

  // Show the window when it's completely built
  this->show();
}

void BodySlideFiltersEditor::setWindowProperties()
{
  this->setModal(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(tr("BodySlide filters editor"));
}

void BodySlideFiltersEditor::initializeGUI()
{
  // Main window container
  auto lMainVertical{new QGridLayout(this)};

  this->setupInterface(*lMainVertical);
}

void BodySlideFiltersEditor::setupInterface(QGridLayout& aLayout)
{
  // Default filter list
  QStringList lList;
  lList.append("MFBO");
  lList.append("CBBE");

  // Create the model based on the default list
  this->mModel = new QStringListModel(lList, this);

  // Create the viewer widget
  this->mListView->setModel(this->mModel);
  aLayout.addWidget(this->mListView);

  // add row button
  auto lAddNewRow{new QPushButton(tr("Add new filter"), this)};
  aLayout.addWidget(lAddNewRow);
  connect(lAddNewRow, &QPushButton::clicked, this, &BodySlideFiltersEditor::addNewFilterRow);
}

void BodySlideFiltersEditor::addNewFilterRow()
{
  this->mModel->insertRow(this->mModel->rowCount());

  if (this->mModel->rowCount() > 0)
  {
    this->mListView->setCurrentIndex(this->mModel->index(this->mModel->rowCount(), 0));
  }

  // WIP: This does not work at all
  this->mListView->setFocus();
}
