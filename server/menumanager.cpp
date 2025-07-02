#include "menumanager.h"

MenuManager* MenuManager::instance = nullptr;

MenuManager* MenuManager::getInstance()
{
    if (instance == nullptr) {
        instance = new MenuManager();
    }
    return instance;
}

MenuManager::MenuManager(QObject *parent)
    : QObject(parent)
    , m_dbManager(DatabaseManager::getInstance())
{
}

MenuManager::~MenuManager() {}

bool MenuManager::createMenu(const QString& restaurantId)
{
    return m_dbManager->createMenu(restaurantId);
}

bool MenuManager::addMenuItem(const QString& menuId, const QString& name, const QString& description, double price, const QString& ingredients, const QString& category, const QString& imageUrl)
{
    return m_dbManager->addMenuItem(menuId, name, description, price, ingredients, category, imageUrl);
}

bool MenuManager::updateMenuItem(const QString& menuId, const QString& itemId, const QVariantMap& updates)
{
    return m_dbManager->updateMenuItem(menuId, itemId, updates);
}

bool MenuManager::deleteMenuItem(const QString& menuId, const QString& itemId)
{
    return m_dbManager->deleteMenuItem(menuId, itemId);
}

QJsonArray MenuManager::getMenu(const QString& restaurantId)
{
    return m_dbManager->getMenuItems(restaurantId);
} 