#include "authmanager.h"
#include <QDebug>

AuthManager* AuthManager::instance = nullptr;

AuthManager* AuthManager::getInstance()
{
    if (instance == nullptr) {
        instance = new AuthManager();
    }
    return instance;
}

AuthManager::AuthManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(NetworkManager::getInstance())
    , m_currentUser(nullptr)
{
    qDebug() << "[AuthManager] Constructor this:" << this << ", m_networkManager:" << m_networkManager;
    // Connect to NetworkManager signals
    connect(m_networkManager, &NetworkManager::loginSuccess,
            this, &AuthManager::handleLoginSuccess);
    connect(m_networkManager, &NetworkManager::loginFailed,
            this, &AuthManager::handleLoginFailed);
    connect(m_networkManager, &NetworkManager::registerSuccess,
            this, &AuthManager::handleRegisterSuccess);
    connect(m_networkManager, &NetworkManager::registerFailed,
            this, &AuthManager::handleRegisterFailed);
}

AuthManager::~AuthManager()
{
    if (m_currentUser) {
        delete m_currentUser;
        m_currentUser = nullptr;
    }
}

void AuthManager::login(const QString &loginId, const QString &password)
{
    m_networkManager->login(loginId, password);
}

void AuthManager::registerCustomer(const QString& firstName, const QString& lastName, const QString& email, const QString& phone, const QString& password, const QString& city, const QString& location)
{
    QJsonObject request;
    request["type"] = "register";
    request["user_type"] = "customer";
    request["first_name"] = firstName;
    request["last_name"] = lastName;
    request["email"] = email;
    request["phone"] = phone;
    request["password"] = password;
    request["city"] = city;
    request["location"] = location;
    m_networkManager->sendTcpRequest(request);
}

void AuthManager::registerRestaurantOwner(const QString& firstName, const QString& lastName, const QString& email, const QString& phone, const QString& password, const QString& restaurantName, const QString& city, const QString& location, const QString& type)
{
    QJsonObject request;
    request["type"] = "register";
    request["user_type"] = "restaurant_owner";
    request["first_name"] = firstName;
    request["last_name"] = lastName;
    request["email"] = email;
    request["phone"] = phone;
    request["password"] = password;
    request["restaurant_name"] = restaurantName;
    request["city"] = city;
    request["location"] = location;
    request["restaurant_type"] = type;
    m_networkManager->sendTcpRequest(request);
}

void AuthManager::logout()
{
    if (m_currentUser) {
        m_networkManager->logout();
        delete m_currentUser;
        m_currentUser = nullptr;
        emit currentUserChanged();
        emit loginStateChanged();
        emit logoutSuccess();
    }
}

User* AuthManager::currentUser() const
{
    return m_currentUser;
}

bool AuthManager::isLoggedIn() const
{
    return m_currentUser != nullptr;
}

void AuthManager::handleLoginSuccess(const QJsonObject &response)
{
    qDebug() << "[AuthManager] handleLoginSuccess called, this:" << this;
    QJsonObject userObj = response["user"].toObject();
    if (m_currentUser) {
        delete m_currentUser;
    }
    m_currentUser = new User(userObj, this);
    emit currentUserChanged();
    emit loginStateChanged();
    qDebug() << "[AuthManager] Emitting loginSuccess for userType:" << m_currentUser->userType();
    emit loginSuccess();
}

void AuthManager::handleLoginFailed(const QString &error)
{
    emit loginFailed(error);
}

void AuthManager::handleRegisterSuccess()
{
    emit registerSuccess();
}

void AuthManager::handleRegisterFailed(const QString &error)
{
    emit registerFailed(error);
} 