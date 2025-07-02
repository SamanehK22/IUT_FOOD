#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QList>
#include "user.h"

class Order; // Forward declaration

class Customer : public User
{
    Q_OBJECT
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)

public:
    explicit Customer(QObject *parent = nullptr);
    Customer(const QJsonObject &json, QObject *parent = nullptr);
    ~Customer();

    // Getters
    QString address() const;
    QString phoneNumber() const;

    // Setters
    void setAddress(const QString &address);
    void setPhoneNumber(const QString &phoneNumber);

    // Order history
    QList<Order*> orderHistory() const;
    void addOrder(Order* order);

    // JSON conversion
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

signals:
    void addressChanged();
    void phoneNumberChanged();

private:
    QString m_address;
    QString m_phoneNumber;
    QList<Order*> m_orderHistory;
};

#endif // CUSTOMER_H
