#include "restaurantlistwindow.h"
#include "cartwindow.h"
#include "history.h"
#include "menu.h"
#include "orderstatuswidget.h" // اضافه کردن این خط
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QFont>
#include <QLineEdit>
#include <QComboBox>
#include "admin_signup.h"
#include "profile.h"

RestaurantListWindow::RestaurantListWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(650, 730);
    this->setStyleSheet("background-color: #EFE1C6;");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // دکمه‌های بالایی
    QHBoxLayout *topButtonLayout = new QHBoxLayout();
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
    topButtonLayout->addWidget(backButton);
    topButtonLayout->addStretch();

    QStringList buttonNames = {"Shopping Cart", "Order History", "Order Status", "Support", "Profile"};
    for (const QString &name : buttonNames) {
        QPushButton *btn = new QPushButton(name);
        btn->setFixedHeight(32);
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #CBB89D;
                color: #4B3621;
                border: 2px solid #A1887F;
                border-radius: 10px;
                padding: 6px 12px;
                font: bold 14px "Segoe UI";
            }
            QPushButton:hover {
                background-color: #EAD8BD;
            }
        )");
        topButtonLayout->addWidget(btn);
        topButtonLayout->addStretch();
    }

    mainLayout->addLayout(topButtonLayout);

    // اتصال دکمه Shopping Cart
    for(int i = 0; i < topButtonLayout->count(); i++) {
        if(QPushButton *btn = qobject_cast<QPushButton*>(topButtonLayout->itemAt(i)->widget())) {
            if(btn->text() == "Shopping Cart") {
                connect(btn, &QPushButton::clicked, this, [this](){
                    this->close();
                    CartWindow *cart = new CartWindow();
                    cart->show();
                });
                break;
            }
        }
    }

    // اتصال دکمه Order History
    for(int i = 0; i < topButtonLayout->count(); i++) {
        if(QPushButton *btn = qobject_cast<QPushButton*>(topButtonLayout->itemAt(i)->widget())) {
            if(btn->text() == "Order History") {
                connect(btn, &QPushButton::clicked, this, [this](){
                    this->close();
                    History *historyItem = new History();
                    historyItem->addSampleOrders();
                    historyItem->show();
                });
                break;
            }
        }
    }

    // اتصال دکمه Order Status - تغییرات اصلی اینجا اعمال شده
    for(int i = 0; i < topButtonLayout->count(); i++) {
        if(QPushButton *btn = qobject_cast<QPushButton*>(topButtonLayout->itemAt(i)->widget())) {
            if(btn->text() == "Order Status") {
                connect(btn, &QPushButton::clicked, this, [this](){
                    this->close();
                    OrderStatusWidget *statusWindow = new OrderStatusWidget();
                    statusWindow->show();
                });
                break;
            }
        }
    }

    // اتصال دکمه Support
    for(int i = 0; i < topButtonLayout->count(); i++) {
        if(QPushButton *btn = qobject_cast<QPushButton*>(topButtonLayout->itemAt(i)->widget())) {
            if(btn->text() == "Support") {
                connect(btn, &QPushButton::clicked, this, [this](){
                    this->close();
                    QMessageBox::information(this, "Support", "Support feature will be implemented here");
                });
                break;
            }
        }
    }

    // اتصال دکمه Profile
    for(int i = 0; i < topButtonLayout->count(); i++) {
        if(QPushButton *btn = qobject_cast<QPushButton*>(topButtonLayout->itemAt(i)->widget())) {
            if(btn->text() == "Profile") {
                connect(btn, &QPushButton::clicked, this, [this](){
                    this->close();
                    Profile *profileWindow = new Profile();
                    profileWindow->show();
                });
                break;
            }
        }
    }

    // بقیه کدهای مربوط به لیست رستوران‌ها
    QLabel *title = new QLabel("Restaurant List");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 30px; font-weight: bold; color: #4B3621; font-family: 'Segoe UI';");
    mainLayout->addWidget(title);

    // فیلترها
    QHBoxLayout *filterLayout = new QHBoxLayout();

    QVBoxLayout *leftFilterLayout = new QVBoxLayout();
    QLabel *cityLabel = new QLabel("Filter by City:");
    cityLabel->setStyleSheet("font: bold 15px 'Segoe UI'; color: #4B3621;");
    cityFilterCombo = new QComboBox();
    cityFilterCombo->addItems({"All", "Tehran", "Esfahan", "Ardebil", "Alborz",
                               "Khorasan Razavi", "Semnan", "Fars", "Qazvin",
                               "Qom", "Kordestan", "Kermanshah", "Gilan",
                               "Mazandaran", "Hormozgan", "Hamedan", "Yazd"});
    cityFilterCombo->setStyleSheet(R"(
        QComboBox {
            background-color: #F1E3C4;
            border: 1px solid #D9CBB6;
            border-radius: 10px;
            padding: 6px 10px;
            font-family: 'Segoe UI';
            min-width: 150px;
        }
    )");
    leftFilterLayout->addWidget(cityLabel);
    leftFilterLayout->addWidget(cityFilterCombo);
    filterLayout->addLayout(leftFilterLayout);
    filterLayout->addStretch();

    QVBoxLayout *rightFilterLayout = new QVBoxLayout();
    QLabel *typeLabel = new QLabel("Filter by Type:");
    typeLabel->setStyleSheet("font: bold 15px 'Segoe UI'; color: #4B3621;");
    typeFilterCombo = new QComboBox();
    typeFilterCombo->addItems({"All", "Fast Food", "Vegeterian", "Iranian", "Sea Food", "Italian"});
    typeFilterCombo->setStyleSheet(R"(
        QComboBox {
            background-color: #F1E3C4;
            border: 1px solid #D9CBB6;
            border-radius: 10px;
            padding: 6px 10px;
            font-family: 'Segoe UI';
            min-width: 120px;
        }
    )");
    rightFilterLayout->addWidget(typeLabel);
    rightFilterLayout->addWidget(typeFilterCombo);
    filterLayout->addLayout(rightFilterLayout);
    mainLayout->addLayout(filterLayout);

    // جستجو
    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Search restaurants...");
    searchBox->setStyleSheet(R"(
        QLineEdit {
            background-color: #F1E3C4;
            border: 1px solid #D9CBB6;
            border-radius: 10px;
            padding: 6px 10px;
            font-family: 'Segoe UI';
            margin: 10px;
        }
    )");
    mainLayout->addWidget(searchBox);

    // Scroll Area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: transparent; border: none; margin: 0 10px;");
    QWidget *container = new QWidget();
    listLayout = new QVBoxLayout(container);
    listLayout->setSpacing(12);
    scrollArea->setWidget(container);
    mainLayout->addWidget(scrollArea);

    // داده‌های اولیه
    addRestaurant("Italiano Restaurant", "123 Roma St, Tehran", 4.5, "Italian", "Tehran");
    addRestaurant("Burger House", "456 Burger Ave, Esfahan", 4.0, "Fast Food", "Esfahan");
    addRestaurant("Sushi World", "789 Tokyo St, Mazandaran", 4.7, "Sea Food", "Mazandaran");
    addRestaurant("Traditional Food", "321 Persian St, Yazd", 4.8, "Iranian", "Yazd");
    addRestaurant("Vegeterian Cafe", "654 Green St, Gilan", 4.3, "Vegeterian", "Gilan");
    addRestaurant("Sea Food House", "987 Ocean St, Hormozgan", 4.6, "Sea Food", "Hormozgan");

    // اتصال فیلترها و جستجو
    connect(cityFilterCombo, &QComboBox::currentTextChanged, this, &RestaurantListWindow::updateRestaurantList);
    connect(typeFilterCombo, &QComboBox::currentTextChanged, this, &RestaurantListWindow::updateRestaurantList);
    connect(searchBox, &QLineEdit::textChanged, this, &RestaurantListWindow::updateRestaurantList);
    connect(backButton, &QPushButton::clicked, this, &RestaurantListWindow::onBackButtonClicked);
}

