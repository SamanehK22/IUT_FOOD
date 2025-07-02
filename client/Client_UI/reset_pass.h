#ifndef RESET_PASS_H
#define RESET_PASS_H

#include <QWidget>

namespace Ui {
class Reset_Pass;
}

class Reset_Pass : public QWidget
{
    Q_OBJECT

public:
    explicit Reset_Pass(QWidget *parent = nullptr);
    ~Reset_Pass();

private slots:
    void on_pushButton_clicked();

    void on_back_pushButton_clicked();

private:
    Ui::Reset_Pass *ui;
};

#endif // RESET_PASS_H
