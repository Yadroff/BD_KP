#ifndef SERVER_COMMANDEXIT_H
#define SERVER_COMMANDEXIT_H

#include "Command.h"

class CommandExit : public Command {
public:
    CommandExit(QString nick);

    QJsonDocument exec() override;

    void setNick(const QString &nick);

private:
    QString nick_;
};


#endif //SERVER_COMMANDEXIT_H
