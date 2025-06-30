#ifndef SIGNUP_RESTAURANT_H
#define SIGNUP_RESTAURANT_H

#include <QWidget>

namespace Ui {
class signup_restaurant;
}

class signup_restaurant : public QWidget
{
    Q_OBJECT

public:
    explicit signup_restaurant(QWidget *parent = nullptr);
    ~signup_restaurant();

private:
    Ui::signup_restaurant *ui;
};

#endif // SIGNUP_RESTAURANT_H
