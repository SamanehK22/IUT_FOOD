#include "restaurantmanager.h"
#include "websocketserver.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <cmath>

RestaurantManager* RestaurantManager::instance = nullptr;

RestaurantManager* RestaurantManager::getInstance()
{
    if (instance == nullptr) {
        instance = new RestaurantManager();
    }
    return instance;
}

RestaurantManager::RestaurantManager(QObject *parent)
    : QObject(parent)
    , m_dbManager(DatabaseManager::getInstance())
    , m_authSystem(AuthSystem::getInstance())
{
}

RestaurantManager::~RestaurantManager()
{
}

bool RestaurantManager::validateRestaurantAccess(const QString& restaurantId, const QString& userId)
{
    QString userType = m_authSystem->getUserTypeFromToken(userId);
    if (userType == "admin") return true;

    QVariantMap params;
    params[":restaurant_id"] = restaurantId;
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT owner_id FROM restaurants WHERE id = :restaurant_id",
        params
    );
    
    if (query.exec() && query.next()) {
        return query.value("owner_id").toString() == userId;
    }
    return false;
}

// Menu Management Methods
bool RestaurantManager::addMenuItem(const QString& restaurantId, const QJsonObject& item)
{
    // First, get or create the menu for this restaurant
    QVariantMap menuParams;
    menuParams[":restaurant_id"] = restaurantId;
    
    QSqlQuery menuQuery = m_dbManager->prepareQuery(
        "SELECT id FROM menus WHERE restaurant_id = :restaurant_id",
        menuParams
    );
    
    QString menuId;
    if (menuQuery.exec() && menuQuery.next()) {
        menuId = menuQuery.value("id").toString();
    } else {
        // Create a new menu if it doesn't exist
        QSqlQuery createMenuQuery = m_dbManager->prepareQuery(
            "INSERT INTO menus (restaurant_id) VALUES (:restaurant_id)",
            menuParams
        );
        if (createMenuQuery.exec()) {
            menuId = createMenuQuery.lastInsertId().toString();
        } else {
            return false;
        }
    }
    
    // Now insert the menu item
    QVariantMap params;
    params[":menu_id"] = menuId;
    params[":name"] = item["name"].toString();
    params[":description"] = item["description"].toString();
    params[":price"] = item["price"].toDouble();
    params[":ingredients"] = item["ingredients"].toString();
    params[":category"] = item["category"].toString();
    params[":available"] = item["is_available"].toBool() ? 1 : 0;

    QSqlQuery query = m_dbManager->prepareQuery(
        "INSERT INTO menu_items (menu_id, name, description, price, ingredients, category, available) "
        "VALUES (:menu_id, :name, :description, :price, :ingredients, :category, :available)",
        params
    );

    return query.exec();
}

bool RestaurantManager::updateMenuItem(const QString& restaurantId, const QString& itemId, const QJsonObject& updates)
{
    if (!validateRestaurantAccess(restaurantId, updates["user_id"].toString())) {
        return false;
    }

    QStringList setClauses;
    QVariantMap params;
    params[":item_id"] = itemId;

    if (updates.contains("name")) {
        setClauses << "name = :name";
        params[":name"] = updates["name"].toString();
    }
    if (updates.contains("description")) {
        setClauses << "description = :description";
        params[":description"] = updates["description"].toString();
    }
    if (updates.contains("price")) {
        setClauses << "price = :price";
        params[":price"] = updates["price"].toDouble();
    }
    if (updates.contains("ingredients")) {
        setClauses << "ingredients = :ingredients";
        params[":ingredients"] = updates["ingredients"].toString();
    }
    if (updates.contains("category")) {
        setClauses << "category = :category";
        params[":category"] = updates["category"].toString();
    }
    if (updates.contains("is_available")) {
        setClauses << "available = :available";
        params[":available"] = updates["is_available"].toBool() ? 1 : 0;
    }

    if (setClauses.isEmpty()) {
        return false;
    }

    QString queryStr = QString("UPDATE menu_items SET %1 WHERE id = :item_id")
        .arg(setClauses.join(", "));
    
    QSqlQuery query = m_dbManager->prepareQuery(queryStr, params);

    return query.exec();
}

