#include "restaurantownersetpass.h"
#include "ui_restaurantownersetpass.h"
#include "homerestaurantowner.h"

RestaurantOwnerSetPass::RestaurantOwnerSetPass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantOwnerSetPass)
{
    ui->setupUi(this);
}

RestaurantOwnerSetPass::~RestaurantOwnerSetPass()
{
    delete ui;
}

void RestaurantOwnerSetPass::on_backPasswordRestaurantOwner_pushButton_clicked()
{
    this->close();

    homerestaurantowner *homeWindow = new homerestaurantowner;
    homeWindow->show();
}
