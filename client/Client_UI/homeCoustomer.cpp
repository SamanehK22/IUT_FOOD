#include "homeCoustomer.h"
#include "ui_homeCoustomer.h"

HomeCoustomer::HomeCoustomer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
}

HomeCoustomer::~HomeCoustomer()
{
    delete ui;
}
