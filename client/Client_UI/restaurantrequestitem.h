#ifndef RESTAURANTREQUESTITEM_H
#define RESTAURANTREQUESTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

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
    QLabel *nameLabel;
    QLabel *ownerLabel;
    QLabel *typeLabel;

    QPushButton *approveButton;
    QPushButton *rejectButton;
    QPushButton *deleteButton;
    QPushButton *blockButton;

    QFrame *actionFrame;

    void setupStyle();
    void switchToApprovedMode();
};

#endif // RESTAURANTREQUESTITEM_H
