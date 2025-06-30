 #include "cart.h"
#include <QJsonArray>
#include <QJsonObject>

Cart::Cart(QObject *parent)
    : QObject(parent)
    , m_restaurantId("")
    , m_totalPrice(0.0)
    , m_itemCount(0)
{
}

double Cart::totalPrice() const
{
    return m_totalPrice;
}

int Cart::itemCount() const
{
    return m_itemCount;
}

QString Cart::restaurantId() const
{
    return m_restaurantId;
}

QMap<MenuItem*, int> Cart::items() const
{
    return m_items;
}

void Cart::addItem(MenuItem* item, int quantity)
{
    if (!item || quantity <= 0) return;

    // Check if we can add this item
    if (!canAddItem(item)) return;

    // If item already exists, update quantity
    if (m_items.contains(item)) {
        updateItemQuantity(item, m_items[item] + quantity);
    } else {
        m_items[item] = quantity;
        m_itemCount += quantity;
        m_totalPrice += item->price() * quantity;
        emit itemAdded(item, quantity);
        emit itemCountChanged();
        emit totalPriceChanged();
    }
}

void Cart::removeItem(MenuItem* item)
{
    if (!item || !m_items.contains(item)) return;

    int quantity = m_items[item];
    m_items.remove(item);
    m_itemCount -= quantity;
    m_totalPrice -= item->price() * quantity;

    emit itemRemoved(item);
    emit itemCountChanged();
    emit totalPriceChanged();
}

void Cart::updateItemQuantity(MenuItem* item, int quantity)
{
    if (!item || !m_items.contains(item) || quantity <= 0) return;

    int oldQuantity = m_items[item];
    m_items[item] = quantity;
    m_itemCount = m_itemCount - oldQuantity + quantity;
    m_totalPrice = m_totalPrice - (item->price() * oldQuantity) + (item->price() * quantity);

    emit itemQuantityChanged(item, quantity);
    emit itemCountChanged();
    emit totalPriceChanged();
}

void Cart::clear()
{
    m_items.clear();
    m_itemCount = 0;
    m_totalPrice = 0.0;
    m_restaurantId.clear();

    emit cartCleared();
    emit itemCountChanged();
    emit totalPriceChanged();
    emit restaurantIdChanged();
}

bool Cart::isEmpty() const
{
    return m_items.isEmpty();
}

bool Cart::canAddItem(MenuItem* item) const
{
    if (!item) return false;

    // If cart is empty, we can add any item
    if (isEmpty()) return true;

    // If cart is not empty, we can only add items from the same restaurant
    return m_restaurantId == item->id().split('_')[0]; // Assuming item ID format is "restaurantId_itemId"
}

void Cart::setRestaurantId(const QString &restaurantId)
{
    if (m_restaurantId != restaurantId) {
        m_restaurantId = restaurantId;
        emit restaurantIdChanged();
    }
}

QJsonObject Cart::toJson() const
{
    QJsonObject json;
    json["restaurantId"] = m_restaurantId;
    json["totalPrice"] = m_totalPrice;
    json["itemCount"] = m_itemCount;

    QJsonArray itemsArray;
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        QJsonObject itemObject;
        itemObject["item"] = it.key()->toJson();
        itemObject["quantity"] = it.value();
        itemsArray.append(itemObject);
    }
    json["items"] = itemsArray;

    return json;
}

void Cart::fromJson(const QJsonObject &json)
{
    clear();

    if (json.contains("restaurantId"))
        setRestaurantId(json["restaurantId"].toString());

    if (json.contains("items")) {
        QJsonArray itemsArray = json["items"].toArray();
        for (const QJsonValue &value : itemsArray) {
            if (value.isObject()) {
                QJsonObject itemObject = value.toObject();
                MenuItem* item = new MenuItem(itemObject["item"].toObject(), this);
                int quantity = itemObject["quantity"].toInt();
                addItem(item, quantity);
            }
        }
    }
}

void Cart::updateTotals()
{
    m_totalPrice = 0.0;
    m_itemCount = 0;

    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        m_totalPrice += it.key()->price() * it.value();
        m_itemCount += it.value();
    }

    emit totalPriceChanged();
    emit itemCountChanged();
}