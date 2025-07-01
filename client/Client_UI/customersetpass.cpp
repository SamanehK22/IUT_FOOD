#include "customersetpass.h"
#include "ui_customersetpass.h"
#include "profile.h"

customersetpass::customersetpass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::customersetpass)
{
    ui->setupUi(this);
}

customersetpass::~customersetpass()
{
    delete ui;
}

void customersetpass::on_back_pushButton_clicked()
{
    Profile* profileWindow = new Profile;
    profileWindow->show();
    this->close();
}
