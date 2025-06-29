#include "databasemanager.h"
#include "logger.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager* DatabaseManager::getInstance()
{
    if (instance == nullptr) {
        instance = new DatabaseManager();
    }
    return instance;
}

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
    , connected(false)
{
    db = QSqlDatabase::addDatabase("QSQLITE"); // Using SQLite instead of MySQL
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::connect(const QString& host, const QString& database, const QString& username, const QString& password)
{
    db.setHostName(host);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);

    Logger::getInstance()->info(QString("Attempting to connect to database: %1@%2/%3")
        .arg(username)
        .arg(host)
        .arg(database), "DatabaseManager");

    if (!db.open()) {
        logError("Connection", db.lastError());
        return false;
    }

    connected = true;
    Logger::getInstance()->info("Database connected successfully", "DatabaseManager");
    return true;
}

void DatabaseManager::disconnect()
{
    if (db.isOpen()) {
        db.close();
        connected = false;
        Logger::getInstance()->info("Database disconnected", "DatabaseManager");
    }
}

bool DatabaseManager::isConnected() const
{
    return connected;
}

bool DatabaseManager::createCustomer(const QString& firstName, const QString& lastName, const QString& username, const QString& email,
                                   const QString& passwordHash, const QString& phone, const QString& address,
                                   const QString& city, const QString& location)
{
    Logger::getInstance()->debug(QString("Creating customer: %1 %2 (%3)")
        .arg(firstName)
        .arg(lastName)
        .arg(email), "DatabaseManager");

    QVariantMap params;
    params[":firstName"] = firstName;
    params[":lastName"] = lastName;
    params[":username"] = username;
    params[":email"] = email;
    params[":passwordHash"] = passwordHash;
    params[":phone"] = phone;
    params[":address"] = address;
    params[":city"] = city;
    params[":location"] = location;

    QString query = "INSERT INTO customers (first_name, last_name, username, email, password_hash, phone, address, city, location) "
                   "VALUES (:firstName, :lastName, :username, :email, :passwordHash, :phone, :address, :city, :location)";

    bool success = executeQuery(query, params);
    if (success) {
        Logger::getInstance()->info(QString("Customer created successfully: %1").arg(email), "DatabaseManager");
    } else {
        Logger::getInstance()->error(QString("Failed to create customer: %1").arg(email), "DatabaseManager");
    }
    return success;
}

bool DatabaseManager::updateCustomer(const QString& customerId, const QVariantMap& updates)
{
    QStringList setClauses;
    QVariantMap params;
    params[":id"] = customerId;

    for (auto it = updates.begin(); it != updates.end(); ++it) {
        setClauses << QString("%1 = :%2").arg(it.key()).arg(it.key());
        params[":" + it.key()] = it.value();
    }

    QString query = QString("UPDATE customers SET %1 WHERE id = :id")
                   .arg(setClauses.join(", "));

    return executeQuery(query, params);
}

bool DatabaseManager::deleteCustomer(const QString& customerId)
{
    QVariantMap params;
    params[":id"] = customerId;

    QString query = "DELETE FROM customers WHERE id = :id";
    return executeQuery(query, params);
}

QString DatabaseManager::getCustomerId(const QString& email)
{
    QVariantMap params;
    params[":email"] = email;

    QString query = "SELECT id FROM customers WHERE email = :email";
    QSqlQuery result = prepareQuery(query, params);

    if (result.next()) {
        return result.value(0).toString();
    }
    return QString();
}

QString DatabaseManager::getCustomerPasswordHash(const QString& customerId)
{
    QVariantMap params;
    params[":id"] = customerId;

    QString query = "SELECT password_hash FROM customers WHERE id = :id";
    QSqlQuery result = prepareQuery(query, params);

    if (result.next()) {
        return result.value(0).toString();
    }
    return QString();
}

QVariantMap DatabaseManager::getCustomerProfile(const QString& customerId)
{
    QVariantMap params;
    params[":id"] = customerId;

    QString query = "SELECT name, last_name, email, phone, address, city, location FROM customers WHERE id = :id";
    QSqlQuery result = prepareQuery(query, params);

    QVariantMap profile;
    if (result.next()) {
        profile["name"] = result.value("name").toString();
        profile["lastName"] = result.value("last_name").toString();
        profile["email"] = result.value("email").toString();
        profile["phone"] = result.value("phone").toString();
        profile["address"] = result.value("address").toString();
        profile["city"] = result.value("city").toString();
        profile["location"] = result.value("location").toString();
    }
    return profile;
}

