#include "menuitem.h"

MenuItem::MenuItem(QObject *parent)
    : QObject(parent)
    , m_id("")
    , m_name("")
    , m_description("")
    , m_price(0.0)
    , m_category("")
    , m_imageUrl("")
    , m_available(true)
    , m_ingredients("")
{
}

MenuItem::MenuItem(const QJsonObject &json, QObject *parent)
    : QObject(parent)
{
    fromJson(json);
}

QString MenuItem::id() const
{
    return m_id;
}

QString MenuItem::name() const
{
    return m_name;
}

QString MenuItem::description() const
{
    return m_description;
}

double MenuItem::price() const
{
    return m_price;
}

QString MenuItem::category() const
{
    return m_category;
}

QString MenuItem::imageUrl() const
{
    return m_imageUrl;
}

bool MenuItem::available() const
{
    return m_available;
}

QString MenuItem::ingredients() const
{
    return m_ingredients;
}

void MenuItem::setId(const QString &id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

void MenuItem::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

void MenuItem::setDescription(const QString &description)
{
    if (m_description != description) {
        m_description = description;
        emit descriptionChanged();
    }
}

void MenuItem::setPrice(double price)
{
    if (m_price != price) {
        m_price = price;
        emit priceChanged();
    }
}

void MenuItem::setCategory(const QString &category)
{
    if (m_category != category) {
        m_category = category;
        emit categoryChanged();
    }
}

void MenuItem::setImageUrl(const QString &url)
{
    if (m_imageUrl != url) {
        m_imageUrl = url;
        emit imageUrlChanged();
    }
}

void MenuItem::setAvailable(bool available)
{
    if (m_available != available) {
        m_available = available;
        emit availableChanged();
    }
}

void MenuItem::setIngredients(const QString &ingredients)
{
    if (m_ingredients != ingredients) {
        m_ingredients = ingredients;
        emit ingredientsChanged();
    }
}

QJsonObject MenuItem::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["description"] = m_description;
    json["price"] = m_price;
    json["category"] = m_category;
    json["imageUrl"] = m_imageUrl;
    json["available"] = m_available;
    json["ingredients"] = m_ingredients;
    return json;
}

void MenuItem::fromJson(const QJsonObject &json)
{
    if (json.contains("id"))
        setId(json["id"].toString());
    if (json.contains("name"))
        setName(json["name"].toString());
    if (json.contains("description"))
        setDescription(json["description"].toString());
    if (json.contains("price"))
        setPrice(json["price"].toDouble());
    if (json.contains("category"))
        setCategory(json["category"].toString());
    if (json.contains("imageUrl"))
        setImageUrl(json["imageUrl"].toString());
    if (json.contains("available"))
        setAvailable(json["available"].toBool());
    if (json.contains("ingredients"))
        setIngredients(json["ingredients"].toString());
}

MenuItem* MenuItem::fromJsonObject(const QJsonObject &json, QObject *parent)
{
    return new MenuItem(json, parent);
}