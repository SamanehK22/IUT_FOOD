#include "adminmanager.h"
#include <QDebug>
#include <QUuid>
#include <QSqlQuery>
#include <QSqlError>
#include <websocketserver.h>

AdminManager* AdminManager::instance = nullptr;

AdminManager* AdminManager::getInstance()
{
    if (instance == nullptr) {
        instance = new AdminManager();
    }
    return instance;
}

AdminManager::AdminManager(QObject *parent)
    : QObject(parent)
    , m_dbManager(DatabaseManager::getInstance())
    , m_authSystem(AuthSystem::getInstance())
{
}

AdminManager::~AdminManager()
{
}

bool AdminManager::validateAdminAccess(const QString& userId)
{
    QString userType = m_authSystem->getUserTypeFromToken(userId);
    return userType == "admin";
}

// Restaurant Management Methods
bool AdminManager::approveRestaurant(const QString& restaurantId)
{
    QVariantMap updates;
    updates["status"] = "approved";
    updates["approved_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    if (m_dbManager->updateRestaurant(restaurantId, updates)) {
        logAdminAction("system", "restaurant_approved", restaurantId);
        return true;
    }
    return false;
}

bool AdminManager::rejectRestaurant(const QString& restaurantId, const QString& reason)
{
    QVariantMap updates;
    updates["status"] = "rejected";
    updates["rejection_reason"] = reason;
    updates["rejected_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    if (m_dbManager->updateRestaurant(restaurantId, updates)) {
        logAdminAction("system", "restaurant_rejected", restaurantId + ": " + reason);
        return true;
    }
    return false;
}

bool AdminManager::suspendRestaurant(const QString& restaurantId, const QString& reason)
{
    QVariantMap updates;
    updates["status"] = "suspended";
    updates["suspension_reason"] = reason;
    updates["suspended_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    if (m_dbManager->updateRestaurant(restaurantId, updates)) {
        logAdminAction("system", "restaurant_suspended", restaurantId + ": " + reason);
        return true;
    }
    return false;
}

bool AdminManager::reactivateRestaurant(const QString& restaurantId)
{
    QVariantMap updates;
    updates["status"] = "active";
    updates["reactivated_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    if (m_dbManager->updateRestaurant(restaurantId, updates)) {
        logAdminAction("system", "restaurant_reactivated", restaurantId);
        return true;
    }
    return false;
}

QJsonArray AdminManager::getPendingRestaurants()
{
    QJsonArray restaurants;
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT * FROM restaurants WHERE status = 'pending' ORDER BY created_at DESC"
    ,
        params);

    while (query.next()) {
        QJsonObject restaurant;
        restaurant["id"] = query.value("id").toString();
        restaurant["name"] = query.value("name").toString();
        restaurant["address"] = query.value("address").toString();
        restaurant["type"] = query.value("type").toString();
        restaurant["created_at"] = query.value("created_at").toString();
        restaurants.append(restaurant);
    }

    return restaurants;
}

QJsonArray AdminManager::getSuspendedRestaurants()
{
    QJsonArray restaurants;
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT * FROM restaurants WHERE status = 'suspended' ORDER BY suspended_at DESC"
    ,
        params);

    while (query.next()) {
        QJsonObject restaurant;
        restaurant["id"] = query.value("id").toString();
        restaurant["name"] = query.value("name").toString();
        restaurant["suspension_reason"] = query.value("suspension_reason").toString();
        restaurant["suspended_at"] = query.value("suspended_at").toString();
        restaurants.append(restaurant);
    }

    return restaurants;
}

// User Management Methods
bool AdminManager::blockUser(const QString& userId, const QString& reason)
{
    QVariantMap updates;
    updates["status"] = "blocked";
    updates["blocked_at"] = QDateTime::currentDateTime();
    updates["block_reason"] = reason;

    if (m_dbManager->updateCustomer(userId, updates)) {
        // logAdminAction("block_user", QString("Blocked user %1: %2").arg(userId, reason));
        logAdminAction("system", "block_user", QString("Blocked user %1: %2").arg(userId, reason));
        return true;
    }
    return false;
}

