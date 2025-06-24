#ifndef ORDERSTATUSOBSERVER_H
#define ORDERSTATUSOBSERVER_H

#include "observer.h"
#include <QTcpSocket>
#include <QMap>

class OrderStatusObserver : public Observer
{
    Q_OBJECT

public:
    explicit OrderStatusObserver(QObject *parent = nullptr);
    ~OrderStatusObserver();

    void update(const QString& orderId, const QString& status) override;
    void addClientSubscription(const QString& orderId, QTcpSocket* client);
    void removeClientSubscription(const QString& orderId, QTcpSocket* client);
    void removeClientSubscriptions(QTcpSocket* client);

private:
    QMap<QString, QList<QTcpSocket*>> orderSubscriptions; // orderId -> list of subscribed clients
};

#endif // ORDERSTATUSOBSERVER_H 