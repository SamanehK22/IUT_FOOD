#ifndef FOODITEMWIDGET_H
#define FOODITEMWIDGET_H

#include <QWidget>

class QLabel;
class QPushButton;
class QSpinBox;

class FoodItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FoodItemWidget(QString name,
                            QString price,
                            QString ingredients,
                            QString category,
                            QString imagePath,
                            QWidget *parent = nullptr);

private:
    QLabel *imageLabel;
    QLabel *nameLabel;
    QLabel *ingredientsLabel;
    QLabel *categoryLabel;
    QLabel *priceLabel;
    QSpinBox *quantitySpinBox;
    QPushButton *addButton;
};

#endif // FOODITEMWIDGET_H
