#ifndef RESTAURANTAPPROVALWINDOW_H
#define RESTAURANTAPPROVALWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>

class RestaurantApprovalWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantApprovalWindow(QWidget *parent = nullptr);

private slots:
    void onBackClicked();

private:
    void setupUI();
    void loadRequests();

    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *requestsLayout;
    QPushButton *backButton;
};

#endif // RESTAURANTAPPROVALWINDOW_H
