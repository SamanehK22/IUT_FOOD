#ifndef CARTITEMWIDGET_H
#define CARTITEMWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

class CartItemWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CartItemWidget(const QString& restaurantName, const QString& items, double totalPrice, QWidget *parent = nullptr);

signals:
    void removeRequested(CartItemWidget* widget);

private slots:
    void handlePlaceOrder();
    void handleRemoveOrder();

private:
    QLabel* restaurantLabel;
    QLabel* itemsLabel;
    QLabel* totalPriceLabel;
    QLabel* paidLabel;
    QPushButton* placeOrderButton;
    QPushButton* removeOrderButton;
    bool isPaid = false;
};

#endif // CARTITEMWIDGET_H
