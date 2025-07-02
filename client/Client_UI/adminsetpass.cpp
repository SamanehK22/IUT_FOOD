#include "adminsetpass.h"
#include "ui_adminsetpass.h"
#include "profileadmin.h"

adminsetpass::adminsetpass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adminsetpass)
{
    ui->setupUi(this);
}

adminsetpass::~adminsetpass()
{
    delete ui;
}

void adminsetpass::on_back_pushButton_2_clicked()
{
    profileadmin *profile = new profileadmin();  // ساخت فرم پروفایل ادمین
    profile->show();                             // نمایش فرم
    this->close();                               // بستن صفحه فعلی
}
