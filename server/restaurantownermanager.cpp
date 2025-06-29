#include "restaurantownermanager.h"

RestaurantOwnerManager* RestaurantOwnerManager::instance = nullptr;

RestaurantOwnerManager* RestaurantOwnerManager::getInstance()
{
    if (instance == nullptr) {
        instance = new RestaurantOwnerManager();
    }
    return instance;
}

RestaurantOwnerManager::RestaurantOwnerManager(QObject *parent)
    : QObject(parent)
    , m_dbManager(DatabaseManager::getInstance())
{
}

RestaurantOwnerManager::~RestaurantOwnerManager() {}

QVariantMap RestaurantOwnerManager::getProfile(const QString& ownerId)
{
    return m_dbManager->getRestaurantOwnerProfile(ownerId);
}

QJsonArray RestaurantOwnerManager::getRestaurants(const QString& ownerId)
{
    return m_dbManager->getRestaurantsByOwner(ownerId);
} 