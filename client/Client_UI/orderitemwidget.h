#ifndef ORDERITEMWIDGET_H
#define ORDERITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

class OrderItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderItemWidget(const QString& customerName,
                              const QStringList& items,
                              const QString& totalPrice,
                              QWidget *parent = nullptr);

signals:
    void rejected();
    void accepted(const QString& status);

private slots:
    void onExpandClicked();
    void onAcceptClicked();

private:
    QWidget* detailWidget;
    QPushButton* expandButton;
    QPushButton* acceptButton;
    QPushButton* rejectButton;
    QWidget* statusWidget;
    QComboBox* statusCombo;

    bool isExpanded;
};

#endif // ORDERITEMWIDGET_H
