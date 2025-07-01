#ifndef RESTAURANTLISTWINDOW_H
#define RESTAURANTLISTWINDOW_H

#include <QWidget>
#include <QVector>

class QComboBox;
class QLineEdit;
class QVBoxLayout;

struct Restaurant {
    QString name;
    QString address;
    double rating;
    QString type;
    QString city;
};

class RestaurantListWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantListWindow(QWidget *parent = nullptr);
    void addRestaurant(const QString &name, const QString &address, double rating,
                      const QString &type, const QString &city);

private slots:
    void updateRestaurantList();
    void onBackButtonClicked();

private:
    QVector<Restaurant> allRestaurants;
    QComboBox *cityFilterCombo;
    QComboBox *typeFilterCombo;
    QLineEdit *searchBox;
    QVBoxLayout *listLayout;
};

#endif // RESTAURANTLISTWINDOW_H
