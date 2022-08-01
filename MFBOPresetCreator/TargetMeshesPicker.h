#pragma once
#include "SliderSetsDBDAO.h"
#include "TitleDialog.h"

class QListWidget;
class QTabWidget;

class TargetMeshesPicker final : public TitleDialog
{
  Q_OBJECT

public:
  explicit TargetMeshesPicker(QWidget* aParent,
                              const Struct::Settings& aSettings,
                              std::map<QString, QString>* aLastPaths,
                              const BodyNameVersion& aPreSelectedBody,
                              const FeetNameVersion& aPreSelectedFeet);

protected:
  void closeEvent(QCloseEvent* aEvent) override;

private:
  // Database of injected slider sets
  std::map<int, Struct::DatabaseSliderSet> mDatabase{SliderSetsDBDAO::loadDatabase()};

  // Original values gotten when constructing this dialog
  const BodyNameVersion mOriginalBody;
  const FeetNameVersion mOriginalFeet;

  // Body related list widgets
  QListWidget* mListBodyName{nullptr};
  int mCurrentBodyBase{-1};
  QListWidget* mListBodyVersion{nullptr};
  int mCurrentBodyVersionNumber{-1};
  QListWidget* mListBodyVariantName{nullptr};
  int mCurrentBodyVersionName{-1};

  QListWidget* mListBodyCustom{nullptr};
  int mCurrentBodyCustom{-1};

  // Feet related list widgets
  QListWidget* mListFeetName{nullptr};
  int mCurrentFeetBase{-1};
  QListWidget* mListFeetVersion{nullptr};
  int mCurrentFeetVersionNumber{-1};
  QListWidget* mListFeetVariantName{nullptr};
  int mCurrentFeetVersionName{-1};

  QListWidget* mListFeetCustom{nullptr};
  int mCurrentFeetCustom{-1};

  // Hands related list widgets
  QListWidget* mListHandsEmbedded{nullptr};
  int mCurrentHandsEmbedded{-1};

  QListWidget* mListHandsCustom{nullptr};
  int mCurrentHandsCustom{-1};

  // Beast hands related list widgets
  QListWidget* mListBeastHandsEmbedded{nullptr};
  int mCurrentBeastHandsEmbedded{-1};

  QListWidget* mListBeastHandsCustom{nullptr};
  int mCurrentBeastHandsCustom{-1};

  // Smarter lists behavior
  bool mIsWindowInitialized{false};
  QString mLastSelectedFeetName;
  QString mLastSelectedFeetVariant;

  // Common functions
  void initializeGUI();

  // Body selection section
  void setupBodyTabWidget(QVBoxLayout& aMainLayout);
  void setupEmbeddedBodyTab(QTabWidget& aTabWidget);
  void setupCustomBodyTab(QTabWidget& aTabWidget);

  // Feet selection section
  void setupFeetTabWidget(QVBoxLayout& aMainLayout);
  void setupEmbeddedFeetTab(QTabWidget& aTabWidget);
  void setupCustomFeetTab(QTabWidget& aTabWidget);

  // Hands selection section
  void setupHandsTabWidget(QVBoxLayout& aMainLayout);
  void setupEmbeddedHandsTab(QTabWidget& aTabWidget);
  void setupCustomHandsTab(QTabWidget& aTabWidget);

  // Beast hands selection section
  void setupBeastHandsTabWidget(QVBoxLayout& aMainLayout);
  void setupEmbeddedBeastHandsTab(QTabWidget& aTabWidget);
  void setupCustomBeastHandsTab(QTabWidget& aTabWidget);

  // Generic selection selection functions
  void setupEmbeddedTab(QTabWidget& aTabWidget,
                        const QString& aIconName,
                        const QString& aTabName,
                        QListWidget& aWidgetModName,
                        QListWidget& aWidgetModVersion,
                        QListWidget& aWidgetModVariantName,
                        const QStringList& aNamesList);
  void setupCustomTab(QTabWidget& aTabWidget,
                      const QString& aIconName,
                      const QString& aTabName,
                      QListWidget& aListWidget,
                      const MeshPartType aResourceType);

  // Inject custom presets
  void openSliderSetsDatabaseManager();
  void updateCustomSliderSetsLists() const;

  // Fetch selected values
  BodyVariant getChosenBodyVariant() const;
  BodyNameVersion getChosenBodyName() const;
  FeetNameVersion getChosenFeetName() const;
  QString getChosenHandsName() const;
  QString getChosenBeastHandsName() const;

  // GUI widgets events
  void validateAndClose();

  // Body GUI widgets events
  void bodyNameIndexChanged(const int aNewIndex);
  void bodyVersionIndexChanged(const int aNewIndex);
  void bodyVariantIndexChanged(const int aNewIndex);

  // Feet GUI widgets events
  void feetNameIndexChanged(const int aNewIndex);
  void feetVersionIndexChanged(const int aNewIndex);
  void feetVariantIndexChanged(const int aNewIndex);

  // Selection preview
  void refreshSelectionPreview() const;

signals:
  void valuesChosen(const BodyNameVersion& aBody, const FeetNameVersion& aFeet);
};