// متدهای دیگر کلاس RestaurantListWindow بدون تغییر باقی می‌مانند
// ...
void RestaurantListWindow::addRestaurant(const QString &name, const QString &address, double rating, const QString &type, const QString &city)
{
    allRestaurants.append({name, address, rating, type, city});
    updateRestaurantList();
}

void RestaurantListWindow::updateRestaurantList()
{
    QLayoutItem *item;
    while ((item = listLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    QString searchText = searchBox->text().trimmed().toLower();
    QString selectedCity = cityFilterCombo->currentText();
    QString selectedType = typeFilterCombo->currentText();

    for (const Restaurant &r : allRestaurants) {
        if ((selectedCity == "All" || r.city == selectedCity) &&
            (selectedType == "All" || r.type == selectedType) &&
            (searchText.isEmpty() || r.name.toLower().contains(searchText)))
        {
            QWidget *card = new QWidget();
            card->setStyleSheet(R"(
                background-color: #F7E9D3;
                border-radius: 16px;
                padding: 12px;
            )");

            QHBoxLayout *mainCardLayout = new QHBoxLayout(card);
            mainCardLayout->setSpacing(15);

            QLabel *imageLabel = new QLabel("Image");
            imageLabel->setFixedSize(100, 100);
            imageLabel->setStyleSheet(R"(
                background-color: #EFE1C6;
                border-radius: 12px;
                border: 1px solid #D9CBB6;
                color: #8D775E;
                font: 12px 'Segoe UI';
            )");
            imageLabel->setAlignment(Qt::AlignCenter);

            QVBoxLayout *infoLayout = new QVBoxLayout();
            QLabel *nameLabel = new QLabel(r.name);
            nameLabel->setStyleSheet("font-weight: bold; font-size: 15px; color: #4B3621; font-family: 'Segoe UI';");
            QLabel *addressLabel = new QLabel("Address: " + r.address);
            addressLabel->setStyleSheet("font-size: 13px; color: #5C4B3B;");
            QLabel *ratingLabel = new QLabel(QString("Rating: %1 / 5").arg(r.rating));
            ratingLabel->setStyleSheet("font-size: 13px; color: #8B5E3C;");
            QLabel *typeLabel = new QLabel("Type: " + r.type);
            typeLabel->setStyleSheet("font-size: 13px; color: #6B4F32;");
            infoLayout->addWidget(nameLabel);
            infoLayout->addWidget(addressLabel);
            infoLayout->addWidget(ratingLabel);
            infoLayout->addWidget(typeLabel);

            QPushButton *menuButton = new QPushButton("Menu");
            menuButton->setFixedSize(80, 32);
            menuButton->setStyleSheet(R"(
                QPushButton {
                    background-color: #D7B899;
                    color: #4B3621;
                    border-radius: 10px;
                    border: 2px solid #BCA38E;
                    font: bold 13px "Segoe UI";
                    padding: 4px 10px;
                }
                QPushButton:hover {
                    background-color: #E8D3B5;
                }
            )");

            connect(menuButton, &QPushButton::clicked, this, [this, r]() {
                this->close();
                Menu *menuWindow = new Menu();
                menuWindow->setWindowTitle(r.name + " - Menu");
                menuWindow->show();
            });

            mainCardLayout->addWidget(imageLabel);
            mainCardLayout->addLayout(infoLayout);
            mainCardLayout->addStretch();
            mainCardLayout->addWidget(menuButton);

            listLayout->addWidget(card);
        }
    }
}

void RestaurantListWindow::onBackButtonClicked()
{
    this->close();
    Admin_Signup *loginWindow = new Admin_Signup();
    loginWindow->show();
}
