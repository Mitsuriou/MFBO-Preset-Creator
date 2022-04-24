#pragma once
#include "TitleDialog.h"

class FileIDPicker final : public TitleDialog
{
  Q_OBJECT

public:
  explicit FileIDPicker(QWidget* aParent,
                        const Struct::Settings& aSettings,
                        std::map<QString, QString>* aLastPaths,
                        const std::vector<Struct::NexusModsFileInformation>& aFilesInformation);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  bool hasEmitedAnythingYet{false};

  // GUI functions
  void setWindowProperties();
  void initializeGUI();
  void fillChoicesList(const std::vector<Struct::NexusModsFileInformation>& aFilesInformation);
  QWidget* createChoiceEntry(const Struct::NexusModsFileInformation& aFileInformation);

signals:
  void fileContentPreviewURLChosen(const QString& aFileName, const QString& aContentPreviewLink);
  void fileContentPreviewURLChosen(const QString& aContentPreviewLink);
};
