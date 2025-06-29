#include "cartwindow.h"
#include <QPushButton>

CartWindow::CartWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Shopping Cart");
    resize(650, 700);
    setStyleSheet("background-color: #EFE1C6;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Shopping Cart");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 26px; font-weight: bold; font-family: 'Segoe UI'; color: #7A4F32;");
    mainLayout->addWidget(titleLabel);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none;");

    QWidget* scrollContent = new QWidget();
    itemsLayout = new QVBoxLayout(scrollContent);
    scrollContent->setLayout(itemsLayout);
    scrollArea->setWidget(scrollContent);

    mainLayout->addWidget(scrollArea);

    // نمونه آیتم‌ها:
    CartItemWidget* item1 = new CartItemWidget("Peach Garden", "Sushi, Green Tea", 29.99);
    CartItemWidget* item2 = new CartItemWidget("Olive Dine", "Steak, French Fries, Juice", 54.75);

    itemsLayout->addWidget(item1);
    itemsLayout->addWidget(item2);

    connect(item1, &CartItemWidget::removeRequested, this, [=](CartItemWidget* w){
        itemsLayout->removeWidget(w);
        w->deleteLater();
    });

    connect(item2, &CartItemWidget::removeRequested, this, [=](CartItemWidget* w){
        itemsLayout->removeWidget(w);
        w->deleteLater();
    });

    QPushButton* backButton = new QPushButton("Back to Menu");
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(R"(
        QPushButton {
            background-color: #d97706;
            color: white;
            font-family: 'Segoe UI';
            font-size: 16px;
            border-radius: 12px;
            padding: 10px 24px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #f59e0b;
        }
    )");
    mainLayout->addWidget(backButton, 0, Qt::AlignCenter);

    connect(backButton, &QPushButton::clicked, this, [=]() {
        close(); // یا emit backToMenu(); برای رفتن به منو واقعی
    });
}
