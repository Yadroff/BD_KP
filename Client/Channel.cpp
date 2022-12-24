#include "Channel.h"

#include <utility>
#include <iostream>
#include <QJsonParseError>

QHash<QString, QPair<QDateTime, int>> Channel::channels;

inline void swap(QJsonValueRef v1, QJsonValueRef v2) {
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

Channel::Channel(const QString &channelName, QDateTime lastMessage, int roots) :
        lastMessage_(std::move(lastMessage)),
        roots_(roots), hasMessages_(false) {
    QStringList strList = channelName.split(DIALOG_SEPARATOR);
    strList.removeOne(userName);
    channelName_ = strList.first();
    cacheFile_.setFileName(PATH_TO_CACHE + channelName_ + CACHE_SUFFIX);
}

void Channel::addMessage(const QSharedPointer<Message> &mess) {
    channels[channelName_].first = qMax(mess->getSendDate(), channels[channelName_].first);
    messages_.append(mess);
    hasMessages_ = true;
}

void Channel::cache() {
    if (messages_.isEmpty()) {
        return;
    }
    if (!cacheFile_.open(QIODevice::WriteOnly | QIODevice::Append)) {
        std::cout << QTime::currentTime().toString().toStdString() << " CHANNEL " << channelName_.toStdString()
                  << " CAN NOT OPEN CACHE FILE: " << cacheFile_.errorString().toStdString() << std::endl;
        return;
    }
    QJsonDocument doc;
    QJsonArray messages;
    for (const auto &mess: messages_) {
        messages.append(mess->toJson());
//        lastCashedMessage_ = mess->getId();
    }
    doc.setArray(messages);
    cacheFile_.write(doc.toJson());
    messages_.clear();
    cacheFile_.close();
}

int Channel::getRoots() const {
    return roots_;
}

QJsonDocument Channel::getAllMessages() {
    cache();
    return std::move(getMessagesFromCache());
}

QJsonDocument Channel::getMessagesFromCache() {
    if (!cacheFile_.open(QIODevice::ReadOnly)) {
        std::cout << QTime::currentTime().toString().toStdString() << " CHANNEL " << channelName_.toStdString()
                  << " CAN NOT OPEN CACHE FILE" << std::endl;
        return {};
    }
    cacheFile_.seek(0);
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(cacheFile_.readAll(), &err);
    if (err.error != QJsonParseError::NoError) {
        std::cout << QTime::currentTime().toString().toStdString() << " " << err.errorString().toStdString()
                  << std::endl;
        cacheFile_.close();
        return {};
    }
    std::cout << QTime::currentTime().toString().toStdString() << " GET MESSAGES FROM CACHE: SUCCESS" << std::endl;
    cacheFile_.close();
    QJsonArray messages = doc.array();
    std::sort(messages.begin(), messages.end(), [](const QJsonValue &v1, const QJsonValue &v2) {
        return v1.toObject()["Send"].toString() < v2.toObject()["Send"].toString();
    });
    doc.setArray(messages);
    return doc;
}

bool Channel::hasMessages() const {
    return hasMessages_;
}

QJsonDocument Channel::getUncachedMessages() {
    QJsonDocument doc;
    QJsonArray messages;
    for (auto &message: messages_) {
        messages.append(message->toJson());
    }
    doc.setArray(messages);
    return std::move(doc);
}
