#ifndef SERVER_COMMANDREPLYKEY_H
#define SERVER_COMMANDREPLYKEY_H

#include <QSharedPointer>

#include "Command.h"
#include "User.h"


class CommandReplyKey : public Command {
public:
    CommandReplyKey(QSharedPointer<User> &user, const long long &key);

    QJsonDocument exec() final;

    ~CommandReplyKey();

private:
    QSharedPointer<User> user_;
    long long key_;
};


#endif //SERVER_COMMANDREPLYKEY_H
