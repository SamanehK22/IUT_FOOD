#include "fooditemwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QPixmap>

FoodItemWidget::FoodItemWidget(QString name,
                               QString price,
                               QString ingredients,
                               QString category,
                               QString imagePath,
                               QWidget *parent)
    : QWidget(parent)
{
    this->setStyleSheet(
        "QLabel { color: #3e2723; font-family: 'Segoe UI'; }"
        "QPushButton {"
        "  background-color: #8d6e63;"
        "  color: white;"
        "  border-radius: 12px;"
        "  padding: 6px 12px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #6d4c41; }"
        "QSpinBox {"
        "  background-color: #fefaf4;"
        "  border: 1px solid #a1887f;"
        "  border-radius: 8px;"
        "  padding: 2px;"
        "  width: 50px;"
        "}"
    );

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(15);


    imageLabel = new QLabel();
    imageLabel->setFixedSize(120, 120);
    imageLabel->setPixmap(QPixmap(imagePath).scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    imageLabel->setStyleSheet("border-radius: 10px;");


    QVBoxLayout *infoLayout = new QVBoxLayout();

    nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    ingredientsLabel = new QLabel(ingredients);
    ingredientsLabel->setStyleSheet("font-size: 14px; color: #5d4037;");
    ingredientsLabel->setWordWrap(true);

    categoryLabel = new QLabel("Category: " + category);
    categoryLabel->setStyleSheet("font-size: 13px; font-style: italic; color: #6d4c41;");

    priceLabel = new QLabel(price);
    priceLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    quantitySpinBox = new QSpinBox();
    quantitySpinBox->setRange(1, 99);
    quantitySpinBox->setFixedSize(50, 28);
    quantitySpinBox->setStyleSheet(
        "QSpinBox {"
        "  background-color: #fdf6e3;"
        "  border: 1px solid #a1887f;"
        "  border-radius: 10px;"
        "  padding: 2px;"
        "  font-size: 13px;"
        "  color: #3e2723;"
        "}"
        "QSpinBox::up-button, QSpinBox::down-button {"
        "  width: 12px;"
        "  background-color: transparent;"
        "  border: none;"
        "}"
        "QSpinBox::up-arrow, QSpinBox::down-arrow {"
        "  width: 8px;"
        "  height: 8px;"
        "}"
    );

    addButton = new QPushButton("Add to Cart");

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(priceLabel);
    bottomLayout->addWidget(quantitySpinBox);
    bottomLayout->addWidget(addButton);

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(ingredientsLabel);
    infoLayout->addWidget(categoryLabel);
    infoLayout->addLayout(bottomLayout);

    mainLayout->addWidget(imageLabel);
    mainLayout->addLayout(infoLayout);
}
