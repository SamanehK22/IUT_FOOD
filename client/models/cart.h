 #ifndef CART_H
#define CART_H

#include <QObject>
#include <QMap>
#include "menuitem.h"

class Cart : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double totalPrice READ totalPrice NOTIFY totalPriceChanged)
    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemCountChanged)
    Q_PROPERTY(QString restaurantId READ restaurantId WRITE setRestaurantId NOTIFY restaurantIdChanged)

public:
    explicit Cart(QObject *parent = nullptr);

    // Getters
    double totalPrice() const;
    int itemCount() const;
    QString restaurantId() const;
    QMap<MenuItem*, int> items() const;

    // Cart operations
    void addItem(MenuItem* item, int quantity = 1);
    void removeItem(MenuItem* item);
    void updateItemQuantity(MenuItem* item, int quantity);
    void clear();
    bool isEmpty() const;
    bool canAddItem(MenuItem* item) const;

    // Setters
    void setRestaurantId(const QString &restaurantId);

    // JSON conversion
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

signals:
    void totalPriceChanged();
    void itemCountChanged();
    void restaurantIdChanged();
    void itemAdded(MenuItem* item, int quantity);
    void itemRemoved(MenuItem* item);
    void itemQuantityChanged(MenuItem* item, int quantity);
    void cartCleared();

private:
    QMap<MenuItem*, int> m_items;
    QString m_restaurantId;
    double m_totalPrice;
    int m_itemCount;

    void updateTotals();
};

#endif // CART_H