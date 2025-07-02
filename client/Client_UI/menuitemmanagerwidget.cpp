#include "menuitemmanagerwidget.h"
#include <QPixmap>

MenuItemManagerWidget::MenuItemManagerWidget(QString name,
                                             QString category,
                                             double price,
                                             QString imagePath,
                                             QWidget *parent)
    : QWidget(parent), currentImagePath(imagePath)
{
    setStyleSheet("background-color: #F9E6C4; border-radius: 12px; padding: 15px;");
    setFixedHeight(100);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(20);

    // Image
    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(80, 80);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("background-color: #D8C9B4; border-radius: 8px;");

    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            imageLabel->setPixmap(pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageLabel->setStyleSheet("border-radius: 8px;");
        }
    } else {
        imageLabel->setText("No Image");
        imageLabel->setStyleSheet("background-color: #D8C9B4; border-radius: 8px; color: #5E4935; font-style: italic;");
    }

    // Info
    QVBoxLayout *infoLayout = new QVBoxLayout();
    nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #3E2C19;");

    categoryLabel = new QLabel("Category: " + category);
    categoryLabel->setStyleSheet("font-size: 13px; color: #5E4935;");

    priceLabel = new QLabel(QString("Price: $%1").arg(price, 0, 'f', 2));
    priceLabel->setStyleSheet("font-size: 14px; color: #A6531C; font-weight: bold;");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(categoryLabel);
    infoLayout->addWidget(priceLabel);
    infoLayout->addStretch();

    // Buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout();

    editButton = new QPushButton("Edit");
    editButton->setStyleSheet(
        "QPushButton { background-color: #E3B07C; border: none; border-radius: 6px; padding: 8px 16px; color: white; font-weight: bold; }"
        "QPushButton:hover { background-color: #D19A60; }");

    deleteButton = new QPushButton("Delete");
    deleteButton->setStyleSheet(
        "QPushButton { background-color: #D46A6A; border: none; border-radius: 6px; padding: 8px 16px; color: white; font-weight: bold; }"
        "QPushButton:hover { background-color: #C85A5A; }");

    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();

    mainLayout->addWidget(imageLabel);
    mainLayout->addLayout(infoLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    connect(editButton, &QPushButton::clicked, this, &MenuItemManagerWidget::editClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MenuItemManagerWidget::deleteClicked);
}

QString MenuItemManagerWidget::getName() const {
    return nameLabel->text();
}

QString MenuItemManagerWidget::getCategory() const {
    return categoryLabel->text().replace("Category: ", "");
}

double MenuItemManagerWidget::getPrice() const {
    return priceLabel->text().replace("Price: $", "").toDouble();
}

QString MenuItemManagerWidget::getImagePath() const {
    return currentImagePath;
}

void MenuItemManagerWidget::setName(const QString &name) {
    nameLabel->setText(name);
}

void MenuItemManagerWidget::setCategory(const QString &category) {
    categoryLabel->setText("Category: " + category);
}

void MenuItemManagerWidget::setPrice(double price) {
    priceLabel->setText(QString("Price: $%1").arg(price, 0, 'f', 2));
}

void MenuItemManagerWidget::setImage(const QString &imagePath) {
    currentImagePath = imagePath;
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            imageLabel->setPixmap(pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageLabel->setStyleSheet("border-radius: 8px;");
        }
    } else {
        imageLabel->clear();
        imageLabel->setText("No Image");
        imageLabel->setStyleSheet("background-color: #D8C9B4; border-radius: 8px; color: #5E4935; font-style: italic;");
    }
}
