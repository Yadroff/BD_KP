#include "Message.h"

#include <utility>

Message::Message(int ID, QString channel, QString sender, QString content, QDateTime send, QDateTime edit) :
        id_(ID), channel_(std::move(channel)), sender_(std::move(sender)), content_(std::move(content)),
        sendDate_(std::move(send)), editDate_(std::move(edit)) {}

QJsonObject Message::toJson() const{
    QJsonObject obj;
    obj["ID"] = id_;
    obj["Channel"] = channel_;
    obj["Sender"] = sender_;
    obj["Content"] = content_;
    obj["Send"] = sendDate_.toString(DATE_FORMAT);
    if (editDate_.isValid()) {
        obj["Edit"] = editDate_.toString(DATE_FORMAT);
    }
    return std::move(obj);
}

void Message::setContent(const QString &content) {
    content_ = content;
}

int Message::getId() const {
    return id_;
}

const QString &Message::getChannel() const {
    return channel_;
}

const QString &Message::getSender() const {
    return sender_;
}

const QDateTime &Message::getSendDate() const {
    return sendDate_;
}

const QDateTime &Message::getEditDate() const {
    return editDate_;
}

const QString &Message::getContent() const {
    return content_;
}
