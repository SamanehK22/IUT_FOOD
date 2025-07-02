#include "restaurantmanager.h"
#include <QDebug>

RestaurantManager* RestaurantManager::instance = nullptr;

RestaurantManager* RestaurantManager::getInstance()
{
    if (instance == nullptr) {
        instance = new RestaurantManager();
    }
    return instance;
}

RestaurantManager::RestaurantManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(NetworkManager::getInstance())
{
    // Connect to NetworkManager signals
    connect(m_networkManager, &NetworkManager::restaurantsReceived,
            this, &RestaurantManager::handleRestaurantsReceived);
    connect(m_networkManager, &NetworkManager::menuReceived,
            this, &RestaurantManager::handleMenuReceived);
}

RestaurantManager::~RestaurantManager()
{
    // Clean up restaurants
    qDeleteAll(m_restaurants);
    m_restaurants.clear();

    // Clean up menus
    for (auto it = m_menus.begin(); it != m_menus.end(); ++it) {
        qDeleteAll(it.value());
    }
    m_menus.clear();
}

void RestaurantManager::getRestaurants()
{
    m_networkManager->getRestaurants();
}

void RestaurantManager::getRestaurantMenu(const QString &restaurantId)
{
    m_networkManager->getRestaurantMenu(restaurantId);
}

QList<Restaurant*> RestaurantManager::getRestaurantList() const
{
    return m_restaurants;
}

Restaurant* RestaurantManager::getRestaurant(const QString &restaurantId) const
{
    for (Restaurant* restaurant : m_restaurants) {
        if (restaurant->id() == restaurantId) {
            return restaurant;
        }
    }
    return nullptr;
}

QList<MenuItem*> RestaurantManager::getMenuItems(const QString &restaurantId) const
{
    return m_menus.value(restaurantId);
}

void RestaurantManager::handleRestaurantsReceived(const QJsonArray &restaurants)
{
    // Clean up existing restaurants
    qDeleteAll(m_restaurants);
    m_restaurants.clear();

    // Create new restaurant objects
    for (const QJsonValue &value : restaurants) {
        if (value.isObject()) {
            Restaurant* restaurant = new Restaurant(value.toObject(), this);
            m_restaurants.append(restaurant);
        }
    }

    emit restaurantsReceived(m_restaurants);
}

void RestaurantManager::handleMenuReceived(const QString &restaurantId, const QJsonArray &menu)
{
    // Clean up existing menu items
    if (m_menus.contains(restaurantId)) {
        qDeleteAll(m_menus[restaurantId]);
        m_menus[restaurantId].clear();
    }

    // Create new menu item objects
    QList<MenuItem*> menuItems;
    for (const QJsonValue &value : menu) {
        if (value.isObject()) {
            MenuItem* item = new MenuItem(value.toObject(), this);
            menuItems.append(item);
        }
    }

    m_menus[restaurantId] = menuItems;
    emit menuReceived(restaurantId, menuItems);
} 