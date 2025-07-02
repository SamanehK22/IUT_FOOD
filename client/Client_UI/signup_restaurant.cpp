#include "signup_restaurant.h"
#include "ui_signup_restaurant.h"
#include "admin_signup.h"
#include <QMessageBox>
#include "../src/network/authmanager.h"
#include "../src/network/networkmanager.h"

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
    QString firstName = ui->FullName_lineEdit->text().trimmed();
    QString lastName = ui->FullName_lineEdit_3->text().trimmed();
    QString phone = ui->Password_lineEdit_3->text().trimmed();
    QString email = ui->Password_lineEdit->text().trimmed();
    QString password = ui->Password_lineEdit_4->text();
    QString restaurantName = ui->FullName_lineEdit_2->text().trimmed();
    QString city = ui->City_comboBox->currentText();
    QString location = ui->Password_lineEdit_2->text().trimmed();

    if (firstName.isEmpty() || lastName.isEmpty() || phone.isEmpty() || email.isEmpty() || password.isEmpty() ||
        restaurantName.isEmpty() || city.isEmpty() || location.isEmpty()) {
        QMessageBox::warning(this, "Signup Error", "Please fill in all fields.");
        return;
    }

    NetworkManager *network = NetworkManager::getInstance();
    if (!network->isConnected()) {
        QMessageBox::critical(this, "Connection Error", "Cannot connect to server. Please make sure the server is running and try again.");
        return;
    }

    AuthManager *auth = AuthManager::getInstance();
    auth->registerUser(firstName, lastName, email, phone, password, "restaurant_owner", restaurantName, city, location);
}

void signup_restaurant::onRegisterSuccess()
{
    QMessageBox::information(this, "Awaiting Approval", "Registration completed. Please wait for admin approval before you can log in.");
}

void signup_restaurant::onRegisterFailed(const QString &error)
{
    QMessageBox::critical(this, "Signup Failed", error);
}

void signup_restaurant::on_back_pushButton_clicked()
{
    Admin_Signup *adminSignupWindow = new Admin_Signup();
    adminSignupWindow->setAttribute(Qt::WA_DeleteOnClose);
    adminSignupWindow->show();
    this->close();
}