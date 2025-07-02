#ifndef RECEIVEDORDERSWINDOW_H
#define RECEIVEDORDERSWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include "orderitemwidget.h"


class ReceivedOrdersWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReceivedOrdersWindow(QWidget* parent = nullptr);

private:
    QVBoxLayout* listLayout;
};

#endif // RECEIVEDORDERSWINDOW_H
