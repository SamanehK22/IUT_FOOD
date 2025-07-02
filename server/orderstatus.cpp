#include "orderstatus.h"
#include <QDebug>

OrderStatus* OrderStatus::instance = nullptr;

// Define static constants
const QString OrderStatus::STATUS_PENDING = "pending";
const QString OrderStatus::STATUS_CONFIRMED = "confirmed";
const QString OrderStatus::STATUS_PREPARING = "preparing";
const QString OrderStatus::STATUS_READY = "ready";
const QString OrderStatus::STATUS_OUT_FOR_DELIVERY = "out_for_delivery";
const QString OrderStatus::STATUS_DELIVERED = "delivered";
const QString OrderStatus::STATUS_CANCELLED = "cancelled";
const QString OrderStatus::STATUS_REJECTED = "rejected";

// Initialize status descriptions
QMap<QString, QString> OrderStatus::statusDescriptions = {
    {STATUS_PENDING, "Order received and waiting for confirmation"},
    {STATUS_CONFIRMED, "Order confirmed by restaurant"},
    {STATUS_PREPARING, "Order is being prepared"},
    {STATUS_READY, "Order is ready for pickup/delivery"},
    {STATUS_OUT_FOR_DELIVERY, "Order is out for delivery"},
    {STATUS_DELIVERED, "Order has been delivered"},
    {STATUS_CANCELLED, "Order has been cancelled"},
    {STATUS_REJECTED, "Order has been rejected by restaurant"}
};

OrderStatus* OrderStatus::getInstance()
{
    if (instance == nullptr) {
        instance = new OrderStatus();
    }
    return instance;
}

OrderStatus::OrderStatus(QObject *parent)
    : QObject(parent)
{
}

OrderStatus::~OrderStatus()
{
    observers.clear();
    orderStatuses.clear();
}

void OrderStatus::attach(Observer* observer)
{
    if (observer && !observers.contains(observer)) {
        observers.append(observer);
        qDebug() << "Observer attached to OrderStatus";
    }
}

void OrderStatus::detach(Observer* observer)
{
    if (observer) {
        observers.removeOne(observer);
        qDebug() << "Observer detached from OrderStatus";
    }
}

void OrderStatus::notifyObservers(const QString& orderId, const QString& status)
{
    // Validate status before updating
    if (!isValidStatus(status)) {
        qDebug() << "Invalid order status:" << status;
        return;
    }

    // Update the status in our map
    orderStatuses[orderId] = status;

    // Notify all observers
    for (Observer* observer : observers) {
        observer->update(orderId, status);
        emit observer->orderStatusChanged(orderId, status);
    }
    qDebug() << "Notified observers about order" << orderId << "status change to" << status;
}

QString OrderStatus::getOrderStatus(const QString& orderId) const
{
    return orderStatuses.value(orderId, STATUS_PENDING);
}

bool OrderStatus::isValidStatus(const QString& status)
{
    return statusDescriptions.contains(status);
}

QStringList OrderStatus::getValidStatuses()
{
    return statusDescriptions.keys();
}

QString OrderStatus::getStatusDescription(const QString& status)
{
    return statusDescriptions.value(status, "Unknown status");
} 