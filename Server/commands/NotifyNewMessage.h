#ifndef SERVER_NOTIFY_NEW_MESSAGE_H
#define SERVER_NOTIFY_NEW_MESSAGE_H

#include "Command.h"

class NotifyNewMessage : public Command {
public:
    NotifyNewMessage(QHash<QString, QSharedPointer<User>> &users, QJsonDocument onlineUsers, QJsonDocument message,
                     QString sender, QString channel, int messageID);

    QJsonDocument exec() override;

private:
    QString channel_;
    QHash<QString, QSharedPointer<User>> users_;
    QJsonDocument onlineUsers_;
    QString sender_;
    QJsonDocument message_;
    int messageID_;
};


#endif //SERVER_NOTIFY_NEW_MESSAGE_H