bool RestaurantManager::deleteMenuItem(const QString& restaurantId, const QString& itemId)
{
    QVariantMap params;
    params[":item_id"] = itemId;

    QSqlQuery query = m_dbManager->prepareQuery(
        "DELETE FROM menu_items WHERE id = :item_id",
        params
    );
    return query.exec();
}

QJsonArray RestaurantManager::getMenuItems(const QString& restaurantId)
{
    QVariantMap params;
    params[":restaurant_id"] = restaurantId;

    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT mi.* FROM menu_items mi "
        "JOIN menus m ON mi.menu_id = m.id "
        "WHERE m.restaurant_id = :restaurant_id "
        "ORDER BY mi.name",
        params
    );
    QJsonArray items;

    if (query.exec()) {
        while (query.next()) {
            QJsonObject item;
            item["id"] = query.value("id").toString();
            item["name"] = query.value("name").toString();
            item["description"] = query.value("description").toString();
            item["price"] = query.value("price").toDouble();
            item["ingredients"] = query.value("ingredients").toString();
            item["category"] = query.value("category").toString();
            item["is_available"] = query.value("available").toInt() == 1;
            items.append(item);
        }
    }

    return items;
}

// Rating and Feedback Methods
bool RestaurantManager::addRating(const QString& restaurantId, const QString& userId, int rating, const QString& comment)
{
    QVariantMap params;
    params[":restaurant_id"] = restaurantId;
    params[":user_id"] = userId;
    params[":rating"] = rating;
    params[":comment"] = comment;
    params[":created_at"] = QDateTime::currentDateTime();

    QSqlQuery query = m_dbManager->prepareQuery(
        "INSERT INTO ratings (restaurant_id, user_id, rating, comment, created_at) "
        "VALUES (:restaurant_id, :user_id, :rating, :comment, :created_at)",
        params
    );

    if (query.exec()) {
        notifyRatingUpdate(restaurantId);
        return true;
    }
    return false;
}

QJsonArray RestaurantManager::getRestaurantRatings(const QString& restaurantId, int limit)
{
    QVariantMap params;
    params[":restaurant_id"] = restaurantId;
    params[":limit"] = limit;

    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT * FROM ratings "
        "WHERE restaurant_id = :restaurant_id "
        "ORDER BY created_at DESC "
        "LIMIT :limit",
        params
    );
    QJsonArray ratings;

    if (query.exec()) {
        while (query.next()) {
            QJsonObject rating;
            rating["id"] = query.value("id").toString();
            rating["user_id"] = query.value("user_id").toString();
            rating["rating"] = query.value("rating").toInt();
            rating["comment"] = query.value("comment").toString();
            rating["created_at"] = query.value("created_at").toString();
            ratings.append(rating);
        }
    }

    return ratings;
}

QJsonObject RestaurantManager::getRestaurantRatingStats(const QString& restaurantId)
{
    QJsonArray ratings = getRestaurantRatings(restaurantId);
    return calculateRatingStats(ratings);
}

QJsonObject RestaurantManager::calculateRatingStats(const QJsonArray& ratings)
{
    QJsonObject stats;
    double totalRating = 0;
    QMap<int, int> ratingDistribution;

    for (const QJsonValue& value : ratings) {
        QJsonObject rating = value.toObject();
        int ratingValue = rating["rating"].toInt();
        totalRating += ratingValue;
        ratingDistribution[ratingValue]++;
    }

    stats["average_rating"] = ratings.isEmpty() ? 0 : totalRating / ratings.size();
    stats["total_ratings"] = ratings.size();
    
    QJsonObject distribution;
    for (auto it = ratingDistribution.begin(); it != ratingDistribution.end(); ++it) {
        distribution[QString::number(it.key())] = it.value();
    }
    stats["distribution"] = distribution;

    return stats;
}

