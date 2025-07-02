#include "orderstatuswidget.h"
#include "restaurantlistwindow.h"

OrderStatusWidget::OrderStatusWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void OrderStatusWidget::setupUI()
{
    // تنظیمات پایه ویجت
    this->setFixedSize(650, 730);
    this->setStyleSheet("background-color: #EFE1C6;");
    this->setWindowTitle("Order Status");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // نوار بالایی با دکمه بازگشت
    QHBoxLayout *headerLayout = new QHBoxLayout();

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
    connect(backButton, &QPushButton::clicked, this, &OrderStatusWidget::onBackButtonClicked);

    headerLayout->addWidget(backButton);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // عنوان صفحه
    QLabel *titleLabel = new QLabel("Order Status");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(R"(
        font-size: 30px;
        font-weight: bold;
        color: #4B3621;
        font-family: 'Segoe UI';
        margin-bottom: 20px;
    )");
    mainLayout->addWidget(titleLabel);

    // ناحیه اسکرول برای لیست سفارشات
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: transparent; border: none;");

    QWidget *container = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(container);
    contentLayout->setSpacing(15);
    contentLayout->setContentsMargins(5, 5, 5, 5);

    QStringList restaurants = {
        "Italiano Restaurant",
        "Burger House",
        "Sushi World",
        "Traditional Food",
        "Vegeterian Cafe",
        "Sea Food House",
        "Pizza Palace"
    };

    QStringList statuses = {
        "Pending",
        "Confirmed",
        "Rejected",
        "Preparing",
        "Ready",
        "Out for Delivery",
        "Delivered"
    };

    QStringList prices = {"$25.99", "$18.50", "$32.75", "$15.25", "$28.90", "$22.40", "$19.99"};

    QMap<QString, QString> statusColors = {
        {"Pending", "#FFA000"},
        {"Confirmed", "#4CAF50"},
        {"Rejected", "#F44336"},
        {"Preparing", "#2196F3"},
        {"Ready", "#8BC34A"},
        {"Out for Delivery", "#3F51B5"},
        {"Delivered", "#009688"}
    };

    for (int i = 0; i < 7; i++) {
        QWidget *orderCard = new QWidget();
        orderCard->setStyleSheet(R"(
            background-color: #F7E9D3;
            border-radius: 16px;
            padding: 15px;
        )");

        QVBoxLayout *orderLayout = new QVBoxLayout(orderCard);
        orderLayout->setSpacing(10);

        QLabel *orderTitle = new QLabel(QString("Order #%1 - %2").arg(i+1).arg(restaurants[i]));
        orderTitle->setStyleSheet(R"(
            font-weight: bold;
            font-size: 16px;
            color: #4B3621;
            margin-bottom: 8px;
        )");

        QLabel *statusLabel = new QLabel("Status: " + statuses[i]);
        statusLabel->setStyleSheet(QString(R"(
            font-size: 14px;
            font-weight: bold;
            color: %1;
            margin-bottom: 5px;
        )").arg(statusColors[statuses[i]]));

        QLabel *priceLabel = new QLabel("Total: " + prices[i]);
        priceLabel->setStyleSheet(R"(
            font-size: 14px;
            font-weight: bold;
            color: #5C4B3B;
        )");

        QWidget *statusBar = new QWidget();
        statusBar->setFixedHeight(6);
        statusBar->setStyleSheet(QString(R"(
            background-color: %1;
            border-radius: 3px;
            margin-top: 5px;
        )").arg(statusColors[statuses[i]]));

        QPushButton *chatButton = new QPushButton("Chat With Restaurant");
        chatButton->setFixedHeight(30);
        chatButton->setStyleSheet(R"(
            QPushButton {
                background-color: #D7B899;
                color: #4B3621;
                border-radius: 10px;
                border: 2px solid #BCA38E;
                font: bold 13px "Segoe UI";
                padding: 4px 10px;
            }
            QPushButton:hover {
                background-color: #E8D3B5;
            }
        )");

        orderLayout->addWidget(orderTitle);
        orderLayout->addWidget(statusLabel);
        orderLayout->addWidget(priceLabel);
        orderLayout->addWidget(statusBar);
        orderLayout->addWidget(chatButton, 0, Qt::AlignRight);

        contentLayout->addWidget(orderCard);
    }

    scrollArea->setWidget(container);
    mainLayout->addWidget(scrollArea);
}

void OrderStatusWidget::onBackButtonClicked()
{
    this->close();
    RestaurantListWindow *restaurantWindow = new RestaurantListWindow();
    restaurantWindow->show();
}
