#include "orderstatus.h"
#include <QDebug>

OrderStatus* OrderStatus::instance = nullptr;

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
    return orderStatuses.value(orderId, "unknown");
} 