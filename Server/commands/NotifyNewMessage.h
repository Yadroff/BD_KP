#ifndef SERVER_NOTIFY_NEW_MESSAGE_H
#define SERVER_NOTIFY_NEW_MESSAGE_H

#include "Command.h"

class NotifyNewMessage : public Command {
public:
    NotifyNewMessage(QHash<QString, QSharedPointer<User>> &users, QJsonDocument doc, QString channel, QString sender);

    QJsonDocument exec() override;

private:
    QString channel_;
    QHash<QString, QSharedPointer<User>> users_;
    QJsonDocument doc_;
    QString sender_;
};


#endif //SERVER_NOTIFY_NEW_MESSAGE_H
