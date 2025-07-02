#include "orderitemwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

OrderItemWidget::OrderItemWidget(const QString& customerName,
                                 const QStringList& items,
                                 const QString& totalPrice,
                                 QWidget *parent)
    : QWidget(parent), isExpanded(false)
{
    // Title label
    QLabel* titleLabel = new QLabel("Order from: " + customerName);

    // Detail widget
    detailWidget = new QWidget;
    detailWidget->setVisible(false);
    QVBoxLayout* detailLayout = new QVBoxLayout(detailWidget);
    for (const QString& item : items) {
        QLabel* itemLabel = new QLabel("• " + item);
        detailLayout->addWidget(itemLabel);
    }

    QLabel* priceLabel = new QLabel("Total: " + totalPrice);
    detailLayout->addWidget(priceLabel);

    // Buttons
    expandButton = new QPushButton("Expand");
    connect(expandButton, &QPushButton::clicked, this, &OrderItemWidget::onExpandClicked);

    acceptButton = new QPushButton("Accept");
    connect(acceptButton, &QPushButton::clicked, this, &OrderItemWidget::onAcceptClicked);

    rejectButton = new QPushButton("Reject");
    connect(rejectButton, &QPushButton::clicked, this, &OrderItemWidget::rejected);

    // ComboBox for status
    statusWidget = new QWidget;
    statusWidget->setVisible(false);
    QHBoxLayout* statusLayout = new QHBoxLayout(statusWidget);
    QLabel* statusLabel = new QLabel("Status:");
    statusCombo = new QComboBox;
    statusCombo->addItems({"Preparing", "Ready", "Out for Delivery", "Delivered"});
    statusLayout->addWidget(statusLabel);
    statusLayout->addWidget(statusCombo);

    // Layouts
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(expandButton);
    topLayout->addWidget(acceptButton);
    topLayout->addWidget(rejectButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(detailWidget);
    mainLayout->addWidget(statusWidget);

    // StyleSheet
    setStyleSheet(R"(
        QWidget {
            background-color: #FFFFFF;
            border: 1px solid #E0E0E0;
            border-radius: 16px;
        }
        QLabel {
            font-family: "Segoe UI";
            font-size: 15px;
            color: #333333;
        }
        QPushButton {
            background-color: #F5F5F5;
            border: 1px solid #CCCCCC;
            border-radius: 10px;
            padding: 6px 12px;
            font-size: 14px;
            font-weight: 500;
            font-family: "Segoe UI";
            color: #333;
        }
        QPushButton:hover {
            background-color: #EAEAEA;
        }
        QComboBox {
            border: 1px solid #CCCCCC;
            border-radius: 10px;
            padding: 6px 10px;
            font-size: 14px;
            font-family: "Segoe UI";
            background-color: #FAFAFA;
        }
    )");

    // Optional: specific styles for Accept/Reject buttons
    acceptButton->setStyleSheet(R"(
        QPushButton {
            background-color: #D8F3DC;
            border: 1px solid #B7E4C7;
            color: #2D6A4F;
        }
        QPushButton:hover {
            background-color: #C2F0D4;
        }
    )");
    rejectButton->setStyleSheet(R"(
        QPushButton {
            background-color: #FADBD8;
            border: 1px solid #F5B7B1;
            color: #A93226;
        }
        QPushButton:hover {
            background-color: #F5C6C5;
        }
    )");
}

void OrderItemWidget::onExpandClicked()
{
    isExpanded = !isExpanded;
    detailWidget->setVisible(isExpanded);
    expandButton->setText(isExpanded ? "Collapse" : "Expand");
}

void OrderItemWidget::onAcceptClicked()
{
    acceptButton->setEnabled(false);
    statusWidget->setVisible(true);
}
