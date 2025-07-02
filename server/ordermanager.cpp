#include "ordermanager.h"

OrderManager* OrderManager::instance = nullptr;

OrderManager* OrderManager::getInstance()
{
    if (instance == nullptr) {
        instance = new OrderManager();
    }
    return instance;
}

OrderManager::OrderManager(QObject *parent)
    : QObject(parent)
    , m_dbManager(DatabaseManager::getInstance())
{
}

OrderManager::~OrderManager() {}

bool OrderManager::createOrder(const QString& customerId, const QString& restaurantId, const QVariantList& items)
{
    return m_dbManager->createOrder(customerId, restaurantId, items);
}

bool OrderManager::updateOrderStatus(const QString& orderId, const QString& status)
{
    return m_dbManager->updateOrderStatus(orderId, status);
}

QJsonArray OrderManager::getOrderHistory(const QString& userId, const QString& userType)
{
    return m_dbManager->getOrderHistory(userId, userType);
} 