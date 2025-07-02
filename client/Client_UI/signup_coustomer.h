#ifndef SIGNUP_COUSTOMER_H
#define SIGNUP_COUSTOMER_H

#include <QWidget>
#include <QMessageBox>
#include "../src/network/authmanager.h"

namespace Ui {
class signup_coustomer;
}

class signup_coustomer : public QWidget
{
    Q_OBJECT

public:
    explicit signup_coustomer(QWidget *parent = nullptr);
    ~signup_coustomer();

private slots:
    void on_Signup_pushButton_clicked();
    void on_back_pushButton_clicked();
    void onRegisterSuccess();
    void onRegisterFailed(const QString &error);

private:
    Ui::signup_coustomer *ui;
    void setupConnections();
};

#endif // SIGNUP_COUSTOMER_H
