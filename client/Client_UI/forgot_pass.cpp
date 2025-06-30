#include "forgot_pass.h"
#include "ui_forgot_pass.h"
#include "reset_pass.h"

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
