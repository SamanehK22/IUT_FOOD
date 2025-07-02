#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>


namespace Ui {
class Profile;
}

class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr);
    ~Profile();

private slots:
    void on_back_pushButton_clicked();

    void on_changepassword_pushButton_2_clicked();

    void on_changepassword_pushButton_3_clicked();

    void on_backCoustomer_pushButton_clicked();

private:
    Ui::Profile *ui;
};

#endif // PROFILE_H
