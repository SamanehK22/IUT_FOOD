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

    // Start server
    Server* server = Server::getInstance();
    if (!server->start(8080)) {
        qDebug() << "Failed to start server";
        return -1;
    }

    qDebug() << "IUT Food Server is running on port 8080";

    return a.exec();
} 