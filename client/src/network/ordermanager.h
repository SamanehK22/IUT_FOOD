#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include "../models/order.h"
#include "networkmanager.h"

class OrderManager : public QObject
{
    Q_OBJECT

public:
    static OrderManager* getInstance();
    void placeOrder(const QString &restaurantId, const QJsonArray &items);
    void getOrderStatus(const QString &orderId);
    void updateOrderStatus(const QString &orderId, const QString &status);
    QList<Order*> getOrderHistory(const QString &userId);
    Order* getOrder(const QString &orderId);

    // Status validation methods
    bool isValidStatus(const QString &status) const;
    QStringList getValidStatuses() const;
    QString getStatusDescription(const QString &status) const;
    bool canUpdateOrderStatus(const QString &orderId, const QString &newStatus) const;

public slots:
    void createOrder(const QString& customerId, const QString& restaurantId, const QJsonArray& items);
    void fetchOrderHistory(const QString& userId, const QString& userType);

signals:
    void orderPlaced(const QString &orderId);
    void orderStatusUpdated(const QString &orderId, const QString &status);
    void orderHistoryUpdated(const QString &userId);
    void error(const QString &message);
    void orderCreated(bool success);
    void statusUpdateFailed(const QString &orderId, const QString &reason);

private slots:
    void orderHistoryReceived(const QJsonArray& orderHistory);

private:
    explicit OrderManager(QObject *parent = nullptr);
    ~OrderManager();
    static OrderManager* instance;

    NetworkManager* m_networkManager;
    QMap<QString, Order*> m_orders; // orderId -> Order
    QMap<QString, QList<Order*>> m_orderHistory; // userId -> orders

    void handleOrderStatusUpdate(const QString &orderId, const QString &status);
    void handleOrderPlaced(const QString &orderId);
};

#endif // ORDERMANAGER_H 