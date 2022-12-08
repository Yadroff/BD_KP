#include "CommandLogin.h"

#include <utility>
#include <iostream>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonArray>

CommandLogin::CommandLogin(QSharedPointer<User> &user, QString login, const unsigned long long int &password)
        : user_(user), login_(std::move(login)), password_(password) {
    start_ = QTime::currentTime();
}

QJsonDocument CommandLogin::exec() {
    if (login() and updateOnline() and contacts() and unreadMessages() and listAdmin()) {
        std::cout << "LOGIN START: " << start_.toString().toStdString() << " LOGIN END: "
                  << QTime::currentTime().toString().toStdString() << " RESULT: SUCCESS" << std::endl;
    } else {
        std::cout << "LOGIN START: " << start_.toString().toStdString() << " LOGIN END: "
                  << QTime::currentTime().toString().toStdString() << " RESULT: FAIL" << std::endl;
    }
    return res_;
}

bool CommandLogin::login() {
    updateQuery(PATH_TO_LOGIN);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString() << " " << queryString_.toStdString() << std::endl;
        return false;
    }
    QJsonObject obj = res_.object();
    obj["Command"] = COMMAND_LOGIN;
    obj["Result"] = "";
    QSqlQuery qry;
    bool ok = qry.prepare(queryString_);
    if (!ok) {
        obj["Result"] = qry.lastError().text();
        res_.setObject(obj);
        return false;
    }
    qry.bindValue(":nickname", login_);
    qry.bindValue(":password_", password_);
    if (!qry.exec()) {
        obj["Result"] = qry.lastError().text();
        res_.setObject(obj);
        return false;
    }
    if (qry.next()) {
        obj["Result"] = "Success";
        user_->setUserIdInDataBase(qry.value(0).toUInt());
//        std::cout << user_->getUserIdInDataBase() << std::endl;
        res_.setObject(obj);
        return true;
    }
    std::cout << qry.lastError().text().toStdString() << std::endl;
    obj["Result"] = "SERVER FAIL";
    res_.setObject(obj);
    return false;
}

bool CommandLogin::unreadMessages() {
    updateQuery(PATH_TO_UNREAD_MESSAGES);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString() << " " << queryString_.toStdString()
                  << " UNREAD MESSAGES" << std::endl;
        return false;
    }
    QJsonObject obj = res_.object();
    obj["Command"] = obj["Command"].toString() + " & " + COMMAND_UNREAD_MESSAGES;
    QJsonArray messages;
    QSqlQuery query;
    bool ok = query.prepare(queryString_);
    if (!ok) {
        std::cout << QTime::currentTime().toString().toStdString() << "COMMAND LOGIN: UNREAD MESSAGES: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        return false;
    }
    query.bindValue(":receiver", user_->getUserIdInDataBase());
    auto mapContacts = user_->getContacts();
            foreach (const auto &nick, mapContacts->keys()) {
            query.bindValue(":sender", (*mapContacts)[nick]);
            if (!query.exec()) {
                std::cout << QTime::currentTime().toString().toStdString() << "COMMAND LOGIN: UNREAD MESSAGES: ERROR: "
                          << query.lastError().text().toStdString() << std::endl;
                obj["Result"] = "SERVER FAIL";
                res_.setObject(obj);
                return false;
            }
            if (query.next()) {
                int unread = query.value("COUNT(*)").toInt();
                if (unread > 0) {
                    QJsonObject _obj;
                    _obj[nick] = query.value("COUNT(*)").toInt();
                    messages.append(_obj);
                }
            } else {
                std::cout << QTime::currentTime().toString().toStdString() << "COMMAND LOGIN: UNREAD MESSAGES: ERROR: "
                          << query.lastError().text().toStdString() << std::endl;
                obj["Result"] = "SERVER FAIL";
                res_.setObject(obj);
                return false;
            }
        }
    obj["Messages"] = messages;
    res_.setObject(obj);
    return true;
}

bool CommandLogin::contacts() {
    updateQuery(PATH_TO_CONTACTS);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString() << " " << queryString_.toStdString() << " CONTACTS"
                  << std::endl;
        return false;
    }
    QJsonObject obj = res_.object();
    obj["Command"] = obj["Command"].toString() + " & " + COMMAND_CONTACTS;
    QJsonArray contacts;
    QSqlQuery query;
    bool ok = query.prepare(queryString_);
    if (!ok) {
        std::cout << QTime::currentTime().toString().toStdString() << "COMMAND LOGIN: GET CONTACTS: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        res_.setObject(obj);
        return false;
    }
    query.bindValue(":nickname", login_);
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << "COMMAND LOGIN: GET CONTACTS: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        res_.setObject(obj);
        return false;
    }
    while (query.next()) {
        QString _nick = query.value("NickName").toString();
        auto _id = query.value("ID").toULongLong();
        user_->addContact(_id, _nick);
        contacts.append(query.value("NickName").toString());
    }
    obj["Contacts"] = contacts;
    obj["Result"] = "SUCCESS";
    res_.setObject(obj);
    return true;
}

bool CommandLogin::updateOnline() {
    updateQuery(PATH_TO_UPDATE_ONLINE);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString() << " " << queryString_.toStdString()
                  << " UPDATE ONLINE" << std::endl;
        return false;
    }
    QJsonObject obj = res_.object();
    obj["Command"] = obj["Command"].toString() + " & " + COMMAND_UPDATE_ONLINE;
    QSqlQuery query;
    bool ok = query.prepare(queryString_);
    if (!ok) {
        std::cout << QTime::currentTime().toString().toStdString() << "COMMAND LOGIN: UPDATE ONLINE: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        res_.setObject(obj);
        return false;
    }
    query.bindValue(":nickname", login_);
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << "COMMAND LOGIN: UPDATE ONLINE: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        res_.setObject(obj);
        return false;
    }
    res_.setObject(obj);
    return true;
}

bool CommandLogin::listAdmin() {
    updateQuery(PATH_TO_ADMIN_LIST);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString() << " " << queryString_.toStdString()
                  << "ADMIN LIST" << std::endl;
        return false;
    }
    QJsonObject obj = res_.object();
    QJsonArray channelsList;
    obj["Command"] = obj["Command"].toString() + " & " + COMMAND_ADMIN_LIST;
    QSqlQuery query;
    bool ok = query.prepare(queryString_);
    if (!ok) {
        std::cout << QTime::currentTime().toString().toStdString() << "COMMAND LOGIN: ADMIN LIST: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        res_.setObject(obj);
        return false;
    }
    query.bindValue(":ID", user_->getUserIdInDataBase());
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << "COMMAND LOGIN: ADMIN LIST: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        res_.setObject(obj);
        return false;
    }
    while (query.next()) {
        channelsList.append(query.value("Name").toString());
    }
    obj["Admin List"] = channelsList;
    res_.setObject(obj);
    return true;
}
