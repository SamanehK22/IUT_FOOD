#include "networkmanager.h"
#include <QDebug>
#include <QJsonArray>

NetworkManager* NetworkManager::instance = nullptr;

NetworkManager* NetworkManager::getInstance()
{
    if (instance == nullptr) {
        instance = new NetworkManager();
    }
    return instance;
}

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_tcpSocket(nullptr)
    , m_webSocket(nullptr)
    , m_reconnectTimer(nullptr)
    , m_isConnected(false)
{
    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setInterval(5000); // 5 seconds
    connect(m_reconnectTimer, &QTimer::timeout, this, &NetworkManager::reconnect);
}

NetworkManager::~NetworkManager()
{
    disconnect();
}

bool NetworkManager::connectToServer(const QString &host, quint16 tcpPort, quint16 wsPort)
{
    // Create TCP socket if not exists
    if (!m_tcpSocket) {
        m_tcpSocket = new QTcpSocket(this);
        setupConnections();
    }

    // Create WebSocket if not exists
    if (!m_webSocket) {
        m_webSocket = new QWebSocket();
        connect(m_webSocket, &QWebSocket::textMessageReceived,
                this, &NetworkManager::handleWebSocketMessage);
        connect(m_webSocket, &QWebSocket::connected,
                this, &NetworkManager::authenticateWebSocket);
        connect(m_webSocket, &QWebSocket::disconnected,
                this, [this]() {
            m_isConnected = false;
            emit disconnected();
            m_reconnectTimer->start();
        });
    }

    // Connect TCP socket
    m_tcpSocket->connectToHost(host, tcpPort);
    if (!m_tcpSocket->waitForConnected(5000)) {
        emit error("Failed to connect to TCP server");
        return false;
    }

    // Connect WebSocket
    m_webSocket->open(QUrl(QString("ws://%1:%2").arg(host).arg(wsPort)));

    m_isConnected = true;
    emit connected();
    return true;
}

void NetworkManager::disconnect()
{
    if (m_tcpSocket) {
        m_tcpSocket->disconnectFromHost();
        m_tcpSocket->deleteLater();
        m_tcpSocket = nullptr;
    }

    if (m_webSocket) {
        m_webSocket->close();
        m_webSocket->deleteLater();
        m_webSocket = nullptr;
    }

    m_isConnected = false;
    m_token.clear();
    emit disconnected();
}

bool NetworkManager::isConnected() const
{
    return m_isConnected;
}

void NetworkManager::login(const QString &loginId, const QString &password)
{
    QJsonObject request;
    request["type"] = "login";
    request["loginId"] = loginId;
    request["password"] = password;
    sendTcpRequest(request);
}

void NetworkManager::registerUser(const QString &firstName, const QString &lastName, const QString &email, const QString &phone, const QString &password, const QString &userType, const QString &restaurantName, const QString &city, const QString &location)
{
    QJsonObject request;
    request["type"] = "register";
    request["firstName"] = firstName;
    request["lastName"] = lastName;
    request["email"] = email;
    request["phone"] = phone;
    request["password"] = password;
    request["userType"] = userType;
    if (userType == "restaurant_owner") {
        if (!restaurantName.isEmpty()) request["restaurantName"] = restaurantName;
        if (!city.isEmpty()) request["city"] = city;
        if (!location.isEmpty()) request["location"] = location;
    }
    sendTcpRequest(request);
}

void NetworkManager::logout()
{
    m_token.clear();
    emit disconnected();
}

void NetworkManager::getRestaurants()
{
    QJsonObject request;
    request["type"] = "get_restaurants";
    request["token"] = m_token;
    sendTcpRequest(request);
}

void NetworkManager::getRestaurantMenu(const QString &restaurantId)
{
    QJsonObject request;
    request["type"] = "get_restaurant_menu";
    request["token"] = m_token;
    request["restaurantId"] = restaurantId;
    sendTcpRequest(request);
}