bool AdminManager::unblockUser(const QString& userId)
{
    QVariantMap updates;
    updates["status"] = "active";
    updates["blocked_at"] = QVariant();
    updates["block_reason"] = QVariant();

    if (m_dbManager->updateCustomer(userId, updates)) {
        // logAdminAction("unblock_user", QString("Unblocked user %1").arg(userId));
        logAdminAction("system", "unblock_user", QString("Unblocked user %1").arg(userId));
        return true;
    }
    return false;
}

QJsonArray AdminManager::getBlockedUsers()
{
    QJsonArray users;
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT * FROM users WHERE status = 'blocked' ORDER BY blocked_at DESC"
    ,
        params);

    while (query.next()) {
        QJsonObject user;
        user["id"] = query.value("id").toString();
        user["username"] = query.value("username").toString();
        user["email"] = query.value("email").toString();
        user["block_reason"] = query.value("block_reason").toString();
        user["blocked_at"] = query.value("blocked_at").toString();
        user["phone"] = query.value("phone").toString();
        user["city"] = query.value("city").toString();
        user["location"] = query.value("location").toString();
        users.append(user);
    }

    return users;
}

QJsonArray AdminManager::getAllUsers()
{
    QJsonArray users;
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT * FROM users ORDER BY created_at DESC"
    ,
        params);

    while (query.next()) {
        QJsonObject user;
        user["id"] = query.value("id").toString();
        user["username"] = query.value("username").toString();
        user["email"] = query.value("email").toString();
        user["user_type"] = query.value("user_type").toString();
        user["status"] = query.value("status").toString();
        user["created_at"] = query.value("created_at").toString();
        user["phone"] = query.value("phone").toString();
        user["city"] = query.value("city").toString();
        user["location"] = query.value("location").toString();
        users.append(user);
    }

    return users;
}

// Reports and Analytics Methods
QJsonObject AdminManager::getSalesReport(const QDateTime& startDate, const QDateTime& endDate)
{
    QJsonObject report;
    QJsonArray salesData;
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT DATE(created_at) as date, COUNT(*) as order_count, SUM(total_amount) as total_sales "
        "FROM orders "
        "WHERE created_at BETWEEN :start_date AND :end_date "
        "GROUP BY DATE(created_at) "
        "ORDER BY date"
    ,
        params);
    query.bindValue(":start_date", startDate.toString(Qt::ISODate));
    query.bindValue(":end_date", endDate.toString(Qt::ISODate));

    while (query.next()) {
        QJsonObject dailySales;
        dailySales["date"] = query.value("date").toString();
        dailySales["order_count"] = query.value("order_count").toInt();
        dailySales["total_sales"] = query.value("total_sales").toDouble();
        salesData.append(dailySales);
    }

    report["sales_data"] = salesData;
    report["sales_chart"] = generateSalesChart(salesData);
    report["report_id"] = generateReportId();
    report["generated_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    return report;
}

QJsonObject AdminManager::getRestaurantPerformanceReport(const QString& restaurantId)
{
    QJsonObject report;
    QJsonArray performanceData;

    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT o.created_at, o.total_amount, o.status, "
        "COUNT(oi.id) as item_count, "
        "AVG(o.total_amount) as avg_order_value "
        "FROM orders o "
        "JOIN order_items oi ON o.id = oi.order_id "
        "WHERE o.restaurant_id = :restaurant_id "
        "GROUP BY o.id "
        "ORDER BY o.created_at DESC"
    ,
        params);
    query.bindValue(":restaurant_id", restaurantId);

    while (query.next()) {
        QJsonObject orderData;
        orderData["date"] = query.value("created_at").toString();
        orderData["total_amount"] = query.value("total_amount").toDouble();
        orderData["status"] = query.value("status").toString();
        orderData["item_count"] = query.value("item_count").toInt();
        orderData["avg_order_value"] = query.value("avg_order_value").toDouble();
        performanceData.append(orderData);
    }

    report["performance_data"] = performanceData;
    report["performance_chart"] = generateRestaurantPerformanceChart(performanceData);
    report["report_id"] = generateReportId();
    report["generated_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    return report;
}

