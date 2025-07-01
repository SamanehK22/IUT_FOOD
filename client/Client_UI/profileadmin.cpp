#include "profileadmin.h"
#include "ui_profileadmin.h"
#include "homeadmin.h"
#include "adminsetpass.h"

profileadmin::profileadmin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::profileadmin)
{
    ui->setupUi(this);
}

profileadmin::~profileadmin()
{
    delete ui;
}

void profileadmin::on_back_pushButton_clicked()
{
    HomeAdmin *home = new HomeAdmin();
    home->show();
    this->close();  // یا this->hide(); اگر نمی‌خوای کامل بسته شه
}

void profileadmin::on_changepassword_pushButton_8_clicked()
{
    adminsetpass *setPassWindow = new adminsetpass();  // ساختن پنجره جدید
    setPassWindow->show();                              // نمایش پنجره جدید
    this->close();                                      // بستن پنجره فعلی
}
