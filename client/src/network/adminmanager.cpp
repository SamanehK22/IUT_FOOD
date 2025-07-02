#include "adminmanager.h"
#include <QDebug>

AdminManager* AdminManager::instance = nullptr;

AdminManager* AdminManager::getInstance()
{
    if (instance == nullptr) {
        instance = new AdminManager();
    }
    return instance;
}

AdminManager::AdminManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(NetworkManager::getInstance())
{
    // Connect to NetworkManager signals
    connect(m_networkManager, &NetworkManager::usersReceived,
            this, &AdminManager::handleUsersReceived);
    connect(m_networkManager, &NetworkManager::restaurantsReceived,
            this, &AdminManager::handleRestaurantsReceived);
    connect(m_networkManager, &NetworkManager::ordersReceived,
            this, &AdminManager::handleOrdersReceived);
    connect(m_networkManager, &NetworkManager::statisticsReceived,
            this, &AdminManager::handleStatisticsReceived);
}

AdminManager::~AdminManager()
{
    // Clean up users
    qDeleteAll(m_users);
    m_users.clear();

    // Clean up restaurants
    qDeleteAll(m_restaurants);
    m_restaurants.clear();

    // Clean up orders
    qDeleteAll(m_orders);
    m_orders.clear();
}

void AdminManager::getUsers()
{
    m_networkManager->getUsers();
}

void AdminManager::updateUserStatus(const QString& userId, bool isActive)
{
    QJsonObject request;
    request["type"] = "update_user_status";
    request["user_id"] = userId;
    request["is_active"] = isActive;
    m_networkManager->sendRequest(request);
}

QList<User*> AdminManager::getUserList() const
{
    return m_users;
}

User* AdminManager::getUser(const QString& userId) const
{
    for (User* user : m_users) {
        if (user->id() == userId) {
            return user;
        }
    }
    return nullptr;
}

void AdminManager::getRestaurants()
{
    m_networkManager->getRestaurants();
}

void AdminManager::updateRestaurantStatus(const QString& restaurantId, bool isApproved)
{
    QJsonObject request;
    request["type"] = "update_restaurant_status";
    request["restaurant_id"] = restaurantId;
    request["is_approved"] = isApproved;
    m_networkManager->sendRequest(request);
}

QList<Restaurant*> AdminManager::getRestaurantList() const
{
    return m_restaurants;
}

Restaurant* AdminManager::getRestaurant(const QString& restaurantId) const
{
    for (Restaurant* restaurant : m_restaurants) {
        if (restaurant->id() == restaurantId) {
            return restaurant;
        }
    }
    return nullptr;
}

void AdminManager::getOrders()
{
    m_networkManager->getOrders();
}

QList<Order*> AdminManager::getOrderList() const
{
    return m_orders;
}

Order* AdminManager::getOrder(const QString& orderId) const
{
    for (Order* order : m_orders) {
        if (order->id() == orderId) {
            return order;
        }
    }
    return nullptr;
}

void AdminManager::getStatistics()
{
    m_networkManager->getStatistics();
}

QJsonObject AdminManager::getCurrentStatistics() const
{
    return m_statistics;
}

void AdminManager::handleUsersReceived(const QJsonArray& users)
{
    // Clean up existing users
    qDeleteAll(m_users);
    m_users.clear();

    // Create new user objects
    for (const QJsonValue& value : users) {
        if (value.isObject()) {
            User* user = new User(value.toObject(), this);
            m_users.append(user);
        }
    }

    emit usersReceived(m_users);
}

void AdminManager::handleRestaurantsReceived(const QJsonArray& restaurants)
{
    // Clean up existing restaurants
    qDeleteAll(m_restaurants);
    m_restaurants.clear();

    // Create new restaurant objects
    for (const QJsonValue& value : restaurants) {
        if (value.isObject()) {
            Restaurant* restaurant = new Restaurant(value.toObject(), this);
            m_restaurants.append(restaurant);
        }
    }

    emit restaurantsReceived(m_restaurants);
}

void AdminManager::handleOrdersReceived(const QJsonArray& orders)
{
    // Clean up existing orders
    qDeleteAll(m_orders);
    m_orders.clear();

    // Create new order objects
    for (const QJsonValue& value : orders) {
        if (value.isObject()) {
            Order* order = new Order(value.toObject(), this);
            m_orders.append(order);
        }
    }

    emit ordersReceived(m_orders);
}

void AdminManager::handleStatisticsReceived(const QJsonObject& stats)
{
    m_statistics = stats;
    emit statisticsReceived(stats);
} 