// Search and Filtering Methods
QJsonArray RestaurantManager::searchRestaurants(const QString& query, const QJsonObject& filters)
{
    QJsonArray restaurants;
    QStringList conditions;
    QVariantMap params;

    QString baseQuery = "SELECT r.*, "
                       "(SELECT AVG(rating) FROM ratings WHERE restaurant_id = r.id) as avg_rating, "
                       "(SELECT COUNT(*) FROM ratings WHERE restaurant_id = r.id) as rating_count "
                       "FROM restaurants r WHERE 1=1";

    if (!query.isEmpty()) {
        conditions << "(r.name LIKE :query OR r.address LIKE :query)";
        params[":query"] = "%" + query + "%";
    }

    if (filters.contains("category")) {
        conditions << "r.type = :type";
        params[":type"] = filters["category"].toString();
    }

    if (filters.contains("min_rating")) {
        conditions << "(SELECT AVG(rating) FROM ratings WHERE restaurant_id = r.id) >= :min_rating";
        params[":min_rating"] = filters["min_rating"].toDouble();
    }

    if (filters.contains("is_open")) {
        conditions << "r.status = 'approved'";
    }

    if (!conditions.isEmpty()) {
        baseQuery += " AND " + conditions.join(" AND ");
    }

    baseQuery += " ORDER BY avg_rating DESC, rating_count DESC";

    QSqlQuery sqlQuery = m_dbManager->prepareQuery(baseQuery, params);

    if (sqlQuery.exec()) {
        while (sqlQuery.next()) {
            QJsonObject restaurant;
            restaurant["id"] = sqlQuery.value("id").toString();
            restaurant["name"] = sqlQuery.value("name").toString();
            restaurant["type"] = sqlQuery.value("type").toString();
            restaurant["address"] = sqlQuery.value("address").toString();
            restaurant["status"] = sqlQuery.value("status").toString();
            restaurant["avg_rating"] = sqlQuery.value("avg_rating").toDouble();
            restaurant["rating_count"] = sqlQuery.value("rating_count").toInt();
            restaurants.append(restaurant);
        }
    }

    return restaurants;
}

QJsonArray RestaurantManager::getNearbyRestaurants(double latitude, double longitude, double radius)
{
    QJsonArray restaurants;
    QVariantMap params;
    params[":lat"] = latitude;
    params[":lon"] = longitude;
    params[":radius"] = radius;

    // Since the restaurants table doesn't have latitude/longitude columns,
    // we'll return all restaurants with their rating information
    QSqlQuery query = m_dbManager->prepareQuery(
        "SELECT r.*, "
        "(SELECT AVG(rating) FROM ratings WHERE restaurant_id = r.id) as avg_rating, "
        "(SELECT COUNT(*) FROM ratings WHERE restaurant_id = r.id) as rating_count "
        "FROM restaurants r "
        "WHERE r.status = 'approved' "
        "ORDER BY avg_rating DESC, rating_count DESC",
        params
    );

    if (query.exec()) {
        while (query.next()) {
            QJsonObject restaurant;
            restaurant["id"] = query.value("id").toString();
            restaurant["name"] = query.value("name").toString();
            restaurant["type"] = query.value("type").toString();
            restaurant["address"] = query.value("address").toString();
            restaurant["avg_rating"] = query.value("avg_rating").toDouble();
            restaurant["rating_count"] = query.value("rating_count").toInt();
            restaurants.append(restaurant);
        }
    }

    return restaurants;
}

void RestaurantManager::notifyRatingUpdate(const QString& restaurantId)
{
    QJsonObject message;
    message["type"] = "rating_update";
    message["restaurant_id"] = restaurantId;
    message["stats"] = getRestaurantRatingStats(restaurantId);
    message["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Notify through WebSocket
    WebSocketServer::getInstance()->broadcastToRestaurant(restaurantId, message);
}

double RestaurantManager::calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371; // Earth's radius in kilometers
    double dLat = (lat2 - lat1) * M_PI / 180;
    double dLon = (lon2 - lon1) * M_PI / 180;
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(lat1 * M_PI / 180) * cos(lat2 * M_PI / 180) *
               sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return R * c;
}

// Enhanced Search Methods Implementation
QJsonArray RestaurantManager::searchRestaurantsByLocation(const QString& location, double radius)
{
    QJsonObject filters;
    filters["location"] = location;
    filters["radius"] = radius;
    return searchRestaurants("", filters);
}

