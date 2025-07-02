#ifndef SIGNUP_COUSTOMER_H
#define SIGNUP_COUSTOMER_H

#include <QWidget>

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

private:
    Ui::signup_coustomer *ui;
};

#endif // SIGNUP_COUSTOMER_H
