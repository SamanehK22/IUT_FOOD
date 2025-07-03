#include "authsystem.h"
#include <QDebug>
#include <QRegularExpression>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlQuery>

AuthSystem* AuthSystem::instance = nullptr;

AuthSystem* AuthSystem::getInstance()
{
    qDebug() << "[AuthSystem::getInstance] Called. Current instance:" << instance;
    if (instance == nullptr) {
        qDebug() << "[AuthSystem::getInstance] Created new instance.";
        instance = new AuthSystem();
    } else {
        qDebug() << "[AuthSystem::getInstance] Reusing existing instance.";
    }
    return instance;
}

AuthSystem::AuthSystem(QObject *parent)
    : QObject(parent)
    , dbManager(DatabaseManager::getInstance())
{
    qDebug() << "[AuthSystem::Constructor] Called. This:" << this;
}

AuthSystem::~AuthSystem()
{
    activeSessions.clear();
}

QString AuthSystem::login(const QString& loginId, const QString& password)
{
    qDebug() << "[login] loginId:" << loginId;
    qDebug() << "[login] password:" << password;
    // Validate input
    if (loginId.isEmpty() || password.isEmpty()) {
        return QString();
    }

    // Try customer first
    QVariantMap user = dbManager->getCustomerByLoginId(loginId);
    QString userType = "customer";
    if (user.isEmpty()) {
        user = dbManager->getRestaurantOwnerByLoginId(loginId);
        if (!user.isEmpty()) {
            userType = "restaurant_owner";
        }
    }
    if (user.isEmpty()) {
        user = dbManager->getAdminByLoginId(loginId);
        if (!user.isEmpty()) {
            userType = "admin";
        }
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

    qDebug() << "[login] Input password:" << password;
    qDebug() << "[login] Stored hash:" << storedHash;
    qDebug() << "[login] Password valid:" << SecurityUtils::verifyPassword(password, storedHash);

    if (!SecurityUtils::verifyPassword(password, storedHash)) {
        qDebug() << "Invalid password for user:" << loginId;
        return QString();
    }

    // Check restaurant approval status for restaurant owners
    if (userType == "restaurant_owner") {
        qDebug() << "[AuthSystem::login] restaurant_id QVariant type:" << user["restaurant_id"].typeName();
        qDebug() << "[AuthSystem::login] restaurant_id value:" << user["restaurant_id"];
        QString restaurantId = user["restaurant_id"].toString();
        qDebug() << "[AuthSystem::login] restaurantId string:" << restaurantId;
        if (!restaurantId.isEmpty()) {
            QVariantMap restaurant = dbManager->getRestaurantById(restaurantId);
            if (restaurant.isEmpty()) {
                qDebug() << "Restaurant not found for owner:" << loginId;
                return QString();
            }
            QString restaurantStatus = restaurant["status"].toString();
            if (restaurantStatus == "pending") {
                qDebug() << "Restaurant pending approval for owner:" << loginId;
                return "PENDING_APPROVAL";
            } else if (restaurantStatus != "approved") {
                qDebug() << "Restaurant not approved for owner:" << loginId << "Status:" << restaurantStatus;
                return QString();
            }
        } else {
            qDebug() << "No restaurant associated with owner:" << loginId;
            return QString();
        }
    }

    // Generate session token
    QString token = SecurityUtils::generateSessionToken();
    qDebug() << "[login] Generated token:" << token;

    // Create session
    Session session;
    session.userId = user["id"].toString();
    session.userType = userType;
    session.token = token;
    session.lastActivity = QDateTime::currentDateTime();
    activeSessions[token] = session;
    qDebug() << "[login] Stored session for token:" << token;
    qDebug() << "[login] activeSessions keys after store:" << activeSessions.keys();

    qDebug() << "User logged in successfully:" << loginId;
    return token;
}

bool AuthSystem::registerCustomer(const QString& firstName, const QString& lastName, const QString& email, const QString& phone, const QString& password, const QString& city, const QString& location) {
    qDebug() << "[Server] registerCustomer called with:" << firstName << lastName << email << phone << password << city << location;
    if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty() || phone.isEmpty() || password.isEmpty() || city.isEmpty() || location.isEmpty()) {
        qDebug() << "[Server] registerCustomer: Missing required fields.";
        return false;
    }
    if (dbManager->customerEmailExists(email) || dbManager->customerPhoneExists(phone)) {
        qDebug() << "[Server] registerCustomer: Duplicate customer email or phone:" << email << phone;
        return false;
    }
    QString hashedPassword = SecurityUtils::hashPassword(password);
    QString username = firstName + " " + lastName;
    qDebug() << "[Server] registerCustomer: Calling dbManager->createCustomer...";
    bool result = dbManager->createCustomer(firstName, lastName, username, email, hashedPassword, phone, city, location);
    qDebug() << "[Server] dbManager->createCustomer returned:" << result;
    return result;
}

