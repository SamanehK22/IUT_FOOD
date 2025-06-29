#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariantMap>
#include <QVariantList>
#include <QJsonArray>
#include <memory>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager* getInstance();

    // Connection Management
    bool connect(const QString& host, const QString& database, const QString& username, const QString& password);
    void disconnect();
    bool isConnected() const;

    // Customer Management
    bool createCustomer(const QString& firstName, const QString& lastName, const QString& username, const QString& email, 
                       const QString& passwordHash, const QString& phone, const QString& address, const QString& city, const QString& location);
    bool updateCustomer(const QString& customerId, const QVariantMap& updates);
    bool deleteCustomer(const QString& customerId);
    QString getCustomerId(const QString& email);
    QString getCustomerPasswordHash(const QString& customerId);
    QVariantMap getCustomerProfile(const QString& customerId);
    QVariantMap getCustomerByLoginId(const QString& loginId);

    // Restaurant Owner Management
    bool createRestaurantOwner(const QString& firstName, const QString& lastName, const QString& username, const QString& email,
                             const QString& passwordHash, const QString& phone, const QString& restaurantId, const QString& city, const QString& location);
    bool updateRestaurantOwner(const QString& ownerId, const QVariantMap& updates);
    bool deleteRestaurantOwner(const QString& ownerId);
    QString getRestaurantOwnerId(const QString& email);
    QString getRestaurantOwnerPasswordHash(const QString& ownerId);
    QVariantMap getRestaurantOwnerProfile(const QString& ownerId);
    QString getRestaurantIdByOwner(const QString& ownerId);
    QVariantMap getRestaurantOwnerByLoginId(const QString& loginId);

    // Restaurant Management
    bool createRestaurant(const QString& name, const QString& address, const QString& type, const QString& imageUrl = "");
    bool updateRestaurant(const QString& restaurantId, const QVariantMap& updates);
    bool deleteRestaurant(const QString& restaurantId);
    QJsonArray getRestaurantsByOwner(const QString& ownerId);
    QJsonArray getAllRestaurants();

    // Menu Management
    bool createMenu(const QString& restaurantId);
    bool addMenuItem(const QString& menuId, const QString& name, const QString& description, double price, const QString& ingredients, const QString& category, const QString& imageUrl = "");
    bool updateMenuItem(const QString& menuId, const QString& itemId, const QVariantMap& updates);
    bool deleteMenuItem(const QString& menuId, const QString& itemId);
    QJsonArray getMenuItems(const QString& restaurantId);

    // Order Management
    bool createOrder(const QString& customerId, const QString& restaurantId, const QVariantList& items);
    bool updateOrderStatus(const QString& orderId, const QString& status);
    bool deleteOrder(const QString& orderId);
    QJsonArray getCustomerOrderHistory(const QString& customerId);
    QJsonArray getOrderHistory(const QString& userId, const QString& userType);

    // Order Feedback (comments only)
    bool addOrderComment(const QString& orderId, const QString& customerId, const QString& comment);
    QJsonArray getOrderComments(const QString& orderId);

    // Query Execution
    bool executeQuery(const QString& query, const QVariantMap& params);
    QSqlQuery prepareQuery(const QString& query, const QVariantMap& params);
    void logError(const QString& operation, const QSqlError& error);

    // Transaction Management
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();
    static DatabaseManager* instance;

    QSqlDatabase db;
    bool connected;
};

#endif // DATABASEMANAGER_H 