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
            background-color: #FAF3E0;
            color: #5E412F;
            font: bold 10pt "Segoe UI";
            border: 2px solid #D2B48C;
            border-radius: 12px;
            padding: 6px 12px;
        }
        QPushButton:hover {
            background-color: #EFE1C6;
            color: #4B3621;
            border: 2px solid #C49E6C;
        }
    )");

    QLabel *title = new QLabel("Pending Restaurant Approvals");
    title->setStyleSheet("font-family: 'Segoe UI'; font-size: 20px; font-weight: bold; color: #4B3621;");
    title->setAlignment(Qt::AlignCenter);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            background-color: #EFE1C6;
            border: none;
        }

        QScrollBar:vertical {
            background: #EFE1C6;
            width: 10px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: #5E412F;
            min-height: 20px;
            border-radius: 4px;
        }

        QScrollBar::handle:vertical:hover {
            background: #4B3621;
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
    scrollWidget->setStyleSheet("background-color: #FAF3E0;");
    requestsLayout = new QVBoxLayout(scrollWidget);
    requestsLayout->setSpacing(15);
    scrollWidget->setLayout(requestsLayout);
    scrollArea->setWidget(scrollWidget);

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
            background-color: #EFE1C6;
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
