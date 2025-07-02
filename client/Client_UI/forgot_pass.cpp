#include "forgot_pass.h"
#include "ui_forgot_pass.h"
#include "reset_pass.h"
#include "admin_signup.h"

forgot_pass::forgot_pass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::forgot_pass)
{
    ui->setupUi(this);
}

forgot_pass::~forgot_pass()
{
    delete ui;
}

void forgot_pass::on_pushButton_clicked()
{
    Reset_Pass *form = new Reset_Pass;
    form->show();
    this->close();
}

void forgot_pass::on_back_pushButton_clicked()
{
    Admin_Signup *loginWindow = new Admin_Signup();
    loginWindow->setAttribute(Qt::WA_DeleteOnClose); // تا بعد از بسته شدن حافظه آزاد بشه
    loginWindow->show();      // نمایش فرم login
    this->close();            // بستن فرم forgot_pass فعلی
}
