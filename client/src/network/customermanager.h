#ifndef CUSTOMERMANAGER_H
#define CUSTOMERMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include "networkmanager.h"
#include "../models/restaurant.h"
#include "../models/order.h"
#include "../models/menuitem.h"

class CustomerManager : public QObject
{
    Q_OBJECT

public:
    static CustomerManager* getInstance();
    ~CustomerManager();

    // Restaurant browsing
    void getRestaurants();
    void getRestaurantMenu(const QString& restaurantId);
    QList<Restaurant*> getRestaurantList() const;
    Restaurant* getRestaurant(const QString& restaurantId) const;
    QList<MenuItem*> getMenuItems(const QString& restaurantId) const;

    // Order management
    void placeOrder(const QString& restaurantId, const QList<MenuItem*>& items);
    void getOrders();
    void cancelOrder(const QString& orderId);
    QList<Order*> getOrderList() const;
    Order* getOrder(const QString& orderId) const;

    // Cart management
    void addToCart(const QString& restaurantId, MenuItem* item, int quantity);
    void removeFromCart(const QString& restaurantId, MenuItem* item);
    void clearCart(const QString& restaurantId);
    QList<MenuItem*> getCartItems(const QString& restaurantId) const;
    int getCartItemQuantity(const QString& restaurantId, MenuItem* item) const;

    // Profile and order history
    void fetchProfile(const QString& customerId);
    void fetchOrderHistory(const QString& customerId);

signals:
    void restaurantsReceived(const QList<Restaurant*>& restaurants);
    void menuReceived(const QString& restaurantId, const QList<MenuItem*>& menu);
    void ordersReceived(const QList<Order*>& orders);
    void orderPlaced(const QString& orderId);
    void orderCancelled(const QString& orderId);
    void error(const QString& message);
    void profileReceived(const QJsonObject& profile);

private slots:
    void orderHistoryReceived(const QJsonArray& orderHistory);

private:
    explicit CustomerManager(QObject *parent = nullptr);
    static CustomerManager* instance;

    NetworkManager* m_networkManager;
    QList<Restaurant*> m_restaurants;
    QMap<QString, QList<MenuItem*>> m_menus; // restaurantId -> menu items
    QList<Order*> m_orders;
    QMap<QString, QMap<MenuItem*, int>> m_cart; // restaurantId -> (MenuItem -> quantity)

    void handleRestaurantsReceived(const QJsonArray& restaurants);
    void handleMenuReceived(const QString& restaurantId, const QJsonArray& menu);
    void handleOrdersReceived(const QJsonArray& orders);
    void handleOrderPlaced(const QJsonObject& response);
    void handleOrderCancelled(const QJsonObject& response);
};

#endif // CUSTOMERMANAGER_H 