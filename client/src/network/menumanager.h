#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

class MenuManager : public QObject
{
    Q_OBJECT
public:
    static MenuManager* getInstance();

    // Menu operations
    void createMenu(const QString& restaurantId);
    void addMenuItem(const QString& menuId, const QString& name, const QString& description, double price, const QString& ingredients, const QString& category, const QString& imageUrl = "");
    void updateMenuItem(const QString& menuId, const QString& itemId, const QJsonObject& updates);
    void deleteMenuItem(const QString& menuId, const QString& itemId);
    void fetchMenu(const QString& restaurantId);

signals:
    void menuCreated(bool success);
    void menuItemAdded(bool success);
    void menuItemUpdated(bool success);
    void menuItemDeleted(bool success);
    void menuReceived(const QJsonArray& menu);

private:
    explicit MenuManager(QObject* parent = nullptr);
    ~MenuManager();
    static MenuManager* instance;
};

#endif // MENUMANAGER_H 