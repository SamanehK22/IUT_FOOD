#ifndef ORDERSTATUSWIDGET_H
#define ORDERSTATUSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMap>

class OrderStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OrderStatusWidget(QWidget *parent = nullptr);

private slots:
    void onBackButtonClicked();

private:
    void setupUI();
};

#endif // ORDERSTATUSWIDGET_H
