//
// Created by yadroff on 08.12.22.
//

#include "CommandUpdateOnline.h"

#include <utility>
#include <iostream>

CommandUpdateOnline::CommandUpdateOnline(QSharedPointer<User> &user, QString nickname) :
        user_(user), nickname_(std::move(nickname)) {
    QFile file(PATH_TO_UPDATE_ONLINE);
    QByteArray data;
    if (!file.open(QIODevice::ReadOnly)) {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " COMMAND LOGIN:ERROR: CAN NOT OPEN FILE WITH QUERY" << std::endl;
        return;
    }
    data = file.readAll();
    query_ = QString(data);
}

QJsonDocument CommandUpdateOnline::exec() {
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
    if (qry.exec()) {
        obj["Result"] = "Success";
    } else {
        obj["Result"] = qry.lastError().text();
    }
    doc.setObject(obj);
    return doc;
}
