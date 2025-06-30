#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include "orderhistoryitem.h"

class History : public QWidget
{
    Q_OBJECT

public:
    explicit History(QWidget *parent = nullptr);
    void addOrder(const QString &restaurantName, const QStringList &items, double totalPrice);
    void filterOrders(const QString &text);

private:
    QVBoxLayout *ordersLayout;
    QList<OrderHistoryItem *> orderItems;
    QLineEdit *searchBox;
    QWidget *scrollContent;
    QScrollArea *scrollArea;
};

#endif // HISTORY_H
