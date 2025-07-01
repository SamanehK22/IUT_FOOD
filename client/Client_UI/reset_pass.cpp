#include "reset_pass.h"
#include "ui_reset_pass.h"
#include "forgot_pass.h"
#include <QMessageBox>
#include "menu.h"

Reset_Pass::Reset_Pass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reset_Pass)
{
    ui->setupUi(this);
}

Reset_Pass::~Reset_Pass()
{
    delete ui;
}

void Reset_Pass::on_pushButton_clicked()
{
    QMessageBox::information(this, "Password Changed", "Your password has been changed successfully.");
}

void Reset_Pass::on_back_pushButton_clicked()
{
    forgot_pass *forgotPassWindow = new forgot_pass();
    forgotPassWindow->setAttribute(Qt::WA_DeleteOnClose); // حافظه آزاد شه بعداً
    forgotPassWindow->show();     // نمایش فرم forgot_pass
    this->close();                // بستن فرم reset_pass فعلی
}
