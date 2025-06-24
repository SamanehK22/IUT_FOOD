#include <QCoreApplication>
#include <QDebug>
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Initialize database connection
    DatabaseManager* dbManager = DatabaseManager::getInstance();
    if (!dbManager->connect("", "iutfood.db", "", "")) {  // SQLite connection parameters
        qDebug() << "Failed to connect to database. Exiting...";
        return 1;
    }

    return a.exec();
} 