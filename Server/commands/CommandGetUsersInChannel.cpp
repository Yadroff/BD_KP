#include "CommandGetUsersInChannel.h"

#include <utility>

CommandGetUsersInChannel::CommandGetUsersInChannel(QString channelName) :
        channelName_(std::move(channelName)) {
    updateQueryString(PATH_TO_GET_USERS_IN_CHANNEL);
}

QJsonDocument CommandGetUsersInChannel::exec() {
    QJsonDocument res;
    QJsonObject obj;
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        obj["Result"] = "SERVER FAIL";
        res.setObject(obj);
        return res;
    }
    QJsonArray users;
    QSqlQuery query;
    if (!query.prepare(queryString_)) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND GET USERS IN CHANNEL: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        res.setObject(obj);
        return res;
    }
    query.bindValue(":channel_name", channelName_);
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND GET USERS IN CHANNEL: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        res.setObject(obj);
        return res;
    }
    while (query.next()) {
        users.append(query.value("Nickname").toString());
    }
    obj["Users"] = users;
    res.setObject(obj);
    return res;
}
