#ifndef CARTWINDOW_H
#define CARTWINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include "cartitemwidget.h"

class CartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CartWindow(QWidget *parent = nullptr);

private:
    QVBoxLayout* itemsLayout;
};

#endif // CARTWINDOW_H
