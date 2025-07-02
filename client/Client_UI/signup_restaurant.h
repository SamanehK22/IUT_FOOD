#ifndef SIGNUP_RESTAURANT_H
#define SIGNUP_RESTAURANT_H

#include <QWidget>
#include <QMessageBox>

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

private:
    Ui::signup_restaurant *ui;
};

#endif // SIGNUP_RESTAURANT_H
