#include "restaurantitemwidget.h"

RestaurantItemWidget::RestaurantItemWidget(QString name, QString address, double rating, QString imagePath, QWidget *parent)
    : QWidget(parent)
{
    this->setStyleSheet("background-color: #FDE8CC; border-radius: 12px; padding: 8px;");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(50, 50);
    imageLabel->setScaledContents(true);
    if (!imagePath.isEmpty())
        imageLabel->setPixmap(QPixmap(imagePath).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        imageLabel->setStyleSheet("background-color: #DDD; border-radius: 8px;");

    QVBoxLayout *infoLayout = new QVBoxLayout();

    nameLabel = new QLabel("🍴 " + name);
    nameLabel->setStyleSheet("font-weight: 600; font-size: 14px; color: #4A3220;");

    addressLabel = new QLabel("📍 " + address);
    addressLabel->setStyleSheet("color: #5E5E5E; font-size: 12px;");

    ratingLabel = new QLabel(QString("⭐ %1 / 5").arg(rating));
    ratingLabel->setStyleSheet("color: #B17D0D; font-size: 12px;");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(addressLabel);
    infoLayout->addWidget(ratingLabel);

    mainLayout->addWidget(imageLabel);
    mainLayout->addLayout(infoLayout);
    mainLayout->setSpacing(12);
}
