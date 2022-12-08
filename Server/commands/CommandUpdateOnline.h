//
// Created by yadroff on 08.12.22.
//

#ifndef SERVER_COMMANDUPDATEONLINE_H
#define SERVER_COMMANDUPDATEONLINE_H

#include "Command.h"

class CommandUpdateOnline : public Command {
public:
    CommandUpdateOnline(QSharedPointer<User> &user, QString nickname);

    QJsonDocument exec() override;

private:
    QSharedPointer<User> user_;
    QString nickname_;
    QString query_;
};


#endif //SERVER_COMMANDUPDATEONLINE_H
