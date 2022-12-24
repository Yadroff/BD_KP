#include "CommandExit.h"

#include <utility>

CommandExit::CommandExit(QString nick) : nick_(std::move(nick)) {
    updateQueryString(PATH_TO_EXIT);
    start_ = QTime::currentTime();
}

QJsonDocument CommandExit::exec() {
    QJsonDocument res;
    QJsonObject obj;
    obj["Command"] = COMMAND_EXIT;
    obj["Result"] = "";
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString() << " EXIT ERROR:" << queryString_.toStdString()
                  << std::endl;
        obj["Result"] = CAN_NOT_OPEN_QUERY_FILE_MESSAGE;
        res.setObject(obj);
        return res;
    }
    QSqlQuery qry;
    QStringList queries = queryString_.split(";\n");
    if (queries.size() != EXIT_QUERIES) {
        std::cout << QTime::currentTime().toString().toStdString() << " EXIT ERROR: WRONG QUERIES SIZE" << std::endl;
        obj["Result"] = "Server error";
        res.setObject(obj);
        return res;
    }
    for (int i = 0; i < EXIT_QUERIES; ++i) {
        if (!qry.prepare(queries[i])) {
            std::cout << QTime::currentTime().toString().toStdString() << " EXIT ERROR: "
                      << qry.lastError().text().toStdString() << std::endl;
            obj["Result"] = "Server error";
            res.setObject(obj);
            return res;
        }
        qry.bindValue(":nickname", nick_);
        if (!qry.exec()) {
            std::cout << QTime::currentTime().toString().toStdString() << " EXIT ERROR: "
                      << qry.lastError().text().toStdString() << std::endl;
            obj["Result"] = "Server error";
            res.setObject(obj);
            return res;
        }
    }
    obj["Result"] = "Success";
    res.setObject(obj);
    return res;
}

void CommandExit::setNick(const QString &nick) {
    nick_ = nick;
}


