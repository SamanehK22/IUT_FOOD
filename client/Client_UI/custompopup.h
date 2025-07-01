#ifndef CUSTOMPOPUP_H
#define CUSTOMPOPUP_H

#include <QWidget>
#include <QTimer>

class CustomPopup : public QWidget
{
    Q_OBJECT

public:
    explicit CustomPopup(const QString &message, QWidget *parent = nullptr);

private:
    QTimer *timer;
};

#endif // CUSTOMPOPUP_H
