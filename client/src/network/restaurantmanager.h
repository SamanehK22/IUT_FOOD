#ifndef RESTAURANTMANAGER_H
#define RESTAURANTMANAGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include "../models/restaurant.h"
#include "../models/menuitem.h"
#include "networkmanager.h"

class RestaurantManager : public QObject
{
    Q_OBJECT

public:
    static RestaurantManager* getInstance();
    void getRestaurants();
    void getRestaurantMenu(const QString &restaurantId);
    QList<Restaurant*> getRestaurantList() const;
    Restaurant* getRestaurant(const QString &restaurantId) const;
    QList<MenuItem*> getMenuItems(const QString &restaurantId) const;

signals:
    void restaurantsReceived(const QList<Restaurant*>& restaurants);
    void menuReceived(const QString &restaurantId, const QList<MenuItem*>& menu);
    void error(const QString &message);

private:
    explicit RestaurantManager(QObject *parent = nullptr);
    ~RestaurantManager();
    static RestaurantManager* instance;

    NetworkManager* m_networkManager;
    QList<Restaurant*> m_restaurants;
    QMap<QString, QList<MenuItem*>> m_menus; // restaurantId -> menu items

    void handleRestaurantsReceived(const QJsonArray &restaurants);
    void handleMenuReceived(const QString &restaurantId, const QJsonArray &menu);
};

#endif // RESTAURANTMANAGER_H 