#include "usermanagementwindow.h"
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include "homeadmin.h"

UserManagementWindow::UserManagementWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(650, 730);
    setStyleSheet("background-color: #FDF6E3;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // ایجاد لایه افقی برای عنوان و دکمه بازگشت
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(15);

    // ایجاد دکمه بازگشت
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
    titleLayout->addWidget(backButton, 0, Qt::AlignLeft);

    // عنوان با تراز وسط
    QLabel* title = new QLabel("User Management");
    title->setStyleSheet("font-size: 30px; font-family: 'Segoe UI'; font-weight: bold; color: #7A4E2D;");
    title->setAlignment(Qt::AlignCenter);
    title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleLayout->addWidget(title, 1, Qt::AlignCenter);

    mainLayout->addLayout(titleLayout);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none;");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* scrollContent = new QWidget(this);
    usersLayout = new QVBoxLayout(scrollContent);
    usersLayout->setSpacing(15);
    usersLayout->setContentsMargins(10, 10, 10, 10);
    scrollContent->setLayout(usersLayout);
    scrollContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // تست نمونه کاربران
    addUserUI("Ali", "Ahmadi", "ali@example.com", "09121234567", "Customer");
    addUserUI("Sara", "Rezaei", "sara@example.com", "09351231234", "Restaurant Owner");
    addUserUI("Mohammad", "Karimi", "m.karimi@example.com", "09101112233", "Customer");
    addUserUI("Ali", "Ahmadi", "ali@example.com", "09121234567", "Customer");
    addUserUI("Sara", "Rezaei", "sara@example.com", "09351231234", "Restaurant Owner");
    addUserUI("Mohammad", "Karimi", "m.karimi@example.com", "09101112233", "Customer");
    addUserUI("Ali", "Ahmadi", "ali@example.com", "09121234567", "Customer");
    addUserUI("Sara", "Rezaei", "sara@example.com", "09351231234", "Restaurant Owner");
    addUserUI("Mohammad", "Karimi", "m.karimi@example.com", "09101112233", "Customer");

    connect(backButton, &QPushButton::clicked, this, [=]() {
        HomeAdmin *home = new HomeAdmin();
        home->show();
        this->close();
    });
}

void UserManagementWindow::addUserUI(const QString& firstName,
                                     const QString& lastName,
                                     const QString& email,
                                     const QString& phone,
                                     const QString& userType)
{
    QFrame* frame = new QFrame;
    frame->setMinimumHeight(100);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    frame->setStyleSheet(R"(
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
        QPushButton {
            font-family: 'Segoe UI';
            padding: 8px 12px;
            border-radius: 10px;
            background-color: #DC5F4A;
            color: white;
            min-width: 80px;
            width: 100%;
        }
        QPushButton:hover {
            background-color: #B33F2E;
        }
    )");

    QVBoxLayout* layout = new QVBoxLayout(frame);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    layout->addStretch(1);

    QLabel* nameLabel = new QLabel("Name: " + firstName + " " + lastName);
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    nameLabel->setWordWrap(true);
    layout->addWidget(nameLabel);

    QLabel* emailLabel = new QLabel("Email: " + email);
    emailLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    emailLabel->setWordWrap(true);
    layout->addWidget(emailLabel);

    QLabel* phoneLabel = new QLabel("Phone: " + phone);
    phoneLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    phoneLabel->setWordWrap(true);
    layout->addWidget(phoneLabel);

    QLabel* typeLabel = new QLabel("Type: " + userType);
    typeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    typeLabel->setWordWrap(true);
    layout->addWidget(typeLabel);

    if (userType.toLower() == "customer") {
        QVBoxLayout* btnLayout = new QVBoxLayout;
        btnLayout->setSpacing(8);

        QPushButton* deleteBtn = new QPushButton("Delete");
        deleteBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        QPushButton* blockBtn = new QPushButton("Block");
        blockBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        btnLayout->addWidget(deleteBtn);
        btnLayout->addWidget(blockBtn);

        layout->addLayout(btnLayout);
        layout->addStretch(1);

        connect(deleteBtn, &QPushButton::clicked, this, [=]() {
            frame->deleteLater();
        });

        connect(blockBtn, &QPushButton::clicked, this, [=]() mutable {
            if (blockBtn->text() == "Block") {
                blockBtn->setText("Unblock");
                blockBtn->setStyleSheet(R"(
                    QPushButton {
                        font-family: 'Segoe UI';
                        padding: 8px 12px;
                        border-radius: 10px;
                        background-color: gray;
                        color: white;
                        width: 100%;
                    }
                    QPushButton:hover {
                        background-color: #666666;
                    }
                )");
            } else {
                blockBtn->setText("Block");
                blockBtn->setStyleSheet(R"(
                    QPushButton {
                        font-family: 'Segoe UI';
                        padding: 8px 12px;
                        border-radius: 10px;
                        background-color: #DC5F4A;
                        color: white;
                        width: 100%;
                    }
                    QPushButton:hover {
                        background-color: #B33F2E;
                    }
                )");
            }
        });
    } else {
        layout->addStretch(1);
    }

    usersLayout->addWidget(frame);
}
