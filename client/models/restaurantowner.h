#ifndef RESTAURANTOWNER_H
#define RESTAURANTOWNER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QList>
#include "user.h"
#include "restaurant.h"

class RestaurantOwner : public User
{
    Q_OBJECT
    Q_PROPERTY(QString restaurantId READ restaurantId WRITE setRestaurantId NOTIFY restaurantIdChanged)
    Q_PROPERTY(bool isApproved READ isApproved WRITE setIsApproved NOTIFY isApprovedChanged)

public:
    explicit RestaurantOwner(QObject *parent = nullptr);
    RestaurantOwner(const QJsonObject &json, QObject *parent = nullptr);
    ~RestaurantOwner();

    // Getters
    QString restaurantId() const;
    bool isApproved() const;
    Restaurant* restaurant() const;

    // Setters
    void setRestaurantId(const QString &restaurantId);
    void setIsApproved(bool isApproved);
    void setRestaurant(Restaurant* restaurant);

    // JSON conversion
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

signals:
    void restaurantIdChanged();
    void isApprovedChanged();

private:
    QString m_restaurantId;
    bool m_isApproved;
    Restaurant* m_restaurant;
};

#endif // RESTAURANTOWNER_H
