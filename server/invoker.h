#ifndef INVOKER_H
#define INVOKER_H

#include <QObject>
#include <QList>
#include <QMap>
#include "command.h"

class Invoker : public QObject
{
    Q_OBJECT

public:
    explicit Invoker(QObject *parent = nullptr);
    ~Invoker();

    void addCommand(Command* command);
    void executeCommands();
    void clearCommands();
    void setCommandPriority(Command* command, int priority);
    int getCommandPriority(Command* command) const;

private:
    QList<Command*> commands;
    QMap<Command*, int> commandPriorities;

    void sortCommandsByPriority();
};

#endif // INVOKER_H 