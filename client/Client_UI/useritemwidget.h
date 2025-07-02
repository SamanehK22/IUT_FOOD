#ifndef USERITEMWIDGET_H
#define USERITEMWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class UserItemWidget : public QFrame
{
    Q_OBJECT
public:
    explicit UserItemWidget(const QString& firstName,
                            const QString& lastName,
                            const QString& email,
                            const QString& phone,
                            const QString& userType,
                            QWidget *parent = nullptr);
};

#endif // USERITEMWIDGET_H
