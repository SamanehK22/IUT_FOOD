#include "menu.h"
#include "fooditemwidget.h"
#include "restaurantlistwindow.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>

Menu::Menu(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(650, 730);
    this->setStyleSheet(
        "QWidget { background-color: #EFE1C6; }"
        "QLabel { color: #4e342e; font-size: 16px; font-family: 'Segoe UI'; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // --- Top bar layout ---
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    topBarLayout->setAlignment(Qt::AlignVCenter);

    // دکمه Back
    QPushButton *backButton = new QPushButton("◀️");
    backButton->setFixedSize(40, 32);
    backButton->setStyleSheet(R"(
        QPushButton {
            background-color: #FFF3E0;
            color: #FF5722;
            font: bold 10pt "Segoe UI";
            border: 2px solid #FFB74D;
            border-radius: 12px;
            padding: 6px 12px;
        }
        QPushButton:hover {
            background-color: #FFE0B2;
            color: #E64A19;
            border: 2px solid #FFA726;
        }
    )");
    topBarLayout->addWidget(backButton);
    topBarLayout->addStretch();

    // --- Price Filter section ---
    QWidget *priceWidget = new QWidget();
    QVBoxLayout *priceLayout = new QVBoxLayout(priceWidget);
    priceLayout->setSpacing(2);
    priceLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *priceLabel = new QLabel("Price Filter");
    priceSpinBox = new QDoubleSpinBox(); // تبدیل به متغیر عضو کلاس
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
    priceSpinBox->setValue(0); // مقدار پیش‌فرض
    priceLayout->addWidget(priceLabel, 0, Qt::AlignLeft);
    priceLayout->addWidget(priceSpinBox, 0, Qt::AlignLeft);
    topBarLayout->addWidget(priceWidget, 0, Qt::AlignVCenter);

    // --- Title in center ---
    titleLabel = new QLabel("Restaurant Menu"); // تبدیل به متغیر عضو کلاس
    titleLabel->setStyleSheet("font-size: 35px; font-weight: bold; color: #3e2723;");
    titleLabel->setAlignment(Qt::AlignCenter);
    topBarLayout->addStretch();
    topBarLayout->addWidget(titleLabel, 0, Qt::AlignVCenter);
    topBarLayout->addStretch();

    // --- Search box on right ---
    searchBox = new QLineEdit(); // تبدیل به متغیر عضو کلاس
    searchBox->setPlaceholderText("Search");
    searchBox->setFixedWidth(150);
    searchBox->setStyleSheet(
        "QLineEdit {"
        "  background-color: #fefaf4;"
        "  border: 2px solid #a1887f;"
        "  border-radius: 12px;"
        "  padding-left: 10px;"
        "  height: 28px;"
        "  font-size: 14px;"
        "  font-family: 'Segoe UI';"
        "  color: #3e2723;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid #6d4c41;"
        "  background-color: #fff6e5;"
        "}"
    );
    topBarLayout->addWidget(searchBox, 0, Qt::AlignVCenter);

    mainLayout->addLayout(topBarLayout);

    // --- Scroll Area for food items ---
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    scrollContent = new QWidget();
    contentLayout = new QVBoxLayout(scrollContent);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(20, 20, 20, 20);

    // لیست آیتم‌های منو
    menuItems = {
        {"Margherita Pizza", 8.99, "Tomato, Mozzarella, Basil", "Pizza", ":/images/pizza.jpg"},
        {"Caesar Salad", 6.49, "Romaine Lettuce, Croutons, Parmesan", "Salad", ":/images/salad.jpg"},
        {"Grilled Salmon", 12.99, "Salmon, Lemon, Garlic, Asparagus", "Main Course", ":/images/salmon.jpg"},
        {"Chocolate Cake", 4.99, "Cocoa, Sugar, Eggs, Butter", "Dessert", ":/images/cake.jpg"},
        {"Cheeseburger", 7.49, "Beef Patty, Cheese, Lettuce, Tomato, Bun", "Main Course", ":/ImageFolder/Burger.png"}

    };

    updateMenuItems(); // نمایش اولیه آیتم‌ها

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // اتصال سیگنال‌ها
    connect(backButton, &QPushButton::clicked, this, [this]() {
        RestaurantListWindow *restaurantWindow = new RestaurantListWindow();
        restaurantWindow->show();
        this->close();
        this->deleteLater();
    });
    connect(searchBox, &QLineEdit::textChanged, this, &Menu::filterMenuItems);
    connect(priceSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &Menu::filterMenuItems);
}

void Menu::updateMenuItems()
{
    // پاک کردن آیتم‌های قبلی
    QLayoutItem* item;
    while ((item = contentLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    // اضافه کردن آیتم‌های جدید
    for (const auto &item : menuItems) {
        FoodItemWidget *widget = new FoodItemWidget(
            item.name,
            QString("$%1").arg(item.price, 0, 'f', 2),
            item.ingredients,
            item.category,
            item.imagePath,
            this
        );
        contentLayout->addWidget(widget);
        foodWidgets.append(widget); // ذخیره ویجت‌ها برای فیلتر کردن
    }

    contentLayout->addStretch();
}

void Menu::filterMenuItems()
{
    QString searchText = searchBox->text().trimmed().toLower();
    double maxPrice = priceSpinBox->value();

    for (int i = 0; i < menuItems.size(); ++i) {
        bool nameMatch = menuItems[i].name.toLower().contains(searchText);
        bool priceMatch = menuItems[i].price <= maxPrice || maxPrice == 0;

        foodWidgets[i]->setVisible(nameMatch && priceMatch);
    }
}