bool AuthSystem::registerRestaurantOwner(const QString& firstName, const QString& lastName, const QString& email, const QString& phone, const QString& password, const QString& restaurantName, const QString& city, const QString& location, const QString& type) {
    qDebug() << "[Server] registerRestaurantOwner called with:" << firstName << lastName << email << phone << password << restaurantName << city << location << type;
    if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty() || phone.isEmpty() || password.isEmpty() || restaurantName.isEmpty() || city.isEmpty() || location.isEmpty() || type.isEmpty()) {
        qDebug() << "[Server] registerRestaurantOwner: Missing required fields.";
        return false;
    }
    if (dbManager->ownerEmailExists(email) || dbManager->ownerPhoneExists(phone)) {
        qDebug() << "[Server] registerRestaurantOwner: Duplicate restaurant owner email or phone:" << email << phone;
        return false;
    }
    QString hashedPassword = SecurityUtils::hashPassword(password);
    QString username = firstName + " " + lastName;
    qDebug() << "[Server] registerRestaurantOwner: Calling dbManager->createRestaurantOwner...";
    QString ownerId = dbManager->createRestaurantOwner(firstName, lastName, username, email, hashedPassword, phone, "", city, location);
    qDebug() << "[Server] dbManager->createRestaurantOwner returned ownerId:" << ownerId;
    if (ownerId.isEmpty()) {
        qDebug() << "[Server] Failed to create restaurant owner or get ID";
        return false;
    }
    // Fix parameter order: name, address, city, location, type, ownerId, imageUrl, status
    qDebug() << "[Server] registerRestaurantOwner: Calling dbManager->createRestaurant with params:" << restaurantName << restaurantName << city << location << type << ownerId << "" << "pending";
    bool restResult = dbManager->createRestaurant(restaurantName, restaurantName, city, location, type, ownerId, "", "pending");
    qDebug() << "[Server] dbManager->createRestaurant returned:" << restResult;
    if (!restResult) {
        return false;
    }
    // Get the last inserted restaurant ID directly
    QSqlQuery lastIdQuery("SELECT last_insert_rowid()");
    QString restaurantId;
    if (lastIdQuery.next()) {
        restaurantId = lastIdQuery.value(0).toString();
        qDebug() << "[registerRestaurantOwner] last_insert_rowid for restaurant:" << restaurantId;
    }
    if (!restaurantId.isEmpty()) {
        qDebug() << "[registerRestaurantOwner] Updating owner with restaurant_id:" << restaurantId;
        QVariantMap updates;
        updates["restaurant_id"] = restaurantId;
        bool updateResult = dbManager->updateRestaurantOwner(ownerId, updates);
        qDebug() << "[Server] dbManager->updateRestaurantOwner returned:" << updateResult;
    }
    dbManager->debugPrintAllRestaurantOwners();
    dbManager->debugPrintAllRestaurants();
    return restResult;
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
    qDebug() << "[getSession] Looking for token:" << token;
    qDebug() << "[getSession] activeSessions keys:" << activeSessions.keys();
    if (activeSessions.contains(token)) {
        return &activeSessions[token];
    }
    return nullptr;
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

