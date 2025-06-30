#include "menumanager.h"
#include "networkmanager.h"

MenuManager::MenuManager(QObject *parent)
    : QObject(parent), m_networkManager(new NetworkManager(this))
{
    connect(m_networkManager, &NetworkManager::menuCreated,
            this, &MenuManager::menuCreated);
    connect(m_networkManager, &NetworkManager::menuItemAdded,
            this, &MenuManager::menuItemAdded);
    connect(m_networkManager, &NetworkManager::menuItemUpdated,
            this, &MenuManager::menuItemUpdated);
    connect(m_networkManager, &NetworkManager::menuItemDeleted,
            this, &MenuManager::menuItemDeleted);
    connect(m_networkManager, &NetworkManager::menuReceived,
            this, &MenuManager::menuReceived);
}

MenuManager::~MenuManager()
{
    delete m_networkManager;
}

void MenuManager::createMenu(const QString& restaurantId)
{
    m_networkManager->createMenu(restaurantId);
}

void MenuManager::addMenuItem(const QString& menuId, const QString& name, const QString& description, double price, const QString& ingredients, const QString& category)
{
    m_networkManager->addMenuItem(menuId, name, description, price, ingredients, category);
}

void MenuManager::updateMenuItem(const QString& menuId, const QString& itemId, const QJsonObject& updates)
{
    m_networkManager->updateMenuItem(menuId, itemId, updates);
}

void MenuManager::deleteMenuItem(const QString& menuId, const QString& itemId)
{
    m_networkManager->deleteMenuItem(menuId, itemId);
}

void MenuManager::fetchMenu(const QString& restaurantId)
{
    m_networkManager->getMenu(restaurantId);
} 