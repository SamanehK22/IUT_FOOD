#include "invoker.h"
#include <algorithm>

Invoker::Invoker(QObject *parent)
    : QObject(parent)
{
}

Invoker::~Invoker()
{
    clearCommands();
}

void Invoker::addCommand(Command* command)
{
    if (command) {
        commands.append(command);
        commandPriorities[command] = 0; // Default priority
        sortCommandsByPriority();
    }
}

void Invoker::executeCommands()
{
    for (Command* command : commands) {
        command->execute();
    }
    clearCommands();
}

void Invoker::clearCommands()
{
    for (Command* command : commands) {
        delete command;
    }
    commands.clear();
    commandPriorities.clear();
}

void Invoker::setCommandPriority(Command* command, int priority)
{
    if (command && commands.contains(command)) {
        commandPriorities[command] = priority;
        sortCommandsByPriority();
    }
}

int Invoker::getCommandPriority(Command* command) const
{
    return commandPriorities.value(command, 0);
}

void Invoker::sortCommandsByPriority()
{
    std::sort(commands.begin(), commands.end(),
        [this](Command* a, Command* b) {
            return commandPriorities[a] > commandPriorities[b];
        });
} 