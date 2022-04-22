#include "ThemeCard.h"
#include "ComponentFactory.h"
#include <QLabel>
#include <QVBoxLayout>

ThemeCard::ThemeCard(const QString& aThemeName, QWidget* aParent)
  : QWidget(aParent)
  , mThemeName(aThemeName)
  , mPreviewImage(QPixmap(QString(":/qss-previews/%2").arg(aThemeName)))
{
  auto lWrapper{new QVBoxLayout(this)};
  this->setLayout(lWrapper);

  // Preview image
  const auto lImageLabel{new QLabel(this)};
  lImageLabel->setObjectName("image_label");
  lWrapper->addWidget(lImageLabel);

  // Load button
  const auto lButton{ComponentFactory::CreateButton(this, aThemeName, "", "", "")};
  lWrapper->addWidget(lButton);

  // Event binding
  QObject::connect(lButton, &QPushButton::clicked, this, [&]() {
    emit loadQSSTheme(this->mThemeName);
  });

  // Simulate a resize event
  this->resizeEvent(nullptr);
}

void ThemeCard::resizeEvent(QResizeEvent*)
{
  auto lImageLabel{this->findChild<QLabel*>("image_label")};

  const auto lOriginalWidth{this->mPreviewImage.width()};
  const auto lOriginalHeight{this->mPreviewImage.height()};

  lImageLabel->setMinimumSize(1, lImageLabel->width() / (lOriginalWidth * lOriginalHeight));

  lImageLabel->setPixmap(this->mPreviewImage.scaledToWidth(lImageLabel->width(),
                                                           Qt::TransformationMode::SmoothTransformation));
}