QJsonObject AdminManager::getSystemAnalytics()
{
    QJsonObject analytics;

    // Get total users
    QVariantMap params;
    QSqlQuery userQuery = m_dbManager->prepareQuery(
        "SELECT COUNT(*) as total_users, "
        "SUM(CASE WHEN user_type = 'customer' THEN 1 ELSE 0 END) as total_customers, "
        "SUM(CASE WHEN user_type = 'restaurant' THEN 1 ELSE 0 END) as total_restaurants "
        "FROM users"
    ,
        params);
    if (userQuery.next()) {
        analytics["total_users"] = userQuery.value("total_users").toInt();
        analytics["total_customers"] = userQuery.value("total_customers").toInt();
        analytics["total_restaurants"] = userQuery.value("total_restaurants").toInt();
    }

    // Get order statistics
    QSqlQuery orderQuery = m_dbManager->prepareQuery(
        "SELECT COUNT(*) as total_orders, "
        "SUM(total_amount) as total_revenue, "
        "AVG(total_amount) as avg_order_value "
        "FROM orders"
    ,
        params);
    if (orderQuery.next()) {
        analytics["total_orders"] = orderQuery.value("total_orders").toInt();
        analytics["total_revenue"] = orderQuery.value("total_revenue").toDouble();
        analytics["avg_order_value"] = orderQuery.value("avg_order_value").toDouble();
    }

    return analytics;
}

QJsonArray AdminManager::getTopSellingItems(const QDateTime& startDate, const QDateTime& endDate)
{
    QJsonArray items;
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT mi.name, COUNT(oi.id) as order_count, SUM(oi.quantity) as total_quantity "
        "FROM order_items oi "
        "JOIN menu_items mi ON oi.item_id = mi.id "
        "JOIN orders o ON oi.order_id = o.id "
        "WHERE o.created_at BETWEEN :start_date AND :end_date "
        "GROUP BY mi.id, mi.name "
        "ORDER BY total_quantity DESC "
        "LIMIT 10"
    ,
        params);
    query.bindValue(":start_date", startDate.toString(Qt::ISODate));
    query.bindValue(":end_date", endDate.toString(Qt::ISODate));

    while (query.next()) {
        QJsonObject item;
        item["name"] = query.value("name").toString();
        item["order_count"] = query.value("order_count").toInt();
        item["total_quantity"] = query.value("total_quantity").toInt();
        items.append(item);
    }

    return items;
}

QJsonArray AdminManager::getMostActiveUsers(const QDateTime& startDate, const QDateTime& endDate)
{
    QJsonArray users;
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT u.username, COUNT(o.id) as order_count, SUM(o.total_amount) as total_spent "
        "FROM users u "
        "JOIN orders o ON u.id = o.customer_id "
        "WHERE o.created_at BETWEEN :start_date AND :end_date "
        "GROUP BY u.id, u.username "
        "ORDER BY order_count DESC "
        "LIMIT 10"
    ,
        params);
    query.bindValue(":start_date", startDate.toString(Qt::ISODate));
    query.bindValue(":end_date", endDate.toString(Qt::ISODate));

    while (query.next()) {
        QJsonObject user;
        user["username"] = query.value("username").toString();
        user["order_count"] = query.value("order_count").toInt();
        user["total_spent"] = query.value("total_spent").toDouble();
        users.append(user);
    }

    return users;
}

