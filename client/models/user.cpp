#include "user.h"

User::User(QObject *parent)
    : QObject(parent)
    , m_id("")
    , m_username("")
    , m_email("")
    , m_userType("")
    , m_token("")
    , m_phoneNumber("")
    , m_city("")
    , m_location("")
{
}

User::User(const QJsonObject &json, QObject *parent)
    : QObject(parent)
{
    fromJson(json);
}

QString User::id() const
{
    return m_id;
}

QString User::username() const
{
    return m_username;
}

QString User::email() const
{
    return m_email;
}

QString User::userType() const
{
    return m_userType;
}

QString User::token() const
{
    return m_token;
}

QString User::phoneNumber() const
{
    return m_phoneNumber;
}

QString User::city() const
{
    return m_city;
}

QString User::location() const
{
    return m_location;
}

void User::setId(const QString &id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

void User::setUsername(const QString &username)
{
    if (m_username != username) {
        m_username = username;
        emit usernameChanged();
    }
}

void User::setEmail(const QString &email)
{
    if (m_email != email) {
        m_email = email;
        emit emailChanged();
    }
}

void User::setUserType(const QString &userType)
{
    if (m_userType != userType) {
        m_userType = userType;
        emit userTypeChanged();
    }
}

void User::setToken(const QString &token)
{
    if (m_token != token) {
        m_token = token;
        emit tokenChanged();
    }
}

void User::setPhoneNumber(const QString &phoneNumber)
{
    if (m_phoneNumber != phoneNumber) {
        m_phoneNumber = phoneNumber;
        emit phoneNumberChanged();
    }
}

void User::setCity(const QString &city)
{
    if (m_city != city) {
        m_city = city;
        emit cityChanged();
    }
}

void User::setLocation(const QString &location)
{
    if (m_location != location) {
        m_location = location;
        emit locationChanged();
    }
}

QJsonObject User::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["username"] = m_username;
    json["email"] = m_email;
    json["userType"] = m_userType;
    json["token"] = m_token;
    json["phoneNumber"] = m_phoneNumber;
    json["city"] = m_city;
    json["location"] = m_location;
    return json;
}

void User::fromJson(const QJsonObject &json)
{
    if (json.contains("id"))
        setId(json["id"].toString());
    if (json.contains("username"))
        setUsername(json["username"].toString());
    if (json.contains("email"))
        setEmail(json["email"].toString());
    if (json.contains("userType"))
        setUserType(json["userType"].toString());
    if (json.contains("token"))
        setToken(json["token"].toString());
    if (json.contains("phoneNumber"))
        setPhoneNumber(json["phoneNumber"].toString());
    if (json.contains("city"))
        setCity(json["city"].toString());
    if (json.contains("location"))
        setLocation(json["location"].toString());
}

User* User::fromJsonObject(const QJsonObject &json, QObject *parent)
{
    return new User(json, parent);
}