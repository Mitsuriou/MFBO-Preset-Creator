#include "TextInputDialog.h"
#include "ComponentFactory.h"
#include "Utils.h"
#include <QCloseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

TextInputDialog::TextInputDialog(const QString& aTitle, const QString& aLabel, const GUITheme& aAppTheme, QWidget* aParent)
  : QDialog(aParent)
  , mAppTheme(aAppTheme)
{
  // Build the window's interface
  this->setWindowProperties(aTitle);
  this->initializeGUI(aLabel);
  this->adjustSize();
}

void TextInputDialog::closeEvent(QCloseEvent* aEvent)
{
  auto lEventButton{qobject_cast<QPushButton*>(sender())};
  auto lAddBtn{this->findChild<QPushButton*>("add_button")};

  if (lEventButton == lAddBtn)
  {
    emit getTextValue(this->findChild<QLineEdit*>("text")->text());
  }
  else
  {
    emit getTextValue("");
  }

  aEvent->accept();
}

void TextInputDialog::updateAddButtonStatus(const QString& aText)
{
  auto lAddBtn{this->findChild<QPushButton*>("add_button")};

  if (aText.trimmed().length() == 0)
  {
    lAddBtn->setDisabled(true);
  }
  else
  {
    lAddBtn->setDisabled(false);
  }
}

void TextInputDialog::setWindowProperties(const QString& aTitle)
{
  this->setModal(true);
  this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
  this->setWindowTitle(aTitle);
}

void TextInputDialog::initializeGUI(const QString& aLabel)
{
  // User theme accent
  const auto& lIconFolder{Utils::getIconRessourceFolder(this->mAppTheme)};

  // Main layout
  auto lMainLayout{new QVBoxLayout(this)};
  this->setLayout(lMainLayout);

  // Top layout
  auto lTopContainer{new QVBoxLayout(this)};
  lMainLayout->addLayout(lTopContainer);

  // Label
  lTopContainer->addWidget(new QLabel(aLabel, this));

  // LineEdit
  auto lLineEdit{new QLineEdit(this)};
  lLineEdit->setObjectName("text");
  lTopContainer->addWidget(lLineEdit);

  // Bottom layout (buttons)
  auto lBtnsContainer{new QHBoxLayout()};
  lBtnsContainer->setSpacing(10);
  lMainLayout->addLayout(lBtnsContainer);

  // Add
  auto lAddBtn{ComponentFactory::createButton(this, tr("Add"), "", "plus", lIconFolder, "add_button")};
  lBtnsContainer->addWidget(lAddBtn);

  // Cancel
  auto lCancelBtn{ComponentFactory::createButton(this, tr("Cancel"), "", "cross", lIconFolder)};
  lBtnsContainer->addWidget(lCancelBtn);

  // Events binding
  this->connect(lLineEdit, &QLineEdit::textChanged, this, &TextInputDialog::updateAddButtonStatus);
  this->connect(lAddBtn, &QPushButton::clicked, this, &TextInputDialog::close);
  this->connect(lCancelBtn, &QPushButton::clicked, this, &TextInputDialog::close);

  // Post-bind initialization functions
  this->updateAddButtonStatus("");
}
