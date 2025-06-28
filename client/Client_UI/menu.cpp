#include "menu.h"
#include "fooditemwidget.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QHBoxLayout>

Menu::Menu(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(761, 791);
    this->setStyleSheet(
        "QWidget { background-color: #f5f0e1; }"
        "QLabel { color: #4e342e; font-size: 16px; font-family: 'Segoe UI'; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // === نوار بالایی ===
    QHBoxLayout *topBarLayout = new QHBoxLayout();

    // -- فیلتر قیمت (چپ)
    QVBoxLayout *priceFilterLayout = new QVBoxLayout();
    QLabel *priceLabel = new QLabel("Price Filter");
    QDoubleSpinBox *priceSpinBox = new QDoubleSpinBox();
    priceSpinBox->setStyleSheet(
        "QDoubleSpinBox {"
        "  background-color: #fefaf4;"
        "  border: 2px solid #a1887f;"
        "  border-radius: 12px;"
        "  padding: 4px 8px;"
        "  font-size: 14px;"
        "  font-family: 'Segoe UI';"
        "  color: #3e2723;"
        "  min-width: 90px;"
        "}"
        "QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {"
        "  background-color: transparent;"
        "  border: none;"
        "  width: 16px;"
        "}"
        "QDoubleSpinBox::up-arrow, QDoubleSpinBox::down-arrow {"
        "  width: 8px; height: 8px;"
        "}"
    );
    priceSpinBox->setPrefix("$");
    priceSpinBox->setRange(0, 1000);
    priceSpinBox->setSingleStep(0.5);
    priceFilterLayout->addWidget(priceLabel);
    priceFilterLayout->addWidget(priceSpinBox);
    topBarLayout->addLayout(priceFilterLayout);

    // -- عنوان
    QLabel *title = new QLabel("Restaurant Menu");
    title->setStyleSheet("font-size: 35px; font-weight: bold; color: #3e2723;");
    title->setAlignment(Qt::AlignCenter);
    topBarLayout->addWidget(title, 1, Qt::AlignCenter);

    // -- سرچ‌باکس
    QLineEdit *searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Search for food... 🍽️");
    searchBox->setFixedWidth(150);
    searchBox->setStyleSheet(
        "QLineEdit {"
        "  background-color: #fdf6e3;"
        "  border: 2px solid #a1887f;"
        "  border-radius: 15px;"
        "  padding-left: 35px;"
        "  height: 30px;"
        "  font-size: 14px;"
        "  font-family: 'Segoe UI';"
        "  color: #3e2723;"
        "  background-image: url(:/icons/search.png);"
        "  background-repeat: no-repeat;"
        "  background-position: 8px center;"
        "  background-origin: content;"
        "  qproperty-alignment: AlignCenter;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid #6d4c41;"
        "}"
    );
    topBarLayout->addWidget(searchBox);

    mainLayout->addLayout(topBarLayout);

    // === اسکرول ایریا ===
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
        QString category;
        QString imagePath;
    };

    QList<MenuItem> menuList = {
        {"Margherita Pizza", "$8.99", "Tomato, Mozzarella, Basil", "Pizza", ":/images/pizza.jpg"},
        {"Caesar Salad", "$6.49", "Romaine Lettuce, Croutons, Parmesan", "Salad", ":/images/salad.jpg"},
        {"Grilled Salmon", "$12.99", "Salmon, Lemon, Garlic, Asparagus", "Main Course", ":/images/salmon.jpg"},
        {"Chocolate Cake", "$4.99", "Cocoa, Sugar, Eggs, Butter", "Dessert", ":/images/cake.jpg"}
    };

    for (const MenuItem &item : menuList) {
        FoodItemWidget *widget = new FoodItemWidget(
            item.name,
            item.price,
            item.ingredients,
            item.category,
            item.imagePath,
            this
        );
        contentLayout->addWidget(widget);
    }

    contentLayout->addStretch();
    scrollContent->setLayout(contentLayout);
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}
