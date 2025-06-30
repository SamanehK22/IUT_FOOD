#ifndef RESTAURANTLISTWINDOW_H
#define RESTAURANTLISTWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>

class RestaurantListWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantListWindow(QWidget *parent = nullptr);
    void addRestaurant(const QString &name, const QString &address, double rating, const QString &type);

private:
    QVBoxLayout *listLayout;
    QComboBox *cityFilterCombo;
    QComboBox *typeFilterCombo;
    QLineEdit *searchBox;
};

#endif // RESTAURANTLISTWINDOW_H
