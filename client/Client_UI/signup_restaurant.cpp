#include "signup_restaurant.h"
#include "ui_signup_restaurant.h"
#include "admin_signup.h"
#include "homerestaurantowner.h"
#include <QMessageBox>
#include "../src/network/authmanager.h"
#include "../src/network/networkmanager.h"
#include <QDebug>

signup_restaurant::signup_restaurant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signup_restaurant)
{
    ui->setupUi(this);
    setupConnections();
}

signup_restaurant::~signup_restaurant()
{
    AuthManager *auth = AuthManager::getInstance();
    disconnect(auth, &AuthManager::registerSuccess, this, &signup_restaurant::onRegisterSuccess);
    disconnect(auth, &AuthManager::registerFailed, this, &signup_restaurant::onRegisterFailed);
    delete ui;
}

void signup_restaurant::setupConnections()
{
    AuthManager *auth = AuthManager::getInstance();
    connect(auth, &AuthManager::registerSuccess, this, &signup_restaurant::onRegisterSuccess);
    connect(auth, &AuthManager::registerFailed, this, &signup_restaurant::onRegisterFailed);
}

void signup_restaurant::on_Signup_pushButton_clicked()
{
    qDebug() << "SIGNUP BUTTON CLICKED!";
    QString firstName = ui->FullName_lineEdit->text().trimmed();
    QString lastName = ui->FullName_lineEdit_3->text().trimmed();
    QString phone = ui->Password_lineEdit_3->text().trimmed();
    QString email = ui->Password_lineEdit->text().trimmed();
    QString password = ui->Password_lineEdit_4->text();
    QString restaurantName = ui->FullName_lineEdit_2->text().trimmed();
    QString city = ui->City_comboBox->currentText();
    QString location = ui->Password_lineEdit_2->text().trimmed();
    QString type = ui->type_comboBox->currentText();

    qDebug() << "[Restaurant Signup] Clicked. Values:";
    qDebug() << "  firstName:" << firstName;
    qDebug() << "  lastName:" << lastName;
    qDebug() << "  email:" << email;
    qDebug() << "  phone:" << phone;
    qDebug() << "  password:" << password;
    qDebug() << "  restaurantName:" << restaurantName;
    qDebug() << "  city:" << city;
    qDebug() << "  location:" << location;
    qDebug() << "  type:" << type;

    if (firstName.isEmpty() || lastName.isEmpty() || phone.isEmpty() || email.isEmpty() || password.isEmpty() ||
        restaurantName.isEmpty() || city.isEmpty() || location.isEmpty() || type.isEmpty()) {
        qDebug() << "[Restaurant Signup] Validation failed: missing fields.";
        QMessageBox::warning(this, "Signup Error", "Please fill in all fields.");
        return;
    }
    qDebug() << "[Restaurant Signup] Validation passed.";

    NetworkManager *network = NetworkManager::getInstance();
    if (!network->isConnected()) {
        qDebug() << "[Restaurant Signup] Network not connected.";
        QMessageBox::critical(this, "Connection Error", "Cannot connect to server. Please make sure the server is running and try again.");
        return;
    }
    qDebug() << "[Restaurant Signup] Network connected. Calling registerRestaurantOwner...";

    AuthManager *auth = AuthManager::getInstance();
    auth->registerRestaurantOwner(firstName, lastName, email, phone, password, restaurantName, city, location, type);
    qDebug() << "[Restaurant Signup] registerRestaurantOwner called.";
}

void signup_restaurant::onRegisterSuccess()
{
    QMessageBox::information(this, "Awaiting Approval", "Registration completed. Please wait for admin approval before you can log in.");
}

void signup_restaurant::onRegisterFailed(const QString &error)
{
    QMessageBox::critical(this, "Signup Failed", error);
    // نمایش پیام موفقیت‌آمیز بودن ثبت‌نام
    QMessageBox::information(this, "Signup Successful",
        "Signup completed successfully. Please wait for admin approval to log in.");

    // باز کردن صفحه‌ی اصلی رستوران‌دار
    homerestaurantowner *homeWindow = new homerestaurantowner();
    homeWindow->setAttribute(Qt::WA_DeleteOnClose); // آزادسازی حافظه پس از بستن
    homeWindow->show();

    // بستن فرم فعلی
    this->close();
}

void signup_restaurant::on_back_pushButton_clicked()
{
    Admin_Signup *adminSignupWindow = new Admin_Signup();
    adminSignupWindow->setAttribute(Qt::WA_DeleteOnClose);
    adminSignupWindow->show();
    this->close();
}