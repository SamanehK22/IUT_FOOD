#include "order.h"
#include <QDebug>

// Define static constants
const QString Order::STATUS_PENDING = "pending";
const QString Order::STATUS_CONFIRMED = "confirmed";
const QString Order::STATUS_PREPARING = "preparing";
const QString Order::STATUS_READY = "ready";
const QString Order::STATUS_OUT_FOR_DELIVERY = "out_for_delivery";
const QString Order::STATUS_DELIVERED = "delivered";
const QString Order::STATUS_CANCELLED = "cancelled";
const QString Order::STATUS_REJECTED = "rejected";

// Initialize static maps
QMap<QString, QString> Order::statusDescriptions = {
    {STATUS_PENDING, "Order received and waiting for confirmation"},
    {STATUS_CONFIRMED, "Order confirmed by restaurant"},
    {STATUS_PREPARING, "Order is being prepared"},
    {STATUS_READY, "Order is ready for pickup/delivery"},
    {STATUS_OUT_FOR_DELIVERY, "Order is out for delivery"},
    {STATUS_DELIVERED, "Order has been delivered"},
    {STATUS_CANCELLED, "Order has been cancelled"},
    {STATUS_REJECTED, "Order has been rejected by restaurant"}
};

QMap<QString, QString> Order::statusColors = {
    {STATUS_PENDING, "#FFA500"},      // Orange
    {STATUS_CONFIRMED, "#4169E1"},    // Royal Blue
    {STATUS_PREPARING, "#FFD700"},    // Gold
    {STATUS_READY, "#32CD32"},        // Lime Green
    {STATUS_OUT_FOR_DELIVERY, "#9370DB"}, // Medium Purple
    {STATUS_DELIVERED, "#228B22"},    // Forest Green
    {STATUS_CANCELLED, "#DC143C"},    // Crimson
    {STATUS_REJECTED, "#8B0000"}      // Dark Red
};

QMap<QString, QString> Order::statusIcons = {
    {STATUS_PENDING, "⏳"},
    {STATUS_CONFIRMED, "✅"},
    {STATUS_PREPARING, "👨‍🍳"},
    {STATUS_READY, "🍽️"},
    {STATUS_OUT_FOR_DELIVERY, "🚚"},
    {STATUS_DELIVERED, "🎉"},
    {STATUS_CANCELLED, "❌"},
    {STATUS_REJECTED, "🚫"}
};

QMap<QString, QStringList> Order::validTransitions = {
    {STATUS_PENDING, {STATUS_CONFIRMED, STATUS_CANCELLED, STATUS_REJECTED}},
    {STATUS_CONFIRMED, {STATUS_PREPARING, STATUS_CANCELLED}},
    {STATUS_PREPARING, {STATUS_READY, STATUS_CANCELLED}},
    {STATUS_READY, {STATUS_OUT_FOR_DELIVERY, STATUS_CANCELLED}},
    {STATUS_OUT_FOR_DELIVERY, {STATUS_DELIVERED}},
    {STATUS_DELIVERED, {}}, // Final state
    {STATUS_CANCELLED, {}}, // Final state
    {STATUS_REJECTED, {}}   // Final state
};

Order::Order(QObject *parent)
    : QObject(parent)
    , m_id("")
    , m_customerId("")
    , m_restaurantId("")
    , m_status("")
    , m_totalPrice(0.0)
    , m_orderTime(QDateTime::currentDateTime())
    , m_deliveryTime(QDateTime())
{
}

Order::Order(const QJsonObject &json, QObject *parent)
    : QObject(parent)
    , m_orderTime(QDateTime::currentDateTime())
    , m_deliveryTime(QDateTime())
{
    fromJson(json);
}

Order::~Order()
{
    qDeleteAll(m_items);
    m_items.clear();
}

QString Order::id() const
{
    return m_id;
}

QString Order::customerId() const
{
    return m_customerId;
}

QString Order::restaurantId() const
{
    return m_restaurantId;
}

QString Order::status() const
{
    return m_status;
}

double Order::totalPrice() const
{
    return m_totalPrice;
}

QDateTime Order::orderTime() const
{
    return m_orderTime;
}

QDateTime Order::deliveryTime() const
{
    return m_deliveryTime;
}

QList<MenuItem*> Order::items() const
{
    return m_items;
}

