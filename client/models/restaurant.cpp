#include "restaurant.h"

Restaurant::Restaurant(QObject *parent)
    : QObject(parent)
    , m_id("")
    , m_name("")
    , m_description("")
    , m_address("")
    , m_latitude(0.0)
    , m_longitude(0.0)
    , m_rating(0.0)
    , m_ratingCount(0)
    , m_type("")
    , m_imageUrl("")
{
}

Restaurant::Restaurant(const QJsonObject &json, QObject *parent)
    : QObject(parent)
{
    fromJson(json);
}

QString Restaurant::id() const
{
    return m_id;
}

QString Restaurant::name() const
{
    return m_name;
}

QString Restaurant::description() const
{
    return m_description;
}

QString Restaurant::address() const
{
    return m_address;
}

double Restaurant::latitude() const
{
    return m_latitude;
}

double Restaurant::longitude() const
{
    return m_longitude;
}

double Restaurant::rating() const
{
    return m_rating;
}

int Restaurant::ratingCount() const
{
    return m_ratingCount;
}

QString Restaurant::type() const
{
    return m_type;
}

QString Restaurant::imageUrl() const
{
    return m_imageUrl;
}

void Restaurant::setId(const QString &id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

void Restaurant::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

void Restaurant::setDescription(const QString &description)
{
    if (m_description != description) {
        m_description = description;
        emit descriptionChanged();
    }
}

void Restaurant::setAddress(const QString &address)
{
    if (m_address != address) {
        m_address = address;
        emit addressChanged();
    }
}

void Restaurant::setLatitude(double latitude)
{
    if (m_latitude != latitude) {
        m_latitude = latitude;
        emit latitudeChanged();
    }
}

void Restaurant::setLongitude(double longitude)
{
    if (m_longitude != longitude) {
        m_longitude = longitude;
        emit longitudeChanged();
    }
}

void Restaurant::setRating(double rating)
{
    if (m_rating != rating) {
        m_rating = rating;
        emit ratingChanged();
    }
}

void Restaurant::setRatingCount(int count)
{
    if (m_ratingCount != count) {
        m_ratingCount = count;
        emit ratingCountChanged();
    }
}

void Restaurant::setType(const QString &type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

void Restaurant::setImageUrl(const QString &imageUrl)
{
    if (m_imageUrl != imageUrl) {
        m_imageUrl = imageUrl;
        emit imageUrlChanged();
    }
}

QJsonObject Restaurant::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["description"] = m_description;
    json["address"] = m_address;
    json["latitude"] = m_latitude;
    json["longitude"] = m_longitude;
    json["rating"] = m_rating;
    json["ratingCount"] = m_ratingCount;
    json["type"] = m_type;
    json["imageUrl"] = m_imageUrl;
    return json;
}

void Restaurant::fromJson(const QJsonObject &json)
{
    if (json.contains("id"))
        setId(json["id"].toString());
    if (json.contains("name"))
        setName(json["name"].toString());
    if (json.contains("description"))
        setDescription(json["description"].toString());
    if (json.contains("address"))
        setAddress(json["address"].toString());
    if (json.contains("latitude"))
        setLatitude(json["latitude"].toDouble());
    if (json.contains("longitude"))
        setLongitude(json["longitude"].toDouble());
    if (json.contains("rating"))
        setRating(json["rating"].toDouble());
    if (json.contains("ratingCount"))
        setRatingCount(json["ratingCount"].toInt());
    if (json.contains("type"))
        setType(json["type"].toString());
    if (json.contains("imageUrl"))
        setImageUrl(json["imageUrl"].toString());
    if (json.contains("image_url"))
        setImageUrl(json["image_url"].toString());
}

Restaurant* Restaurant::fromJsonObject(const QJsonObject &json, QObject *parent)
{
    return new Restaurant(json, parent);
}

QList<Restaurant*> Restaurant::fromJsonArray(const QJsonArray &array, QObject *parent)
{
    QList<Restaurant*> restaurants;
    for (const QJsonValue &value : array) {
        if (value.isObject()) {
            restaurants.append(new Restaurant(value.toObject(), parent));
        }
    }
    return restaurants;
}