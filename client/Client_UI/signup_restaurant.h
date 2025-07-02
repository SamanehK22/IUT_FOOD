#ifndef SIGNUP_RESTAURANT_H
#define SIGNUP_RESTAURANT_H

#include <QWidget>
#include <QMessageBox>
#include "../src/network/authmanager.h"
#include "../src/network/networkmanager.h"

namespace Ui {
class signup_restaurant;
}

class signup_restaurant : public QWidget
{
    Q_OBJECT

public:
    explicit signup_restaurant(QWidget *parent = nullptr);
    ~signup_restaurant();

private slots:
    void on_Signup_pushButton_clicked();
    void on_back_pushButton_clicked();
    void onRegisterSuccess();
    void onRegisterFailed(const QString &error);

private:
    void setupConnections();
    Ui::signup_restaurant *ui;
};

#endif // SIGNUP_RESTAURANT_H