QJsonArray RestaurantManager::searchRestaurantsByType(const QString& type)
{
    QJsonObject filters;
    filters["type"] = type;
    return searchRestaurants("", filters);
}

QJsonArray RestaurantManager::searchRestaurantsByPriceRange(double minPrice, double maxPrice)
{
    QJsonObject filters;
    filters["min_price"] = minPrice;
    filters["max_price"] = maxPrice;
    return searchRestaurants("", filters);
}

QJsonArray RestaurantManager::searchRestaurantsByCuisine(const QString& cuisine)
{
    QJsonObject filters;
    filters["type"] = cuisine;
    return searchRestaurants("", filters);
}

QJsonArray RestaurantManager::searchRestaurantsByOperatingHours(const QString& day, const QString& time)
{
    QJsonObject filters;
    filters["day"] = day;
    filters["time"] = time;
    return searchRestaurants("", filters);
}

QJsonArray RestaurantManager::searchRestaurantsByFeatures(const QStringList& features)
{
    QJsonObject filters;
    filters["features"] = QJsonArray::fromStringList(features);
    return searchRestaurants("", filters);
}

QJsonArray RestaurantManager::advancedRestaurantSearch(const QJsonObject& criteria)
{
    QStringList conditions;
    QVariantMap bindValues;

    QString baseQuery = "SELECT r.*, "
                       "(SELECT AVG(rating) FROM ratings WHERE restaurant_id = r.id) as avg_rating, "
                       "(SELECT COUNT(*) FROM ratings WHERE restaurant_id = r.id) as rating_count "
                       "FROM restaurants r WHERE 1=1";

    if (criteria.contains("name")) {
        conditions << "r.name LIKE :name";
        bindValues[":name"] = "%" + criteria["name"].toString() + "%";
    }

    if (criteria.contains("cuisine")) {
        conditions << "r.type = :type";
        bindValues[":type"] = criteria["cuisine"].toString();
    }

    if (criteria.contains("min_price") && criteria.contains("max_price")) {
        conditions << "EXISTS (SELECT 1 FROM menu_items mi "
                   "JOIN menus m ON mi.menu_id = m.id "
                   "WHERE m.restaurant_id = r.id "
                   "AND mi.price BETWEEN :min_price AND :max_price)";
        bindValues[":min_price"] = criteria["min_price"].toDouble();
        bindValues[":max_price"] = criteria["max_price"].toDouble();
    }

    if (criteria.contains("min_rating")) {
        conditions << "(SELECT AVG(rating) FROM ratings WHERE restaurant_id = r.id) >= :min_rating";
        bindValues[":min_rating"] = criteria["min_rating"].toDouble();
    }

    if (criteria.contains("is_open")) {
        conditions << "r.status = 'approved'";
    }

    return executeSearchQuery(buildSearchQuery(baseQuery, conditions, bindValues), bindValues);
}

// Menu Item Search and Filtering Methods
QJsonArray RestaurantManager::searchMenuItems(const QString& query, const QJsonObject& filters)
{
    QStringList conditions;
    QVariantMap bindValues;

    QString baseQuery = "SELECT mi.*, r.name as restaurant_name, r.type as restaurant_type "
                       "FROM menu_items mi "
                       "JOIN menus m ON mi.menu_id = m.id "
                       "JOIN restaurants r ON m.restaurant_id = r.id "
                       "WHERE 1=1";

    if (!query.isEmpty()) {
        conditions << "(mi.name LIKE :query OR mi.description LIKE :query)";
        bindValues[":query"] = "%" + query + "%";
    }

    if (filters.contains("min_price")) {
        conditions << "mi.price >= :min_price";
        bindValues[":min_price"] = filters["min_price"].toDouble();
    }

    if (filters.contains("max_price")) {
        conditions << "mi.price <= :max_price";
        bindValues[":max_price"] = filters["max_price"].toDouble();
    }

    if (filters.contains("is_available")) {
        conditions << "mi.available = :available";
        bindValues[":available"] = filters["is_available"].toBool() ? 1 : 0;
    }

    return executeMenuItemSearchQuery(buildMenuItemSearchQuery(baseQuery, conditions, bindValues), bindValues);
}

