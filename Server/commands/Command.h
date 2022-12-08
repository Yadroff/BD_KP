#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <iostream>

#include "../User.h"


const QString SEPARATOR = QString(QChar(7));

#define COMMAND_SEND_KEY "SEND KEY"
#define COMMAND_SEND_MESSAGE "SEND MESSAGE"
#define COMMAND_LOGIN "LOGIN"
#define COMMAND_REGIST "REGIST"
#define COMMAND_UPDATE_ONLINE "UPDATE"
#define COMMAND_CONTACTS "CONTACTS"
#define COMMAND_UNREAD_MESSAGES "UNREAD MESSAGES"
#define COMMAND_ADMIN_LIST "ADMIN LIST"
#define COMMAND_EXIT "USER EXIT"

#define PATH_TO_LOGIN "./sql/CheckInDB.sql"
#define PATH_TO_REGIST "./sql/AddUser.sql"
#define PATH_TO_UPDATE_ONLINE "./sql/UpdateOnline.sql"
#define PATH_TO_CONTACTS "./sql/GetContactsByNickname.sql"
#define PATH_TO_UNREAD_MESSAGES "./sql/CountUnreadMessage.sql"
#define PATH_TO_ADMIN_LIST "./sql/CheckIsUserAdmin.sql"
#define PATH_TO_EXIT "./sql/ExitUser.sql"

#define PATH_TO_DATABASE "database.db"

#define CAN_NOT_OPEN_QUERY_FILE_MESSAGE "ERROR: CAN NOT OPEN FILE"

class Command {
public:
    virtual QJsonDocument exec() = 0;

protected:
    void updateQuery(const QString &file) {
        if (queryFile_.isOpen()) {
            queryFile_.close();
        }
        queryFile_.setFileName(file);
        queryString_ = CAN_NOT_OPEN_QUERY_FILE_MESSAGE;
        if (queryFile_.open(QIODevice::ReadOnly)) {
            queryString_ = QString(queryFile_.readAll());
        }
    }

    QTime start_;
    QFile queryFile_;
    QString queryString_;
};


#endif //SERVER_COMMAND_H
