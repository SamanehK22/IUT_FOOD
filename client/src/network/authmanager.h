#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QString>
#include "../models/user.h"
#include "networkmanager.h"

class AuthManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(User* currentUser READ currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY loginStateChanged)

public:
    static AuthManager* getInstance();
    void login(const QString &loginId, const QString &password);
    void registerUser(const QString &firstName, const QString &lastName, const QString &email, const QString &phone, const QString &password, const QString &userType, const QString &restaurantName = "", const QString &city = "", const QString &location = "");
    void logout();
    User* currentUser() const;
    bool isLoggedIn() const;

signals:
    void loginSuccess();
    void loginFailed(const QString &error);
    void registerSuccess();
    void registerFailed(const QString &error);
    void logoutSuccess();
    void currentUserChanged();
    void loginStateChanged();

private:
    explicit AuthManager(QObject *parent = nullptr);
    ~AuthManager();
    static AuthManager* instance;

    NetworkManager* m_networkManager;
    User* m_currentUser;

    void handleLoginSuccess(const QJsonObject &userData);
    void handleLoginFailed(const QString &error);
    void handleRegisterSuccess();
    void handleRegisterFailed(const QString &error);
};

#endif // AUTHMANAGER_H