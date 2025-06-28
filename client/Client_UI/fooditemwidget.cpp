#include "fooditemwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>

FoodItemWidget::FoodItemWidget(QString name, QString price, QString ingredients, QString imagePath, QWidget *parent)
    : QWidget(parent)
{
    this->setStyleSheet(
        "QWidget {"
        " background-color: #fffaf3;"
        " border: 1px solid #a1887f;"
        " border-radius: 12px;"
        " padding: 10px;"
        " }"
        "QLabel { font-size: 14px; color: #5d4037; }"
        "QPushButton {"
        " background-color: #8d6e63;"
        " color: white;"
        " padding: 6px 15px;"
        " border: none;"
        " border-radius: 8px;"
        " font-weight: bold;"
        " }"
        "QPushButton:hover { background-color: #6d4c41; }"
    );

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(15);

    imgLabel = new QLabel();
    QPixmap pix(imagePath);
    imgLabel->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imgLabel->setFixedSize(100, 100);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    priceLabel = new QLabel(price);
    ingredientsLabel = new QLabel("Ingredients: " + ingredients);

    addButton = new QPushButton("Add to Cart");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(priceLabel);
    infoLayout->addWidget(ingredientsLabel);
    infoLayout->addWidget(addButton);

    mainLayout->addWidget(imgLabel);
    mainLayout->addLayout(infoLayout);
}
