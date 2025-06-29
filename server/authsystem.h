#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QDateTime>
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
    bool registerUser(const QString& firstName, const QString& lastName, const QString& email, const QString& phone, const QString& password, const QString& userType);
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