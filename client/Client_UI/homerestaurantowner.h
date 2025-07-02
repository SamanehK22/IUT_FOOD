#ifndef HOMERESTAURANTOWNER_H
#define HOMERESTAURANTOWNER_H

#include <QWidget>

namespace Ui {
class homerestaurantowner;
}

class homerestaurantowner : public QWidget
{
    Q_OBJECT

public:
    explicit homerestaurantowner(QWidget *parent = nullptr);
    ~homerestaurantowner();

private slots:
    void on_RestaurantOwnerProfile_pushButton_clicked();

    void on_backRestaurantOwner_pushButton_clicked();

    void on_RestaurantOwner_ReceivedOrders_pushButton_clicked();

    void on_RestaurantOwner_MangeMenu_pushButton_clicked();

private:
    Ui::homerestaurantowner *ui;
};

#endif // HOMERESTAURANTOWNER_H
