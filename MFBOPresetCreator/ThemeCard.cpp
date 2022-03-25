#include "ThemeCard.h"
#include "ComponentFactory.h"
#include <QLabel>
#include <QVBoxLayout>

ThemeCard::ThemeCard(const QString& aThemeName, QWidget* aParent)
  : QWidget(aParent)
  , mThemeName(aThemeName)
{
  auto lWrapper{new QVBoxLayout(this)};
  this->setLayout(lWrapper);

  // Preview image
  auto lImageLabel{new QLabel(this)};
  lImageLabel->setObjectName("image_label");
  this->mPreviewImage = QPixmap(QString(":/qss-previews/%2").arg(aThemeName));
  lImageLabel->setPixmap(this->mPreviewImage.scaled(lImageLabel->size(),
                                                    Qt::AspectRatioMode::KeepAspectRatio,
                                                    Qt::TransformationMode::SmoothTransformation));
  lWrapper->addWidget(lImageLabel);

  // Load button
  auto lButton{ComponentFactory::CreateButton(this, aThemeName, "", "", "")};
  lWrapper->addWidget(lButton);

  // Event binding
  QObject::connect(lButton, &QPushButton::clicked, this, &ThemeCard::buttonClicked);
}

void ThemeCard::resizeEvent(QResizeEvent*)
{
  auto lImageLabel{this->findChild<QLabel*>("image_label")};
  lImageLabel->setPixmap(this->mPreviewImage.scaledToWidth(lImageLabel->width(),
                                                           Qt::TransformationMode::SmoothTransformation));
}

void ThemeCard::buttonClicked()
{
  emit loadQSSTheme(this->mThemeName);
}
