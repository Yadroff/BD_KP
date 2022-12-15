#ifndef SERVER_COMMANDGETUSERSINCHANNEL_H
#define SERVER_COMMANDGETUSERSINCHANNEL_H

#include "Command.h"

class CommandGetUsersInChannel : public Command {
public:
    CommandGetUsersInChannel(QString channelName);

    QJsonDocument exec() override;

private:
    QString channelName_;
};


#endif //SERVER_COMMANDGETUSERSINCHANNEL_H