QVariantMap DatabaseManager::getCustomerByLoginId(const QString& loginId)
{
    QVariantMap params;
    params[":loginId"] = loginId;
    QString query = "SELECT * FROM customers WHERE email = :loginId OR phone = :loginId";
    QSqlQuery result = prepareQuery(query, params);
    QVariantMap user;
    if (result.next()) {
        QSqlRecord rec = result.record();
        for (int i = 0; i < rec.count(); ++i) {
            user[rec.fieldName(i)] = result.value(i);
        }
    }
    return user;
}

bool DatabaseManager::createRestaurantOwner(const QString& firstName, const QString& lastName, const QString& username, const QString& email,
                                          const QString& passwordHash, const QString& phone, const QString& restaurantId,
                                          const QString& city, const QString& location)
{
    Logger::getInstance()->debug(QString("Creating restaurant owner: %1 %2 (%3)")
        .arg(firstName)
        .arg(lastName)
        .arg(email), "DatabaseManager");

    QVariantMap params;
    params[":firstName"] = firstName;
    params[":lastName"] = lastName;
    params[":username"] = username;
    params[":email"] = email;
    params[":passwordHash"] = passwordHash;
    params[":phone"] = phone;
    params[":restaurantId"] = restaurantId;
    params[":city"] = city;
    params[":location"] = location;

    QString query = "INSERT INTO restaurant_owners (first_name, last_name, username, email, password_hash, phone, restaurant_id, city, location) "
                   "VALUES (:firstName, :lastName, :username, :email, :passwordHash, :phone, :restaurantId, :city, :location)";

    bool success = executeQuery(query, params);
    if (success) {
        Logger::getInstance()->info(QString("Restaurant owner created successfully: %1").arg(email), "DatabaseManager");
    } else {
        Logger::getInstance()->error(QString("Failed to create restaurant owner: %1").arg(email), "DatabaseManager");
    }
    return success;
}

bool DatabaseManager::updateRestaurantOwner(const QString& ownerId, const QVariantMap& updates)
{
    QStringList setClauses;
    QVariantMap params;
    params[":id"] = ownerId;

    for (auto it = updates.begin(); it != updates.end(); ++it) {
        setClauses << QString("%1 = :%2").arg(it.key()).arg(it.key());
        params[":" + it.key()] = it.value();
    }

    QString query = QString("UPDATE restaurant_owners SET %1 WHERE id = :id")
                   .arg(setClauses.join(", "));

    return executeQuery(query, params);
}

bool DatabaseManager::deleteRestaurantOwner(const QString& ownerId)
{
    QVariantMap params;
    params[":id"] = ownerId;

    QString query = "DELETE FROM restaurant_owners WHERE id = :id";
    return executeQuery(query, params);
}

QString DatabaseManager::getRestaurantOwnerId(const QString& email)
{
    QVariantMap params;
    params[":email"] = email;

    QString query = "SELECT id FROM restaurant_owners WHERE email = :email";
    QSqlQuery result = prepareQuery(query, params);

    if (result.next()) {
        return result.value(0).toString();
    }
    return QString();
}

QString DatabaseManager::getRestaurantOwnerPasswordHash(const QString& ownerId)
{
    QVariantMap params;
    params[":id"] = ownerId;

    QString query = "SELECT password_hash FROM restaurant_owners WHERE id = :id";
    QSqlQuery result = prepareQuery(query, params);

    if (result.next()) {
        return result.value(0).toString();
    }
    return QString();
}

QVariantMap DatabaseManager::getRestaurantOwnerProfile(const QString& ownerId)
{
    QVariantMap params;
    params[":id"] = ownerId;

    QString query = "SELECT name, last_name, email, phone, restaurant_id, city, location FROM restaurant_owners WHERE id = :id";
    QSqlQuery result = prepareQuery(query, params);

    QVariantMap profile;
    if (result.next()) {
        profile["name"] = result.value("name").toString();
        profile["lastName"] = result.value("last_name").toString();
        profile["email"] = result.value("email").toString();
        profile["phone"] = result.value("phone").toString();
        profile["restaurantId"] = result.value("restaurant_id").toString();
        profile["city"] = result.value("city").toString();
        profile["location"] = result.value("location").toString();
    }
    return profile;
}

QVariantMap DatabaseManager::getRestaurantOwnerByLoginId(const QString& loginId)
{
    QVariantMap params;
    params[":loginId"] = loginId;
    QString query = "SELECT * FROM restaurant_owners WHERE email = :loginId OR phone = :loginId";
    QSqlQuery result = prepareQuery(query, params);
    QVariantMap user;
    if (result.next()) {
        QSqlRecord rec = result.record();
        for (int i = 0; i < rec.count(); ++i) {
            user[rec.fieldName(i)] = result.value(i);
        }
    }
    return user;
}

