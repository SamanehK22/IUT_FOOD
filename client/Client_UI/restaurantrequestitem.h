#ifndef RESTAURANTREQUESTITEM_H
#define RESTAURANTREQUESTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>

class RestaurantRequestItem : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantRequestItem(QString name, QString owner, QString type, QWidget *parent = nullptr);

private slots:
    void handleApproveClicked();
    void handleRejectClicked();
    void handleDeleteClicked();
    void handleBlockClicked();

private:
    void setupStyle();
    void switchToApprovedMode();

    QLabel *nameLabel;
    QLabel *ownerLabel;
    QLabel *typeLabel;

    QPushButton *approveButton;
    QPushButton *rejectButton;
    QPushButton *deleteButton;
    QPushButton *blockButton;

    QFrame *actionFrame;
};

#endif // RESTAURANTREQUESTITEM_H
