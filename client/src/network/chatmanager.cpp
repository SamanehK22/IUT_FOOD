#include "chatmanager.h"
#include <QDebug>

ChatManager* ChatManager::instance = nullptr;

ChatManager* ChatManager::getInstance()
{
    if (instance == nullptr) {
        instance = new ChatManager();
    }
    return instance;
}

ChatManager::ChatManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(NetworkManager::getInstance())
{
    // Connect to NetworkManager signals
    connect(m_networkManager, &NetworkManager::chatMessageReceived,
            this, [this](const QString &fromUserId, const QString &message) {
        ChatMessage* chatMessage = new ChatMessage(this);
        chatMessage->setFromUserId(fromUserId);
        chatMessage->setContent(message);
        chatMessage->setTimestamp(QDateTime::currentDateTime());
        handleNewMessage(chatMessage);
    });
}

ChatManager::~ChatManager()
{
    // Clean up chat history
    for (auto it = m_chatHistory.begin(); it != m_chatHistory.end(); ++it) {
        qDeleteAll(it.value());
    }
    m_chatHistory.clear();
}

void ChatManager::sendMessage(const QString &toUserId, const QString &content)
{
    if (content.isEmpty()) return;

    ChatMessage* message = new ChatMessage(this);
    message->setToUserId(toUserId);
    message->setContent(content);
    message->setTimestamp(QDateTime::currentDateTime());

    m_networkManager->sendChatMessage(toUserId, content);
    handleNewMessage(message);
    emit messageSent(message);
}

QList<ChatMessage*> ChatManager::getChatHistory(const QString &userId)
{
    return m_chatHistory.value(userId);
}

void ChatManager::markMessageAsRead(const QString &messageId)
{
    for (auto it = m_chatHistory.begin(); it != m_chatHistory.end(); ++it) {
        for (ChatMessage* message : it.value()) {
            if (message->id() == messageId && !message->isRead()) {
                message->setIsRead(true);
                updateUnreadCount(it.key());
                emit messageRead(messageId);
                break;
            }
        }
    }
}

void ChatManager::markAllMessagesAsRead(const QString &userId)
{
    if (!m_chatHistory.contains(userId)) return;

    for (ChatMessage* message : m_chatHistory[userId]) {
        if (!message->isRead()) {
            message->setIsRead(true);
        }
    }

    updateUnreadCount(userId);
}

int ChatManager::getUnreadMessageCount(const QString &userId) const
{
    return m_unreadCounts.value(userId, 0);
}

void ChatManager::handleNewMessage(ChatMessage* message)
{
    if (!message) return;

    QString userId = message->fromUserId();
    if (!m_chatHistory.contains(userId)) {
        m_chatHistory[userId] = QList<ChatMessage*>();
    }

    m_chatHistory[userId].append(message);
    updateUnreadCount(userId);
    emit messageReceived(message);
}

void ChatManager::updateUnreadCount(const QString &userId)
{
    if (!m_chatHistory.contains(userId)) {
        m_unreadCounts.remove(userId);
        emit unreadCountChanged(userId, 0);
        return;
    }

    int count = 0;
    for (const ChatMessage* message : m_chatHistory[userId]) {
        if (!message->isRead()) {
            count++;
        }
    }

    m_unreadCounts[userId] = count;
    emit unreadCountChanged(userId, count);
} 