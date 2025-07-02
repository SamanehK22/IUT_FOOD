#include "restaurantrequestitem.h"

RestaurantRequestItem::RestaurantRequestItem(QString name, QString owner, QString type, QWidget *parent)
    : QWidget(parent)
{
    nameLabel = new QLabel("<b>" + name + "</b>");
    ownerLabel = new QLabel("<b>Owner:</b> " + owner);
    typeLabel = new QLabel("<b>Type:</b> <i>" + type + "</i>");

    approveButton = new QPushButton("Approve");
    rejectButton = new QPushButton("Reject");
    deleteButton = new QPushButton("Delete");
    blockButton = new QPushButton("Block");

    actionFrame = new QFrame();
    QHBoxLayout *actionLayout = new QHBoxLayout(actionFrame);
    actionLayout->addWidget(deleteButton);
    actionLayout->addWidget(blockButton);
    actionFrame->setVisible(false);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(ownerLabel);
    mainLayout->addWidget(typeLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(approveButton);
    mainLayout->addWidget(rejectButton);
    mainLayout->addWidget(actionFrame);

    setupStyle();

    connect(approveButton, &QPushButton::clicked, this, &RestaurantRequestItem::handleApproveClicked);
    connect(rejectButton, &QPushButton::clicked, this, &RestaurantRequestItem::handleRejectClicked);
    connect(deleteButton, &QPushButton::clicked, this, &RestaurantRequestItem::handleDeleteClicked);
    connect(blockButton, &QPushButton::clicked, this, &RestaurantRequestItem::handleBlockClicked);
}

void RestaurantRequestItem::setupStyle()
{
    this->setStyleSheet(R"(
        QWidget {
            background-color: #EFE1C6;
            border: 2px solid #D2B48C;
            border-radius: 18px;
            padding: 18px;
        }

        QLabel {
            font-family: 'Segoe UI';
            font-size: 16px;
            color: #3E2723;
        }

        QPushButton {
            font-family: 'Segoe UI';
            font-size: 14px;
            font-weight: bold;
            padding: 8px 20px;
            border-radius: 15px;
            color: white;
            border: none;
        }

        QPushButton#approve {
            background-color: #4CAF50;
        }
        QPushButton#approve:hover {
            background-color: #43A047;
        }

        QPushButton#reject {
            background-color: #F44336;
        }
        QPushButton#reject:hover {
            background-color: #D32F2F;
        }

        QPushButton#delete {
            background-color: #D32F2F;
        }
        QPushButton#delete:hover {
            background-color: #B71C1C;
        }

        QPushButton#block {
            background-color: #FB8C00;
        }
        QPushButton#block:hover {
            background-color: #EF6C00;
        }
    )");

    approveButton->setObjectName("approve");
    rejectButton->setObjectName("reject");
    deleteButton->setObjectName("delete");
    blockButton->setObjectName("block");
}

void RestaurantRequestItem::handleApproveClicked()
{
    switchToApprovedMode();
}

void RestaurantRequestItem::handleRejectClicked()
{
    this->deleteLater();
}

void RestaurantRequestItem::handleDeleteClicked()
{
    this->deleteLater();
}

void RestaurantRequestItem::handleBlockClicked()
{
    if (blockButton->text() == "Block") {
        blockButton->setText("Unblock");
        blockButton->setStyleSheet("background-color: #9E9E9E; color: white; border-radius: 15px; padding: 8px 20px;");
    } else {
        blockButton->setText("Block");
        blockButton->setStyleSheet("background-color: #FB8C00; color: white; border-radius: 15px; padding: 8px 20px;");
    }
}

void RestaurantRequestItem::switchToApprovedMode()
{
    approveButton->hide();
    rejectButton->hide();
    actionFrame->show();
}
