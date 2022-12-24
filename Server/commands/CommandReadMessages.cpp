#include "CommandReadMessages.h"

#include <utility>

CommandReadMessages::CommandReadMessages(QString channelName, QDateTime dateTime) :
        channelName_(std::move(channelName)), dateTime_(std::move(dateTime)) {}

QJsonDocument CommandReadMessages::exec() {
    QJsonDocument doc;
    QJsonObject obj;
    obj["Command"] = COMMAND_READ_MESSAGES;
    updateQueryString(PATH_TO_READ_MESSAGES);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " READ MESSAGES: ERROR: CAN NOT OPEN QUERY FILE" << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    QSqlQuery query;
    if (!query.prepare(queryString_)) {
        std::cout << QTime::currentTime().toString().toStdString() << " READ MESSAGES: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    query.bindValue(":channel_name", channelName_);
    query.bindValue(":date", dateTime_.toString(DATE_FORMAT));
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << " READ MESSAGES: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    std::cout << dateTime_.toString(DATE_FORMAT).toStdString() << std::endl;
    QJsonArray messages;
    QJsonObject message;
    while (query.next()) {
        message["Sender"] = query.value("Nickname").toString();
        message["Date"] = query.value("DateCreate").toString();
        message["ID"] = query.value("MessageID").toInt();
        if (!query.value("DateEdit").isNull()) {
            message["Edit"] = query.value("DateEdit").toString();
        }
        std::string str = query.value("Content").toString().toStdString();
        std::string key = SECRET_KEY.toStdString();
        for (int i = 0; i < str.size(); ++i) {
            str[i] = static_cast<char>(str[i] ^ key[i]);
        }
        message["Text"] = QString(str.c_str());
        messages.append(message);
    }
    obj["Messages"] = messages;
    obj["Result"] = "SUCCESS";
    obj["Channel"] = channelName_;
    doc.setObject(obj);
    return doc;
}
