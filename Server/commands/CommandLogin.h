#ifndef SERVER_COMMANDLOGIN_H
#define SERVER_COMMANDLOGIN_H

#include "Command.h"

class CommandLogin : public Command {
public:
    CommandLogin(QSharedPointer<User> &user, QString login, QString password);

    QJsonDocument exec() override;

private:
    QSharedPointer<User> user_;
    QString login_;
    QString password_;
    QJsonDocument res_;

    bool login();

    bool updateOnline();

    bool unreadMessages();

    bool channels();

    bool addInTempTable();
};


#endif //SERVER_COMMANDLOGIN_H
