#ifndef SERVER_COMMANDREADMESSAGES_H
#define SERVER_COMMANDREADMESSAGES_H

#include "Command.h"

class CommandReadMessages : public Command {
public:
    CommandReadMessages(QString  channelName, QDateTime dateTime);

    QJsonDocument exec() override;

private:
    QString channelName_;
    QDateTime dateTime_;
};


#endif //SERVER_COMMANDREADMESSAGES_H
