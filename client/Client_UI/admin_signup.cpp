#include "admin_signup.h"
#include "ui_admin_signup.h"
#include "forgot_pass.h"

Admin_Signup::Admin_Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_Signup)
{
    ui->setupUi(this);
}

Admin_Signup::~Admin_Signup()
{
    delete ui;
}

void Admin_Signup::on_Coustomer_Button_clicked()
{
   signup_coustomer *signupForm = new signup_coustomer;
   signupForm->show();
   this->close();
}

void Admin_Signup::on_Restaurant_Button_clicked()
{
    signup_restaurant *signupForm = new signup_restaurant;
    signupForm->show();
    this->close();
}

void Admin_Signup::on_pushButton_clicked()
{
    forgot_pass *form = new forgot_pass;
    form->show();
    this->close();
}
