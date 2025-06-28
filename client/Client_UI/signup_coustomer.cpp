#include "signup_coustomer.h"
#include "ui_signup_coustomer.h"

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
