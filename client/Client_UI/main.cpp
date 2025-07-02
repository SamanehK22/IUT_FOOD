#include "admin_signup.h"
#include <../src/network/networkmanager.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetworkManager::getInstance()->connectToServer("127.0.0.1", 8080, 8081);
    Admin_Signup w;
    w.show();
    return a.exec();

}