void Order::setId(const QString &id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

void Order::setCustomerId(const QString &customerId)
{
    if (m_customerId != customerId) {
        m_customerId = customerId;
        emit customerIdChanged();
    }
}

void Order::setRestaurantId(const QString &restaurantId)
{
    if (m_restaurantId != restaurantId) {
        m_restaurantId = restaurantId;
        emit restaurantIdChanged();
    }
}

void Order::setStatus(const QString &status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

void Order::setTotalPrice(double totalPrice)
{
    if (m_totalPrice != totalPrice) {
        m_totalPrice = totalPrice;
        emit totalPriceChanged();
    }
}

void Order::setOrderTime(const QDateTime &orderTime)
{
    if (m_orderTime != orderTime) {
        m_orderTime = orderTime;
        emit orderTimeChanged();
    }
}

void Order::setDeliveryTime(const QDateTime &deliveryTime)
{
    if (m_deliveryTime != deliveryTime) {
        m_deliveryTime = deliveryTime;
        emit deliveryTimeChanged();
    }
}

void Order::setItems(const QList<MenuItem*> &items)
{
    if (m_items != items) {
        qDeleteAll(m_items);
        m_items = items;
        emit itemsChanged();
    }
}

QJsonObject Order::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["customerId"] = m_customerId;
    json["restaurantId"] = m_restaurantId;
    json["status"] = m_status;
    json["totalPrice"] = m_totalPrice;
    json["orderTime"] = m_orderTime.toString(Qt::ISODate);
    if (m_deliveryTime.isValid()) {
        json["deliveryTime"] = m_deliveryTime.toString(Qt::ISODate);
    }

    QJsonArray itemsArray;
    for (const MenuItem* item : m_items) {
        itemsArray.append(item->toJson());
    }
    json["items"] = itemsArray;

    return json;
}

void Order::fromJson(const QJsonObject &json)
{
    if (json.contains("id"))
        setId(json["id"].toString());
    if (json.contains("customerId"))
        setCustomerId(json["customerId"].toString());
    if (json.contains("restaurantId"))
        setRestaurantId(json["restaurantId"].toString());
    if (json.contains("status"))
        setStatus(json["status"].toString());
    if (json.contains("totalPrice"))
        setTotalPrice(json["totalPrice"].toDouble());
    if (json.contains("orderTime"))
        setOrderTime(QDateTime::fromString(json["orderTime"].toString(), Qt::ISODate));
    if (json.contains("deliveryTime"))
        setDeliveryTime(QDateTime::fromString(json["deliveryTime"].toString(), Qt::ISODate));

    if (json.contains("items")) {
        QList<MenuItem*> items;
        QJsonArray itemsArray = json["items"].toArray();
        for (const QJsonValue &value : itemsArray) {
            if (value.isObject()) {
                items.append(new MenuItem(value.toObject(), this));
            }
        }
        setItems(items);
    }
}

Order* Order::fromJsonObject(const QJsonObject &json, QObject *parent)
{
    return new Order(json, parent);
}

QList<Order*> Order::fromJsonArray(const QJsonArray &array, QObject *parent)
{
    QList<Order*> orders;
    for (const QJsonValue &value : array) {
        if (value.isObject()) {
            orders.append(new Order(value.toObject(), parent));
        }
    }
    return orders;
}

QString Order::statusDescription() const
{
    return getStatusDescription(m_status);
}

QString Order::statusColor() const
{
    return getStatusColor(m_status);
}

QString Order::statusIcon() const
{
    return getStatusIcon(m_status);
}

bool Order::isValidStatus(const QString &status)
{
    return statusDescriptions.contains(status);
}

QStringList Order::getValidStatuses()
{
    return statusDescriptions.keys();
}

QString Order::getStatusDescription(const QString &status)
{
    return statusDescriptions.value(status, "Unknown status");
}

QString Order::getStatusColor(const QString &status)
{
    return statusColors.value(status, "#808080"); // Default gray
}

QString Order::getStatusIcon(const QString &status)
{
    return statusIcons.value(status, "❓"); // Default question mark
}

bool Order::canTransitionTo(const QString &newStatus) const
{
    if (!isValidStatus(newStatus)) {
        return false;
    }
    
    QStringList allowedTransitions = validTransitions.value(m_status, QStringList());
    return allowedTransitions.contains(newStatus);
}