void NetworkManager::getRestaurantOrders(const QString &restaurantId)
{
    QJsonObject request;
    request["type"] = "get_orders";
    request["token"] = m_token;
    request["restaurantId"] = restaurantId;
    sendTcpRequest(request);
}

void NetworkManager::placeOrder(const QString &restaurantId, const QJsonArray &items)
{
    QJsonObject request;
    request["type"] = "order";
    request["token"] = m_token;
    request["restaurantId"] = restaurantId;
    request["items"] = items;
    sendTcpRequest(request);
}

void NetworkManager::getOrderStatus(const QString &orderId)
{
    QJsonObject request;
    request["type"] = "get_order_status";
    request["token"] = m_token;
    request["orderId"] = orderId;
    sendTcpRequest(request);
}

void NetworkManager::updateOrderStatus(const QString &orderId, const QString &status)
{
    QJsonObject request;
    request["type"] = "update_order_status";
    request["token"] = m_token;
    request["orderId"] = orderId;
    request["status"] = status;
    sendTcpRequest(request);
}

void NetworkManager::sendChatMessage(const QString &toUserId, const QString &message)
{
    QJsonObject request;
    request["type"] = "chat";
    request["token"] = m_token;
    request["toUserId"] = toUserId;
    request["content"] = message;
    sendTcpRequest(request);
}

void NetworkManager::getCustomerProfile(const QString& customerId) {
    QJsonObject request;
    request["type"] = "get_customer_profile";
    request["customerId"] = customerId;
    sendTcpRequest(request);
}

void NetworkManager::getCustomerOrderHistory(const QString& customerId) {
    QJsonObject request;
    request["type"] = "get_customer_order_history";
    request["customerId"] = customerId;
    sendTcpRequest(request);
}

void NetworkManager::getOwnerProfile(const QString& ownerId) {
    QJsonObject request;
    request["type"] = "get_owner_profile";
    request["ownerId"] = ownerId;
    sendTcpRequest(request);
}

void NetworkManager::getOwnerRestaurants(const QString& ownerId) {
    QJsonObject request;
    request["type"] = "get_owner_restaurants";
    request["ownerId"] = ownerId;
    sendTcpRequest(request);
}

void NetworkManager::createOrder(const QString& customerId, const QString& restaurantId, const QJsonArray& items) {
    QJsonObject request;
    request["type"] = "create_order";
    request["customerId"] = customerId;
    request["restaurantId"] = restaurantId;
    request["items"] = items;
    sendTcpRequest(request);
}

void NetworkManager::getOrderHistory(const QString& userId, const QString& userType) {
    QJsonObject request;
    request["type"] = "get_order_history";
    request["userId"] = userId;
    request["userType"] = userType;
    sendTcpRequest(request);
}

void NetworkManager::createMenu(const QString& restaurantId) {
    QJsonObject request;
    request["type"] = "create_menu";
    request["restaurantId"] = restaurantId;
    sendTcpRequest(request);
}

void NetworkManager::addMenuItem(const QString& menuId, const QString& name, const QString& description, double price, const QString& ingredients, const QString& category, const QString& imageUrl) {
    QJsonObject request;
    request["type"] = "add_menu_item";
    request["menuId"] = menuId;
    request["name"] = name;
    request["description"] = description;
    request["price"] = price;
    request["ingredients"] = ingredients;
    request["category"] = category;
    request["imageUrl"] = imageUrl;
    sendTcpRequest(request);
}

void NetworkManager::updateMenuItem(const QString& menuId, const QString& itemId, const QJsonObject& updates) {
    QJsonObject request;
    request["type"] = "update_menu_item";
    request["menuId"] = menuId;
    request["itemId"] = itemId;
    request["updates"] = updates;
    sendTcpRequest(request);
}

