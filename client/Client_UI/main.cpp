#include "admin_signup.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Admin_Signup w;
    w.show();
    return a.exec();

}
