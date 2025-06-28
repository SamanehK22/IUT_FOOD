#include "menu.h"
#include "fooditemwidget.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>

Menu::Menu(QWidget *parent)
    : QWidget(parent)
{
    this->setStyleSheet(
        "QWidget { background-color: #f5f0e1; }"
        "QLabel { color: #4e342e; font-size: 16px; font-family: 'Segoe UI'; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Restaurant Menu");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #3e2723;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Scroll Area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    QWidget *scrollContent = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(scrollContent);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(20, 20, 20, 20);

    struct MenuItem {
        QString name;
        QString price;
        QString ingredients;
        QString imagePath;
    };

    QList<MenuItem> menuList = {
        {"Margherita Pizza", "$8.99", "Tomato, Mozzarella, Basil", ":/images/pizza.jpg"},
        {"Caesar Salad", "$6.49", "Romaine Lettuce, Croutons, Parmesan", ":/images/salad.jpg"},
        {"Grilled Salmon", "$12.99", "Salmon, Lemon, Garlic, Asparagus", ":/images/salmon.jpg"},
        {"Chocolate Cake", "$4.99", "Cocoa, Sugar, Eggs, Butter", ":/images/cake.jpg"}
    };

    for (const MenuItem &item : menuList) {
        FoodItemWidget *widget = new FoodItemWidget(item.name, item.price, item.ingredients, item.imagePath);
        contentLayout->addWidget(widget);
    }

    contentLayout->addStretch();
    scrollContent->setLayout(contentLayout);
    scrollArea->setWidget(scrollContent);

    mainLayout->addWidget(scrollArea);
}
