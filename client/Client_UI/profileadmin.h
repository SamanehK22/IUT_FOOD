#ifndef PROFILEADMIN_H
#define PROFILEADMIN_H

#include <QWidget>

namespace Ui {
class profileadmin;
}

class profileadmin : public QWidget
{
    Q_OBJECT

public:
    explicit profileadmin(QWidget *parent = nullptr);
    ~profileadmin();

private slots:
    void on_back_pushButton_clicked();

    void on_changepassword_pushButton_8_clicked();

    void on_changepassword_pushButton_9_clicked();

private:
    Ui::profileadmin *ui;
};

#endif // PROFILEADMIN_H