void NetworkManager::deleteMenuItem(const QString& menuId, const QString& itemId) {
    QJsonObject request;
    request["type"] = "delete_menu_item";
    request["menuId"] = menuId;
    request["itemId"] = itemId;
    sendTcpRequest(request);
}

void NetworkManager::getMenu(const QString& restaurantId) {
    QJsonObject request;
    request["type"] = "get_menu";
    request["restaurantId"] = restaurantId;
    sendTcpRequest(request);
}

void NetworkManager::addOrderComment(const QString& orderId, const QString& customerId, const QString& comment) {
    QJsonObject request;
    request["type"] = "add_order_comment";
    request["orderId"] = orderId;
    request["customerId"] = customerId;
    request["comment"] = comment;
    sendTcpRequest(request);
}

void NetworkManager::getOrderComments(const QString& orderId) {
    QJsonObject request;
    request["type"] = "get_order_comments";
    request["orderId"] = orderId;
    sendTcpRequest(request);
}

void NetworkManager::sendOrderChatMessage(const QString& orderId, const QString& toUserId, const QString& content)
{
    QJsonObject msg;
    msg["type"] = "order_chat";
    msg["orderId"] = orderId;
    msg["fromUserId"] = m_token; // or userId if available
    msg["toUserId"] = toUserId;
    msg["content"] = content;
    m_webSocket->sendTextMessage(QJsonDocument(msg).toJson());
}

void NetworkManager::getOrderChatHistory(const QString& orderId)
{
    QJsonObject req;
    req["type"] = "get_order_chat_history";
    req["orderId"] = orderId;
    m_webSocket->sendTextMessage(QJsonDocument(req).toJson());
}

void NetworkManager::sendUserChatMessage(const QString& toUserId, const QString& content)
{
    QJsonObject msg;
    msg["type"] = "user_chat";
    msg["fromUserId"] = m_token; // or userId if available
    msg["toUserId"] = toUserId;
    msg["content"] = content;
    m_webSocket->sendTextMessage(QJsonDocument(msg).toJson());
}

void NetworkManager::getUserChatHistory(const QString& userA, const QString& userB)
{
    QJsonObject req;
    req["type"] = "get_user_chat_history";
    req["userA"] = userA;
    req["userB"] = userB;
    m_webSocket->sendTextMessage(QJsonDocument(req).toJson());
}

void NetworkManager::handleTcpResponse(const QJsonObject &response)
{
    QString type = response["type"].toString();
    QString status = response["status"].toString();

    if (status == "error") {
        emit error(response["message"].toString());
        return;
    }

    if (type == "login") {
        if (status == "success") {
            m_token = response["token"].toString();
            emit loginSuccess(response);
        } else {
            emit loginFailed(response["message"].toString());
        }
    }
    else if (type == "register") {
        if (status == "success") {
            emit registerSuccess();
        } else {
            emit registerFailed(response["message"].toString());
        }
    }
    else if (type == "get_restaurants") {
        emit restaurantsReceived(response["restaurants"].toArray());
    }
    else if (type == "get_restaurant_menu") {
        emit menuReceived(response["restaurantId"].toString(),
                         response["menu"].toArray());
    }
    else if (type == "get_menu") {
        emit menuDataReceived(response["menu"].toArray());
    }
    else if (type == "order") {
        if (status == "success") {
            emit orderPlaced(response["orderId"].toString());
        }
    }
    else if (type == "get_order_status") {
        emit orderStatusUpdated(response["orderId"].toString(),
                              response["status"].toString());
    }
    else if (type == "get_customer_profile") {
        emit customerProfileReceived(response["profile"].toObject());
    }
    else if (type == "get_customer_order_history") {
        emit customerOrderHistoryReceived(response["order_history"].toArray());
    }
    else if (type == "get_owner_profile") {
        emit ownerProfileReceived(response["profile"].toObject());
    }
    else if (type == "get_owner_restaurants") {
        emit ownerRestaurantsReceived(response["restaurants"].toArray());
    }
    else if (type == "create_order") {
        emit orderCreated(status == "success");
    }
    else if (type == "update_order_status") {
        // Removed invalid emit orderStatusUpdated(status == "success");
    }
    else if (type == "get_order_history") {
        emit orderHistoryReceived(response["order_history"].toArray());
    }
    else if (type == "create_menu") {
        emit menuCreated(status == "success");
    }
    else if (type == "add_menu_item") {
        emit menuItemAdded(status == "success");
    }
    else if (type == "update_menu_item") {
        emit menuItemUpdated(status == "success");
    }
    else if (type == "delete_menu_item") {
        emit menuItemDeleted(status == "success");
    }
    else if (type == "add_order_comment") {
        emit orderCommentAdded(status == "success");
    }
    else if (type == "get_order_comments") {
        emit orderCommentsReceived(response["comments"].toArray());
    }
    else if (type == "order_chat") {
        emit orderChatMessageReceived(response["orderId"].toString(), response["fromUserId"].toString(), response["toUserId"].toString(), response["content"].toString(), response["timestamp"].toString());
    }
    else if (type == "order_chat_history") {
        emit orderChatHistoryReceived(response["orderId"].toString(), response["messages"].toArray());
    }
    else if (type == "user_chat") {
        emit userChatMessageReceived(response["fromUserId"].toString(), response["toUserId"].toString(), response["content"].toString(), response["timestamp"].toString());
    }
    else if (type == "user_chat_history") {
        emit userChatHistoryReceived(response["userA"].toString(), response["userB"].toString(), response["messages"].toArray());
    }
}

