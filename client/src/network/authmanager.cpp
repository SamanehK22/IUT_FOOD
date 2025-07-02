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

void AuthManager::registerUser(const QString &firstName, const QString &lastName, const QString &email, const QString &phone, const QString &password, const QString &userType, const QString &restaurantName, const QString &city, const QString &location)
{
    m_networkManager->registerUser(firstName, lastName, email, phone, password, userType, restaurantName, city, location);
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

void AuthManager::handleLoginSuccess(const QJsonObject &userData)
{
    if (m_currentUser) {
        delete m_currentUser;
    }
    m_currentUser = new User(userData, this);
    emit currentUserChanged();
    emit loginStateChanged();
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