#ifndef SERVER_COMMANDSENDMESSAGE_H
#define SERVER_COMMANDSENDMESSAGE_H

#include "Command.h"

class CommandSendMessage : public Command {
public:
    CommandSendMessage(QSharedPointer<User> &user, QString channelName, QString text,
                       QDateTime time);

    QJsonDocument exec() override;

private:
    QSharedPointer<User> user_;
    QString channel_;
    QString text_;
    QDateTime date_;
};


#endif //SERVER_COMMANDSENDMESSAGE_H
