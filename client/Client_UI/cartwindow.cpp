#include "cartwindow.h"
#include <QPushButton>
#include <QHBoxLayout>
#include "restaurantlistwindow.h"

CartWindow::CartWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Shopping Cart");
    resize(650, 700);
    setStyleSheet("background-color: #EFE1C6;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // --- بخش هدر با عنوان و دکمه Back ---
    QHBoxLayout* headerLayout = new QHBoxLayout();

    // دکمه Back با استایل درخواستی
    QPushButton* backButton = new QPushButton("◀️");
    backButton->setFixedSize(40, 32);
    backButton->setStyleSheet(R"(
        QPushButton {
            background-color: #FFF3E0;
            color: #FF5722;
            font: bold 10pt "Segoe UI";
            border: 2px solid #FFB74D;
            border-radius: 12px;
            padding: 6px 12px;
        }
        QPushButton:hover {
            background-color: #FFE0B2;
            color: #E64A19;
            border: 2px solid #FFA726;
        }
    )");

    headerLayout->addWidget(backButton);
    headerLayout->addStretch(); // فاصله بین دکمه و عنوان

    QLabel* titleLabel = new QLabel("Shopping Cart");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 26px; font-weight: bold; font-family: 'Segoe UI'; color: ;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch(); // برای بالانس کردن عنوان در مرکز

    mainLayout->addLayout(headerLayout);

    // --- ناحیه اسکرول ---
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none;");

    QWidget* scrollContent = new QWidget();
    itemsLayout = new QVBoxLayout(scrollContent);
    scrollContent->setLayout(itemsLayout);
    scrollArea->setWidget(scrollContent);

    mainLayout->addWidget(scrollArea);

    // --- نمونه آیتم‌ها ---
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

    // --- تغییر اتصال سیگنال دکمه Back ---
    connect(backButton, &QPushButton::clicked, this, [this](){
        this->close(); // بستن پنجره فعلی
        RestaurantListWindow *restaurantWindow = new RestaurantListWindow();
        restaurantWindow->show();
    });
}
