#ifndef RESTAURANTOWNERMANAGER_H
#define RESTAURANTOWNERMANAGER_H

#include <QObject>
#include <QVariantMap>
#include <QJsonArray>
#include "databasemanager.h"

class RestaurantOwnerManager : public QObject
{
    Q_OBJECT
public:
    static RestaurantOwnerManager* getInstance();
    ~RestaurantOwnerManager();

    QVariantMap getProfile(const QString& ownerId);
    QJsonArray getRestaurants(const QString& ownerId);
    // Add more owner-specific methods as needed

private:
    explicit RestaurantOwnerManager(QObject *parent = nullptr);
    static RestaurantOwnerManager* instance;
    DatabaseManager* m_dbManager;
};

#endif // RESTAURANTOWNERMANAGER_H 