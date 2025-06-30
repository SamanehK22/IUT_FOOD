#include "customermanager.h"
#include <QDebug>

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
    , m_networkManager(NetworkManager::getInstance())
{
    // Connect to NetworkManager signals
    connect(m_networkManager, &NetworkManager::restaurantsReceived,
            this, &CustomerManager::handleRestaurantsReceived);
    connect(m_networkManager, &NetworkManager::menuReceived,
            this, &CustomerManager::handleMenuReceived);
    connect(m_networkManager, &NetworkManager::ordersReceived,
            this, &CustomerManager::handleOrdersReceived);
    connect(m_networkManager, &NetworkManager::orderPlaced,
            this, &CustomerManager::handleOrderPlaced);
    connect(m_networkManager, &NetworkManager::orderCancelled,
            this, &CustomerManager::handleOrderCancelled);
    connect(m_networkManager, &NetworkManager::customerProfileReceived,
            this, &CustomerManager::profileReceived);
    connect(m_networkManager, &NetworkManager::customerOrderHistoryReceived,
            this, &CustomerManager::orderHistoryReceived);
}

CustomerManager::~CustomerManager()
{
    // Clean up restaurants
    qDeleteAll(m_restaurants);
    m_restaurants.clear();

    // Clean up menus
    for (auto it = m_menus.begin(); it != m_menus.end(); ++it) {
        qDeleteAll(it.value());
    }
    m_menus.clear();

    // Clean up orders
    qDeleteAll(m_orders);
    m_orders.clear();

    // Clean up cart
    m_cart.clear();
}

void CustomerManager::getRestaurants()
{
    m_networkManager->getRestaurants();
}

void CustomerManager::getRestaurantMenu(const QString& restaurantId)
{
    m_networkManager->getRestaurantMenu(restaurantId);
}

QList<Restaurant*> CustomerManager::getRestaurantList() const
{
    return m_restaurants;
}

Restaurant* CustomerManager::getRestaurant(const QString& restaurantId) const
{
    for (Restaurant* restaurant : m_restaurants) {
        if (restaurant->id() == restaurantId) {
            return restaurant;
        }
    }
    return nullptr;
}

QList<MenuItem*> CustomerManager::getMenuItems(const QString& restaurantId) const
{
    return m_menus.value(restaurantId);
}

void CustomerManager::placeOrder(const QString& restaurantId, const QList<MenuItem*>& items)
{
    QJsonObject request;
    request["type"] = "place_order";
    request["restaurant_id"] = restaurantId;

    QJsonArray itemsArray;
    for (MenuItem* item : items) {
        QJsonObject itemObj;
        itemObj["id"] = item->id();
        itemObj["quantity"] = m_cart.value(restaurantId).value(item, 0);
        itemsArray.append(itemObj);
    }
    request["items"] = itemsArray;

    m_networkManager->sendRequest(request);
}

void CustomerManager::getOrders()
{
    m_networkManager->getOrders();
}

void CustomerManager::cancelOrder(const QString& orderId)
{
    QJsonObject request;
    request["type"] = "cancel_order";
    request["order_id"] = orderId;
    m_networkManager->sendRequest(request);
}

QList<Order*> CustomerManager::getOrderList() const
{
    return m_orders;
}

Order* CustomerManager::getOrder(const QString& orderId) const
{
    for (Order* order : m_orders) {
        if (order->id() == orderId) {
            return order;
        }
    }
    return nullptr;
}

void CustomerManager::addToCart(const QString& restaurantId, MenuItem* item, int quantity)
{
    if (!m_cart.contains(restaurantId)) {
        m_cart[restaurantId] = QMap<MenuItem*, int>();
    }
    m_cart[restaurantId][item] = quantity;
}

void CustomerManager::removeFromCart(const QString& restaurantId, MenuItem* item)
{
    if (m_cart.contains(restaurantId)) {
        m_cart[restaurantId].remove(item);
    }
}

void CustomerManager::clearCart(const QString& restaurantId)
{
    m_cart.remove(restaurantId);
}

QList<MenuItem*> CustomerManager::getCartItems(const QString& restaurantId) const
{
    return m_cart.value(restaurantId).keys();
}

int CustomerManager::getCartItemQuantity(const QString& restaurantId, MenuItem* item) const
{
    return m_cart.value(restaurantId).value(item, 0);
}

void CustomerManager::handleRestaurantsReceived(const QJsonArray& restaurants)
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

void CustomerManager::handleMenuReceived(const QString& restaurantId, const QJsonArray& menu)
{
    // Clean up existing menu items
    if (m_menus.contains(restaurantId)) {
        qDeleteAll(m_menus[restaurantId]);
        m_menus[restaurantId].clear();
    }

    // Create new menu item objects
    QList<MenuItem*> menuItems;
    for (const QJsonValue& value : menu) {
        if (value.isObject()) {
            MenuItem* item = new MenuItem(value.toObject(), this);
            menuItems.append(item);
        }
    }

    m_menus[restaurantId] = menuItems;
    emit menuReceived(restaurantId, menuItems);
}

void CustomerManager::handleOrdersReceived(const QJsonArray& orders)
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

void CustomerManager::handleOrderPlaced(const QJsonObject& response)
{
    if (response.contains("error")) {
        emit error(response["error"].toString());
        return;
    }

    if (response.contains("order_id")) {
        emit orderPlaced(response["order_id"].toString());
    }
}

void CustomerManager::handleOrderCancelled(const QJsonObject& response)
{
    if (response.contains("error")) {
        emit error(response["error"].toString());
        return;
    }

    if (response.contains("order_id")) {
        emit orderCancelled(response["order_id"].toString());
    }
}

void CustomerManager::fetchProfile(const QString& customerId)
{
    m_networkManager->getCustomerProfile(customerId);
}

void CustomerManager::fetchOrderHistory(const QString& customerId)
{
    m_networkManager->getCustomerOrderHistory(customerId);
}

void CustomerManager::orderHistoryReceived(const QJsonArray& orderHistory)
{
    // Clean up existing orders
    qDeleteAll(m_orders);
    m_orders.clear();

    // Create new order objects from the order history
    for (const QJsonValue& value : orderHistory) {
        if (value.isObject()) {
            Order* order = new Order(value.toObject(), this);
            m_orders.append(order);
        }
    }

    emit orderHistoryReceived(orderHistory);
}