#include "profile.h"
#include "ui_profile.h"
#include "restaurantlistwindow.h"
#include "reset_pass.h"
#include "customersetpass.h"


Profile::Profile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Profile)
{
    ui->setupUi(this);
}

Profile::~Profile()
{
    delete ui;
}

void Profile::on_back_pushButton_clicked()
{
    // ایجاد پنجره جدید لیست رستوران‌ها
    RestaurantListWindow *restaurantWindow = new RestaurantListWindow();
    restaurantWindow->show();

    // بستن پنجره پروفایل
    this->close();
}


void Profile::on_changepassword_pushButton_2_clicked()
{
    // ایجاد و نمایش پنجره تغییر رمز عبور
    Reset_Pass *resetPassWindow = new Reset_Pass();
    resetPassWindow->show();

    // بستن پنجره پروفایل فعلی
    this->close();
}


void Profile::on_changepassword_pushButton_3_clicked()
{
    customersetpass* setPassWindow = new customersetpass;
    setPassWindow->show();
    this->close();
}

void Profile::on_backCoustomer_pushButton_clicked()
{
    RestaurantListWindow* restaurantWindow = new RestaurantListWindow();
    restaurantWindow->show();
    this->close();
}
