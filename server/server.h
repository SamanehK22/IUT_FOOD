#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVector>
#include <memory>
#include "invoker.h"
#include "orderstatus.h"
#include "orderstatusobserver.h"
#include "databasemanager.h"
#include "authsystem.h"
#include "websocketserver.h"

class DatabaseManager;
class AuthSystem;
class WebSocketServer;

class Server : public QObject
{
    Q_OBJECT

public:
    static Server* getInstance();
    bool start(quint16 tcpPort = 1234, quint16 wsPort = 8081);
    void stop();

private:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    static Server* instance;

    QTcpServer* m_tcpServer;
    QMap<QTcpSocket*, QString> m_clients; // socket -> userId mapping
    DatabaseManager* m_dbManager;
    AuthSystem* m_authSystem;
    WebSocketServer* m_wsServer;
    std::unique_ptr<Invoker> invoker;
    std::unique_ptr<OrderStatus> orderStatus;
    std::unique_ptr<OrderStatusObserver> orderStatusObserver;

    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnection();
    void processRequest(QTcpSocket* client, const QJsonObject& request);
    void sendResponse(QTcpSocket* client, const QJsonObject& response);
    void broadcastOrderUpdate(const QString& orderId, const QString& status);

signals:
    void clientConnected(QTcpSocket* client);
    void clientDisconnected(QTcpSocket* client);
    void messageReceived(QTcpSocket* client, const QByteArray& message);
};

#endif // SERVER_H 