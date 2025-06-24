#ifndef ORDERSTATUS_H
#define ORDERSTATUS_H

#include "orderstatusobserver.h"
#include <QObject>
#include <QList>
#include <QMap>
#include "observer.h"

class OrderStatus : public QObject
{
    Q_OBJECT

public:
    static OrderStatus* getInstance();
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notifyObservers(const QString& orderId, const QString& status);
    QString getOrderStatus(const QString& orderId) const;
    explicit OrderStatus(QObject *parent = nullptr);
    ~OrderStatus();

    void addObserver(OrderStatusObserver* observer);
    void removeObserver(OrderStatusObserver* observer);
    void setStatus(const QString& status);
    QString getStatus() const;

private:
    static OrderStatus* instance;

    QList<Observer*> observers;
    QMap<QString, QString> orderStatuses; // orderId -> status mapping
    QString m_status;
    QList<OrderStatusObserver*> m_observers;
};

#endif // ORDERSTATUS_H 
