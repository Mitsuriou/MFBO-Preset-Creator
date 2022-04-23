#include "FileIDPicker.h"
#include "ComponentFactory.h"
#include "Utils.h"
#include <QCloseEvent>
#include <QLabel>

FileIDPicker::FileIDPicker(QWidget* aParent, const Struct::Settings& aSettings, const std::vector<Struct::NexusModsFileInformation>& aFilesInformation)
  : QDialog(aParent, Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::Window | Qt::WindowCloseButtonHint)
  , mSettings(aSettings)
{
  // Build the window's interface
  this->setWindowProperties();
  this->initializeGUI();
  this->fillChoicesList(aFilesInformation);

  // Show the window when it's completely built
  this->adjustSize();
  this->show();
}

void FileIDPicker::closeEvent(QCloseEvent* aEvent)
{
  if (!this->hasEmitedAnythingYet)
  {
    emit fileContentPreviewURLChosen("", "");
    emit fileContentPreviewURLChosen("");
  }

  aEvent->accept();
}

void FileIDPicker::reject()
{
  this->close();
}

void FileIDPicker::setWindowProperties()
{
  this->setModal(true);
  this->setMinimumWidth(700);
  this->setMinimumHeight(600);
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setWindowTitle(tr("NexusMods File Picker"));
  this->setWindowIcon(QIcon(QPixmap(":/black/nexus-logo")));
}

void FileIDPicker::initializeGUI()
{
  // Main layout
  auto lMainLayout{new QGridLayout(this)};
  lMainLayout->setAlignment(Qt::AlignTop);
  this->setLayout(lMainLayout);

  // Main title
  auto lMainTitle{new QLabel(tr("Choose a distant file"), this)};
  lMainTitle->setAlignment(Qt::AlignCenter);
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(this->mSettings.display.font.pointSize * 2));
  lMainLayout->addWidget(lMainTitle, 0, 0, Qt::AlignmentFlag::AlignTop);

  // List wrapper
  ComponentFactory::CreateScrollAreaComponentLayout(this, 1, 0);
}

void FileIDPicker::fillChoicesList(const std::vector<Struct::NexusModsFileInformation>& aFilesInformation)
{
  const auto lListWrapper{this->findChild<QGridLayout*>("data_container")};
  for (size_t i = 0; i < aFilesInformation.size(); i++)
  {
    lListWrapper->addWidget(this->createChoiceEntry(aFilesInformation.at(i)),
                            static_cast<int>(i),
                            0,
                            Qt::AlignmentFlag::AlignTop);
  }
}

QWidget* FileIDPicker::createChoiceEntry(const Struct::NexusModsFileInformation& aFileInformation)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Wrapper
  auto lWrapper{new QGroupBox(aFileInformation.getName(), this)};

  // Layout
  auto lLayout{new QGridLayout(lWrapper)};
  lWrapper->setLayout(lLayout);

  auto lLineIndex{0};

  // Version
  auto lVersion{new QLabel(tr("Version: ") + aFileInformation.getVersion(), lWrapper)};
  lLayout->addWidget(lVersion, lLineIndex++, 0, Qt::AlignmentFlag::AlignLeft);

  // Category
  auto lCategoryName{new QLabel(tr("Category: ") + aFileInformation.getCategoryName(), lWrapper)};
  lLayout->addWidget(lCategoryName, lLineIndex++, 0, Qt::AlignmentFlag::AlignLeft);

  // Date
  auto lDate{new QLabel(tr("Upload date: ")
                          + aFileInformation.getDate()
                              .toString(QLocale::system().dateTimeFormat(QLocale::FormatType::ShortFormat)),
                        lWrapper)};
  lLayout->addWidget(lDate, lLineIndex++, 0, Qt::AlignmentFlag::AlignLeft);

  // File ID
  auto lFileID{new QLabel(tr("File ID: ") + QString::number(aFileInformation.getFileID()), lWrapper)};
  lLayout->addWidget(lFileID, lLineIndex++, 0, Qt::AlignmentFlag::AlignLeft);

  // Selection button
  auto lSelectionButton{ComponentFactory::CreateButton(lWrapper, tr("Select this file"), "", "done", lIconFolder)};
  lLayout->addWidget(lSelectionButton, 0, 1, lLineIndex + 1, 1, Qt::AlignmentFlag::AlignRight);

  QObject::connect(lSelectionButton, &QPushButton::clicked, [=]() {
    emit fileContentPreviewURLChosen(aFileInformation.getName(), aFileInformation.getContentPreviewURL());
    emit fileContentPreviewURLChosen(aFileInformation.getContentPreviewURL());
    this->hasEmitedAnythingYet = true;
    this->close();
  });

  return lWrapper;
}
