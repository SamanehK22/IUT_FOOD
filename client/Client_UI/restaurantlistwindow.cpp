#include "restaurantlistwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QFont>
#include <QLineEdit>
#include <QComboBox>

RestaurantListWindow::RestaurantListWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(650, 730);
    this->setStyleSheet("background-color: #EFE1C6;");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // دکمه‌های بالایی
    QHBoxLayout *topButtonLayout = new QHBoxLayout();
    topButtonLayout->setContentsMargins(10, 0, 10, 0);
    topButtonLayout->setSpacing(0);

    QPushButton *backButton = new QPushButton("◀️");
    backButton->setObjectName("back_pushButton");
    backButton->setFixedSize(40, 32);
    backButton->setStyleSheet(R"(
        QPushButton#back_pushButton {
            background-color: #FFF3E0;
            color: #FF5722;
            font: bold 10pt "Segoe UI";
            border: 2px solid #FFB74D;
            border-radius: 12px;
            padding: 6px 12px;
        }
        QPushButton#back_pushButton:hover {
            background-color: #FFE0B2;
            color: #E64A19;
            border: 2px solid #FFA726;
        }
    )");
    topButtonLayout->addWidget(backButton);
    topButtonLayout->addStretch();

    QStringList buttonNames = {"Shopping Cart", "Order History", "Order Status", "Support", "Profile"};

    for (int i = 0; i < buttonNames.size(); ++i) {
        QPushButton *btn = new QPushButton(buttonNames[i]);
        btn->setFixedHeight(32);
        btn->setMinimumWidth(80);
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
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
        if (i < buttonNames.size() - 1)
            topButtonLayout->addStretch();
    }

    topButtonLayout->addStretch();
    mainLayout->addLayout(topButtonLayout);

    // عنوان
    QLabel *title = new QLabel("Restaurant List");
    title->setStyleSheet("font-size: 30px; font-weight: bold; color: #4B3621; font-family: 'Segoe UI'; margin-bottom: 10px;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // فیلترها
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setContentsMargins(10, 0, 10, 0);
    filterLayout->setSpacing(30);

    QVBoxLayout *leftFilterLayout = new QVBoxLayout();
    leftFilterLayout->setSpacing(4);
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
    rightFilterLayout->setSpacing(4);
    QLabel *typeLabel = new QLabel("Filter by Type:");
    typeLabel->setStyleSheet("font: bold 15px 'Segoe UI'; color: #4B3621;");
    typeFilterCombo = new QComboBox();
    typeFilterCombo->addItems({"All", "Fast Food", "Vegeterian", "Iranian", "Sea Food"});
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

    // ناحیه لیست رستوران‌ها
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: transparent; border: none; margin: 0 10px;");

    QWidget *container = new QWidget();
    listLayout = new QVBoxLayout(container);
    listLayout->setSpacing(12);
    listLayout->setContentsMargins(0, 0, 10, 10);

    scrollArea->setWidget(container);
    mainLayout->addWidget(scrollArea);

    // داده نمونه
    addRestaurant("Italiano Restaurant", "123 Roma St, Tehran", 4.5, "Italian");
    addRestaurant("Burger House", "456 Burger Ave, Esfahan", 4.0, "Fast Food");
    addRestaurant("Sushi World", "789 Tokyo St, Mazandaran", 4.7, "Sea Food");
    addRestaurant("Traditional Food", "321 Persian St, Yazd", 4.8, "Iranian");
    addRestaurant("Vegeterian Cafe", "654 Green St, Gilan", 4.3, "Vegeterian");
    addRestaurant("Sea Food House", "987 Ocean St, Hormozgan", 4.6, "Sea Food");
}

void RestaurantListWindow::addRestaurant(const QString &name, const QString &address, double rating, const QString &type)
{
    QWidget *card = new QWidget();
    card->setStyleSheet(R"(
        background-color: #F7E9D3;
        border-radius: 16px;
        padding: 12px;
        margin: 0 5px;
    )");

    QHBoxLayout *mainCardLayout = new QHBoxLayout(card);
    mainCardLayout->setSpacing(15);

    QLabel *imageLabel = new QLabel("Image");
    imageLabel->setFixedSize(90, 90);
    imageLabel->setStyleSheet(R"(
        background-color: #EFE1C6;
        border-radius: 12px;
        border: 1px solid #D9CBB6;
        color: #8D775E;
        font: 12px 'Segoe UI';
    )");
    imageLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(4);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 15px; font-family: 'Segoe UI'; color: #4B3621;");

    QLabel *addressLabel = new QLabel("Address: " + address);
    addressLabel->setStyleSheet("font-size: 13px; color: #5C4B3B; font-family: 'Segoe UI';");

    QLabel *ratingLabel = new QLabel(QString("Rating: %1 / 5").arg(rating));
    ratingLabel->setStyleSheet("font-size: 13px; color: #8B5E3C; font-family: 'Segoe UI';");

    QLabel *typeLabel = new QLabel("Type: " + type);
    typeLabel->setStyleSheet("font-size: 13px; color: #6B4F32; font-family: 'Segoe UI';");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(addressLabel);
    infoLayout->addWidget(ratingLabel);
    infoLayout->addWidget(typeLabel);

    mainCardLayout->addWidget(imageLabel);
    mainCardLayout->addLayout(infoLayout);

    listLayout->addWidget(card);
}
