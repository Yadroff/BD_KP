#include "CommandSearch.h"

#include <utility>

CommandSearch::CommandSearch(QSharedPointer<User> &user, QString name) : nickname_(std::move(name)), user_(user) {}

QJsonDocument CommandSearch::exec() {
    QJsonDocument doc;
    QJsonObject obj;
    obj["Command"] = COMMAND_SEARCH;
    updateQueryString(PATH_TO_SEARCH);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString() << " SEARCH ERROR:" << queryString_.toStdString()
                  << std::endl;
        doc.setObject(obj);
        return doc;
    }
    QSqlQuery query;
    QStringList queries = queryString_.split(";\n");
    if (queries.size() != SEARCH_QUERIES) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND SEARCH: ERROR: "
                  << "WRONG QUERY SIZE" << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    if (!query.prepare(queries[0])) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND SEARCH: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    query.bindValue(":name", nickname_);
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND SEARCH: ERROR:"
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    QJsonArray users;
    while (query.next()) {
        users.append(query.value("Nickname").toString());
    }
    obj["Users"] = users;
    if (!query.prepare(queries[1])) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND SEARCH: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    query.bindValue(":name", nickname_);
    query.bindValue(":id", user_->getUserIdInDataBase());
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND SEARCH: ERROR:"
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    QJsonArray channels;
    while (query.next()) {
        channels.append(query.value("ChannelName").toString());
    }
    obj["Result"] = "SUCCESS";
    obj["Channels"] = channels;
    doc.setObject(obj);
    return doc;
}
