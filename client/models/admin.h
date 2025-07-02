#ifndef ADMIN_H
#define ADMIN_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include "user.h"

class Admin : public User
{
    Q_OBJECT

public:
    explicit Admin(QObject *parent = nullptr);
    Admin(const QJsonObject &json, QObject *parent = nullptr);
    ~Admin();

    // Admin-specific methods
    bool canManageUsers() const;
    bool canManageRestaurants() const;
    bool canViewStatistics() const;

    // JSON conversion
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

private:
    bool m_canManageUsers;
    bool m_canManageRestaurants;
    bool m_canViewStatistics;
};

#endif // ADMIN_H
