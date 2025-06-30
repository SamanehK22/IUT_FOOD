#ifndef RESTAURANTAPPROVALWINDOW_H
#define RESTAURANTAPPROVALWINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include "restaurantrequestitem.h"

class RestaurantApprovalWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantApprovalWindow(QWidget *parent = nullptr);

private:
    QVBoxLayout *requestsLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    void setupUI();
    void loadRequests(); // simulate loading
};

#endif // RESTAURANTAPPROVALWINDOW_H
