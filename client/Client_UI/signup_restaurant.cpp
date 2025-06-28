#include "signup_restaurant.h"
#include "ui_signup_restaurant.h"

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
