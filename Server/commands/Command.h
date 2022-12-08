#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

#include "../User.h"


const QString SEPARATOR = QString(QChar(7));

#define COMMAND_SEND_KEY "SEND KEY"
#define COMMAND_SEND_MESSAGE "SEND MESSAGE"
#define COMMAND_LOGIN "LOGIN"
#define COMMAND_REGIST "REGIST"

#define PATH_TO_LOGIN "./sql/CheckInDB.sql"
#define PATH_TO_REGIST "./sql/AddUser.sql"
#define PATH_TO_UPDATE_ONLINE "./sql/updateOnline.sql"
#define PATH_TO_DATABASE "database.db"

class Command {
public:
    virtual QJsonDocument exec() = 0;

private:
    QTime start_;
};


#endif //SERVER_COMMAND_H
