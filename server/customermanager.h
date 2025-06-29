#ifndef CUSTOMERMANAGER_H
#define CUSTOMERMANAGER_H

#include <QObject>
#include <QVariantMap>
#include <QJsonArray>
#include "databasemanager.h"

class CustomerManager : public QObject
{
    Q_OBJECT
public:
    static CustomerManager* getInstance();
    ~CustomerManager();

    QVariantMap getProfile(const QString& customerId);
    QJsonArray getOrderHistory(const QString& customerId);
    // Add more customer-specific methods as needed

private:
    explicit CustomerManager(QObject *parent = nullptr);
    static CustomerManager* instance;
    DatabaseManager* m_dbManager;
};

#endif // CUSTOMERMANAGER_H 