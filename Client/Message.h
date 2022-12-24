#ifndef CLIENT_MESSAGE_H
#define CLIENT_MESSAGE_H

#include <QObject>
#include <QDateTime>
#include <QJsonObject>

#include "CommonHeader.h"

class Message : public QObject {
Q_OBJECT
public:
    Message(int ID, QString channel, QString sender, QString content, QDateTime send, QDateTime edit = QDateTime());

    [[nodiscard]] QJsonObject toJson() const;

    void setContent(const QString &content);

    [[nodiscard]] int getId() const;

    [[nodiscard]] const QString &getChannel() const;

    [[nodiscard]] const QString &getSender() const;

    [[nodiscard]] const QDateTime &getSendDate() const;

    [[nodiscard]] const QDateTime &getEditDate() const;

    [[nodiscard]] const QString &getContent() const;

private:
    int id_;
    QString channel_;
    QString sender_;
    QDateTime sendDate_;
    QString content_;
    QDateTime editDate_;
};


#endif //CLIENT_MESSAGE_H
