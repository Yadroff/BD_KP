#include "CommandRegist.h"

#include <utility>
#include <iostream>

CommandRegist::CommandRegist(QSharedPointer<User> &user, QString nick, QString name,
                             QString surname, QString password)
        : user_(user), nickname_(std::move(nick)), name_(std::move(name)), surname_(std::move(surname)),
          password_(std::move(password)) {
    QFile file(PATH_TO_REGIST);
    QByteArray data;
    if (!file.open(QIODevice::ReadOnly)) {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " COMMAND REGIST: ERROR: CAN NOT OPEN FILE WITH QUERY" << std::endl;
        return;
    }
    data = file.readAll();
    query_ = QString(data);
//    std::cout << queryString_.toStdString() << std::endl;
}

QJsonDocument CommandRegist::exec() {
    QJsonDocument doc;
    QJsonObject obj;
    obj["Command"] = COMMAND_REGIST;
    QSqlQuery qry;
    bool ok = qry.prepare(query_);
    if (!ok) {
        obj["Result"] = qry.lastError().text();
        doc.setObject(obj);
        return doc;
    }
    qry.bindValue(":nickname", nickname_);
    qry.bindValue(":name", name_);
    qry.bindValue(":surname", surname_);
    qry.bindValue(":password", password_);
    if (qry.exec()) {
        obj["Result"] = "SUCCESS";
        user_->setUserName(nickname_);
    } else {
        std::cout << " COMMAND REGIST: ERROR\n" << qry.lastError().text().toStdString() << std::endl;
        obj["Result"] = "Fail";
    }
    doc.setObject(obj);
    return doc;
}
