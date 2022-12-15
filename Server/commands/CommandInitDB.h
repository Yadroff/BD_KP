#ifndef SERVER_COMMANDINITDB_H
#define SERVER_COMMANDINITDB_H

#include "Command.h"

class CommandInitDB : public Command {
public:
    CommandInitDB();

    QJsonDocument exec() override;

private:

};


#endif //SERVER_COMMANDINITDB_H
