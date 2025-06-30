#include "websocketserver.h"
#include <QDebug>

WebSocketServer* WebSocketServer::instance = nullptr;

WebSocketServer* WebSocketServer::getInstance()
{
    if (instance == nullptr) {
        instance = new WebSocketServer();
    }
    return instance;
}

WebSocketServer::WebSocketServer(QObject *parent)
    : QObject(parent)
    , m_server(nullptr)
    , m_authSystem(AuthSystem::getInstance())
{
}

WebSocketServer::~WebSocketServer()
{
    stop();
}

bool WebSocketServer::start(quint16 port)
{
    m_server = new QWebSocketServer(QStringLiteral("IUT Food Server"), QWebSocketServer::NonSecureMode, this);

    if (!m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "WebSocket server failed to start:" << m_server->errorString();
        return false;
    }

    connect(m_server, &QWebSocketServer::newConnection, this, &WebSocketServer::handleNewConnection);
    qDebug() << "WebSocket server started on port" << port;
    return true;
}

void WebSocketServer::stop()
{
    if (m_server) {
        m_server->close();
        qDeleteAll(m_clients);
        m_clients.clear();
        delete m_server;
        m_server = nullptr;
    }
}

void WebSocketServer::handleNewConnection()
{
    QWebSocket* socket = m_server->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, [this, socket](const QString& message) {
        handleTextMessage(message, socket);
    });
    connect(socket, &QWebSocket::disconnected, this, &WebSocketServer::handleDisconnection);
}

void WebSocketServer::handleTextMessage(const QString& message, QWebSocket* client)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) {
        sendError(client, "Invalid message format");
        return;
    }

    QJsonObject json = doc.object();
    QString type = json["type"].toString();

    if (type == "auth") {
        QString token = json["token"].toString();
        authenticateClient(client, token);
    }
    else if (type == "chat") {
        // Legacy chat (user-based, not order-based)
        handleChatMessage(json);
    }
    else if (type == "order_chat") {
        handleOrderChatMessage(json, client);
    }
    else if (type == "user_chat") {
        handleUserChatMessage(json, client);
    }
    else if (type == "order_update") {
        handleOrderUpdate(json);
    }
    else if (type == "get_order_chat_history") {
        // Fetch order-based chat history
        QString orderId = json["orderId"].toString();
        QJsonObject response;
        response["type"] = "order_chat_history";
        response["orderId"] = orderId;
        response["messages"] = DatabaseManager::getInstance()->getChatHistory(orderId);
        client->sendTextMessage(QJsonDocument(response).toJson());
    }
    else if (type == "get_user_chat_history") {
        QString userA = json["userA"].toString();
        QString userB = json["userB"].toString();
        QJsonObject response;
        response["type"] = "user_chat_history";
        response["userA"] = userA;
        response["userB"] = userB;
        response["messages"] = DatabaseManager::getInstance()->getUserChatHistory(userA, userB);
        client->sendTextMessage(QJsonDocument(response).toJson());
    }
    else {
        sendError(client, "Unknown message type");
    }
}

void WebSocketServer::handleDisconnection()
{
    QWebSocket* client = qobject_cast<QWebSocket*>(sender());
    if (client) {
        QString userId = m_clients.key(client);
        if (!userId.isEmpty()) {
            m_clients.remove(userId);
            qDebug() << "Client disconnected:" << userId;
        }
        client->deleteLater();
    }
}

void WebSocketServer::authenticateClient(QWebSocket* client, const QString& token)
{
    if (!m_authSystem->validateSession(token)) {
        sendError(client, "Invalid session token");
        return;
    }

    QString userId = m_authSystem->getUserIdFromToken(token);
    if (userId.isEmpty()) {
        sendError(client, "Invalid user ID");
        return;
    }

    // Remove any existing connection for this user
    if (m_clients.contains(userId)) {
        m_clients[userId]->close();
        m_clients.remove(userId);
    }

    m_clients[userId] = client;
    qDebug() << "Client authenticated:" << userId;

    // Send confirmation
    QJsonObject response;
    response["type"] = "auth_success";
    response["userId"] = userId;
    client->sendTextMessage(QJsonDocument(response).toJson());
}

