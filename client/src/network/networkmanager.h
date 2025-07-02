#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include "../models/user.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    static NetworkManager* getInstance();
    bool connectToServer(const QString &host = "localhost", quint16 tcpPort = 8080, quint16 wsPort = 8081);
    void disconnect();
    bool isConnected() const;

    // Authentication
    void login(const QString &loginId, const QString &password);
    void registerUser(const QString &firstName, const QString &lastName, const QString &email, const QString &phone, const QString &password, const QString &userType, const QString &restaurantName = "", const QString &city = "", const QString &location = "");
    void logout();

    // Restaurant operations
    void getRestaurants();
    void getRestaurantMenu(const QString &restaurantId);
    void getRestaurantOrders(const QString &restaurantId);

    // Order operations
    void placeOrder(const QString &restaurantId, const QJsonArray &items);
    void getOrderStatus(const QString &orderId);
    void updateOrderStatus(const QString &orderId, const QString &status);

    // Chat operations
    void sendChatMessage(const QString &toUserId, const QString &message);

    // New server endpoints
    void getCustomerProfile(const QString& customerId);
    void getCustomerOrderHistory(const QString& customerId);
    void getOwnerProfile(const QString& ownerId);
    void getOwnerRestaurants(const QString& ownerId);
    void createOrder(const QString& customerId, const QString& restaurantId, const QJsonArray& items);
    void getOrderHistory(const QString& userId, const QString& userType);
    void createMenu(const QString& restaurantId);
    void addMenuItem(const QString& menuId, const QString& name, const QString& description, double price, const QString& ingredients, const QString& category, const QString& imageUrl = "");
    void updateMenuItem(const QString& menuId, const QString& itemId, const QJsonObject& updates);
    void deleteMenuItem(const QString& menuId, const QString& itemId);
    void getMenu(const QString& restaurantId);

    // Order feedback (comments)
    void addOrderComment(const QString& orderId, const QString& customerId, const QString& comment);
    void getOrderComments(const QString& orderId);

    // Order-based chat
    void sendOrderChatMessage(const QString& orderId, const QString& toUserId, const QString& content);
    void getOrderChatHistory(const QString& orderId);

    // User-based chat
    void sendUserChatMessage(const QString& toUserId, const QString& content);
    void getUserChatHistory(const QString& userA, const QString& userB);

signals:
    void connected();
    void disconnected();
    void error(const QString &message);
    void loginSuccess(const QJsonObject &userData);
    void loginFailed(const QString &error);
    void registerSuccess();
    void registerFailed(const QString &error);
    void restaurantsReceived(const QJsonArray &restaurants);
    void menuReceived(const QString &restaurantId, const QJsonArray &menu);
    void orderStatusUpdated(const QString &orderId, const QString &status);
    void chatMessageReceived(const QString &fromUserId, const QString &message);
    void orderPlaced(const QString &orderId);
    void customerProfileReceived(const QJsonObject& profile);
    void customerOrderHistoryReceived(const QJsonArray& orderHistory);
    void ownerProfileReceived(const QJsonObject& profile);
    void ownerRestaurantsReceived(const QJsonArray& restaurants);
    void orderCreated(bool success);
    void orderHistoryReceived(const QJsonArray& orderHistory);
    void menuCreated(bool success);
    void menuItemAdded(bool success);
    void menuItemUpdated(bool success);
    void menuItemDeleted(bool success);
    void menuDataReceived(const QJsonArray& menu);
    void orderCommentAdded(bool success);
    void orderCommentsReceived(const QJsonArray& comments);
    void orderChatMessageReceived(const QString& orderId, const QString& fromUserId, const QString& toUserId, const QString& content, const QString& timestamp);
    void orderChatHistoryReceived(const QString& orderId, const QJsonArray& messages);
    void userChatMessageReceived(const QString& fromUserId, const QString& toUserId, const QString& content, const QString& timestamp);
    void userChatHistoryReceived(const QString& userA, const QString& userB, const QJsonArray& messages);

private:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();
    static NetworkManager* instance;

    QTcpSocket* m_tcpSocket;
    QWebSocket* m_webSocket;
    QString m_token;
    QTimer* m_reconnectTimer;
    bool m_isConnected;

    void handleTcpResponse(const QJsonObject &response);
    void handleWebSocketMessage(const QString &message);
    void sendTcpRequest(const QJsonObject &request);
    void authenticateWebSocket();
    void setupConnections();
    void reconnect();
};

#endif // NETWORKMANAGER_H 