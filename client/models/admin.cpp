 #include "admin.h"

Admin::Admin(QObject *parent)
    : User(parent)
    , m_canManageUsers(true)
    , m_canManageRestaurants(true)
    , m_canViewStatistics(true)
{
    setUserType("admin");
}

Admin::Admin(const QJsonObject &json, QObject *parent)
    : User(parent)
{
    fromJson(json);
}

Admin::~Admin()
{
}

bool Admin::canManageUsers() const
{
    return m_canManageUsers;
}

bool Admin::canManageRestaurants() const
{
    return m_canManageRestaurants;
}

bool Admin::canViewStatistics() const
{
    return m_canViewStatistics;
}

QJsonObject Admin::toJson() const
{
    QJsonObject json = User::toJson();
    json["can_manage_users"] = m_canManageUsers;
    json["can_manage_restaurants"] = m_canManageRestaurants;
    json["can_view_statistics"] = m_canViewStatistics;
    return json;
}

void Admin::fromJson(const QJsonObject &json)
{
    User::fromJson(json);
    m_canManageUsers = json["can_manage_users"].toBool(true);
    m_canManageRestaurants = json["can_manage_restaurants"].toBool(true);
    m_canViewStatistics = json["can_view_statistics"].toBool(true);
}