#include "useritemwidget.h"

UserItemWidget::UserItemWidget(const QString& firstName,
                               const QString& lastName,
                               const QString& email,
                               const QString& phone,
                               const QString& userType,
                               QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet(R"(
        QFrame {
            background-color: #EFE1C6;
            border: 2px solid #D9BFAF;
            border-radius: 15px;
            padding: 10px;
        }
        QLabel {
            font-family: 'Segoe UI';
            font-size: 14px;
        }
    )");
    setMinimumHeight(100);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);

    QLabel* nameLabel = new QLabel("Name: " + firstName + " " + lastName);
    QLabel* emailLabel = new QLabel("Email: " + email);
    QLabel* phoneLabel = new QLabel("Phone: " + phone);
    QLabel* typeLabel = new QLabel("Type: " + userType);

    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(emailLabel);
    mainLayout->addWidget(phoneLabel);
    mainLayout->addWidget(typeLabel);
}
