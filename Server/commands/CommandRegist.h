#ifndef SERVER_COMMANDREGIST_H
#define SERVER_COMMANDREGIST_H

#include "Command.h"

class CommandRegist : public Command {
public:
    CommandRegist(QSharedPointer<User> &user, QString nick, QString name, QString surname,
                  QString password);

    QJsonDocument exec() override;

private:
    QSharedPointer<User> user_;
    QString nickname_;
    QString name_;
    QString surname_;
    QString password_;
    QString query_;
};


#endif //SERVER_COMMANDREGIST_H