QJsonArray RestaurantManager::getMenuItemsByCategory(const QString& restaurantId, const QString& category)
{
    QStringList conditions;
    QVariantMap bindValues;

    QString baseQuery = "SELECT mi.* FROM menu_items mi "
                       "JOIN menus m ON mi.menu_id = m.id "
                       "WHERE m.restaurant_id = :restaurant_id";

    bindValues[":restaurant_id"] = restaurantId;
    conditions << "mi.category = :category";
    bindValues[":category"] = category;

    return executeMenuItemSearchQuery(buildMenuItemSearchQuery(baseQuery, conditions, bindValues), bindValues);
}

QJsonArray RestaurantManager::getMenuItemsByPriceRange(const QString& restaurantId, double minPrice, double maxPrice)
{
    QStringList conditions;
    QVariantMap bindValues;

    QString baseQuery = "SELECT mi.* FROM menu_items mi "
                       "JOIN menus m ON mi.menu_id = m.id "
                       "WHERE m.restaurant_id = :restaurant_id";

    bindValues[":restaurant_id"] = restaurantId;
    conditions << "mi.price >= :min_price";
    conditions << "mi.price <= :max_price";
    bindValues[":min_price"] = minPrice;
    bindValues[":max_price"] = maxPrice;

    return executeMenuItemSearchQuery(buildMenuItemSearchQuery(baseQuery, conditions, bindValues), bindValues);
}

QJsonArray RestaurantManager::getMenuItemsByDietaryRestrictions(const QString& restaurantId, const QStringList& restrictions)
{
    // Since the menu_items table doesn't have dietary_info column,
    // we'll return all menu items for the restaurant
    QVariantMap bindValues;
    bindValues[":restaurant_id"] = restaurantId;

    QString baseQuery = "SELECT mi.* FROM menu_items mi "
                       "JOIN menus m ON mi.menu_id = m.id "
                       "WHERE m.restaurant_id = :restaurant_id";

    return executeMenuItemSearchQuery(baseQuery, bindValues);
}

QJsonArray RestaurantManager::getMenuItemsByIngredients(const QString& restaurantId, const QStringList& ingredients)
{
    QStringList conditions;
    QVariantMap bindValues;

    QString baseQuery = "SELECT mi.* FROM menu_items mi "
                       "JOIN menus m ON mi.menu_id = m.id "
                       "WHERE m.restaurant_id = :restaurant_id";

    bindValues[":restaurant_id"] = restaurantId;

    // Add ingredient filtering conditions
    for (int i = 0; i < ingredients.size(); ++i) {
        QString paramName = QString(":ingredient_%1").arg(i);
        conditions << QString("mi.ingredients LIKE %1").arg(paramName);
        bindValues[paramName] = "%" + ingredients[i] + "%";
    }

    if (!conditions.isEmpty()) {
        baseQuery += " AND (" + conditions.join(" OR ") + ")";
    }

    return executeMenuItemSearchQuery(baseQuery, bindValues);
}

QJsonArray RestaurantManager::getMenuItemsByAvailability(const QString& restaurantId, bool available)
{
    QStringList conditions;
    QVariantMap bindValues;

    QString baseQuery = "SELECT mi.* FROM menu_items mi "
                       "JOIN menus m ON mi.menu_id = m.id "
                       "WHERE m.restaurant_id = :restaurant_id";

    bindValues[":restaurant_id"] = restaurantId;
    conditions << "mi.available = :available";
    bindValues[":available"] = available ? 1 : 0;

    return executeMenuItemSearchQuery(buildMenuItemSearchQuery(baseQuery, conditions, bindValues), bindValues);
}

