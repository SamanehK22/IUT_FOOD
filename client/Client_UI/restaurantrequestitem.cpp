#include "restaurantrequestitem.h"

RestaurantRequestItem::RestaurantRequestItem(QString name, QString owner, QString type, QWidget *parent)
    : QWidget(parent)
{
    nameLabel = new QLabel("<b>" + name + "</b>");
    ownerLabel = new QLabel("<b>Owner:</b> " + owner);
    typeLabel = new QLabel("<b>Type:</b> <i>" + type + "</i>");

    approveButton = new QPushButton("Approve");
    rejectButton = new QPushButton("Reject");
    deleteButton = new QPushButton("🗑Delete");
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
            background-color: #FAEEDB;
            border: 2px solid #d6c4aa;
            border-radius: 20px;
            padding: 20px;
        }
        QLabel {
            font-family: Segoe UI;
            font-size: 16px;
            color: #4E342E;
        }
        QPushButton {
            font-family: Segoe UI;
            font-weight: bold;
            padding: 8px 20px;
            border-radius: 15px;
            color: white;
        }
        QPushButton#approve {
            background-color: #4CAF50;
        }
        QPushButton#reject {
            background-color: #F44336;
        }
        QPushButton#delete {
            background-color: #e53935;
        }
        QPushButton#block {
            background-color: #ff9800;
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
        blockButton->setStyleSheet("background-color: #9e9e9e; color: white; border-radius: 15px; padding: 8px 20px;");
    } else {
        blockButton->setText("Block");
        blockButton->setStyleSheet("background-color: #ff9800; color: white; border-radius: 15px; padding: 8px 20px;");
    }
}

void RestaurantRequestItem::switchToApprovedMode()
{
    approveButton->hide();
    rejectButton->hide();
    actionFrame->show();
}
