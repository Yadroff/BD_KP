#ifndef CLIENT_CHANNEL_H
#define CLIENT_CHANNEL_H

#include "CommonHeader.h"

#include <QListWidgetItem>

class Channel : public QListWidgetItem {
public:
    Channel(const QString &defaultChannelName, const QString &channelName, QListWidget *parent = nullptr,
            int type = Type);

    bool operator<(const QListWidgetItem &other) const final;

    static QHash<QString, QPair<QDateTime, int>> channels;

private:
    QString channelName_;
};

#endif //CLIENT_CHANNEL_H
