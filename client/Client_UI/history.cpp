#include "history.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSpacerItem>

History::History(QWidget *parent)
    : QWidget(parent)
{
    this->setStyleSheet("background-color: #EFE1C6;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Order History");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #3e2723; font-family: 'Segoe UI';");
    title->setAlignment(Qt::AlignCenter);

    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Search by restaurant name...");
    searchBox->setStyleSheet(
        "QLineEdit {"
        "  border: 2px solid #CEAB81;"
        "  border-radius: 10px;"
        "  padding: 8px;"
        "  font-size: 14px;"
        "  background-color: #fdf6e3;"
        "  color: #3e2723;"
        "}"
    );

    connect(searchBox, &QLineEdit::textChanged, this, &History::filterOrders);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none;");

    scrollContent = new QWidget();
    scrollContent->setStyleSheet("background-color: #f6e5cb;"); // ← این خط جدید
    ordersLayout = new QVBoxLayout(scrollContent);
    ordersLayout->setSpacing(12);
    ordersLayout->setContentsMargins(10, 10, 10, 10);

    scrollArea->setWidget(scrollContent);

    mainLayout->addWidget(title);
    mainLayout->addWidget(searchBox);
    mainLayout->addWidget(scrollArea);
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
