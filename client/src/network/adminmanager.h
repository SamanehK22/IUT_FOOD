 #ifndef ADMINMANAGER_H
#define ADMINMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include "networkmanager.h"
#include "../models/user.h"
#include "../models/restaurant.h"
#include "../models/order.h"

class AdminManager : public QObject
{
    Q_OBJECT

public:
    static AdminManager* getInstance();
    ~AdminManager();

    // User management
    void getUsers();
    void updateUserStatus(const QString& userId, bool isActive);
    QList<User*> getUserList() const;
    User* getUser(const QString& userId) const;

    // Restaurant management
    void getRestaurants();
    void updateRestaurantStatus(const QString& restaurantId, bool isApproved);
    QList<Restaurant*> getRestaurantList() const;
    Restaurant* getRestaurant(const QString& restaurantId) const;

    // Order management
    void getOrders();
    QList<Order*> getOrderList() const;
    Order* getOrder(const QString& orderId) const;

    // Statistics
    void getStatistics();
    QJsonObject getCurrentStatistics() const;

signals:
    void usersReceived(const QList<User*>& users);
    void restaurantsReceived(const QList<Restaurant*>& restaurants);
    void ordersReceived(const QList<Order*>& orders);
    void statisticsReceived(const QJsonObject& stats);
    void error(const QString& message);

private:
    explicit AdminManager(QObject *parent = nullptr);
    static AdminManager* instance;

    NetworkManager* m_networkManager;
    QList<User*> m_users;
    QList<Restaurant*> m_restaurants;
    QList<Order*> m_orders;
    QJsonObject m_statistics;

    void handleUsersReceived(const QJsonArray& users);
    void handleRestaurantsReceived(const QJsonArray& restaurants);
    void handleOrdersReceived(const QJsonArray& orders);
    void handleStatisticsReceived(const QJsonObject& stats);
};

#endif // ADMINMANAGER_H