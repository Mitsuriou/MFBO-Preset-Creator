#include "PresetsDatabaseManager.h"
#include "ComponentFactory.h"
#include "SliderSetsImporter.h"
#include "Utils.h"
#include <QApplication>
#include <QDomDocument>
#include <QFile>
#include <QProgressBar>
#include <QProgressDialog>

PresetsDatabaseManager::PresetsDatabaseManager(QWidget* aParent,
                                               const Struct::Settings& aSettings,
                                               std::map<QString, QString>* aLastPaths)
  : TitleDialog(aParent, aSettings, aLastPaths)
{
  const auto lMainLayout = new QVBoxLayout(this);
  this->setLayout(lMainLayout);

  const auto lButton{ComponentFactory::CreateButton(this, "DEBUG - IMPORT PRESETS", "", "", "")};
  lMainLayout->addWidget(lButton);

  QObject::connect(lButton, &QPushButton::clicked, this, &PresetsDatabaseManager::openSliderSetsImporter);

  this->show();
}

void PresetsDatabaseManager::closeEvent(QCloseEvent* /*aEvent*/)
{
}

void PresetsDatabaseManager::openSliderSetsImporter()
{
  const auto lSliderSetsImporter{new SliderSetsImporter(this, this->settings(), this->lastPaths())};
  QObject::connect(lSliderSetsImporter, &SliderSetsImporter::valuesChosen, this, &PresetsDatabaseManager::importNewPresets);
}

void PresetsDatabaseManager::importNewPresets(const std::vector<Struct::SliderSetResult>& aChosenPresets)
{
  std::map<QString, std::vector<const Struct::SliderSetResult*>> lPaths;
  auto lEntriesCount{0};

  // Iterate over the results
  for (const auto& lChosenPreset : aChosenPresets)
  {
    // Filter the paths to know which file to open and parse
    lPaths[lChosenPreset.getFilePath()].push_back(&lChosenPreset);
    lEntriesCount++;
  }

  // Progress bar
  auto lProgressBar{new QProgressBar(this)};
  lProgressBar->setFormat("%v / %m");
  lProgressBar->setRange(0, lEntriesCount);
  lProgressBar->setValue(0);
  lProgressBar->setTextVisible(true);

  // Progress dialog
  QProgressDialog lProgressDialog(tr("Parsing XML files. Please wait..."), tr("Cancel treatment"), 0, 0, this);
  lProgressDialog.setBar(lProgressBar);
  lProgressDialog.setModal(true);
  lProgressDialog.show();

  QCoreApplication::processEvents();

  auto lTreatedFiles{0};

  // Start the scan
  for (const auto& lEntry : lPaths)
  {
    QFile lReadFile(lEntry.first);
    QDomDocument lOSPDocument;
    if (!lReadFile.open(QFile::ReadOnly | QFile::Text))
    {
      // Display a message in case the file cannot be found
      Utils::DisplayWarningMessage(tr("Error while trying to read the file \"%1\".").arg(lEntry.first));

      lTreatedFiles += static_cast<int>(lEntry.second.size()); // Skip all the entries coming from this file
      lProgressDialog.setValue(lTreatedFiles);
      QCoreApplication::processEvents();
      continue;
    }

    lOSPDocument.setContent(&lReadFile);
    lReadFile.close();

    const auto lRoot{lOSPDocument.documentElement()};
    auto lNextNodeToParse{lRoot.firstChild().toElement()};

    while (!lNextNodeToParse.isNull())
    {
      if (lNextNodeToParse.tagName().compare("SliderSet", Qt::CaseSensitivity::CaseInsensitive) == 0)
      {
        const auto lSliderSetName{lNextNodeToParse.attribute("name", "")};

        // Check if a preset chosen by the user has the same name than the current one
        for (const auto& lSliderSet : lEntry.second)
        {
          if (lSliderSetName == lSliderSet->getOriginalSliderSetName())
          {
            // TODO: Change the QDomDocument to a QJsonDocument
            QTextStream lTS(stdout);
            lNextNodeToParse.save(lTS, 0);
            qDebug() << lTS.readAll();
          }

          lProgressDialog.setValue(++lTreatedFiles);
          QCoreApplication::processEvents();
        }

        // const auto lSplittedName{lSliderSetEntry.getName().split('-')};
        // if (lSplittedName.isEmpty())
        //{
        //   lNextNodeToParse = lNextNodeToParse.nextSiblingElement();
        //   continue;
        // }

        // auto lNextChildNodeToParse{lNextNodeToParse.firstChild().toElement()};
        // while (!lNextChildNodeToParse.isNull())
        //{
        //   if (lNextChildNodeToParse.tagName() == "OutputPath")
        //   {
        //     lSliderSetEntry.setOutputPath(lNextChildNodeToParse.firstChild().toText().data()); // OutputPath
        //   }
        //   else if (lNextChildNodeToParse.tagName() == "OutputFile")
        //   {
        //     lSliderSetEntry.setOutputFile(lNextChildNodeToParse.firstChild().toText().data()); // OutputFile
        //   }

        //  if (!lSliderSetEntry.getOutputPath().isEmpty() && !lSliderSetEntry.getOutputFile().isEmpty())
        //  {
        //    break;
        //  }

        //  lNextChildNodeToParse = lNextChildNodeToParse.nextSiblingElement();
        //}

        // if (lSliderSetEntry.isValid() || (aSkipTypeAnalysis && lSliderSetEntry.isValidNoMeshPartCheck()))
        //   lSliderSets.push_back(lSliderSetEntry);
      }

      lNextNodeToParse = lNextNodeToParse.nextSiblingElement();
    }
  }
}
