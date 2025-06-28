#ifndef FOODITEMWIDGET_H
#define FOODITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class FoodItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FoodItemWidget(QString name, QString price, QString ingredients, QString imagePath, QWidget *parent = nullptr);

private:
    QLabel *imgLabel;
    QLabel *nameLabel;
    QLabel *priceLabel;
    QLabel *ingredientsLabel;
    QPushButton *addButton;
};

#endif // FOODITEMWIDGET_H
