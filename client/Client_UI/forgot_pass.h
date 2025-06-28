#ifndef FORGOT_PASS_H
#define FORGOT_PASS_H

#include <QWidget>

namespace Ui {
class forgot_pass;
}

class forgot_pass : public QWidget
{
    Q_OBJECT

public:
    explicit forgot_pass(QWidget *parent = nullptr);
    ~forgot_pass();

private slots:
    void on_pushButton_clicked();

private:
    Ui::forgot_pass *ui;
};

#endif // FORGOT_PASS_H
