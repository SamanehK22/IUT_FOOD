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

private:
    Ui::signup_coustomer *ui;
};

#endif // SIGNUP_COUSTOMER_H
