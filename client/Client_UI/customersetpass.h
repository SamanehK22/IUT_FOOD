#ifndef CUSTOMERSETPASS_H
#define CUSTOMERSETPASS_H

#include <QWidget>

namespace Ui {
class customersetpass;
}

class customersetpass : public QWidget
{
    Q_OBJECT

public:
    explicit customersetpass(QWidget *parent = nullptr);
    ~customersetpass();

private slots:
    void on_back_pushButton_clicked();

private:
    Ui::customersetpass *ui;
};

#endif // CUSTOMERSETPASS_H
