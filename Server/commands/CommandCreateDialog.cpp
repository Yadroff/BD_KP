#include "CommandCreateDialog.h"

#include <utility>

CommandCreateDialog::CommandCreateDialog(QSharedPointer<User> &user, QString second) :
        user_(user), secondUser_(std::move(second)) {}

QJsonDocument CommandCreateDialog::exec() {
    QJsonDocument doc;
    QJsonObject obj;
    obj["Command"] = COMMAND_CREATE_DIALOG;
    updateQueryString(PATH_TO_GET_ID);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " CREATE DIALOG: ERROR: CAN NOT OPEN QUERY FILE" << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    QSqlQuery query;
    if (!query.prepare(queryString_)) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    query.bindValue(":nickname", secondUser_);
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    if (!query.next()) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: ERROR: "
                  << "CAN NOT FIND USER " << secondUser_.toStdString() << " IN DATABASE" << std::endl;
        obj["Result"] = "CAN NOT FIND USER " + secondUser_;
        doc.setObject(obj);
        return doc;
    }
    int secondID = query.value("UserID").toInt();
    int firstID = user_->getUserIdInDataBase();
    std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: SECOND ID: "
              << secondID << std::endl;
    updateQueryString(PATH_TO_CREATE_DIALOG);
    if (queryString_ == CAN_NOT_OPEN_QUERY_FILE_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " CREATE DIALOG: ERROR: CAN NOT OPEN QUERY FILE" << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    QStringList queries = queryString_.split(";\n");
    if (queries.size() != CREATE_DIALOG_QUERIES) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: ERROR: "
                  << "WRONG QUERY SIZE" << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    if (!query.prepare(queries[0])) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    QString dialogName =
            qMin(user_->getUserName(), secondUser_) + DIALOG_SEPARATOR + qMax(user_->getUserName(), secondUser_);
    query.bindValue(":name", dialogName);
    query.bindValue(":first", firstID);
    if (!query.exec()) {
        std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: ERROR: "
                  << query.lastError().text().toStdString() << std::endl;
        obj["Result"] = "SERVER FAIL";
        doc.setObject(obj);
        return doc;
    }
    obj["Channel"] = dialogName;
    obj["ID"] = query.lastInsertId().toInt();
    for (int i = 1; i < CREATE_DIALOG_QUERIES; ++i) {
        if (!query.prepare(queries[i])) {
            std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: ERROR: "
                      << query.lastError().text().toStdString() << std::endl;
            obj["Result"] = "SERVER FAIL";
            doc.setObject(obj);
            return doc;
        }
        query.bindValue(":name", dialogName);
        if (i % 2 == 0) {
            query.bindValue(":second", secondID);
        } else {
            query.bindValue(":first", firstID);
        }
        if (!query.exec()) {
            std::cout << QTime::currentTime().toString().toStdString() << " COMMAND CREATE DIALOG: ERROR: "
                      << query.lastError().text().toStdString() << std::endl;
            obj["Result"] = "SERVER FAIL";
            doc.setObject(obj);
            return doc;
        }
    }
    obj["Result"] = "SUCCESS";
    doc.setObject(obj);
    return doc;
}
