#include "ordermanager.h"
#include <QDebug>

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
    , m_networkManager(NetworkManager::getInstance())
{
    // Connect to NetworkManager signals
    connect(m_networkManager, &NetworkManager::orderStatusUpdated,
            this, &OrderManager::handleOrderStatusUpdate);
    connect(m_networkManager, &NetworkManager::orderPlaced,
            this, &OrderManager::handleOrderPlaced);
    connect(m_networkManager, &NetworkManager::orderCreated,
            this, &OrderManager::orderCreated);
    connect(m_networkManager, &NetworkManager::orderHistoryReceived,
            this, &OrderManager::orderHistoryReceived);
}

OrderManager::~OrderManager()
{
    // Clean up orders
    qDeleteAll(m_orders);
    m_orders.clear();

    // Clean up order history
    for (auto it = m_orderHistory.begin(); it != m_orderHistory.end(); ++it) {
        qDeleteAll(it.value());
    }
    m_orderHistory.clear();
}

void OrderManager::placeOrder(const QString &restaurantId, const QJsonArray &items)
{
    m_networkManager->placeOrder(restaurantId, items);
}

void OrderManager::getOrderStatus(const QString &orderId)
{
    m_networkManager->getOrderStatus(orderId);
}

void OrderManager::updateOrderStatus(const QString &orderId, const QString &status)
{
    // Validate status before sending to server
    if (!isValidStatus(status)) {
        emit statusUpdateFailed(orderId, "Invalid status: " + status);
        emit error("Invalid order status. Valid statuses: " + getValidStatuses().join(", "));
        return;
    }

    // Check if the order can transition to this status
    if (!canUpdateOrderStatus(orderId, status)) {
        emit statusUpdateFailed(orderId, "Invalid status transition");
        emit error("Cannot transition order to status: " + status);
        return;
    }

    m_networkManager->updateOrderStatus(orderId, status);
}

QList<Order*> OrderManager::getOrderHistory(const QString &userId)
{
    return m_orderHistory.value(userId);
}

Order* OrderManager::getOrder(const QString &orderId)
{
    return m_orders.value(orderId);
}

void OrderManager::handleOrderStatusUpdate(const QString &orderId, const QString &status)
{
    Order* order = m_orders.value(orderId);
    if (order) {
        order->setStatus(status);
        emit orderStatusUpdated(orderId, status);

        // Update order history
        QString userId = order->customerId();
        if (!m_orderHistory.contains(userId)) {
            m_orderHistory[userId] = QList<Order*>();
        }
        emit orderHistoryUpdated(userId);
    }
}

void OrderManager::handleOrderPlaced(const QString &orderId)
{
    // Create a new order object
    Order* order = new Order(this);
    order->setId(orderId);
    m_orders[orderId] = order;

    // Add to order history
    QString userId = order->customerId();
    if (!m_orderHistory.contains(userId)) {
        m_orderHistory[userId] = QList<Order*>();
    }
    m_orderHistory[userId].append(order);

    emit orderPlaced(orderId);
    emit orderHistoryUpdated(userId);
}

void OrderManager::createOrder(const QString& customerId, const QString& restaurantId, const QJsonArray& items)
{
    m_networkManager->createOrder(customerId, restaurantId, items);
}

void OrderManager::fetchOrderHistory(const QString& userId, const QString& userType)
{
    m_networkManager->getOrderHistory(userId, userType);
}

void OrderManager::orderHistoryReceived(const QJsonArray& orderHistory)
{
    // Convert JSON array to Order objects
    QList<Order*> orders;
    for (const QJsonValue& value : orderHistory) {
        if (value.isObject()) {
            Order* order = new Order(value.toObject(), this);
            orders.append(order);
            // Also add to the orders map
            m_orders[order->id()] = order;
        }
    }
    // Update order history for the current user
    // Note: We need to determine the userId from the orders or pass it as parameter
    // For now, we'll use the first order's customer_id if available
    QString userId;
    if (!orders.isEmpty()) {
        userId = orders.first()->customerId();
    }
    if (!userId.isEmpty()) {
        m_orderHistory[userId] = orders;
        emit orderHistoryUpdated(userId);
    }
}

bool OrderManager::isValidStatus(const QString &status) const
{
    return Order::isValidStatus(status);
}

QStringList OrderManager::getValidStatuses() const
{
    return Order::getValidStatuses();
}

QString OrderManager::getStatusDescription(const QString &status) const
{
    return Order::getStatusDescription(status);
}

bool OrderManager::canUpdateOrderStatus(const QString &orderId, const QString &newStatus) const
{
    Order* order = m_orders.value(orderId);
    if (!order) {
        return false; // Order not found
    }
    
    return order->canTransitionTo(newStatus);
}