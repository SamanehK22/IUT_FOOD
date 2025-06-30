#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPixmap>
#include <QLineEdit>
#include <QDoubleSpinBox>

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
};

#endif // MENU_H
