#ifndef HOMECOUSTOMER_H
#define HOMECOUSTOMER_H

#include <QWidget>

namespace Ui {
class Home;
}

class HomeCoustomer : public QWidget
{
    Q_OBJECT

public:
    explicit HomeCoustomer(QWidget *parent = nullptr);
    ~HomeCoustomer();

private:
    Ui::Home *ui;
};

#endif // HOMECOUSTOMER_H
