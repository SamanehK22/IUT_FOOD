#ifndef OBSERVER_H
#define OBSERVER_H

#include <QString>
#include <QObject>

class Observer : public QObject
{
    Q_OBJECT

public:
    explicit Observer(QObject *parent = nullptr);
    virtual ~Observer();

    virtual void update(const QString& orderId, const QString& status) = 0;

signals:
    void orderStatusChanged(const QString& orderId, const QString& status);
};

#endif // OBSERVER_H 