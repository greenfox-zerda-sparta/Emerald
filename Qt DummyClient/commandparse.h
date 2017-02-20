#ifndef COMMANDPARSE_H
#define COMMANDPARSE_H
#include <QtCore>
#include "utils.h"


class CommandParse
{
public:
    CommandParse();
    QString getNextMessageCommandId(QString message, Dev device, QString retMessageCommandId);
};

#endif // COMMANDPARSE_H
