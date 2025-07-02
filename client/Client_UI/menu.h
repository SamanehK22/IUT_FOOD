#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPixmap>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include "fooditemwidget.h"

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    void setRestaurantName(const QString &name);

private slots:
    void filterMenuItems();
    void updateMenuItems();

private:
    struct MenuItem {
        QString name;
        double price;
        QString ingredients;
        QString category;
        QString imagePath;
    };

    QList<MenuItem> menuItems;
    QList<FoodItemWidget*> foodWidgets;

    QLabel *titleLabel;
    QLineEdit *searchBox;
    QDoubleSpinBox *priceSpinBox;
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *contentLayout;
};

#endif // MENU_H
