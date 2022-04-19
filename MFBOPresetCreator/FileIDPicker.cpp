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
  emit fileContentPreviewURLChosen("");
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
  auto lMainTitle{new QLabel(tr("Choose a file"), this)};
  lMainTitle->setAlignment(Qt::AlignCenter);
  lMainTitle->setStyleSheet(QString("font-size: %1pt").arg(this->mSettings.display.font.pointSize * 2));
  lMainLayout->addWidget(lMainTitle, 0, 0, Qt::AlignmentFlag::AlignTop);

  // List wrapper
  ComponentFactory::CreateScrollAreaComponentLayout(this, 1, 0);
}

void FileIDPicker::fillChoicesList(const std::vector<Struct::NexusModsFileInformation>& aFilesInformation)
{
  const auto lListWrapper{this->findChild<QGridLayout*>("data_container")};
  for (int i = 0; i < aFilesInformation.size(); i++)
  {
    lListWrapper->addWidget(this->createChoiceEntry(aFilesInformation.at(i)),
                            i,
                            0,
                            Qt::AlignmentFlag::AlignTop);
  }
}

QWidget* FileIDPicker::createChoiceEntry(const Struct::NexusModsFileInformation& aFileInformation)
{
  // User theme accent
  const auto& lIconFolder{Utils::GetIconResourceFolder(this->mSettings.display.applicationTheme)};

  // Wrapper
  auto lWidgetPtr{new QWidget(this)};

  // Layout
  auto lLayout{new QGridLayout(lWidgetPtr)};
  lWidgetPtr->setLayout(lLayout);

  // File name
  auto lFileName{new QLabel(tr("File name: ") + aFileInformation.getName(), lWidgetPtr)};
  lLayout->addWidget(lFileName, 0, 0, Qt::AlignmentFlag::AlignLeft);

  // File ID
  auto lFileID{new QLabel(tr("File ID: ") + QString::number(aFileInformation.getFileID()), lWidgetPtr)};
  lLayout->addWidget(lFileID, 1, 0, Qt::AlignmentFlag::AlignLeft);

  // Date
  auto lDate{new QLabel(tr("Upload date: ")
                          + aFileInformation.getDate()
                              .toString(QLocale::system().dateTimeFormat(QLocale::FormatType::ShortFormat)),
                        lWidgetPtr)};
  lLayout->addWidget(lDate, 2, 0, Qt::AlignmentFlag::AlignLeft);

  // Version
  auto lVersion{new QLabel(tr("Version: ") + aFileInformation.getVersion(), lWidgetPtr)};
  lLayout->addWidget(lVersion, 3, 0, Qt::AlignmentFlag::AlignLeft);

  // Selection button
  auto lSelectionButton{ComponentFactory::CreateButton(lWidgetPtr, tr("Select this file"), "", "done", lIconFolder)};
  lLayout->addWidget(lSelectionButton, 0, 1, 4, 1, Qt::AlignmentFlag::AlignRight);

  QObject::connect(lSelectionButton, &QPushButton::clicked, [=]() {
    emit fileContentPreviewURLChosen(aFileInformation.getContentPreviewURL());
    this->close();
  });

  return lWidgetPtr;
}
