#ifndef USER_H
#define USER_H

#include <QString>
#include <QObject>
#include <QJsonObject>

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString userType READ userType WRITE setUserType NOTIFY userTypeChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)

public:
    explicit User(QObject *parent = nullptr);
    User(const QJsonObject &json, QObject *parent = nullptr);

    // Getters
    QString id() const;
    QString username() const;
    QString email() const;
    QString userType() const;
    QString token() const;
    QString phoneNumber() const;
    QString city() const;
    QString location() const;

    // Setters
    void setId(const QString &id);
    void setUsername(const QString &username);
    void setEmail(const QString &email);
    void setUserType(const QString &userType);
    void setToken(const QString &token);
    void setPhoneNumber(const QString &phoneNumber);
    void setCity(const QString &city);
    void setLocation(const QString &location);

    // JSON conversion
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    // Static methods
    static User* fromJsonObject(const QJsonObject &json, QObject *parent = nullptr);

signals:
    void idChanged();
    void usernameChanged();
    void emailChanged();
    void userTypeChanged();
    void tokenChanged();
    void phoneNumberChanged();
    void cityChanged();
    void locationChanged();

private:
    QString m_id;
    QString m_username;
    QString m_email;
    QString m_userType;
    QString m_token;
    QString m_phoneNumber;
    QString m_city;
    QString m_location;
};

#endif // USER_H
