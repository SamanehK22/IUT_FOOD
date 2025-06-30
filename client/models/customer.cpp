#include "customer.h"
#include "order.h"

Customer::Customer(QObject *parent)
    : User(parent)
{
    setUserType("customer");
}

Customer::~Customer()
{
    qDeleteAll(m_orderHistory);
    m_orderHistory.clear();
}

QString Customer::address() const
{
    return m_address;
}

QString Customer::phoneNumber() const
{
    return m_phoneNumber;
}

void Customer::setAddress(const QString &address)
{
    if (m_address != address) {
        m_address = address;
        emit addressChanged();
    }
}

void Customer::setPhoneNumber(const QString &phoneNumber)
{
    if (m_phoneNumber != phoneNumber) {
        m_phoneNumber = phoneNumber;
        emit phoneNumberChanged();
    }
}

QList<Order*> Customer::orderHistory() const
{
    return m_orderHistory;
}

void Customer::addOrder(Order* order)
{
    if (order && !m_orderHistory.contains(order)) {
        m_orderHistory.append(order);
    }
}

QJsonObject Customer::toJson() const
{
    QJsonObject json = User::toJson();
    json["address"] = m_address;
    json["phone_number"] = m_phoneNumber;
    // Order history serialization can be added here if needed
    return json;
}

void Customer::fromJson(const QJsonObject &json)
{
    User::fromJson(json);
    m_address = json["address"].toString();
    m_phoneNumber = json["phone_number"].toString();
    // Order history deserialization can be added here if needed
}
