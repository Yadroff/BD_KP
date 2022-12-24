#include "ChannelItem.h"

#include <utility>

bool ChannelItem::operator<(const QListWidgetItem &other) const {
    return Channel::channels[channelName_].first >= Channel::channels[((ChannelItem *) (&other))->channelName_].first;
}

ChannelItem::ChannelItem(QString defaultChannelName, const QString &channelName, QListWidget *parent, int type)
        : QListWidgetItem(channelName, parent, type),
          channelName_(std::move(defaultChannelName)) {}

const QString &ChannelItem::getChannelName() const {
    return channelName_;
}
