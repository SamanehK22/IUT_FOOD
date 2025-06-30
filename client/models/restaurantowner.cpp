#include "restaurantowner.h"

RestaurantOwner::RestaurantOwner(QObject *parent)
    : User(parent)
    , m_isApproved(false)
    , m_restaurant(nullptr)
{
    setUserType("restaurant_owner");
}

RestaurantOwner::RestaurantOwner(const QJsonObject &json, QObject *parent)
    : User(parent)
    , m_restaurant(nullptr)
{
    fromJson(json);
}

RestaurantOwner::~RestaurantOwner()
{
    delete m_restaurant;
}

QString RestaurantOwner::restaurantId() const
{
    return m_restaurantId;
}

bool RestaurantOwner::isApproved() const
{
    return m_isApproved;
}

Restaurant* RestaurantOwner::restaurant() const
{
    return m_restaurant;
}

void RestaurantOwner::setRestaurantId(const QString &restaurantId)
{
    if (m_restaurantId != restaurantId) {
        m_restaurantId = restaurantId;
        emit restaurantIdChanged();
    }
}

void RestaurantOwner::setIsApproved(bool isApproved)
{
    if (m_isApproved != isApproved) {
        m_isApproved = isApproved;
        emit isApprovedChanged();
    }
}

void RestaurantOwner::setRestaurant(Restaurant* restaurant)
{
    if (m_restaurant != restaurant) {
        delete m_restaurant;
        m_restaurant = restaurant;
        if (m_restaurant) {
            setRestaurantId(m_restaurant->id());
        }
    }
}

QJsonObject RestaurantOwner::toJson() const
{
    QJsonObject json = User::toJson();
    json["restaurant_id"] = m_restaurantId;
    json["is_approved"] = m_isApproved;
    if (m_restaurant) {
        json["restaurant"] = m_restaurant->toJson();
    }
    return json;
}

void RestaurantOwner::fromJson(const QJsonObject &json)
{
    User::fromJson(json);
    m_restaurantId = json["restaurant_id"].toString();
    m_isApproved = json["is_approved"].toBool();
    
    if (json.contains("restaurant")) {
        delete m_restaurant;
        m_restaurant = new Restaurant(json["restaurant"].toObject(), this);
    }
}
