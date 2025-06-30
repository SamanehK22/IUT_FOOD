#include "restaurantownermanager.h"
#include <QDebug>

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
    , m_networkManager(NetworkManager::getInstance())
    , m_restaurant(nullptr)
{
    // Connect to NetworkManager signals
    connect(m_networkManager, &NetworkManager::restaurantInfoReceived,
            this, &RestaurantOwnerManager::handleRestaurantInfoReceived);
    connect(m_networkManager, &NetworkManager::menuReceived,
            this, &RestaurantOwnerManager::handleMenuReceived);
    connect(m_networkManager, &NetworkManager::ordersReceived,
            this, &RestaurantOwnerManager::handleOrdersReceived);
    connect(m_networkManager, &NetworkManager::updateResponse,
            this, &RestaurantOwnerManager::handleUpdateResponse);
    connect(m_networkManager, &NetworkManager::ownerProfileReceived,
            this, &RestaurantOwnerManager::profileReceived);
    connect(m_networkManager, &NetworkManager::ownerRestaurantsReceived,
            this, &RestaurantOwnerManager::restaurantsReceived);
}

RestaurantOwnerManager::~RestaurantOwnerManager()
{
    delete m_restaurant;
    qDeleteAll(m_menuItems);
    qDeleteAll(m_orders);
}

void RestaurantOwnerManager::getRestaurantInfo()
{
    m_networkManager->getRestaurantInfo();
}

void RestaurantOwnerManager::updateRestaurantInfo(const QJsonObject& info)
{
    QJsonObject request;
    request["type"] = "update_restaurant_info";
    request["info"] = info;
    m_networkManager->sendRequest(request);
}

void RestaurantOwnerManager::updateRestaurantStatus(bool isOpen)
{
    QJsonObject request;
    request["type"] = "update_restaurant_status";
    request["is_open"] = isOpen;
    m_networkManager->sendRequest(request);
}

Restaurant* RestaurantOwnerManager::getRestaurant() const
{
    return m_restaurant;
}

void RestaurantOwnerManager::getMenu()
{
    m_networkManager->getRestaurantMenu(m_restaurant->id());
}

void RestaurantOwnerManager::addMenuItem(const QJsonObject& item)
{
    QJsonObject request;
    request["type"] = "add_menu_item";
    request["item"] = item;
    m_networkManager->sendRequest(request);
}

void RestaurantOwnerManager::addMenuItem(const QString& name, const QString& description, double price, const QString& ingredients, const QString& category, const QString& imageUrl)
{
    // Get the menu ID from the restaurant
    if (!m_restaurant) {
        emit error("No restaurant loaded");
        return;
    }
    
    // For now, we'll use a placeholder menu ID - in a real implementation,
    // you'd need to get the actual menu ID from the restaurant
    QString menuId = "1"; // This should be the actual menu ID
    
    m_networkManager->addMenuItem(menuId, name, description, price, ingredients, category, imageUrl);
}

void RestaurantOwnerManager::updateMenuItem(const QString& itemId, const QJsonObject& item)
{
    QJsonObject request;
    request["type"] = "update_menu_item";
    request["item_id"] = itemId;
    request["item"] = item;
    m_networkManager->sendRequest(request);
}

void RestaurantOwnerManager::removeMenuItem(const QString& itemId)
{
    QJsonObject request;
    request["type"] = "remove_menu_item";
    request["item_id"] = itemId;
    m_networkManager->sendRequest(request);
}

QList<MenuItem*> RestaurantOwnerManager::getMenuItems() const
{
    return m_menuItems;
}

MenuItem* RestaurantOwnerManager::getMenuItem(const QString& itemId) const
{
    for (MenuItem* item : m_menuItems) {
        if (item->id() == itemId) {
            return item;
        }
    }
    return nullptr;
}

void RestaurantOwnerManager::getOrders()
{
    m_networkManager->getRestaurantOrders(m_restaurant->id());
}

void RestaurantOwnerManager::updateOrderStatus(const QString& orderId, const QString& status)
{
    QJsonObject request;
    request["type"] = "update_order_status";
    request["order_id"] = orderId;
    request["status"] = status;
    m_networkManager->sendRequest(request);
}

QList<Order*> RestaurantOwnerManager::getOrderList() const
{
    return m_orders;
}

Order* RestaurantOwnerManager::getOrder(const QString& orderId) const
{
    for (Order* order : m_orders) {
        if (order->id() == orderId) {
            return order;
        }
    }
    return nullptr;
}

void RestaurantOwnerManager::handleRestaurantInfoReceived(const QJsonObject& response)
{
    if (response.contains("error")) {
        emit error(response["error"].toString());
        return;
    }

    if (response.contains("restaurant")) {
        delete m_restaurant;
        m_restaurant = new Restaurant(response["restaurant"].toObject(), this);
        emit restaurantInfoReceived(response["restaurant"].toObject());
    }
}

void RestaurantOwnerManager::handleMenuReceived(const QJsonArray& menu)
{
    // Clean up existing menu items
    qDeleteAll(m_menuItems);
    m_menuItems.clear();

    // Create new menu item objects
    for (const QJsonValue& value : menu) {
        if (value.isObject()) {
            MenuItem* item = new MenuItem(value.toObject(), this);
            m_menuItems.append(item);
        }
    }

    emit menuReceived(m_menuItems);
}

void RestaurantOwnerManager::handleOrdersReceived(const QJsonArray& orders)
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

void RestaurantOwnerManager::handleUpdateResponse(const QJsonObject& response)
{
    if (response.contains("error")) {
        emit error(response["error"].toString());
        return;
    }

    emit updateSuccess();
}

void RestaurantOwnerManager::fetchProfile(const QString& ownerId)
{
    m_networkManager->getOwnerProfile(ownerId);
}

void RestaurantOwnerManager::fetchRestaurants(const QString& ownerId)
{
    m_networkManager->getOwnerRestaurants(ownerId);
}