#include "CommandInitDB.h"

CommandInitDB::CommandInitDB() {
    updateQueryString(PATH_TO_INIT_DB);
}

QJsonDocument CommandInitDB::exec() {
    QJsonDocument doc;
    QJsonObject obj;
    obj["Command"] = COMMAND_INIT_DB;
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " INIT DB: ERROR: CAN NOT OPEN QUERY FILE" << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    QSqlQuery query;
    if (!query.prepare(queryString_)) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND INIT DB: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND INIT DB: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    obj["Result"] = "SUCCESS";
    doc.setObject(obj);
    return doc;
}