QJsonArray RestaurantManager::advancedMenuItemSearch(const QString& restaurantId, const QJsonObject& criteria)
{
    QStringList conditions;
    QVariantMap bindValues;

    QString baseQuery = "SELECT mi.*, r.name as restaurant_name, r.type as restaurant_type "
                       "FROM menu_items mi "
                       "JOIN menus m ON mi.menu_id = m.id "
                       "JOIN restaurants r ON m.restaurant_id = r.id "
                       "WHERE m.restaurant_id = :restaurant_id";

    bindValues[":restaurant_id"] = restaurantId;

    if (criteria.contains("name")) {
        conditions << "mi.name LIKE :name";
        bindValues[":name"] = "%" + criteria["name"].toString() + "%";
    }

    if (criteria.contains("min_price")) {
        conditions << "mi.price >= :min_price";
        bindValues[":min_price"] = criteria["min_price"].toDouble();
    }

    if (criteria.contains("max_price")) {
        conditions << "mi.price <= :max_price";
        bindValues[":max_price"] = criteria["max_price"].toDouble();
    }

    if (criteria.contains("is_available")) {
        conditions << "mi.available = :available";
        bindValues[":available"] = criteria["is_available"].toBool() ? 1 : 0;
    }

    return executeMenuItemSearchQuery(buildMenuItemSearchQuery(baseQuery, conditions, bindValues), bindValues);
}

// Helper Methods for Search
QString RestaurantManager::buildSearchQuery(const QString& baseQuery, const QStringList& conditions, const QVariantMap& bindValues)
{
    QString query = baseQuery;
    if (!conditions.isEmpty()) {
        query += " AND " + conditions.join(" AND ");
    }
    query += " ORDER BY avg_rating DESC, rating_count DESC";
    
    // Log the query and bind values for debugging
    Q_UNUSED(bindValues); // Suppress warning while keeping parameter for future use
    return query;
}

QString RestaurantManager::buildMenuItemSearchQuery(const QString& baseQuery, const QStringList& conditions, const QVariantMap& bindValues)
{
    QString query = baseQuery;
    if (!conditions.isEmpty()) {
        query += " AND " + conditions.join(" AND ");
    }
    query += " ORDER BY mi.name";
    
    // Log the query and bind values for debugging
    Q_UNUSED(bindValues); // Suppress warning while keeping parameter for future use
    return query;
}

QJsonArray RestaurantManager::executeSearchQuery(const QString& query, const QVariantMap& bindValues)
{
    QJsonArray results;
    QSqlQuery sqlQuery = m_dbManager->prepareQuery(query, bindValues);

    if (sqlQuery.exec()) {
        while (sqlQuery.next()) {
            QJsonObject restaurant;
            restaurant["id"] = sqlQuery.value("id").toString();
            restaurant["name"] = sqlQuery.value("name").toString();
            restaurant["type"] = sqlQuery.value("type").toString();
            restaurant["address"] = sqlQuery.value("address").toString();
            restaurant["status"] = sqlQuery.value("status").toString();
            restaurant["avg_rating"] = sqlQuery.value("avg_rating").toDouble();
            restaurant["rating_count"] = sqlQuery.value("rating_count").toInt();
            results.append(restaurant);
        }
    }

    return results;
}

QJsonArray RestaurantManager::executeMenuItemSearchQuery(const QString& query, const QVariantMap& bindValues)
{
    QJsonArray results;
    QSqlQuery sqlQuery = m_dbManager->prepareQuery(query, bindValues);

    if (sqlQuery.exec()) {
        while (sqlQuery.next()) {
            QJsonObject item;
            item["id"] = sqlQuery.value("id").toString();
            item["name"] = sqlQuery.value("name").toString();
            item["description"] = sqlQuery.value("description").toString();
            item["price"] = sqlQuery.value("price").toDouble();
            item["ingredients"] = sqlQuery.value("ingredients").toString();
            item["category"] = sqlQuery.value("category").toString();
            item["is_available"] = sqlQuery.value("available").toInt() == 1;
            
            // Add restaurant info if available
            if (sqlQuery.value("restaurant_name").isValid()) {
                item["restaurant_name"] = sqlQuery.value("restaurant_name").toString();
                item["restaurant_type"] = sqlQuery.value("restaurant_type").toString();
            }
            
            results.append(item);
        }
    }

    return results;
} 