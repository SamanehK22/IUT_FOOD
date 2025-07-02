#include "restaurantapprovalwindow.h"
#include "restaurantrequestitem.h"
#include "homeadmin.h"

RestaurantApprovalWindow::RestaurantApprovalWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadRequests();

    connect(backButton, &QPushButton::clicked, this, &RestaurantApprovalWindow::onBackClicked);
}

void RestaurantApprovalWindow::onBackClicked()
{
    HomeAdmin *homeAdmin = new HomeAdmin();
    homeAdmin->show();
    this->close();
}

void RestaurantApprovalWindow::setupUI()
{
    this->setFixedSize(650, 730);
    backButton = new QPushButton("◀️", this);
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

    QLabel *title = new QLabel("Pending Restaurant Approvals");
    title->setStyleSheet("font-family: 'Segoe UI'; font-size: 20px; font-weight: bold; color: #4B3621;");
    title->setAlignment(Qt::AlignCenter);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            background-color: #F5EBDD; /* نود روشن */
            border: none;
        }

        QScrollBar:vertical {
            background: #F5EBDD; /* زمینه کرم روشن */
            width: 10px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: #B97C4C; /* قهوه‌ای گرم */
            min-height: 20px;
            border-radius: 4px;
        }

        QScrollBar::handle:vertical:hover {
            background: #A66A3A; /* قهوه‌ای تیره‌تر در هاور */
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            background: none;
            height: 0px;
        }

        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
        }
    )");


    scrollWidget = new QWidget();
    scrollWidget->setStyleSheet("background-color: #F5F5DC;");
    requestsLayout = new QVBoxLayout(scrollWidget);
    requestsLayout->setSpacing(15);
    scrollWidget->setLayout(requestsLayout);
    scrollArea->setWidget(scrollWidget);

    // Create a horizontal layout for title and button
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(backButton);
    headerLayout->addWidget(title);
    headerLayout->setStretch(1, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(scrollArea);
    this->setLayout(mainLayout);

    this->setStyleSheet(R"(
        QWidget {
            background-color: #F5F5DC;
        }
    )");
    this->setMinimumSize(450, 600);
}

void RestaurantApprovalWindow::loadRequests()
{
    QList<QPair<QString, QPair<QString, QString>>> requests = {
        {"Burger Boss", {"Ali", "Fast Food"}},
        {"Pizza Palace", {"Sara", "Italian"}},
        {"Grill Master", {"Omid", "Barbecue"}},
        {"Healthy House", {"Leila", "Vegan"}}
    };

    for (const auto &request : requests) {
        RestaurantRequestItem *item = new RestaurantRequestItem(
            request.first,
            request.second.first,
            request.second.second
        );
        requestsLayout->addWidget(item);
    }

    requestsLayout->addStretch();
}
