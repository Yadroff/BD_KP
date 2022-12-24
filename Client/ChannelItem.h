#ifndef CLIENT_CHANNELITEM_H
#define CLIENT_CHANNELITEM_H

#include <QListWidgetItem>
#include <QFile>

#include "CommonHeader.h"
#include  "Channel.h"

class ChannelItem : public QListWidgetItem {
public:
    ChannelItem(QString defaultChannelName, const QString &channelName, QListWidget *parent = nullptr,
                int type = Type);

    bool operator<(const QListWidgetItem &other) const final;

    const QString &getChannelName() const;

private:
    QString channelName_;
};

#endif //CLIENT_CHANNELITEM_H
