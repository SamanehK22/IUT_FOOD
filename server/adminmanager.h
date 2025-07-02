#ifndef ADMINMANAGER_H
#define ADMINMANAGER_H

#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include "databasemanager.h"
#include "authsystem.h"

class AdminManager : public QObject
{
    Q_OBJECT

public:
    static AdminManager* getInstance();

    // Restaurant Management
    bool approveRestaurant(const QString& restaurantId);
    bool rejectRestaurant(const QString& restaurantId, const QString& reason);
    bool suspendRestaurant(const QString& restaurantId, const QString& reason);
    bool reactivateRestaurant(const QString& restaurantId);
    QJsonArray getPendingRestaurants();
    QJsonArray getSuspendedRestaurants();

    // User Management
    bool blockUser(const QString& userId, const QString& reason);
    bool unblockUser(const QString& userId);
    bool deleteUser(const QString& userId);
    QJsonArray getBlockedUsers();
    QJsonArray getAllUsers();

    // Reports and Analytics
    QJsonObject getSalesReport(const QDateTime& startDate, const QDateTime& endDate);
    QJsonObject getRestaurantPerformanceReport(const QString& restaurantId);
    QJsonObject getSystemAnalytics();
    QJsonArray getTopSellingItems(const QDateTime& startDate, const QDateTime& endDate);
    QJsonArray getMostActiveUsers(const QDateTime& startDate, const QDateTime& endDate);
    QJsonArray getMostPopularRestaurants(const QDateTime& startDate, const QDateTime& endDate);

    // Report Distribution
    void broadcastReport(const QString& reportType, const QJsonObject& report);
    void sendReportToUser(const QString& userId, const QString& reportType, const QJsonObject& report);

private:
    explicit AdminManager(QObject *parent = nullptr);
    ~AdminManager();
    static AdminManager* instance;

    DatabaseManager* m_dbManager;
    AuthSystem* m_authSystem;

    bool validateAdminAccess(const QString& userId);
    QJsonObject generateSalesChart(const QJsonArray& salesData);
    QJsonObject generateUserActivityChart(const QJsonArray& userData);
    QJsonObject generateRestaurantPerformanceChart(const QJsonArray& performanceData);
    QString generateReportId();
    void logAdminAction(const QString& adminId, const QString& action, const QString& details);
};

#endif // ADMINMANAGER_H 