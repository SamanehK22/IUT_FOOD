#include "signup_coustomer.h"
#include "ui_signup_coustomer.h"
#include "admin_signup.h"
#include <QMessageBox>
#include "../src/network/authmanager.h"
#include "../src/network/networkmanager.h"
#include <QDebug>

signup_coustomer::signup_coustomer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signup_coustomer)
{
    ui->setupUi(this);
    setupConnections();
}

signup_coustomer::~signup_coustomer()
{
    AuthManager *auth = AuthManager::getInstance();
    disconnect(auth, &AuthManager::registerSuccess, this, &signup_coustomer::onRegisterSuccess);
    disconnect(auth, &AuthManager::registerFailed, this, &signup_coustomer::onRegisterFailed);
    delete ui;
}

void signup_coustomer::setupConnections()
{
    AuthManager *auth = AuthManager::getInstance();
    connect(auth, &AuthManager::registerSuccess, this, &signup_coustomer::onRegisterSuccess);
    connect(auth, &AuthManager::registerFailed, this, &signup_coustomer::onRegisterFailed);
}

void signup_coustomer::on_Signup_pushButton_clicked()
{
    QString firstName = ui->FullName_lineEdit->text().trimmed();
    QString lastName = ui->FullName_lineEdit_2->text().trimmed();
    QString phone = ui->Password_lineEdit_3->text().trimmed();
    QString email = ui->Password_lineEdit->text().trimmed();
    QString password = ui->Password_lineEdit_4->text();
    QString city = ui->City_comboBox->currentText();
    QString location = ui->Password_lineEdit_2->text().trimmed();

    qDebug() << "[Customer Signup] firstName:" << firstName << ", lastName:" << lastName << ", email:" << email << ", phone:" << phone << ", password:" << password << ", city:" << city << ", location:" << location;

    if (firstName.isEmpty() || lastName.isEmpty() || phone.isEmpty() || email.isEmpty() || password.isEmpty() || city.isEmpty() || location.isEmpty()) {
        QMessageBox::warning(this, "Signup Error", "Please fill in all fields.");
        return;
    }

    // Check if server is connected
    NetworkManager *network = NetworkManager::getInstance();
    if (!network->isConnected()) {
        QMessageBox::critical(this, "Connection Error", "Cannot connect to server. Please make sure the server is running and try again.");
        return;
    }

    AuthManager *auth = AuthManager::getInstance();
    // For customers, restaurantName is empty
    auth->registerUser(firstName, lastName, email, phone, password, "customer", "", city, location);
}

void signup_coustomer::onRegisterSuccess()
{
    QMessageBox::information(this, "Signup Successful", "Signup completed successfully. You can now log in.");
    // Do not close the window
}

void signup_coustomer::onRegisterFailed(const QString &error)
{
    QMessageBox::critical(this, "Signup Failed", error);
    // Do not close the window
}

void signup_coustomer::on_back_pushButton_clicked()
{
    Admin_Signup *adminSignupWindow = new Admin_Signup();
    adminSignupWindow->setAttribute(Qt::WA_DeleteOnClose);
    adminSignupWindow->show();
    this->close();
}