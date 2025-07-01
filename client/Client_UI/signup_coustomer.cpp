#include "signup_coustomer.h"
#include "ui_signup_coustomer.h"
#include "cartwindow.h"
#include "admin_signup.h"
#include "homeadmin.h"



#include "usermanagementwindow.h"


signup_coustomer::signup_coustomer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signup_coustomer)
{
    ui->setupUi(this);
}

signup_coustomer::~signup_coustomer()
{
    delete ui;
}

void signup_coustomer::on_Signup_pushButton_clicked()
{
    HomeAdmin *home = new HomeAdmin();                      // ساخت فرم HomeAdmin
    home->setAttribute(Qt::WA_DeleteOnClose);               // آزادسازی حافظه هنگام بستن
    home->show();                                            // نمایش فرم HomeAdmin

    this->close();                                           // بستن فرم ثبت‌نام
}

void signup_coustomer::on_back_pushButton_clicked()
{
    Admin_Signup *adminSignupWindow = new Admin_Signup();
    adminSignupWindow->setAttribute(Qt::WA_DeleteOnClose);
    adminSignupWindow->show();
    this->close();
}
