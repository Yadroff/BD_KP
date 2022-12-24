#include "CommandAddToTempTable.h"

#include <utility>

CommandAddToTempTable::CommandAddToTempTable(QString userName, QString channel) : userName_(std::move(userName)),
                                                                                  channel_(std::move(channel)) {}

QJsonDocument CommandAddToTempTable::exec() {
    QJsonDocument doc;
    QJsonObject obj;
    obj["Command"] = COMMAND_ADD_IN_TEMP_TABLE;
    updateQueryString(PATH_TO_ADD_IN_TEMP_TABLE);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " ADD IN TEMP TABLE: ERROR: CAN NOT OPEN QUERY FILE" << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    QSqlQuery query;
    if (!query.prepare(queryString_)) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND ADD IN TEMP TABLE: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    query.bindValue(":nickname", userName_);
    query.bindValue(":channel_name", channel_);
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    obj["Result"] = "SUCCESS";
    doc.setObject(obj);
    return doc;
}
