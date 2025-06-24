#include "server.h"
#include <QDebug>

Server* Server::instance = nullptr;

Server* Server::getInstance()
{
    if (instance == nullptr) {
        instance = new Server();
    }
    return instance;
}

Server::Server(QObject *parent)
    : QObject(parent)
    , m_tcpServer(nullptr)
    , m_dbManager(DatabaseManager::getInstance())
    , m_authSystem(AuthSystem::getInstance())
    , m_wsServer(WebSocketServer::getInstance())
{
}

Server::~Server()
{
    stop();
}

bool Server::start(quint16 tcpPort, quint16 wsPort)
{
    // Start TCP server
    m_tcpServer = new QTcpServer(this);
    if (!m_tcpServer->listen(QHostAddress::Any, tcpPort)) {
        qDebug() << "TCP server failed to start:" << m_tcpServer->errorString();
        return false;
    }

    // Connect signals to handler methods directly
    connect(m_tcpServer, &QTcpServer::newConnection, this, &Server::handleNewConnection);
    qDebug() << "TCP server started on port" << tcpPort;

    // Start WebSocket server
    if (!m_wsServer->start(wsPort)) {
        qDebug() << "WebSocket server failed to start";
        m_tcpServer->close();
        return false;
    }

    qDebug() << "Server started successfully";
    return true;
}

void Server::stop()
{
    if (m_tcpServer) {
        m_tcpServer->close();
        delete m_tcpServer;
        m_tcpServer = nullptr;
    }

    m_wsServer->stop();
        qDebug() << "Server stopped";
}

void Server::handleNewConnection()
{
    QTcpSocket* client = m_tcpServer->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &Server::handleReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &Server::handleDisconnection);
    qDebug() << "New client connected:" << client->peerAddress().toString();
}

void Server::handleReadyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QByteArray data = client->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        QJsonObject error;
        error["status"] = "error";
        error["message"] = "Invalid request format";
        sendResponse(client, error);
        return;
    }

    processRequest(client, doc.object());
}

void Server::handleDisconnection()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        m_clients.remove(client);
        client->deleteLater();
        qDebug() << "Client disconnected:" << client->peerAddress().toString();
    }
}

void Server::processRequest(QTcpSocket* client, const QJsonObject& request)
{
    QString type = request["type"].toString();
    QJsonObject response;

    if (type == "login") {
        QString username = request["username"].toString();
        QString password = request["password"].toString();
        QString token = m_authSystem->login(username, password);
        
        if (!token.isEmpty()) {
        response["status"] = "success";
            response["token"] = token;
            m_clients[client] = m_authSystem->getUserIdFromToken(token);
        } else {
            response["status"] = "error";
            response["message"] = "Invalid credentials";
        }
    }
    else if (type == "register") {
        QString username = request["username"].toString();
        QString password = request["password"].toString();
        QString email = request["email"].toString();
        QString userType = request["userType"].toString();

        if (m_authSystem->registerUser(username, password, email, userType)) {
        response["status"] = "success";
        response["message"] = "Registration successful";
        } else {
            response["status"] = "error";
            response["message"] = "Registration failed";
        }
    }
    else if (type == "order") {
        QString token = request["token"].toString();
        if (!m_authSystem->validateSession(token)) {
            response["status"] = "error";
            response["message"] = "Invalid session";
            sendResponse(client, response);
            return;
        }

        QString customerId = m_authSystem->getUserIdFromToken(token);
        QString restaurantId = request["restaurantId"].toString();
        QJsonArray items = request["items"].toArray();

        if (m_dbManager->createOrder(customerId, restaurantId, items.toVariantList())) {
            response["status"] = "success";
            response["message"] = "Order created successfully";
            
            // Notify restaurant about new order
            QJsonObject notification;
            notification["type"] = "new_order";
            notification["orderId"] = response["orderId"].toString();
            m_wsServer->broadcastToRestaurant(restaurantId, notification);
        } else {
            response["status"] = "error";
            response["message"] = "Failed to create order";
        }
    }
    else if (type == "update_order_status") {
        QString token = request["token"].toString();
        if (!m_authSystem->validateSession(token)) {
            response["status"] = "error";
            response["message"] = "Invalid session";
            sendResponse(client, response);
            return;
        }

        QString orderId = request["orderId"].toString();
        QString status = request["status"].toString();
        QString userId = m_authSystem->getUserIdFromToken(token);

        if (m_dbManager->updateOrderStatus(orderId, status)) {
            response["status"] = "success";
            response["message"] = "Order status updated";
            
            // Broadcast status update
            broadcastOrderUpdate(orderId, status);
        } else {
            response["status"] = "error";
            response["message"] = "Failed to update order status";
        }
    }
    else {
        response["status"] = "error";
        response["message"] = "Unknown request type";
    }

    sendResponse(client, response);
}

void Server::sendResponse(QTcpSocket* client, const QJsonObject& response)
{
    QJsonDocument doc(response);
    client->write(doc.toJson());
}

void Server::broadcastOrderUpdate(const QString& orderId, const QString& status)
{
    m_wsServer->broadcastOrderUpdate(orderId, status);
} 