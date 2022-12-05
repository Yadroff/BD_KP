#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include <QString>
#include <QJsonDocument>

const QString SEPARATOR = QString(QChar(7));

#define COMMAND_SEND_KEY "SEND KEY"
#define COMMAND_SEND_MESSAGE "SEND MESSAGE"
#define COMMAND_LOGIN "LOGIN"

class Command {
public:
    virtual QJsonDocument exec() = 0;

private:
    QTime start_;
};


#endif //SERVER_COMMAND_H
