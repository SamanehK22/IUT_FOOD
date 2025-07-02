#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <QObject>
#include <QVariantMap>
#include <QJsonArray>
#include "databasemanager.h"

class MenuManager : public QObject
{
    Q_OBJECT
public:
    static MenuManager* getInstance();
    ~MenuManager();

    bool createMenu(const QString& restaurantId);
    bool addMenuItem(const QString& menuId, const QString& name, const QString& description, double price, const QString& ingredients, const QString& category, const QString& imageUrl = "");
    bool updateMenuItem(const QString& menuId, const QString& itemId, const QVariantMap& updates);
    bool deleteMenuItem(const QString& menuId, const QString& itemId);
    QJsonArray getMenu(const QString& restaurantId);
    // Add more menu-related methods as needed

private:
    explicit MenuManager(QObject *parent = nullptr);
    static MenuManager* instance;
    DatabaseManager* m_dbManager;
};

#endif // MENUMANAGER_H 