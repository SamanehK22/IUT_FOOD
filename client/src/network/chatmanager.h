#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include "../models/chatmessage.h"
#include "networkmanager.h"

class ChatManager : public QObject
{
    Q_OBJECT

public:
    static ChatManager* getInstance();
    void sendMessage(const QString &toUserId, const QString &content);
    QList<ChatMessage*> getChatHistory(const QString &userId);
    void markMessageAsRead(const QString &messageId);
    void markAllMessagesAsRead(const QString &userId);
    int getUnreadMessageCount(const QString &userId) const;

signals:
    void messageReceived(ChatMessage* message);
    void messageSent(ChatMessage* message);
    void messageRead(const QString &messageId);
    void unreadCountChanged(const QString &userId, int count);

private:
    explicit ChatManager(QObject *parent = nullptr);
    ~ChatManager();
    static ChatManager* instance;

    NetworkManager* m_networkManager;
    QMap<QString, QList<ChatMessage*>> m_chatHistory; // userId -> messages
    QMap<QString, int> m_unreadCounts; // userId -> unread count

    void handleNewMessage(ChatMessage* message);
    void updateUnreadCount(const QString &userId);
};

#endif // CHATMANAGER_H 