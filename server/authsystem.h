#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QDateTime>
#include <QVariantMap>
#include "databasemanager.h"
#include "securityutils.h"

struct Session {
    QString userId;
    QString userType;
    QString token;
    QDateTime lastActivity;
};

class AuthSystem : public QObject
{
    Q_OBJECT

public:
    static AuthSystem* getInstance();
    
    // Authentication methods
    QString login(const QString& loginId, const QString& password);
    bool registerCustomer(const QString& firstName, const QString& lastName, const QString& email, const QString& phone, const QString& password, const QString& city, const QString& location);
    bool registerRestaurantOwner(const QString& firstName, const QString& lastName, const QString& email, const QString& phone, const QString& password, const QString& restaurantName, const QString& city, const QString& location, const QString& type);
    bool registerAdmin(const QString& firstName, const QString& lastName, const QString& username, const QString& email, const QString& password, const QString& phone);
    bool logout(const QString& token);
    
    // Session management
    bool validateSession(const QString& token);
    bool refreshSession(const QString& token);
    Session* getSession(const QString& token);
    QString getUserIdFromToken(const QString& token);
    QString getUserTypeFromToken(const QString& token);
    
    // User management
    bool updateProfile(const QString& token, const QVariantMap& updates);
    bool changePassword(const QString& token, const QString& oldPassword, const QString& newPassword);
    bool deleteAccount(const QString& token);

    bool handleRegisterCustomer(const QJsonObject& data);
    bool handleRegisterRestaurantOwner(const QJsonObject& data);

    QVariantMap getUserProfile(const QString& token);

private:
    explicit AuthSystem(QObject *parent = nullptr);
    ~AuthSystem();
    static AuthSystem* instance;

    QMap<QString, Session> activeSessions; // token -> Session mapping
    DatabaseManager* dbManager;

    bool validatePassword(const QString& password) const;
    bool validateUsername(const QString& username) const;
    bool validateEmail(const QString& email) const;
    void cleanupExpiredSessions();
};

#endif // AUTHSYSTEM_H 