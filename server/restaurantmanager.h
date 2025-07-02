#ifndef RESTAURANTMANAGER_H
#define RESTAURANTMANAGER_H

#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include "databasemanager.h"
#include "authsystem.h"

class RestaurantManager : public QObject
{
    Q_OBJECT

public:
    static RestaurantManager* getInstance();

    // Menu Management
    bool addMenuItem(const QString& restaurantId, const QJsonObject& item);
    bool updateMenuItem(const QString& restaurantId, const QString& itemId, const QJsonObject& updates);
    bool deleteMenuItem(const QString& restaurantId, const QString& itemId);
    QJsonArray getMenuItems(const QString& restaurantId);
    bool updateMenuCategory(const QString& restaurantId, const QString& categoryId, const QJsonObject& updates);
    bool reorderMenuItems(const QString& restaurantId, const QJsonArray& itemOrder);

    // Rating and Feedback
    bool addRating(const QString& restaurantId, const QString& userId, int rating, const QString& comment);
    bool updateRating(const QString& ratingId, int newRating, const QString& newComment);
    bool deleteRating(const QString& ratingId);
    QJsonArray getRestaurantRatings(const QString& restaurantId, int limit = 10);
    QJsonObject getRestaurantRatingStats(const QString& restaurantId);
    bool reportRating(const QString& ratingId, const QString& reason);

    // Restaurant Search and Filtering
    QJsonArray searchRestaurants(const QString& query, const QJsonObject& filters);
    QJsonArray getRestaurantsByCategory(const QString& category);
    QJsonArray getTopRatedRestaurants(int limit = 10);
    QJsonArray getPopularRestaurants(int limit = 10);
    QJsonArray getNearbyRestaurants(double latitude, double longitude, double radius);
    
    // Enhanced Search Methods
    QJsonArray searchRestaurantsByLocation(const QString& location, double radius);
    QJsonArray searchRestaurantsByType(const QString& type);
    QJsonArray searchRestaurantsByPriceRange(double minPrice, double maxPrice);
    QJsonArray searchRestaurantsByCuisine(const QString& cuisine);
    QJsonArray searchRestaurantsByOperatingHours(const QString& day, const QString& time);
    QJsonArray searchRestaurantsByFeatures(const QStringList& features);
    QJsonArray advancedRestaurantSearch(const QJsonObject& criteria);

    // Menu Item Search and Filtering
    QJsonArray searchMenuItems(const QString& query, const QJsonObject& filters);
    QJsonArray getMenuItemsByCategory(const QString& restaurantId, const QString& category);
    QJsonArray getMenuItemsByPriceRange(const QString& restaurantId, double minPrice, double maxPrice);
    QJsonArray getMenuItemsByDietaryRestrictions(const QString& restaurantId, const QStringList& restrictions);
    QJsonArray getMenuItemsByIngredients(const QString& restaurantId, const QStringList& ingredients);
    QJsonArray getMenuItemsByAvailability(const QString& restaurantId, bool available);
    QJsonArray advancedMenuItemSearch(const QString& restaurantId, const QJsonObject& criteria);

    // Restaurant Profile
    bool updateRestaurantProfile(const QString& restaurantId, const QJsonObject& updates);
    QJsonObject getRestaurantProfile(const QString& restaurantId);
    bool updateRestaurantHours(const QString& restaurantId, const QJsonObject& hours);
    bool updateRestaurantLocation(const QString& restaurantId, double latitude, double longitude);

private:
    explicit RestaurantManager(QObject *parent = nullptr);
    ~RestaurantManager();
    static RestaurantManager* instance;

    DatabaseManager* m_dbManager;
    AuthSystem* m_authSystem;

    bool validateRestaurantAccess(const QString& restaurantId, const QString& userId);
    double calculateDistance(double lat1, double lon1, double lat2, double lon2);
    QJsonObject calculateRatingStats(const QJsonArray& ratings);
    void notifyRatingUpdate(const QString& restaurantId);
    
    // Helper methods for search
    QString buildSearchQuery(const QString& baseQuery, const QStringList& conditions, const QVariantMap& bindValues);
    QString buildMenuItemSearchQuery(const QString& baseQuery, const QStringList& conditions, const QVariantMap& bindValues);
    QJsonArray executeSearchQuery(const QString& query, const QVariantMap& bindValues);
    QJsonArray executeMenuItemSearchQuery(const QString& query, const QVariantMap& bindValues);
};

#endif // RESTAURANTMANAGER_H 