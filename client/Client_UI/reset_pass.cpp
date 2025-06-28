#include "reset_pass.h"
#include "ui_reset_pass.h"
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
    Menu *menuPage = new Menu();  // ساخت صفحه منو
    menuPage->show();             // نمایش صفحه منو
    this->close();                // بستن صفحه فعلی (Reset_Pass)
}
