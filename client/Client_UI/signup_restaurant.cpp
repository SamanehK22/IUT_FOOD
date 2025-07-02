#include "signup_restaurant.h"
#include "ui_signup_restaurant.h"
#include "restaurantapprovalwindow.h"
#include "admin_signup.h"


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
}


void signup_restaurant::on_back_pushButton_clicked()
{
    Admin_Signup *adminSignupWindow = new Admin_Signup();
    adminSignupWindow->setAttribute(Qt::WA_DeleteOnClose);
    adminSignupWindow->show();
    this->close();
}