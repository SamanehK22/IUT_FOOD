#include "admin_signup.h"
#include "ui_admin_signup.h"
#include "forgot_pass.h"
#include "history.h"
#include "homeadmin.h"
#include "restaurantlistwindow.h"
#include "../src/network/authmanager.h"
#include "../src/models/user.h"
#include <QDebug>




Admin_Signup::Admin_Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_Signup)
{
    ui->setupUi(this);
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
    QString loginId = ui->Email_lineEdit->text().trimmed();
    QString password = ui->Password_lineEdit->text().trimmed();
    AuthManager* auth = AuthManager::getInstance();
    connect(auth, &AuthManager::loginSuccess, this, [this]() {
        qDebug() << "[Login] Success lambda triggered";
        User* user = AuthManager::getInstance()->currentUser();
        if (!user) return;
        QString type = user->userType();
        QMessageBox::information(this, "Login Successful", "You have logged in successfully.");
        if (type == "admin") {
            HomeAdmin* adminHome = new HomeAdmin();
            adminHome->show();
        } else if (type == "customer") {
            RestaurantListWindow* restaurantList = new RestaurantListWindow();
            restaurantList->setAttribute(Qt::WA_DeleteOnClose);
            restaurantList->resize(700, 600);
            restaurantList->show();
        } else if (type == "restaurant_owner") {
            // TODO: Show restaurant owner window when UI is ready
        }
        this->close();
    });
    connect(auth, &AuthManager::loginFailed, this, [this](const QString& error) {
        qDebug() << "[Login] Failure lambda triggered: " << error;
        QMessageBox::critical(this, "Login Failed", error);
    });
    auth->login(loginId, password);
}





void Admin_Signup::on_pushButton_2_clicked()
{
}

void Admin_Signup::on_pushButton_3_clicked()
{
}


void Admin_Signup::on_mina_clicked()
{
    ManageMenuWindow *manageMenuWindow = new ManageMenuWindow();

      // نمایش پنجره جدید
      manageMenuWindow->show();

      // بستن پنجره فعلی
      this->close();
}

void Admin_Signup::on_mahtab_clicked()
{
    ReceivedOrdersWindow* ordersWindow = new ReceivedOrdersWindow();
    ordersWindow->setAttribute(Qt::WA_DeleteOnClose); // وقتی بسته شد حذف شود
    ordersWindow->setWindowTitle("Received Orders");
    ordersWindow->resize(500, 600);
    ordersWindow->show();
}
