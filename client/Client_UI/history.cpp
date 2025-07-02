#include "history.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSpacerItem>
#include "restaurantlistwindow.h"

History::History(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(650, 730);
    this->setStyleSheet("background-color: #EFE1C6;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // --- بخش هدر با دکمه Back و عنوان ---
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // دکمه Back
    QPushButton *backButton = new QPushButton("◀️");
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

    // عنوان وسط
    QLabel *title = new QLabel("Order History");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #3e2723; font-family: 'Segoe UI';");
    title->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(title);
    headerLayout->addStretch(); // برای تراز کردن عنوان در مرکز

    mainLayout->addLayout(headerLayout);

    // --- جعبه جستجو ---
    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Search by restaurant name...");
    searchBox->setStyleSheet(R"(
        QLineEdit {
            border: 2px solid #CEAB81;
            border-radius: 10px;
            padding: 8px;
            font-size: 14px;
            background-color: #fdf6e3;
            color: #3e2723;
        }
    )");
    mainLayout->addWidget(searchBox);

    // --- ناحیه اسکرول ---
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none;");

    scrollContent = new QWidget();
    scrollContent->setStyleSheet("background-color: #f6e5cb;");

    ordersLayout = new QVBoxLayout(scrollContent);
    ordersLayout->setSpacing(12);
    ordersLayout->setContentsMargins(10, 10, 10, 10);
    ordersLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // --- اتصال سیگنال‌ها ---
    connect(searchBox, &QLineEdit::textChanged, this, &History::filterOrders);

    // تغییر اتصال دکمه Back
    connect(backButton, &QPushButton::clicked, this, [this](){
        this->close(); // بستن پنجره فعلی
        RestaurantListWindow *restaurantWindow = new RestaurantListWindow();
        restaurantWindow->show();
    });
}

void History::addOrder(const QString &restaurantName, const QStringList &items, double totalPrice)
{
    OrderHistoryItem *item = new OrderHistoryItem(restaurantName, items, totalPrice);
    orderItems.append(item);
    ordersLayout->addWidget(item);
}

void History::filterOrders(const QString &text)
{
    for (OrderHistoryItem *item : orderItems) {
        bool match = item->getRestaurantName().contains(text, Qt::CaseInsensitive);
        item->setVisible(match);
    }
}

void History::addSampleOrders()
{
    addOrder("Italian Bistro", {"Pizza Margherita", "Garlic Bread"}, 24.50);
    addOrder("Sushi Master", {"California Roll", "Miso Soup", "Green Tea"}, 31.75);
    addOrder("Burger House", {"Double Cheeseburger", "Fries", "Coke"}, 18.20);
    addOrder("Curry Palace", {"Chicken Tikka Masala", "Naan", "Lassi"}, 27.90);
    addOrder("Green Garden", {"Avocado Salad", "Smoothie Bowl"}, 22.10);
}
