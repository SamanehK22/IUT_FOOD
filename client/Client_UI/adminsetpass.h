#ifndef ADMINSETPASS_H
#define ADMINSETPASS_H

#include <QWidget>

namespace Ui {
class adminsetpass;
}

class adminsetpass : public QWidget
{
    Q_OBJECT

public:
    explicit adminsetpass(QWidget *parent = nullptr);
    ~adminsetpass();

private slots:
    void on_back_pushButton_2_clicked();

private:
    Ui::adminsetpass *ui;
};

#endif // ADMINSETPASS_H
