#include "receivedorderswindow.h"
#include <QScrollArea>
#include <QFrame>
#include <QStringList>

ReceivedOrdersWindow::ReceivedOrdersWindow(QWidget* parent)
    : QWidget(parent)
{
    this->setWindowTitle("Received Orders");
    this->resize(700, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: #F8F6F0; border: none;");

    QWidget* scrollContent = new QWidget;
    listLayout = new QVBoxLayout(scrollContent);
    listLayout->setSpacing(20);

    // Sample orders
    QStringList names = {"John Smith", "Emily Davis", "Michael Johnson"};
    QList<QStringList> items = {
        {"Burger", "Fries", "Coke"},
        {"Pizza", "Salad"},
        {"Sushi", "Green Tea", "Miso Soup"}
    };
    QStringList prices = {"$14.90", "$10.25", "$19.80"};

    for (int i = 0; i < names.size(); ++i)
    {
        // ویجت سفارش
        OrderItemWidget* widget = new OrderItemWidget(names[i], items[i], prices[i]);

        // فریم اطراف ویجت برای زیبایی
        QFrame* frame = new QFrame;
        frame->setStyleSheet(R"(
            QFrame {
                background-color: white;
                border: 1px solid #CCCCCC;
                border-radius: 20px;
            }
        )");
        QVBoxLayout* frameLayout = new QVBoxLayout(frame);
        frameLayout->setContentsMargins(16, 16, 16, 16);
        frameLayout->addWidget(widget);

        listLayout->addWidget(frame);

        // حذف سفارش پس از کلیک روی دکمه Delete
        connect(widget, &OrderItemWidget::rejected, this, [=]() {
            listLayout->removeWidget(frame);
            frame->deleteLater();
        });
    }

    scrollContent->setLayout(listLayout);
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    this->setLayout(mainLayout);
}
