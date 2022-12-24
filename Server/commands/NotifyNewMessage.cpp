#include "NotifyNewMessage.h"

#include <utility>

NotifyNewMessage::NotifyNewMessage(QHash<QString, QSharedPointer<User>> &users, QJsonDocument onlineUsers,
                                   QJsonDocument message,
                                   QString sender, QString channel, int messageID) :
        users_(users), onlineUsers_(std::move(onlineUsers)), channel_(std::move(channel)), sender_(std::move(sender)),
        message_(std::move(message)), messageID_(messageID) {}

QJsonDocument NotifyNewMessage::exec() {
    QJsonDocument res;
    QJsonObject obj;
    obj["Command"] = COMMAND_NOTIFY_NEW_MESSAGE;
    QJsonDocument send;
    QJsonObject objSend = message_.object();
    objSend["Command"] = COMMAND_NOTIFY_NEW_MESSAGE;
    objSend["MessageID"] = messageID_;
    objSend["Sender"] = sender_;
    send.setObject(objSend);
    auto userNames = onlineUsers_.object()["Users"].toArray();
    for (auto &&item: userNames) {
        QString name = item.toString();
        users_[name]->writeJSON(send);
    }
    obj["Result"] = "SUCCESS";
    res.setObject(obj);
    return res;
}
