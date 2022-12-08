#include "CommandLogin.h"

#include <utility>
#include <iostream>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlError>

CommandLogin::CommandLogin(QSharedPointer<User> &user, QString login, const unsigned long long int &password)
        : user_(user), login_(std::move(login)), password_(password) {
    QFile file(PATH_TO_LOGIN);
    QByteArray data;
    if (!file.open(QIODevice::ReadOnly)) {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " COMMAND LOGIN:ERROR: CAN NOT OPEN FILE WITH QUERY" << std::endl;
        return;
    }
    data = file.readAll();
    query_ = QString(data);
//    std::cout << query_.toStdString() << std::endl;
}

QJsonDocument CommandLogin::exec() {
    QJsonDocument res;
    QJsonObject obj;
    obj["Command"] = COMMAND_LOGIN;
    obj["Result"] = "";
    QSqlQuery qry;
    bool ok = qry.prepare(query_);
    if (!ok) {
        obj["Result"] = qry.lastError().text();
        res.setObject(obj);
        return res;
    }
    qry.bindValue(":nickname", login_);
    qry.bindValue(":password_", password_);
    if (!qry.exec()) {
        obj["Result"] = qry.lastError().text();
        res.setObject(obj);
        return res;
    }
    if (qry.next()) {
        obj["Result"] = "Success";
        user_->setUserIdInDataBase(qry.value(0).toUInt());
//        std::cout << user_->getUserIdInDataBase() << std::endl;
    } else {
        std::cout << qry.lastError().text().toStdString() << std::endl;
        obj["Result"] = "Fail";
    }
    res.setObject(obj);
    return res;
}
