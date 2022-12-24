#include "CommandSendMessage.h"

#include <utility>

CommandSendMessage::CommandSendMessage(QSharedPointer<User> &user, QString channelName, QString text,
                                       QDateTime date) :
        user_(user), channel_(std::move(channelName)), text_(std::move(text)), date_(std::move(date)) {
    start_ = QTime::currentTime();
    updateQueryString(PATH_TO_SEND_MESSAGE);
    std::string str = text_.toStdString();
    std::string key = SECRET_KEY.toStdString();
    for (int i = 0; i < text_.size(); ++i) {
        str[i] = static_cast<char>(str[i] ^ key[i]);
    }
    text_ = QString(str.c_str());
}

QJsonDocument CommandSendMessage::exec() {
    QJsonDocument doc;
    QJsonObject obj;
//    std::cout << queryString_.toStdString() << std::endl;
//    std::cout << date_.toString("yyyy-MM-dd hh:mm:ss").toStdString() << std::endl;
    obj["Command"] = COMMAND_SEND_MESSAGE;
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " SEND MESSAGE: ERROR: CAN NOT OPEN QUERY FILE" << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    auto stringList = queryString_.split(";\n");
    auto [id, roots] = user_->getIDRootsFromChannel(channel_);
    if (roots < ROOTS_ADMINISTRATOR) {
        obj["Result"] = "FAIL";
        obj["Error"] = "You should have ADMIN roots to send messages";
        doc.setObject(obj);
        return doc;
    }
    QSqlQuery query;
    bool ok = query.prepare(stringList[0]);
    if (!ok) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND SEND MESSAGE: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    query.bindValue(":sender_id", user_->getUserIdInDataBase());
    query.bindValue(":date", date_.toString(DATE_FORMAT));
    query.bindValue(":channel_name", channel_);
//    query.bindValue(":text", text_);
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND SEND MESSAGE: ERROR:"
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    ok = query.prepare(stringList[1]);
    if (!ok) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND SEND MESSAGE: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    query.bindValue(":text", text_);
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << "COMMAND SEND MESSAGE: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    obj["Result"] = "SUCCESS";
    obj["MessageID"] = query.lastInsertId().toInt();
    doc.setObject(obj);
    return doc;
}
