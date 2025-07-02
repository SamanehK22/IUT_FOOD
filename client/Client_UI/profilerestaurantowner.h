#ifndef PROFILERESTAURANTOWNER_H
#define PROFILERESTAURANTOWNER_H

#include <QWidget>

namespace Ui {
class ProfileRestaurantOwner;
}

class ProfileRestaurantOwner : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileRestaurantOwner(QWidget *parent = nullptr);
    ~ProfileRestaurantOwner();

private slots:
    void on_back_pushButton_clicked();

private:
    Ui::ProfileRestaurantOwner *ui;
};

#endif // PROFILERESTAURANTOWNER_H
