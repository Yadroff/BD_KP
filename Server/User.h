#ifndef SERVER_USER_H
#define SERVER_USER_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QTcpSocket>
#include <QJsonDocument>

#include "Cryptograph.h"

class User : public QObject {
Q_OBJECT
public:
    User(QTcpSocket *socket);

    void setKeyFromAlice(const long long &key);

    QByteArray encode(const QByteArray &arr);

    [[nodiscard]] bool hasSessionKey() const;

    void setHasSessionKey(bool hasSessionKey);

    [[nodiscard]] QByteArray decode(const QByteArray &arr) const;

    [[nodiscard]] long long getKeyToAlice() const;

    [[nodiscard]] unsigned int getUserIdInDataBase() const;

    void setUserIdInDataBase(unsigned int userIdInDataBase);

    void addChannel(const QString &channel, const unsigned long long &id, const int &roots);

    QPair<unsigned long long, int> getIDRootsFromChannel(const QString &channelName);

    QSharedPointer<QMap<QString, QPair<unsigned long long int, int>>> &getContacts();

    [[nodiscard]] const QString &getUserName() const;

    void setUserName(const QString &userName);

    void writeJSON(const QJsonDocument &doc);

private:
    bool hasSessionKey_;
    CryptographBob crypto_;
    QString userName_;
    unsigned int UserIDInDataBase_ = 0;
    QSharedPointer<QMap<QString, QPair<unsigned long long, int>>> contacts_;
    QTcpSocket *socket_;
};


#endif //SERVER_USER_H
