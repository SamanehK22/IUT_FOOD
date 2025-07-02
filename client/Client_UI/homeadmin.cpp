#include "homeadmin.h"
#include "ui_homeadmin.h"
#include "profileadmin.h"
#include "restaurantapprovalwindow.h"
#include "usermanagementwindow.h"
#include "admin_signup.h"

HomeAdmin::HomeAdmin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeAdmin)
{
    ui->setupUi(this);
}

HomeAdmin::~HomeAdmin()
{
    delete ui;
}


void HomeAdmin::on_admin_profile_pushButton_clicked()
{
    profileadmin *profileadminWindow = new profileadmin();
    profileadminWindow->show();   // یا exec() اگر profile از QDialog ارث برده
    this->close();           // بستن پنجره HomeAdmin
}

void HomeAdmin::on_manage_Restaurant_pushButton_clicked()
{
    RestaurantApprovalWindow *window = new RestaurantApprovalWindow();  // ساخت پنجره مدیریت رستوران‌ها
    window->show();  // نمایش پنجره جدید
    this->close();   // بستن صفحه HomeAdmin
}

void HomeAdmin::on_manage_users_pushButton_clicked()
{
    UserManagementWindow *userWindow = new UserManagementWindow();  // ساخت پنجره مدیریت کاربران
    userWindow->show();   // نمایش پنجره جدید
    this->close();        // بستن صفحه HomeAdmin
}

void HomeAdmin::on_Track_order_pushButton_clicked()
{

}

void HomeAdmin::on_back_pushButton_clicked()
{
    Admin_Signup *signupPage = new Admin_Signup();  // ایجاد صفحه ثبت‌نام ادمین
    signupPage->show();                             // نمایش آن
    this->close();                                  // بستن صفحه HomeAdmin
}