void NetworkManager::handleWebSocketMessage(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) {
        emit error("Invalid WebSocket message format");
        return;
    }
    QJsonObject json = doc.object();
    QString type = json["type"].toString();
    if (type == "order_update") {
        emit orderStatusUpdated(json["orderId"].toString(), json["status"].toString());
    }
    else if (type == "chat") {
        emit chatMessageReceived(json["fromUserId"].toString(), json["content"].toString());
    }
    else if (type == "order_chat") {
        emit orderChatMessageReceived(json["orderId"].toString(), json["fromUserId"].toString(), json["toUserId"].toString(), json["content"].toString(), json["timestamp"].toString());
    }
    else if (type == "order_chat_history") {
        emit orderChatHistoryReceived(json["orderId"].toString(), json["messages"].toArray());
    }
    else if (type == "user_chat") {
        emit userChatMessageReceived(json["fromUserId"].toString(), json["toUserId"].toString(), json["content"].toString(), json["timestamp"].toString());
    }
    else if (type == "user_chat_history") {
        emit userChatHistoryReceived(json["userA"].toString(), json["userB"].toString(), json["messages"].toArray());
    }
}

void NetworkManager::sendTcpRequest(const QJsonObject &request)
{
    if (!m_tcpSocket || !m_tcpSocket->isValid()) {
        emit error("Not connected to server");
        return;
    }

    QJsonDocument doc(request);
    m_tcpSocket->write(doc.toJson());
}

void NetworkManager::authenticateWebSocket()
{
    if (!m_token.isEmpty()) {
        QJsonObject auth;
        auth["type"] = "auth";
        auth["token"] = m_token;
        m_webSocket->sendTextMessage(QJsonDocument(auth).toJson());
    }
}

void NetworkManager::setupConnections()
{
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, [this]() {
        QByteArray data = m_tcpSocket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isObject()) {
            handleTcpResponse(doc.object());
        }
    });

    connect(m_tcpSocket, &QTcpSocket::disconnected, this, [this]() {
        m_isConnected = false;
        emit disconnected();
        m_reconnectTimer->start();
    });

    connect(m_tcpSocket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError /*error*/) {
        emit this->error(m_tcpSocket->errorString());
    });
}

void NetworkManager::reconnect()
{
    if (!m_isConnected) {
        m_reconnectTimer->stop();
        connectToServer();
    }
} 