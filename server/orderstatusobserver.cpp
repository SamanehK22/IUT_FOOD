#include "orderstatusobserver.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

OrderStatusObserver::OrderStatusObserver(QObject *parent)
    : Observer(parent)
{
}

OrderStatusObserver::~OrderStatusObserver()
{
    orderSubscriptions.clear();
}

void OrderStatusObserver::update(const QString& orderId, const QString& status)
{
    // Create notification message
    QJsonObject notification;
    notification["type"] = "order_status_update";
    notification["orderId"] = orderId;
    notification["status"] = status;

    QJsonDocument doc(notification);
    QByteArray message = doc.toJson();

    // Send notification to all subscribed clients
    if (orderSubscriptions.contains(orderId)) {
        for (QTcpSocket* client : orderSubscriptions[orderId]) {
            if (client && client->isValid()) {
                client->write(message);
                qDebug() << "Sent status update to client for order" << orderId;
            }
        }
    }
}

void OrderStatusObserver::addClientSubscription(const QString& orderId, QTcpSocket* client)
{
    if (!client) return;

    if (!orderSubscriptions.contains(orderId)) {
        orderSubscriptions[orderId] = QList<QTcpSocket*>();
    }

    if (!orderSubscriptions[orderId].contains(client)) {
        orderSubscriptions[orderId].append(client);
        qDebug() << "Client subscribed to order" << orderId;
    }
}

void OrderStatusObserver::removeClientSubscription(const QString& orderId, QTcpSocket* client)
{
    if (!client || !orderSubscriptions.contains(orderId)) return;

    orderSubscriptions[orderId].removeOne(client);
    if (orderSubscriptions[orderId].isEmpty()) {
        orderSubscriptions.remove(orderId);
    }
    qDebug() << "Client unsubscribed from order" << orderId;
}

void OrderStatusObserver::removeClientSubscriptions(QTcpSocket* client)
{
    if (!client) return;

    QList<QString> ordersToRemove;
    for (auto it = orderSubscriptions.begin(); it != orderSubscriptions.end(); ++it) {
        it.value().removeOne(client);
        if (it.value().isEmpty()) {
            ordersToRemove.append(it.key());
        }
    }

    for (const QString& orderId : ordersToRemove) {
        orderSubscriptions.remove(orderId);
    }
    qDebug() << "Removed all subscriptions for client";
} 