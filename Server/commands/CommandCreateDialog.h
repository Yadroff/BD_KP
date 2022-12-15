#ifndef SERVER_COMMANDCREATEDIALOG_H
#define SERVER_COMMANDCREATEDIALOG_H

#include "Command.h"

class CommandCreateDialog : public Command {
public:
    CommandCreateDialog(QSharedPointer<User> &user, QString second);

    QJsonDocument exec() final;

private:
    QSharedPointer<User> user_;
    QString secondUser_;
};


#endif //SERVER_COMMANDCREATEDIALOG_H
