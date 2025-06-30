#include "restaurantapprovalwindow.h"

RestaurantApprovalWindow::RestaurantApprovalWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadRequests();
}

void RestaurantApprovalWindow::setupUI()
{
    QLabel *title = new QLabel("Pending Restaurant Approvals");
    title->setStyleSheet("font-family: 'Segoe UI'; font-size: 20px; font-weight: bold; color: #4B3621;");
    title->setAlignment(Qt::AlignCenter);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none;");

    scrollWidget = new QWidget();
    requestsLayout = new QVBoxLayout(scrollWidget);
    requestsLayout->setSpacing(15);
    scrollWidget->setLayout(requestsLayout);
    scrollArea->setWidget(scrollWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
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
    // شبیه‌سازی درخواست‌های ثبت‌نام رستوران
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

    requestsLayout->addStretch(); // فضای خالی آخر لیست
}
