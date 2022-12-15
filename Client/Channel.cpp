#include "Channel.h"

QHash<QString, QPair<QDateTime, int>> Channel::channels;

bool Channel::operator<(const QListWidgetItem &other) const {
    return channels[this->text()].first > channels[other.text()].first;
}

Channel::Channel(const QString &defaultChannelName, const QString &channelName, QListWidget *parent, int type)
        : QListWidgetItem(channelName, parent, type),
          channelName_(defaultChannelName) {}
