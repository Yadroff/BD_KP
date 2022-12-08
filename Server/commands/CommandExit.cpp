#include "CommandExit.h"

#include <utility>

CommandExit::CommandExit(QString nick) : nick_(std::move(nick)) {
    updateQuery(PATH_TO_EXIT);
    start_ = QTime::currentTime();
}

QJsonDocument CommandExit::exec() {
    QJsonDocument res;
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString() << " EXIT ERROR:" << queryString_.toStdString()
                  << std::endl;
        return res;
    }
    QJsonObject obj;
    obj["Command"] = COMMAND_EXIT;
    obj["Result"] = "";
    QSqlQuery qry;
    bool ok = qry.prepare(queryString_);
    if (!ok) {
        obj["Result"] = qry.lastError().text();
        res.setObject(obj);
        return res;
    }
    qry.bindValue(":nickname", nick_);
    if (!qry.exec()) {
        obj["Result"] = qry.lastError().text();
        res.setObject(obj);
        return res;
    }
    obj["Result"] = "Success";
    res.setObject(obj);
    return res;
}

void CommandExit::setNick(const QString &nick) {
    nick_ = nick;
}
