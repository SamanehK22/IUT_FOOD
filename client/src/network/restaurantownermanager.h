#ifndef RESTAURANTOWNERMANAGER_H
#define RESTAURANTOWNERMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include "networkmanager.h"
#include "../models/restaurant.h"
#include "../models/order.h"
#include "../models/menuitem.h"

class RestaurantOwnerManager : public QObject
{
    Q_OBJECT

public:
    static RestaurantOwnerManager* getInstance();
    ~RestaurantOwnerManager();

    // Restaurant management
    void getRestaurantInfo();
    void updateRestaurantInfo(const QJsonObject& info);
    void updateRestaurantStatus(bool isOpen);
    Restaurant* getRestaurant() const;

    // Menu management
    void getMenu();
    void addMenuItem(const QJsonObject& item);
    void addMenuItem(const QString& name, const QString& description, double price, const QString& ingredients, const QString& category, const QString& imageUrl = "");
    void updateMenuItem(const QString& itemId, const QJsonObject& item);
    void removeMenuItem(const QString& itemId);
    QList<MenuItem*> getMenuItems() const;
    MenuItem* getMenuItem(const QString& itemId) const;

    // Order management
    void getOrders();
    void updateOrderStatus(const QString& orderId, const QString& status);
    QList<Order*> getOrderList() const;
    Order* getOrder(const QString& orderId) const;

    // New slots and signals
    void fetchProfile(const QString& ownerId);
    void fetchRestaurants(const QString& ownerId);

signals:
    void restaurantInfoReceived(const QJsonObject& info);
    void menuReceived(const QList<MenuItem*>& menu);
    void ordersReceived(const QList<Order*>& orders);
    void updateSuccess();
    void error(const QString& message);
    void profileReceived(const QJsonObject& profile);
    void restaurantsReceived(const QJsonArray& restaurants);

private:
    explicit RestaurantOwnerManager(QObject *parent = nullptr);
    static RestaurantOwnerManager* instance;

    NetworkManager* m_networkManager;
    Restaurant* m_restaurant;
    QList<MenuItem*> m_menuItems;
    QList<Order*> m_orders;

    void handleRestaurantInfoReceived(const QJsonObject& response);
    void handleMenuReceived(const QJsonArray& menu);
    void handleOrdersReceived(const QJsonArray& orders);
    void handleUpdateResponse(const QJsonObject& response);
};

#endif // RESTAURANTOWNERMANAGER_H 