#ifndef ORDERHISTORYITEM_H
#define ORDERHISTORYITEM_H

#include <QWidget>
#include <QLabel>
#include <QStringList>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSignalMapper>

class OrderHistoryItem : public QWidget
{
    Q_OBJECT

public:
    explicit OrderHistoryItem(const QString &restaurantName, const QStringList &items, double totalPrice, QWidget *parent = nullptr);
    QString getRestaurantName() const;

private slots:
    void handleStarClicked(int star);

private:
    QLabel *restaurantLabel;
    QLabel *itemsLabel;
    QLabel *totalPriceLabel;
    QTextEdit *commentBox;
    QVector<QPushButton*> starButtons;
    int currentRating;
};

#endif // ORDERHISTORYITEM_H
