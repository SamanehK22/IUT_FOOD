#include "homerestaurantowner.h"
#include "ui_homerestaurantowner.h"
#include "profilerestaurantowner.h"
#include "admin_signup.h"
#include "receivedorderswindow.h"
#include "managemenuwindow.h"

homerestaurantowner::homerestaurantowner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::homerestaurantowner)
{
    ui->setupUi(this);
}

homerestaurantowner::~homerestaurantowner()
{
    delete ui;
}

void homerestaurantowner::on_RestaurantOwnerProfile_pushButton_clicked()
{
    this->close();
    ProfileRestaurantOwner *profileWindow = new ProfileRestaurantOwner;
    profileWindow->show();
}

void homerestaurantowner::on_backRestaurantOwner_pushButton_clicked()
{
    Admin_Signup *adminWindow = new Admin_Signup();
    adminWindow->setAttribute(Qt::WA_DeleteOnClose);
    adminWindow->show();
    this->close();
}

void homerestaurantowner::on_RestaurantOwner_ReceivedOrders_pushButton_clicked()
{
    ReceivedOrdersWindow* receivedOrdersWindow = new ReceivedOrdersWindow(this); // parent برای مدیریت حافظه
    receivedOrdersWindow->show();
    this->close();
}

void homerestaurantowner::on_RestaurantOwner_MangeMenu_pushButton_clicked()
{
    this->close();
    ManageMenuWindow *menuwindow = new ManageMenuWindow;
    menuwindow->show();
}
