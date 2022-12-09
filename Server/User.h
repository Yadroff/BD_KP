#ifndef SERVER_USER_H
#define SERVER_USER_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>

#include "Cryptograph.h"

class User : public QObject {
Q_OBJECT
public:
    User();

    void setKeyFromAlice(const long long &key);

    QByteArray encode(const QByteArray &arr);

    [[nodiscard]] bool hasSessionKey() const;

    void setHasSessionKey(bool hasSessionKey);

    [[nodiscard]] QByteArray decode(const QByteArray &arr) const;

    [[nodiscard]] long long getKeyToAlice() const;

    [[nodiscard]] unsigned int getUserIdInDataBase() const;

    void setUserIdInDataBase(unsigned int userIdInDataBase);

    void addChannel(const unsigned long long &id, const QString &nick);

    unsigned long long getIDContact(const QString &id);

    QSharedPointer<QMap<QString, unsigned long long int>> &getContacts();

    [[nodiscard]] const QString &getUserName() const;

    void setUserName(const QString &userName);

private:
    bool hasSessionKey_;
    CryptographBob crypto_;
    QString userName_;
    unsigned int UserIDInDataBase_ = 0;
    QSharedPointer<QMap<QString, unsigned long long>> contacts_;
};


#endif //SERVER_USER_H
