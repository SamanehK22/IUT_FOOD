#include "command.h"

Command::Command(QObject *parent)
    : QObject(parent)
    , priority(0)
{
}

Command::~Command()
{
} 