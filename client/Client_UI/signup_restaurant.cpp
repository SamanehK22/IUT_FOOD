#include "signup_restaurant.h"
#include "ui_signup_restaurant.h"
#include "restaurantapprovalwindow.h"
#include "admin_signup.h"
#include "homerestaurantowner.h"


signup_restaurant::signup_restaurant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signup_restaurant)
{
    ui->setupUi(this);
}

signup_restaurant::~signup_restaurant()
{
    delete ui;
}

void signup_restaurant::on_Signup_pushButton_clicked()
{
    // نمایش پیام موفقیت‌آمیز بودن ثبت‌نام
    QMessageBox::information(this, "Signup Successful",
        "Signup completed successfully. Please wait for admin approval to log in.");
<<<<<<< HEAD

    // باز کردن صفحه‌ی اصلی رستوران‌دار
    homerestaurantowner *homeWindow = new homerestaurantowner();
    homeWindow->setAttribute(Qt::WA_DeleteOnClose); // آزادسازی حافظه پس از بستن
    homeWindow->show();
=======
}
>>>>>>> 86fecec34e62335f33139d0cd026c6a352ae5751

    // بستن فرم فعلی
    this->close();
}

void signup_restaurant::on_back_pushButton_clicked()
{
    Admin_Signup *adminSignupWindow = new Admin_Signup();
    adminSignupWindow->setAttribute(Qt::WA_DeleteOnClose);
    adminSignupWindow->show();
    this->close();
}