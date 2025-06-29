#include "customermanager.h"

CustomerManager* CustomerManager::instance = nullptr;

CustomerManager* CustomerManager::getInstance()
{
    if (instance == nullptr) {
        instance = new CustomerManager();
    }
    return instance;
}

CustomerManager::CustomerManager(QObject *parent)
    : QObject(parent)
    , m_dbManager(DatabaseManager::getInstance())
{
}

CustomerManager::~CustomerManager() {}

QVariantMap CustomerManager::getProfile(const QString& customerId)
{
    return m_dbManager->getCustomerProfile(customerId);
}

QJsonArray CustomerManager::getOrderHistory(const QString& customerId)
{
    return m_dbManager->getCustomerOrderHistory(customerId);
} 