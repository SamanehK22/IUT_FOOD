#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QObject>
#include <QVariantMap>
#include <QJsonArray>
#include "databasemanager.h"

class OrderManager : public QObject
{
    Q_OBJECT
public:
    static OrderManager* getInstance();
    ~OrderManager();

    bool createOrder(const QString& customerId, const QString& restaurantId, const QVariantList& items);
    bool updateOrderStatus(const QString& orderId, const QString& status);
    QJsonArray getOrderHistory(const QString& userId, const QString& userType);
    // Add more order-related methods as needed

private:
    explicit OrderManager(QObject *parent = nullptr);
    static OrderManager* instance;
    DatabaseManager* m_dbManager;
};

#endif // ORDERMANAGER_H 