#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QString>
#include <QObject>
#include <QDateTime>
#include <QJsonObject>

class ChatMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString fromUserId READ fromUserId WRITE setFromUserId NOTIFY fromUserIdChanged)
    Q_PROPERTY(QString toUserId READ toUserId WRITE setToUserId NOTIFY toUserIdChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
    Q_PROPERTY(bool isRead READ isRead WRITE setIsRead NOTIFY isReadChanged)

public:
    explicit ChatMessage(QObject *parent = nullptr);
    ChatMessage(const QJsonObject &json, QObject *parent = nullptr);

    // Getters
    QString id() const;
    QString fromUserId() const;
    QString toUserId() const;
    QString content() const;
    QDateTime timestamp() const;
    bool isRead() const;

    // Setters
    void setId(const QString &id);
    void setFromUserId(const QString &fromUserId);
    void setToUserId(const QString &toUserId);
    void setContent(const QString &content);
    void setTimestamp(const QDateTime &timestamp);
    void setIsRead(bool isRead);

    // JSON conversion
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    // Static methods
    static ChatMessage* fromJsonObject(const QJsonObject &json, QObject *parent = nullptr);
    static QList<ChatMessage*> fromJsonArray(const QJsonArray &array, QObject *parent = nullptr);

signals:
    void idChanged();
    void fromUserIdChanged();
    void toUserIdChanged();
    void contentChanged();
    void timestampChanged();
    void isReadChanged();

private:
    QString m_id;
    QString m_fromUserId;
    QString m_toUserId;
    QString m_content;
    QDateTime m_timestamp;
    bool m_isRead;
};

#endif // CHATMESSAGE_H 