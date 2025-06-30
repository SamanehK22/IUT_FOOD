#include "chatmessage.h"
#include <QJsonArray>
#include <QJsonValue>

ChatMessage::ChatMessage(QObject *parent)
    : QObject(parent)
    , m_id("")
    , m_fromUserId("")
    , m_toUserId("")
    , m_content("")
    , m_timestamp(QDateTime::currentDateTime())
    , m_isRead(false)
{
}

ChatMessage::ChatMessage(const QJsonObject &json, QObject *parent)
    : QObject(parent)
    , m_timestamp(QDateTime::currentDateTime())
    , m_isRead(false)
{
    fromJson(json);
}

QString ChatMessage::id() const
{
    return m_id;
}

QString ChatMessage::fromUserId() const
{
    return m_fromUserId;
}

QString ChatMessage::toUserId() const
{
    return m_toUserId;
}

QString ChatMessage::content() const
{
    return m_content;
}

QDateTime ChatMessage::timestamp() const
{
    return m_timestamp;
}

bool ChatMessage::isRead() const
{
    return m_isRead;
}

void ChatMessage::setId(const QString &id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

void ChatMessage::setFromUserId(const QString &fromUserId)
{
    if (m_fromUserId != fromUserId) {
        m_fromUserId = fromUserId;
        emit fromUserIdChanged();
    }
}

void ChatMessage::setToUserId(const QString &toUserId)
{
    if (m_toUserId != toUserId) {
        m_toUserId = toUserId;
        emit toUserIdChanged();
    }
}

void ChatMessage::setContent(const QString &content)
{
    if (m_content != content) {
        m_content = content;
        emit contentChanged();
    }
}

void ChatMessage::setTimestamp(const QDateTime &timestamp)
{
    if (m_timestamp != timestamp) {
        m_timestamp = timestamp;
        emit timestampChanged();
    }
}

void ChatMessage::setIsRead(bool isRead)
{
    if (m_isRead != isRead) {
        m_isRead = isRead;
        emit isReadChanged();
    }
}

QJsonObject ChatMessage::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["fromUserId"] = m_fromUserId;
    json["toUserId"] = m_toUserId;
    json["content"] = m_content;
    json["timestamp"] = m_timestamp.toString(Qt::ISODate);
    json["isRead"] = m_isRead;
    return json;
}

void ChatMessage::fromJson(const QJsonObject &json)
{
    if (json.contains("id"))
        setId(json["id"].toString());
    if (json.contains("fromUserId"))
        setFromUserId(json["fromUserId"].toString());
    if (json.contains("toUserId"))
        setToUserId(json["toUserId"].toString());
    if (json.contains("content"))
        setContent(json["content"].toString());
    if (json.contains("timestamp"))
        setTimestamp(QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate));
    if (json.contains("isRead"))
        setIsRead(json["isRead"].toBool());
}

ChatMessage* ChatMessage::fromJsonObject(const QJsonObject &json, QObject *parent)
{
    return new ChatMessage(json, parent);
}

QList<ChatMessage*> ChatMessage::fromJsonArray(const QJsonArray &array, QObject *parent)
{
    QList<ChatMessage*> messages;
    for (const QJsonValue &value : array) {
        if (value.isObject()) {
            messages.append(new ChatMessage(value.toObject(), parent));
        }
    }
    return messages;
} 