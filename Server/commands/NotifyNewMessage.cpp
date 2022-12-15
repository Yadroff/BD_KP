#include "NotifyNewMessage.h"

#include <utility>

NotifyNewMessage::NotifyNewMessage(QHash<QString, QSharedPointer<User>> &users, QJsonDocument doc, QString channel,
                                   QString sender) :
        users_(users), doc_(std::move(doc)), channel_(std::move(channel)), sender_(std::move(sender)) {}

QJsonDocument NotifyNewMessage::exec() {
    QJsonDocument res;
    QJsonObject obj;
    obj["Command"] = COMMAND_NOTIFY_NEW_MESSAGE;
    QJsonDocument send;
    QJsonObject objSend;
    objSend["Command"] = COMMAND_NOTIFY_NEW_MESSAGE;
    objSend["Channel"] = channel_;
    send.setObject(objSend);
    auto userNames = doc_.array();
    for (auto &&item: userNames) {
        QJsonObject nameObject = item.toObject();
        QString name = nameObject["Nickname"].toString();
        if (name == sender_) {
            continue;
        }
        users_[name]->writeJSON(send);
    }
    obj["Result"] = "SUCCESS";
    res.setObject(obj);
}