QString DatabaseManager::getRestaurantIdByOwner(const QString& ownerId)
{
    QVariantMap params;
    params[":id"] = ownerId;

    QString query = "SELECT restaurant_id FROM restaurant_owners WHERE id = :id";
    QSqlQuery result = prepareQuery(query, params);

    if (result.next()) {
        return result.value(0).toString();
    }
    return QString();
}

bool DatabaseManager::createRestaurant(const QString& name, const QString& address, const QString& type, const QString& imageUrl)
{
    QVariantMap params;
    params[":name"] = name;
    params[":address"] = address;
    params[":type"] = type;
    params[":imageUrl"] = imageUrl;

    QString query = "INSERT INTO restaurants (name, address, type, image_url) "
                   "VALUES (:name, :address, :type, :imageUrl)";

    return executeQuery(query, params);
}

bool DatabaseManager::updateRestaurant(const QString& restaurantId, const QVariantMap& updates)
{
    QStringList setClauses;
    QVariantMap params;
    params[":id"] = restaurantId;

    for (auto it = updates.begin(); it != updates.end(); ++it) {
        setClauses << QString("%1 = :%2").arg(it.key()).arg(it.key());
        params[":" + it.key()] = it.value();
    }

    QString query = QString("UPDATE restaurants SET %1 WHERE id = :id")
                   .arg(setClauses.join(", "));

    return executeQuery(query, params);
}

bool DatabaseManager::deleteRestaurant(const QString& restaurantId)
{
    QVariantMap params;
    params[":id"] = restaurantId;

    QString query = "DELETE FROM restaurants WHERE id = :id";
    return executeQuery(query, params);
}

bool DatabaseManager::createMenu(const QString& restaurantId)
{
    QVariantMap params;
    params[":restaurantId"] = restaurantId;

    QString query = "INSERT INTO menus (restaurant_id) VALUES (:restaurantId)";
    return executeQuery(query, params);
}

bool DatabaseManager::addMenuItem(const QString& menuId, const QString& name, const QString& description, double price, const QString& ingredients, const QString& category, const QString& imageUrl)
{
    QVariantMap params;
    params[":menuId"] = menuId;
    params[":name"] = name;
    params[":description"] = description;
    params[":price"] = price;
    params[":ingredients"] = ingredients;
    params[":category"] = category;
    params[":imageUrl"] = imageUrl;

    QString query = "INSERT INTO menu_items (menu_id, name, description, price, ingredients, category, image_url) "
                   "VALUES (:menuId, :name, :description, :price, :ingredients, :category, :imageUrl)";

    return executeQuery(query, params);
}

bool DatabaseManager::updateMenuItem(const QString& menuId, const QString& itemId, const QVariantMap& updates)
{
    QStringList setClauses;
    QVariantMap params;
    params[":menuId"] = menuId;
    params[":itemId"] = itemId;

    for (auto it = updates.begin(); it != updates.end(); ++it) {
        setClauses << QString("%1 = :%2").arg(it.key()).arg(it.key());
        params[":" + it.key()] = it.value();
    }

    QString query = QString("UPDATE menu_items SET %1 WHERE menu_id = :menuId AND id = :itemId")
                   .arg(setClauses.join(", "));

    return executeQuery(query, params);
}

bool DatabaseManager::deleteMenuItem(const QString& menuId, const QString& itemId)
{
    QVariantMap params;
    params[":menuId"] = menuId;
    params[":itemId"] = itemId;

    QString query = "DELETE FROM menu_items WHERE menu_id = :menuId AND id = :itemId";
    return executeQuery(query, params);
}