void WebSocketServer::handleChatMessage(const QJsonObject& message)
{
    QString fromUserId = message["fromUserId"].toString();
    QString toUserId = message["toUserId"].toString();
    QString content = message["content"].toString();

    if (fromUserId.isEmpty() || toUserId.isEmpty() || content.isEmpty()) {
        return;
    }

    // Create chat message object
    QJsonObject chatMessage;
    chatMessage["type"] = "chat";
    chatMessage["fromUserId"] = fromUserId;
    chatMessage["content"] = content;
    chatMessage["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Send to recipient if online
    if (m_clients.contains(toUserId)) {
        m_clients[toUserId]->sendTextMessage(QJsonDocument(chatMessage).toJson());
    }

    // Send confirmation to sender
    if (m_clients.contains(fromUserId)) {
        QJsonObject confirmation;
        confirmation["type"] = "chat_sent";
        confirmation["toUserId"] = toUserId;
        confirmation["timestamp"] = chatMessage["timestamp"];
        m_clients[fromUserId]->sendTextMessage(QJsonDocument(confirmation).toJson());
    }
}

void WebSocketServer::handleOrderUpdate(const QJsonObject& message)
{
    QString orderId = message["orderId"].toString();
    QString status = message["status"].toString();
    QString userId = message["userId"].toString();

    if (orderId.isEmpty() || status.isEmpty() || userId.isEmpty()) {
        return;
    }

    broadcastOrderUpdate(orderId, status);
}

void WebSocketServer::broadcastOrderUpdate(const QString& orderId, const QString& status)
{
    QJsonObject update;
    update["type"] = "order_update";
    update["orderId"] = orderId;
    update["status"] = status;
    update["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Broadcast to all connected clients
    for (QWebSocket* client : m_clients) {
        client->sendTextMessage(QJsonDocument(update).toJson());
    }
}

void WebSocketServer::sendChatMessage(const QString& fromUserId, const QString& toUserId, const QString& message)
{
    QJsonObject chatMessage;
    chatMessage["type"] = "chat";
    chatMessage["fromUserId"] = fromUserId;
    chatMessage["content"] = message;
    chatMessage["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    if (m_clients.contains(toUserId)) {
        m_clients[toUserId]->sendTextMessage(QJsonDocument(chatMessage).toJson());
    }
}

void WebSocketServer::sendError(QWebSocket* client, const QString& error)
{
    QJsonObject errorMessage;
    errorMessage["type"] = "error";
    errorMessage["message"] = error;
    client->sendTextMessage(QJsonDocument(errorMessage).toJson());
}

void WebSocketServer::broadcastToUser(const QString& userId, const QJsonObject& message)
{
    if (m_clients.contains(userId)) {
        m_clients[userId]->sendTextMessage(QJsonDocument(message).toJson());
    }
}

void WebSocketServer::broadcastToRestaurant(const QString& restaurantId, const QJsonObject& message)
{
    // Find all restaurant staff members and send them the message
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
        QString userId = it.key();
        QString userType = m_authSystem->getUserTypeFromToken(userId);
        if (userType == "restaurant" && userId.startsWith(restaurantId)) {
            it.value()->sendTextMessage(QJsonDocument(message).toJson());
        }
    }
}

void WebSocketServer::handleOrderChatMessage(const QJsonObject& message, QWebSocket* client)
{
    QString orderId = message["orderId"].toString();
    QString fromUserId = message["fromUserId"].toString();
    QString toUserId = message["toUserId"].toString();
    QString content = message["content"].toString();
    if (orderId.isEmpty() || fromUserId.isEmpty() || toUserId.isEmpty() || content.isEmpty()) {
        sendError(client, "Missing fields in order chat message");
        return;
    }
    // Store in DB
    DatabaseManager::getInstance()->addChatMessage(orderId, fromUserId, toUserId, content);
    // Deliver in real time
    sendOrderChatMessage(orderId, fromUserId, toUserId, content);
}

void WebSocketServer::sendOrderChatMessage(const QString& orderId, const QString& fromUserId, const QString& toUserId, const QString& content)
{
    QJsonObject chatMessage;
    chatMessage["type"] = "order_chat";
    chatMessage["orderId"] = orderId;
    chatMessage["fromUserId"] = fromUserId;
    chatMessage["toUserId"] = toUserId;
    chatMessage["content"] = content;
    chatMessage["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    if (m_clients.contains(toUserId)) {
        m_clients[toUserId]->sendTextMessage(QJsonDocument(chatMessage).toJson());
    }
    // Optionally, send confirmation to sender
    if (m_clients.contains(fromUserId)) {
        QJsonObject confirmation = chatMessage;
        confirmation["type"] = "order_chat_sent";
        m_clients[fromUserId]->sendTextMessage(QJsonDocument(confirmation).toJson());
    }
}

void WebSocketServer::handleUserChatMessage(const QJsonObject& message, QWebSocket* client)
{
    QString fromUserId = message["fromUserId"].toString();
    QString toUserId = message["toUserId"].toString();
    QString content = message["content"].toString();
    if (fromUserId.isEmpty() || toUserId.isEmpty() || content.isEmpty()) {
        sendError(client, "Missing fields in user chat message");
        return;
    }
    // Store in DB
    DatabaseManager::getInstance()->addUserChatMessage(fromUserId, toUserId, content);
    // Deliver in real time
    sendUserChatMessage(fromUserId, toUserId, content);
}

void WebSocketServer::sendUserChatMessage(const QString& fromUserId, const QString& toUserId, const QString& content)
{
    QJsonObject chatMessage;
    chatMessage["type"] = "user_chat";
    chatMessage["fromUserId"] = fromUserId;
    chatMessage["toUserId"] = toUserId;
    chatMessage["content"] = content;
    chatMessage["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    if (m_clients.contains(toUserId)) {
        m_clients[toUserId]->sendTextMessage(QJsonDocument(chatMessage).toJson());
    }
    // Optionally, send confirmation to sender
    if (m_clients.contains(fromUserId)) {
        QJsonObject confirmation = chatMessage;
        confirmation["type"] = "user_chat_sent";
        m_clients[fromUserId]->sendTextMessage(QJsonDocument(confirmation).toJson());
    }
} 
