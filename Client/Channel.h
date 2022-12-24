#ifndef CLIENT_CHANNEL_H
#define CLIENT_CHANNEL_H

#include <QObject>
#include <QFile>
#include <QHash>
#include <QDateTime>

#include "Message.h"

class Channel : public QObject {
Q_OBJECT
public:
    Channel(const QString& channelName, QDateTime lastMessage, int roots);

    void addMessage(const QSharedPointer<Message> &mess);

    void cache();

    [[nodiscard]] int getRoots() const;

    static QHash<QString, QPair<QDateTime, int>> channels;

    QJsonDocument getAllMessages();

    QJsonDocument getUncachedMessages();

    [[nodiscard]] bool hasMessages() const;

private:
    QString channelName_;
//    int lastCashedMessage_;
    QFile cacheFile_;
    QDateTime lastMessage_;
    int roots_;
    QList<QSharedPointer<Message>> messages_;

    bool hasMessages_;

    QJsonDocument getMessagesFromCache();
};


#endif //CLIENT_CHANNEL_H
