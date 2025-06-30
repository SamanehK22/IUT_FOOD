#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QStringList>
#include "menuitem.h"

class Order : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString customerId READ customerId WRITE setCustomerId NOTIFY customerIdChanged)
    Q_PROPERTY(QString restaurantId READ restaurantId WRITE setRestaurantId NOTIFY restaurantIdChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString statusDescription READ statusDescription NOTIFY statusChanged)
    Q_PROPERTY(QString statusColor READ statusColor NOTIFY statusChanged)
    Q_PROPERTY(QString statusIcon READ statusIcon NOTIFY statusChanged)
    Q_PROPERTY(double totalPrice READ totalPrice WRITE setTotalPrice NOTIFY totalPriceChanged)
    Q_PROPERTY(QDateTime orderTime READ orderTime WRITE setOrderTime NOTIFY orderTimeChanged)
    Q_PROPERTY(QDateTime deliveryTime READ deliveryTime WRITE setDeliveryTime NOTIFY deliveryTimeChanged)
    Q_PROPERTY(QList<MenuItem*> items READ items WRITE setItems NOTIFY itemsChanged)

public:
    // Order status constants (matching server)
    static const QString STATUS_PENDING;
    static const QString STATUS_CONFIRMED;
    static const QString STATUS_PREPARING;
    static const QString STATUS_READY;
    static const QString STATUS_OUT_FOR_DELIVERY;
    static const QString STATUS_DELIVERED;
    static const QString STATUS_CANCELLED;
    static const QString STATUS_REJECTED;

    explicit Order(QObject *parent = nullptr);
    Order(const QJsonObject &json, QObject *parent = nullptr);
    ~Order();

    // Getters
    QString id() const;
    QString customerId() const;
    QString restaurantId() const;
    QString status() const;
    QString statusDescription() const;
    QString statusColor() const;
    QString statusIcon() const;
    double totalPrice() const;
    QDateTime orderTime() const;
    QDateTime deliveryTime() const;
    QList<MenuItem*> items() const;

    // Setters
    void setId(const QString &id);
    void setCustomerId(const QString &customerId);
    void setRestaurantId(const QString &restaurantId);
    void setStatus(const QString &status);
    void setTotalPrice(double totalPrice);
    void setOrderTime(const QDateTime &orderTime);
    void setDeliveryTime(const QDateTime &deliveryTime);
    void setItems(const QList<MenuItem*> &items);

    // Status validation and utilities
    static bool isValidStatus(const QString &status);
    static QStringList getValidStatuses();
    static QString getStatusDescription(const QString &status);
    static QString getStatusColor(const QString &status);
    static QString getStatusIcon(const QString &status);
    bool canTransitionTo(const QString &newStatus) const;

    // JSON conversion
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    // Static methods
    static Order* fromJsonObject(const QJsonObject &json, QObject *parent = nullptr);
    static QList<Order*> fromJsonArray(const QJsonArray &array, QObject *parent = nullptr);

signals:
    void idChanged();
    void customerIdChanged();
    void restaurantIdChanged();
    void statusChanged();
    void totalPriceChanged();
    void orderTimeChanged();
    void deliveryTimeChanged();
    void itemsChanged();

private:
    QString m_id;
    QString m_customerId;
    QString m_restaurantId;
    QString m_status;
    double m_totalPrice;
    QDateTime m_orderTime;
    QDateTime m_deliveryTime;
    QList<MenuItem*> m_items;

    // Status descriptions and UI properties
    static QMap<QString, QString> statusDescriptions;
    static QMap<QString, QString> statusColors;
    static QMap<QString, QString> statusIcons;
    static QMap<QString, QStringList> validTransitions;
};

#endif // ORDER_H 