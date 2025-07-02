#ifndef ORDERSTATUS_H
#define ORDERSTATUS_H

#include "orderstatusobserver.h"
#include <QObject>
#include <QList>
#include <QMap>
#include <QStringList>
#include "observer.h"

class OrderStatus : public QObject
{
    Q_OBJECT

public:
    // Predefined order status values
    static const QString STATUS_PENDING;
    static const QString STATUS_CONFIRMED;
    static const QString STATUS_PREPARING;
    static const QString STATUS_READY;
    static const QString STATUS_OUT_FOR_DELIVERY;
    static const QString STATUS_DELIVERED;
    static const QString STATUS_CANCELLED;
    static const QString STATUS_REJECTED;

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

    // Validation methods
    static bool isValidStatus(const QString& status);
    static QStringList getValidStatuses();
    static QString getStatusDescription(const QString& status);

private:
    static OrderStatus* instance;

    QList<Observer*> observers;
    QMap<QString, QString> orderStatuses; // orderId -> status mapping
    QString m_status;
    QList<OrderStatusObserver*> m_observers;
    
    // Status descriptions
    static QMap<QString, QString> statusDescriptions;
};

#endif // ORDERSTATUS_H 
