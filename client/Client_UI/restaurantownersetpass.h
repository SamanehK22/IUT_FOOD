#ifndef RESTAURANTOWNERSETPASS_H
#define RESTAURANTOWNERSETPASS_H

#include <QWidget>

namespace Ui {
class RestaurantOwnerSetPass;
}

class RestaurantOwnerSetPass : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerSetPass(QWidget *parent = nullptr);
    ~RestaurantOwnerSetPass();

private slots:
    void on_backPasswordRestaurantOwner_pushButton_clicked();

private:
    Ui::RestaurantOwnerSetPass *ui;
};

#endif // RESTAURANTOWNERSETPASS_H
