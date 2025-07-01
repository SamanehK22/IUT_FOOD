#ifndef HOMEADMIN_H
#define HOMEADMIN_H

#include <QWidget>

namespace Ui {
class HomeAdmin;
}

class HomeAdmin : public QWidget
{
    Q_OBJECT

public:
    explicit HomeAdmin(QWidget *parent = nullptr);
    ~HomeAdmin();

private slots:
    void on_admin_profile_pushButton_clicked();

    void on_manage_Restaurant_pushButton_clicked();

    void on_manage_users_pushButton_clicked();

    void on_Track_order_pushButton_clicked();

    void on_back_pushButton_clicked();

private:
    Ui::HomeAdmin *ui;
};

#endif // HOMEADMIN_H
