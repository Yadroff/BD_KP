#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <iostream>

#include "../Definitions.h"
#include "../User.h"

class Command {
public:
    virtual QJsonDocument exec() = 0;

protected:
    void updateQueryString(const QString &file) {
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
