#ifndef SERVER_COMMAND_ADD_TO_TEMP_TABLE_H
#define SERVER_COMMAND_ADD_TO_TEMP_TABLE_H

#include "Command.h"

class CommandAddToTempTable : public Command {
public:
    CommandAddToTempTable(QString userName, QString channel);

    QJsonDocument exec() override;

private:
    QString userName_;
    QString channel_;
};


#endif //SERVER_COMMAND_ADD_TO_TEMP_TABLE_H