bool DatabaseManager::createOrder(const QString& customerId, const QString& restaurantId, const QVariantList& items)
{
    Logger::getInstance()->debug(QString("Creating order for customer %1 at restaurant %2")
        .arg(customerId)
        .arg(restaurantId), "DatabaseManager");

    if (!beginTransaction()) {
        Logger::getInstance()->error("Failed to begin transaction for order creation", "DatabaseManager");
        return false;
    }

    try {
        // Create order
        QVariantMap orderParams;
        orderParams[":customer_id"] = customerId;
        orderParams[":restaurant_id"] = restaurantId;
        orderParams[":status"] = "pending";
        orderParams[":created_at"] = QDateTime::currentDateTime();

        QSqlQuery query = prepareQuery(
            "INSERT INTO orders (customer_id, restaurant_id, status, created_at) "
            "VALUES (:customer_id, :restaurant_id, :status, :created_at)",
            orderParams
        );

        if (!query.exec()) {
            throw std::runtime_error("Failed to create order");
        }

        QString orderId = query.lastInsertId().toString();
        Logger::getInstance()->debug(QString("Order created with ID: %1").arg(orderId), "DatabaseManager");

        // Add order items
        for (const QVariant& item : items) {
            QVariantMap itemMap = item.toMap();
            QVariantMap itemParams;
            itemParams[":order_id"] = orderId;
            itemParams[":menu_item_id"] = itemMap["menuItemId"];
            itemParams[":quantity"] = itemMap["quantity"];
            itemParams[":price"] = itemMap["price"];

            QString itemQuery = "INSERT INTO order_items (order_id, menu_item_id, quantity, price) "
                              "VALUES (:order_id, :menu_item_id, :quantity, :price)";
            
            if (!executeQuery(itemQuery, itemParams)) {
                throw std::runtime_error("Failed to add order item");
            }
        }

        if (!commitTransaction()) {
            throw std::runtime_error("Failed to commit transaction");
        }

        Logger::getInstance()->info(QString("Order %1 created successfully").arg(orderId), "DatabaseManager");
        return true;
    } catch (const std::exception& e) {
        rollbackTransaction();
        Logger::getInstance()->error(QString("Error creating order: %1").arg(e.what()), "DatabaseManager");
        return false;
    }
}

bool DatabaseManager::updateOrderStatus(const QString& orderId, const QString& status)
{
    QVariantMap params;
    params[":id"] = orderId;
    params[":status"] = status;

    QString query = "UPDATE orders SET status = :status WHERE id = :id";
    return executeQuery(query, params);
}

bool DatabaseManager::deleteOrder(const QString& orderId)
{
    QVariantMap params;
    params[":id"] = orderId;

    QString query = "DELETE FROM orders WHERE id = :id";
    return executeQuery(query, params);
}

bool DatabaseManager::executeQuery(const QString& query, const QVariantMap& params)
{
    QSqlQuery sqlQuery = prepareQuery(query, params);
    if (!sqlQuery.exec()) {
        logError("Query execution", sqlQuery.lastError());
        return false;
    }
    return true;
}

QSqlQuery DatabaseManager::prepareQuery(const QString& query, const QVariantMap& params)
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare(query);

    for (auto it = params.begin(); it != params.end(); ++it) {
        sqlQuery.bindValue(":" + it.key(), it.value());
    }

    return sqlQuery;
}

void DatabaseManager::logError(const QString& operation, const QSqlError& error)
{
    qDebug() << "Database error in" << operation << ":"
             << error.text()
             << "Error code:" << error.nativeErrorCode()
             << "Driver text:" << error.driverText();
}

bool DatabaseManager::beginTransaction()
{
    return db.transaction();
}

bool DatabaseManager::commitTransaction()
{
    return db.commit();
}

bool DatabaseManager::rollbackTransaction()
{
    return db.rollback();
}

QJsonArray DatabaseManager::getCustomerOrderHistory(const QString& customerId)
{
    QVariantMap params;
    params[":customerId"] = customerId;

    QString query = "SELECT o.*, r.name as restaurant_name FROM orders o "
                   "JOIN restaurants r ON o.restaurant_id = r.id "
                   "WHERE o.customer_id = :customerId "
                   "ORDER BY o.created_at DESC";

    QSqlQuery result = prepareQuery(query, params);
    QJsonArray orders;

    while (result.next()) {
        QJsonObject order;
        order["id"] = result.value("id").toString();
        order["restaurant_id"] = result.value("restaurant_id").toString();
        order["restaurant_name"] = result.value("restaurant_name").toString();
        order["status"] = result.value("status").toString();
        order["created_at"] = result.value("created_at").toString();
        order["total_amount"] = result.value("total_amount").toDouble();
        orders.append(order);
    }

    return orders;
}

QJsonArray DatabaseManager::getRestaurantsByOwner(const QString& ownerId)
{
    QVariantMap params;
    params[":ownerId"] = ownerId;

    QString query = "SELECT r.* FROM restaurants r "
                   "JOIN restaurant_owners ro ON r.id = ro.restaurant_id "
                   "WHERE ro.id = :ownerId";

    QSqlQuery result = prepareQuery(query, params);
    QJsonArray restaurants;

    while (result.next()) {
        QJsonObject restaurant;
        restaurant["id"] = result.value("id").toString();
        restaurant["name"] = result.value("name").toString();
        restaurant["address"] = result.value("address").toString();
        restaurant["type"] = result.value("type").toString();
        restaurant["image_url"] = result.value("image_url").toString();
        restaurants.append(restaurant);
    }

    return restaurants;
}

