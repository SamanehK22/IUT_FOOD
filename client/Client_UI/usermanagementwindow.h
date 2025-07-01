#ifndef USERMANAGEMENTWINDOW_H
#define USERMANAGEMENTWINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

class UserManagementWindow : public QWidget
{
    Q_OBJECT
public:
    explicit UserManagementWindow(QWidget *parent = nullptr);

private:
    QVBoxLayout* usersLayout;
    void addUserUI(const QString& firstName,
                   const QString& lastName,
                   const QString& email,
                   const QString& phone,
                   const QString& userType);
};

#endif // USERMANAGEMENTWINDOW_H
