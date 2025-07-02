#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class Restaurant : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(double rating READ rating WRITE setRating NOTIFY ratingChanged)
    Q_PROPERTY(int ratingCount READ ratingCount WRITE setRatingCount NOTIFY ratingCountChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)

public:
    explicit Restaurant(QObject *parent = nullptr);
    Restaurant(const QJsonObject &json, QObject *parent = nullptr);

    // Getters
    QString id() const;
    QString name() const;
    QString description() const;
    QString address() const;
    double latitude() const;
    double longitude() const;
    double rating() const;
    int ratingCount() const;
    QString type() const;
    QString imageUrl() const;

    // Setters
    void setId(const QString &id);
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setAddress(const QString &address);
    void setLatitude(double latitude);
    void setLongitude(double longitude);
    void setRating(double rating);
    void setRatingCount(int count);
    void setType(const QString &type);
    void setImageUrl(const QString &imageUrl);

    // JSON conversion
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    // Static methods
    static Restaurant* fromJsonObject(const QJsonObject &json, QObject *parent = nullptr);
    static QList<Restaurant*> fromJsonArray(const QJsonArray &array, QObject *parent = nullptr);

signals:
    void idChanged();
    void nameChanged();
    void descriptionChanged();
    void addressChanged();
    void latitudeChanged();
    void longitudeChanged();
    void ratingChanged();
    void ratingCountChanged();
    void typeChanged();
    void imageUrlChanged();

private:
    QString m_id;
    QString m_name;
    QString m_description;
    QString m_address;
    double m_latitude;
    double m_longitude;
    double m_rating;
    int m_ratingCount;
    QString m_type;
    QString m_imageUrl;
};

#endif // RESTAURANT_H 