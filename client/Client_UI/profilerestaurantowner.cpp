#include "profilerestaurantowner.h"
#include "ui_profilerestaurantowner.h"
#include "homerestaurantowner.h"

ProfileRestaurantOwner::ProfileRestaurantOwner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileRestaurantOwner)
{
    ui->setupUi(this);
}

ProfileRestaurantOwner::~ProfileRestaurantOwner()
{
    delete ui;
}

void ProfileRestaurantOwner::on_back_pushButton_clicked()
{
    this->close();
    homerestaurantowner *homeWindow = new homerestaurantowner;
    homeWindow->show();
}
