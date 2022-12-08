//
// Created by yadroff on 07.12.22.
//

#ifndef SERVER_COMMANDLOGIN_H
#define SERVER_COMMANDLOGIN_H

#include "Command.h"
#include <QFile>

class CommandLogin : public Command {
public:
    CommandLogin(QSharedPointer<User> &user, QString login, const unsigned long long &password);

    QJsonDocument exec() override;

private:
    QSharedPointer<User> user_;
    QString login_;
    unsigned long long password_;
    QString query_;
};


#endif //SERVER_COMMANDLOGIN_H
