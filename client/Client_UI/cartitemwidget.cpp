#include "cartitemwidget.h"
#include <QMessageBox>

CartItemWidget::CartItemWidget(const QString& restaurantName, const QString& items, double totalPrice, QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet(R"(
        QFrame {
            background-color: #FDF3E7;
            border: 2px solid #E6CFC1;
            border-radius: 16px;
            padding: 14px;
        }
        QLabel {
            font-family: 'Segoe UI';
            font-size: 15px;
        }
        QLabel#itemsLabel, QLabel#priceLabel {
            font-weight: bold;
        }
        QPushButton {
            border-radius: 12px;
            padding: 8px 20px;
            font-weight: bold;
            font-family: 'Segoe UI';
        }
        QPushButton#place {
            background-color: #4CAF50;
            color: white;
        }
        QPushButton#place:hover {
            background-color: #45A049;
        }
        QPushButton#remove {
            background-color: #F44336;
            color: white;
        }
        QPushButton#remove:hover {
            background-color: #D32F2F;
        }
    )");

    restaurantLabel = new QLabel("<b>Restaurant: " + restaurantName + "</b>");

    itemsLabel = new QLabel("Items: " + items);
    itemsLabel->setObjectName("itemsLabel");

    totalPriceLabel = new QLabel("Total Price: $" + QString::number(totalPrice, 'f', 2));
    totalPriceLabel->setObjectName("priceLabel");

    paidLabel = new QLabel();
    paidLabel->setStyleSheet("color: #2E8B57; font-weight: bold; font-size: 14px;");
    paidLabel->hide();

    placeOrderButton = new QPushButton("Place Order ✓");
    placeOrderButton->setObjectName("place");

    removeOrderButton = new QPushButton("Remove Order ✖");
    removeOrderButton->setObjectName("remove");

    connect(placeOrderButton, &QPushButton::clicked, this, &CartItemWidget::handlePlaceOrder);
    connect(removeOrderButton, &QPushButton::clicked, this, &CartItemWidget::handleRemoveOrder);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(restaurantLabel);
    layout->addWidget(itemsLabel);
    layout->addWidget(totalPriceLabel);
    layout->addWidget(paidLabel);

    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(placeOrderButton);
    buttonLayout->addWidget(removeOrderButton);
    layout->addLayout(buttonLayout);
}

void CartItemWidget::handlePlaceOrder() {
    if (isPaid) return;

    QMessageBox msgBox;
    msgBox.setWindowTitle("Order Placed!");
    msgBox.setText("Your order has been placed successfully!🎉");
    msgBox.setStyleSheet(R"(
        QMessageBox {
            background-color: #FFF7ED;
            font-family: 'Segoe UI';
            font-size: 16px;
        }
        QLabel {
            color: #5C4033;
        }
        QPushButton {
            background-color: #D2A679;
            color: white;
            border-radius: 10px;
            padding: 6px 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #c2916a;
        }
    )");
    msgBox.exec();

    isPaid = true;
    paidLabel->setText("PAID SUCCESSFULLY");
    paidLabel->show();

    placeOrderButton->setEnabled(false);
    removeOrderButton->setEnabled(false);
}

void CartItemWidget::handleRemoveOrder() {
    QMessageBox::information(this, "Removed", "Order has been removed from the cart❌");
    emit removeRequested(this);
}
