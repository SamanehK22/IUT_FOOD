#include "orderhistoryitem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTextEdit>
#include <QIcon>

OrderHistoryItem::OrderHistoryItem(const QString &restaurantName, const QStringList &items, double totalPrice, QWidget *parent)
    : QWidget(parent), currentRating(0)
{
    this->setStyleSheet(
        "QLabel { color: #3e2723; font-family: 'Segoe UI'; }"
        "QFrame { background-color: #FFF6E9; border-radius: 15px; border: 1px solid #D7CCC8; }"
        "QTextEdit {"
        "  background-color: #fefaf4;"
        "  border-radius: 10px;"
        "  padding: 6px;"
        "  font-size: 14px;"
        "  font-family: 'Segoe UI';"
        "  color: #3e2723;"
        "}"
        "QPushButton {"
        "  background: transparent;"
        "  border: none;"
        "  font-size: 18px;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFrame *frame = new QFrame();
    QVBoxLayout *frameLayout = new QVBoxLayout(frame);

    restaurantLabel = new QLabel("Restaurant: " + restaurantName);
    restaurantLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    QLabel *itemsLabel = new QLabel("Items: " + items.join(", "));
    itemsLabel->setStyleSheet("font-size: 14px;");

    QLabel *priceLabel = new QLabel(QString("Total: $%1").arg(totalPrice, 0, 'f', 2));
    priceLabel->setStyleSheet("font-size: 15px; font-weight: bold;");

    commentBox = new QTextEdit();
    commentBox->setPlaceholderText("Write your review...");

    // --- ستاره‌ها برای امتیازدهی ---
    QHBoxLayout *starLayout = new QHBoxLayout();
    for (int i = 1; i <= 5; ++i) {
        QPushButton *starBtn = new QPushButton("☆");
        starBtn->setFixedSize(24, 24);
        starBtn->setStyleSheet("color: gray; font-size: 20px;");
        connect(starBtn, &QPushButton::clicked, [this, i]() {
            handleStarClicked(i);
        });
        starButtons.append(starBtn);
        starLayout->addWidget(starBtn);
    }

    // اضافه کردن ویجت‌ها به لایه
    frameLayout->addWidget(restaurantLabel);
    frameLayout->addWidget(itemsLabel);
    frameLayout->addWidget(priceLabel);
    frameLayout->addWidget(commentBox);
    frameLayout->addLayout(starLayout);

    frame->setLayout(frameLayout);
    mainLayout->addWidget(frame);
}

// تابع برای انتخاب ستاره‌ها
void OrderHistoryItem::handleStarClicked(int star)
{
    currentRating = star;
    for (int i = 0; i < 5; ++i) {
        if (i < star)
            starButtons[i]->setText("★"); // پر
        else
            starButtons[i]->setText("☆"); // خالی
    }
}

QString OrderHistoryItem::getRestaurantName() const
{
    return restaurantLabel->text();
}
