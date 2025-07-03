#ifndef ADMIN_SIGNUP_H
#define ADMIN_SIGNUP_H

#include <QWidget>
#include "signup_coustomer.h"
#include "signup_restaurant.h"
#include "restaurantlistwindow.h"

class HomeAdmin; // Forward declaration added
class homerestaurantowner; // Forward declaration

namespace Ui {
class Admin_Signup;
}

class Admin_Signup : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_Signup(QWidget *parent = nullptr);
    ~Admin_Signup();

private slots:
    void on_Coustomer_Button_clicked();

    void on_Restaurant_Button_clicked();

    void on_pushButton_clicked();

    void on_Login_Button_clicked();


    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Admin_Signup *ui;
    HomeAdmin *adminHome = nullptr;
    RestaurantListWindow *restaurantList = nullptr;
    homerestaurantowner *restaurantOwnerHome = nullptr;
};

#endif // ADMIN_SIGNUP_H
