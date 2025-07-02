#ifndef MENUITEM_H
#define MENUITEM_H

#include <QString>
#include <QObject>
#include <QJsonObject>

class MenuItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(double price READ price WRITE setPrice NOTIFY priceChanged)
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(bool available READ available WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(QString ingredients READ ingredients WRITE setIngredients NOTIFY ingredientsChanged)

public:
    explicit MenuItem(QObject *parent = nullptr);
    MenuItem(const QJsonObject &json, QObject *parent = nullptr);

    // Getters
    QString id() const;
    QString name() const;
    QString description() const;
    double price() const;
    QString category() const;
    QString imageUrl() const;
    bool available() const;
    QString ingredients() const;

    // Setters
    void setId(const QString &id);
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setPrice(double price);
    void setCategory(const QString &category);
    void setImageUrl(const QString &url);
    void setAvailable(bool available);
    void setIngredients(const QString &ingredients);

    // JSON conversion
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    // Static methods
    static MenuItem* fromJsonObject(const QJsonObject &json, QObject *parent = nullptr);

signals:
    void idChanged();
    void nameChanged();
    void descriptionChanged();
    void priceChanged();
    void categoryChanged();
    void imageUrlChanged();
    void availableChanged();
    void ingredientsChanged();

private:
    QString m_id;
    QString m_name;
    QString m_description;
    double m_price;
    QString m_category;
    QString m_imageUrl;
    bool m_available;
    QString m_ingredients;
};

#endif // MENUITEM_H