bool AuthSystem::handleRegisterCustomer(const QJsonObject& data)
{
    QString firstName = data["first_name"].toString();
    QString lastName = data["last_name"].toString();
    QString email = data["email"].toString();
    QString phone = data["phone"].toString();
    QString password = data["password"].toString();
    QString city = data["city"].toString();
    QString location = data["location"].toString();

    if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty() || phone.isEmpty() || password.isEmpty() || city.isEmpty() || location.isEmpty()) {
        qDebug() << "Missing required fields for customer registration.";
        return false;
    }

    if (dbManager->customerEmailExists(email) || dbManager->customerPhoneExists(phone)) {
        qDebug() << "Duplicate customer email or phone:" << email << phone;
        return false;
    }

    QString hashedPassword = SecurityUtils::hashPassword(password);
    QString username = firstName + " " + lastName;

    return dbManager->createCustomer(firstName, lastName, username, email, hashedPassword, phone, city, location);
}

bool AuthSystem::handleRegisterRestaurantOwner(const QJsonObject& data)
{
    QString firstName = data["first_name"].toString();
    QString lastName = data["last_name"].toString();
    QString email = data["email"].toString();
    QString phone = data["phone"].toString();
    QString password = data["password"].toString();
    QString restaurantName = data["restaurant_name"].toString();
    QString city = data["city"].toString();
    QString location = data["location"].toString();
    QString type = data["restaurant_type"].toString();

    if (firstName.isEmpty() || lastName.isEmpty() || email.isEmpty() || phone.isEmpty() || password.isEmpty() || restaurantName.isEmpty() || city.isEmpty() || location.isEmpty() || type.isEmpty()) {
        qDebug() << "Missing required fields for restaurant owner registration.";
        return false;
    }

    if (dbManager->ownerEmailExists(email) || dbManager->ownerPhoneExists(phone)) {
        qDebug() << "Duplicate restaurant owner email or phone:" << email << phone;
        return false;
    }

    QString hashedPassword = SecurityUtils::hashPassword(password);
    QString username = firstName + " " + lastName;

    QString ownerId = dbManager->createRestaurantOwner(firstName, lastName, username, email, hashedPassword, phone, "", city, location);
    if (ownerId.isEmpty()) {
        // error handling
        return false;
    }

    if (!dbManager->createRestaurant(restaurantName, location, city, location, type, ownerId, "", "pending")) {
        return false;
    }

    QJsonArray ownerRestaurants = dbManager->getRestaurantsByOwner(ownerId);
    if (ownerRestaurants.isEmpty()) {
        return false;
    }

    QString restaurantId = ownerRestaurants.last().toObject()["id"].toString();
    QVariantMap updates;
    updates["restaurant_id"] = restaurantId;

    return dbManager->updateRestaurantOwner(ownerId, updates);
}

QVariantMap AuthSystem::getUserProfile(const QString& token) {
    qDebug() << "[getUserProfile] token:" << token;
    Session* session = getSession(token);
    if (!session) {
        qDebug() << "[getUserProfile] No session found for token!";
        return QVariantMap();
    }
    qDebug() << "[getUserProfile] userType:" << session->userType << "userId:" << session->userId;
    if (session->userType == "customer") {
        QVariantMap profile = dbManager->getCustomerProfile(session->userId);
        profile["userType"] = "customer";
        profile["id"] = session->userId;
        return profile;
    } else if (session->userType == "restaurant_owner") {
        QVariantMap profile = dbManager->getRestaurantOwnerProfile(session->userId);
        profile["userType"] = "restaurant_owner";
        profile["id"] = session->userId;
        return profile;
    } else if (session->userType == "admin") {
        QVariantMap profile;
        profile["userType"] = "admin";
        profile["id"] = session->userId;
        return profile;
    }
    return QVariantMap();
}

bool AuthSystem::registerAdmin(const QString& firstName, const QString& lastName, const QString& username, const QString& email, const QString& password, const QString& phone) {
    if (firstName.isEmpty() || lastName.isEmpty() || username.isEmpty() || email.isEmpty() || password.isEmpty() || phone.isEmpty()) {
        qDebug() << "[Server] registerAdmin: Missing required fields.";
        return false;
    }
    if (dbManager->adminEmailExists(email) || dbManager->adminPhoneExists(phone)) {
        qDebug() << "[Server] registerAdmin: Duplicate admin email or phone:" << email << phone;
        return false;
    }
    QString hashedPassword = SecurityUtils::hashPassword(password);
    bool result = dbManager->createAdmin(firstName, lastName, username, email, hashedPassword, phone);
    qDebug() << "[Server] dbManager->createAdmin returned:" << result;
    return result;
} 