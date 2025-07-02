#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

class Command : public QObject
{
    Q_OBJECT

public:
    explicit Command(QObject *parent = nullptr);
    virtual ~Command();

    virtual void execute() = 0;
    virtual int getPriority() const = 0;
    virtual void setPriority(int priority) = 0;

protected:
    int priority;
};

#endif // COMMAND_H 