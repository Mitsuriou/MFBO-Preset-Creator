#pragma once
#include "Struct.h"
#include <QDialog>

class FileIDPicker final : public QDialog
{
  Q_OBJECT

public:
  explicit FileIDPicker(QWidget* aParent, const Struct::Settings& aSettings, const std::vector<Struct::NexusModsFileInformation>& aFilesInformation);

protected:
  void closeEvent(QCloseEvent* aEvent) override;
  void reject() override;

private:
  const Struct::Settings mSettings;
  bool hasEmitedAnythingYet{false};

  // GUI functions
  void setWindowProperties();
  void initializeGUI();
  void fillChoicesList(const std::vector<Struct::NexusModsFileInformation>& aFilesInformation);
  QWidget* createChoiceEntry(const Struct::NexusModsFileInformation& aFileInformation);

  explicit FileIDPicker(const FileIDPicker&) = delete;
  FileIDPicker& operator=(const FileIDPicker&) = delete;

signals:
  void fileContentPreviewURLChosen(QString aFileName, QString aContentPreviewLink);
  void fileContentPreviewURLChosen(QString aContentPreviewLink);
};
