#ifndef RESTAURANTITEMWIDGET_H
#define RESTAURANTITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>

class RestaurantItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RestaurantItemWidget(QString name, QString address, double rating, QString imagePath = "", QWidget *parent = nullptr);

private:
    QLabel *nameLabel;
    QLabel *addressLabel;
    QLabel *ratingLabel;
    QLabel *imageLabel;
};

#endif // RESTAURANTITEMWIDGET_H
