#ifndef SERVER_COMMAND_SEARCH_H
#define SERVER_COMMAND_SEARCH_H

#include "Command.h"

class CommandSearch : public Command {
public:
    CommandSearch(QSharedPointer<User> &user, QString name);

    QJsonDocument exec() override;

private:
    QSharedPointer<User> user_;
    QString nickname_;
};


#endif //SERVER_COMMAND_SEARCH_H