QJsonArray DatabaseManager::getAllRestaurants()
{
    QString query = "SELECT * FROM restaurants WHERE status = 'approved' ORDER BY name";

    QSqlQuery result = prepareQuery(query, QVariantMap());
    QJsonArray restaurants;

    while (result.next()) {
        QJsonObject restaurant;
        restaurant["id"] = result.value("id").toString();
        restaurant["name"] = result.value("name").toString();
        restaurant["address"] = result.value("address").toString();
        restaurant["type"] = result.value("type").toString();
        restaurant["image_url"] = result.value("image_url").toString();
        restaurant["status"] = result.value("status").toString();
        restaurants.append(restaurant);
    }

    return restaurants;
}

QJsonArray DatabaseManager::getOrderHistory(const QString& userId, const QString& userType)
{
    QVariantMap params;
    params[":userId"] = userId;

    QString query;
    if (userType == "customer") {
        query = "SELECT o.*, r.name as restaurant_name FROM orders o "
                "JOIN restaurants r ON o.restaurant_id = r.id "
                "WHERE o.customer_id = :userId "
                "ORDER BY o.created_at DESC";
    } else if (userType == "restaurant_owner") {
        query = "SELECT o.*, c.first_name, c.last_name FROM orders o "
                "JOIN restaurants r ON o.restaurant_id = r.id "
                "JOIN restaurant_owners ro ON r.id = ro.restaurant_id "
                "JOIN customers c ON o.customer_id = c.id "
                "WHERE ro.id = :userId "
                "ORDER BY o.created_at DESC";
    } else {
        return QJsonArray();
    }

    QSqlQuery result = prepareQuery(query, params);
    QJsonArray orders;

    while (result.next()) {
        QJsonObject order;
        order["id"] = result.value("id").toString();
        order["customer_id"] = result.value("customer_id").toString();
        order["restaurant_id"] = result.value("restaurant_id").toString();
        order["status"] = result.value("status").toString();
        order["created_at"] = result.value("created_at").toString();
        order["total_amount"] = result.value("total_amount").toDouble();
        
        if (userType == "customer") {
            order["restaurant_name"] = result.value("restaurant_name").toString();
        } else if (userType == "restaurant_owner") {
            order["customer_name"] = result.value("first_name").toString() + " " + result.value("last_name").toString();
        }
        
        orders.append(order);
    }

    return orders;
}

QJsonArray DatabaseManager::getMenuItems(const QString& restaurantId)
{
    QVariantMap params;
    params[":restaurantId"] = restaurantId;

    QString query = "SELECT mi.* FROM menu_items mi "
                   "JOIN menus m ON mi.menu_id = m.id "
                   "WHERE m.restaurant_id = :restaurantId "
                   "ORDER BY mi.name";

    QSqlQuery result = prepareQuery(query, params);
    QJsonArray menuItems;

    while (result.next()) {
        QJsonObject item;
        item["id"] = result.value("id").toString();
        item["menu_id"] = result.value("menu_id").toString();
        item["name"] = result.value("name").toString();
        item["description"] = result.value("description").toString();
        item["price"] = result.value("price").toDouble();
        item["ingredients"] = result.value("ingredients").toString();
        item["category"] = result.value("category").toString();
        item["image_url"] = result.value("image_url").toString();
        item["available"] = result.value("available").toBool();
        menuItems.append(item);
    }

    return menuItems;
}

bool DatabaseManager::addOrderComment(const QString& orderId, const QString& customerId, const QString& comment)
{
    QVariantMap params;
    params[":orderId"] = orderId;
    params[":customerId"] = customerId;
    params[":comment"] = comment;
    params[":createdAt"] = QDateTime::currentDateTime();

    QString query = "INSERT INTO order_feedback (order_id, customer_id, comment, created_at) "
                   "VALUES (:orderId, :customerId, :comment, :createdAt)";
    return executeQuery(query, params);
}

QJsonArray DatabaseManager::getOrderComments(const QString& orderId)
{
    QVariantMap params;
    params[":orderId"] = orderId;
    QString query = "SELECT * FROM order_feedback WHERE order_id = :orderId ORDER BY created_at DESC";
    QSqlQuery result = prepareQuery(query, params);
    QJsonArray comments;
    while (result.next()) {
        QJsonObject commentObj;
        commentObj["id"] = result.value("id").toString();
        commentObj["order_id"] = result.value("order_id").toString();
        commentObj["customer_id"] = result.value("customer_id").toString();
        commentObj["comment"] = result.value("comment").toString();
        commentObj["created_at"] = result.value("created_at").toString();
        comments.append(commentObj);
    }
    return comments;
} 