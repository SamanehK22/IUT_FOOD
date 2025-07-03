#include "admin_signup.h"
#include "ui_admin_signup.h"
#include "forgot_pass.h"
#include "history.h"
#include "homeadmin.h"
#include "restaurantlistwindow.h"
#include "homerestaurantowner.h"
#include "../src/network/authmanager.h"
#include "../src/models/user.h"
#include <QDebug>
#include <QMessageBox>

Admin_Signup::Admin_Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_Signup)
{
    qDebug() << "[Admin_Signup] Constructor this:" << this;
    ui->setupUi(this);

    AuthManager* auth = AuthManager::getInstance();
    qDebug() << "[Admin_Signup] AuthManager instance in constructor:" << auth;
    connect(auth, &AuthManager::loginSuccess, this, [this]() {
        qDebug() << "[Login] Success lambda triggered, this:" << this << ", AuthManager instance:" << AuthManager::getInstance();
        User* user = AuthManager::getInstance()->currentUser();
        if (!user) {
            qDebug() << "[Login] No user object after login!";
            return;
        }
        QString type = user->userType();
        qDebug() << "[Login] User type:" << type;
        QMessageBox::information(this, "Login Successful", "You have logged in successfully.");

        if (type == "admin") {
            qDebug() << "[Login] Showing HomeAdmin window.";
            this->adminHome = new HomeAdmin();
            this->adminHome->setAttribute(Qt::WA_DeleteOnClose);
            this->adminHome->show();
        } else if (type == "customer") {
            qDebug() << "[Login] Showing RestaurantListWindow for customer.";
            this->restaurantList = new RestaurantListWindow();
            this->restaurantList->setAttribute(Qt::WA_DeleteOnClose);
            this->restaurantList->resize(700, 600);
            this->restaurantList->show();
        } else if (type == "restaurant_owner") {
            qDebug() << "[Login] Showing homerestaurantowner window.";
            this->restaurantOwnerHome = new homerestaurantowner();
            this->restaurantOwnerHome->setAttribute(Qt::WA_DeleteOnClose);
            this->restaurantOwnerHome->show();
        } else {
            qDebug() << "[Login] Unknown user type:" << type;
        }

        qDebug() << "[Login] Closing login window.";
        this->close();
    });
    qDebug() << "[Admin_Signup] Connected loginSuccess signal.";

    connect(auth, &AuthManager::loginFailed, this, [this](const QString& error) {
        qDebug() << "[Login] Failure lambda triggered: " << error;
        if (error.contains("pending", Qt::CaseInsensitive)) {
            QMessageBox::information(this, "Pending Approval", "Your restaurant is pending admin approval. Please wait for approval before logging in.");
        } else {
            QMessageBox::critical(this, "Login Failed", error);
        }
    });
}

Admin_Signup::~Admin_Signup()
{
    delete ui;
}

void Admin_Signup::on_Coustomer_Button_clicked()
{
    signup_coustomer *signupForm = new signup_coustomer;
    signupForm->show();
    this->close();
}

void Admin_Signup::on_Restaurant_Button_clicked()
{
    signup_restaurant *signupForm = new signup_restaurant;
    signupForm->show();
    this->close();
}

void Admin_Signup::on_pushButton_clicked()
{
    forgot_pass *form = new forgot_pass;
    form->show();
    this->close();
}

void Admin_Signup::on_Login_Button_clicked()
{
    qDebug() << "[Admin_Signup] on_Login_Button_clicked this:" << this;
    QString loginId = ui->Email_lineEdit->text().trimmed();
    QString password = ui->Password_lineEdit->text().trimmed();
    AuthManager* auth = AuthManager::getInstance();
    auth->login(loginId, password);
}

void Admin_Signup::on_pushButton_2_clicked() {}
void Admin_Signup::on_pushButton_3_clicked() {}
