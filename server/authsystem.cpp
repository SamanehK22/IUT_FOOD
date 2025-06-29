#include "authsystem.h"
#include <QDebug>
#include <QRegularExpression>

AuthSystem* AuthSystem::instance = nullptr;

AuthSystem* AuthSystem::getInstance()
{
    if (instance == nullptr) {
        instance = new AuthSystem();
    }
    return instance;
}

AuthSystem::AuthSystem(QObject *parent)
    : QObject(parent)
    , dbManager(DatabaseManager::getInstance())
{
}

AuthSystem::~AuthSystem()
{
    activeSessions.clear();
}

QString AuthSystem::login(const QString& loginId, const QString& password)
{
    // Validate input
    if (loginId.isEmpty() || password.isEmpty()) {
        return QString();
    }

    // Try customer first
    QVariantMap user = dbManager->getCustomerByLoginId(loginId);
    QString userType = "customer";
    if (user.isEmpty()) {
        user = dbManager->getRestaurantOwnerByLoginId(loginId);
        userType = "restaurant_owner";
    }
    if (user.isEmpty()) {
        qDebug() << "User not found:" << loginId;
        return QString();
    }

    QString storedHash = user["password_hash"].toString();
    if (storedHash.isEmpty()) {
        qDebug() << "No password hash found for user:" << loginId;
        return QString();
    }

    if (!SecurityUtils::verifyPassword(password, storedHash)) {
        qDebug() << "Invalid password for user:" << loginId;
        return QString();
    }

    // Generate session token
    QString token = SecurityUtils::generateSessionToken();

    // Create session
    Session session;
    session.userId = user["id"].toString();
    session.userType = userType;
    session.token = token;
    session.lastActivity = QDateTime::currentDateTime();
    activeSessions[token] = session;

    qDebug() << "User logged in successfully:" << loginId;
    return token;
}

bool AuthSystem::registerUser(const QString& firstName, const QString& lastName, const QString& email, const QString& phone, const QString& password, const QString& userType)
{
    if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty() || phone.isEmpty() || password.isEmpty() || userType.isEmpty()) {
        return false;
    }
    // Hash the password
    QString hashedPassword = SecurityUtils::hashPassword(password);
    QString username = firstName + " " + lastName;
    bool success = false;
    if (userType == "restaurant_owner") {
        success = dbManager->createRestaurantOwner(firstName, lastName, username, email, hashedPassword, phone, "", "", "");
    } else {
        success = dbManager->createCustomer(firstName, lastName, username, email, hashedPassword, phone, "", "", "");
    }
    if (!success) {
        qDebug() << "Failed to create user:" << username;
        return false;
    }
    qDebug() << "User registered successfully:" << username;
    return true;
}

bool AuthSystem::logout(const QString& token)
{
    if (!activeSessions.contains(token)) {
        qDebug() << "No active session found for token";
        return false;
    }

    activeSessions.remove(token);
    qDebug() << "User logged out successfully";
    return true;
}

bool AuthSystem::validateSession(const QString& token)
{
    cleanupExpiredSessions();

    if (!activeSessions.contains(token)) {
        return false;
    }

    Session& session = activeSessions[token];
    if (SecurityUtils::isTokenExpired(token)) {
        activeSessions.remove(token);
        return false;
    }

    // Update last activity
    session.lastActivity = QDateTime::currentDateTime();
    return true;
}

bool AuthSystem::refreshSession(const QString& token)
{
    if (!validateSession(token)) {
        return false;
    }

    // Generate new token
    QString newToken = SecurityUtils::generateSessionToken();
    Session session = activeSessions[token];
    session.token = newToken;
    session.lastActivity = QDateTime::currentDateTime();

    // Update session
    activeSessions.remove(token);
    activeSessions[newToken] = session;

    return true;
}

Session* AuthSystem::getSession(const QString& token)
{
    if (!validateSession(token)) {
        return nullptr;
    }
    return &activeSessions[token];
}

QString AuthSystem::getUserIdFromToken(const QString& token)
{
    Session* session = getSession(token);
    return session ? session->userId : QString();
}

QString AuthSystem::getUserTypeFromToken(const QString& token)
{
    Session* session = getSession(token);
    return session ? session->userType : QString();
}

bool AuthSystem::updateProfile(const QString& token, const QVariantMap& updates)
{
    if (!validateSession(token)) {
        qDebug() << "Invalid session token";
        return false;
    }

    QString userId = getUserIdFromToken(token);

    // Validate updates
    if (updates.contains("password")) {
        if (!validatePassword(updates["password"].toString())) {
            return false;
        }
        updates["password"] = SecurityUtils::hashPassword(updates["password"].toString());
    }

    if (updates.contains("email") && !validateEmail(updates["email"].toString())) {
        return false;
    }

    // Update user in database
    if (!dbManager->updateCustomer(userId, updates)) {
        qDebug() << "Failed to update profile for user:" << userId;
        return false;
    }

    qDebug() << "Profile updated successfully for user:" << userId;
    return true;
}

bool AuthSystem::changePassword(const QString& token, const QString& oldPassword, const QString& newPassword)
{
    if (!validateSession(token)) {
        qDebug() << "Invalid session token";
        return false;
    }

    QString userId = getUserIdFromToken(token);

    // Validate new password
    if (!validatePassword(newPassword)) {
        return false;
    }

    // Verify old password
    QString storedHash = dbManager->getCustomerPasswordHash(userId);
    if (!SecurityUtils::verifyPassword(oldPassword, storedHash)) {
        qDebug() << "Invalid old password";
        return false;
    }

    // Update password
    QVariantMap updates;
    updates["password"] = SecurityUtils::hashPassword(newPassword);
    return updateProfile(token, updates);
}

bool AuthSystem::deleteAccount(const QString& token)
{
    if (!validateSession(token)) {
        qDebug() << "Invalid session token";
        return false;
    }

    QString userId = getUserIdFromToken(token);

    // Delete user from database
    if (!dbManager->deleteCustomer(userId)) {
        qDebug() << "Failed to delete account for user:" << userId;
        return false;
    }

    // Remove session
    activeSessions.remove(token);
    qDebug() << "Account deleted successfully for user:" << userId;
    return true;
}

bool AuthSystem::validatePassword(const QString& password) const
{
    // Password must be at least 8 characters long and contain at least one number
    return password.length() >= 8 && password.contains(QRegularExpression("\\d"));
}

bool AuthSystem::validateUsername(const QString& username) const
{
    // Username must be 3-20 characters long and contain only letters, numbers, and underscores
    return username.length() >= 3 && username.length() <= 20 &&
           username.contains(QRegularExpression("^[a-zA-Z0-9_]+$"));
}

bool AuthSystem::validateEmail(const QString& email) const
{
    // Basic email validation
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return emailRegex.match(email).hasMatch();
}

void AuthSystem::cleanupExpiredSessions()
{
    QList<QString> expiredTokens;
    for (auto it = activeSessions.begin(); it != activeSessions.end(); ++it) {
        if (SecurityUtils::isTokenExpired(it.key())) {
            expiredTokens.append(it.key());
        }
    }

    for (const QString& token : expiredTokens) {
        activeSessions.remove(token);
    }
} 