QJsonArray AdminManager::getMostPopularRestaurants(const QDateTime& startDate, const QDateTime& endDate)
{
    QJsonArray restaurants;
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT r.name, COUNT(o.id) as order_count, SUM(o.total_amount) as total_revenue "
        "FROM restaurants r "
        "JOIN orders o ON r.id = o.restaurant_id "
        "WHERE o.created_at BETWEEN :start_date AND :end_date "
        "GROUP BY r.id, r.name "
        "ORDER BY order_count DESC "
        "LIMIT 10"
    ,
        params);
    query.bindValue(":start_date", startDate.toString(Qt::ISODate));
    query.bindValue(":end_date", endDate.toString(Qt::ISODate));

    while (query.next()) {
        QJsonObject restaurant;
        restaurant["name"] = query.value("name").toString();
        restaurant["order_count"] = query.value("order_count").toInt();
        restaurant["total_revenue"] = query.value("total_revenue").toDouble();
        restaurants.append(restaurant);
    }

    return restaurants;
}

// Report Distribution Methods
void AdminManager::broadcastReport(const QString& reportType, const QJsonObject& report)
{
    QJsonObject message;
    message["type"] = "admin_report";
    message["report_type"] = reportType;
    message["report"] = report;
    message["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Get all active users
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT id FROM users WHERE status = 'active'"
    ,
        params);

    while (query.next()) {
        QString userId = query.value("id").toString();
        sendReportToUser(userId, reportType, report);
    }
}

void AdminManager::sendReportToUser(const QString& userId, const QString& reportType, const QJsonObject& report)
{
    QJsonObject message;
    message["type"] = "admin_report";
    message["report_type"] = reportType;
    message["report"] = report;
    message["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Send through WebSocket if user is online
    WebSocketServer::getInstance()->broadcastToUser(userId, message);
}

// Helper Methods
QString AdminManager::generateReportId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

void AdminManager::logAdminAction(const QString& adminId, const QString& action, const QString& details)
{
    QVariantMap params;
    QSqlQuery query = m_dbManager->prepareQuery(
        "INSERT INTO admin_logs (admin_id, action, details, created_at) "
        "VALUES (:admin_id, :action, :details, :created_at)"
    ,
        params);
    query.bindValue(":admin_id", adminId);
    query.bindValue(":action", action);
    query.bindValue(":details", details);
    query.bindValue(":created_at", QDateTime::currentDateTime().toString(Qt::ISODate));
    query.exec();
}

QJsonObject AdminManager::generateSalesChart(const QJsonArray& salesData)
{
    QJsonObject chart;
    QJsonArray labels;
    QJsonArray values;

    for (const QJsonValue& data : salesData) {
        QJsonObject dailyData = data.toObject();
        labels.append(dailyData["date"].toString());
        values.append(dailyData["total_sales"].toDouble());
    }

    chart["type"] = "line";
    chart["labels"] = labels;
    chart["values"] = values;
    return chart;
}

QJsonObject AdminManager::generateUserActivityChart(const QJsonArray& userData)
{
    QJsonObject chart;
    QJsonArray labels;
    QJsonArray values;

    for (const QJsonValue& data : userData) {
        QJsonObject user = data.toObject();
        labels.append(user["username"].toString());
        values.append(user["order_count"].toInt());
    }

    chart["type"] = "bar";
    chart["labels"] = labels;
    chart["values"] = values;
    return chart;
}

QJsonObject AdminManager::generateRestaurantPerformanceChart(const QJsonArray& performanceData)
{
    QJsonObject chart;
    QJsonArray labels;
    QJsonArray revenue;
    QJsonArray orders;

    for (const QJsonValue& data : performanceData) {
        QJsonObject perf = data.toObject();
        labels.append(perf["date"].toString());
        revenue.append(perf["total_amount"].toDouble());
        orders.append(perf["item_count"].toInt());
    }

    chart["type"] = "multi";
    chart["labels"] = labels;
    chart["revenue"] = revenue;
    chart["orders"] = orders;
    return chart;
}
