#ifndef MANAGEMENUWINDOW_H
#define MANAGEMENUWINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include "menuitemmanagerwidget.h"

class ManageMenuWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ManageMenuWindow(QWidget *parent = nullptr);

private slots:
    void addMenuItem();
    void handleEdit(MenuItemManagerWidget *item);
    void handleDelete(MenuItemManagerWidget *item);

private:
    QVBoxLayout *itemsLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    void addItemToList(const QString &name, const QString &category, double price, const QString &imagePath);
};

#endif // MANAGEMENUWINDOW_H
