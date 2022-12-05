#ifndef SERVER_USER_H
#define SERVER_USER_H

#include <QObject>

#include "Cryptograph.h"

class User : public QObject {
Q_OBJECT
public:
    User();

    void setKeyFromAlice(const long long &key);

    QByteArray encode(const QByteArray &arr);

    [[nodiscard]] bool hasSessionKey() const;

    void setHasSessionKey(bool hasSessionKey);

    QByteArray decode(const QByteArray &arr) const;

    long long getKeyToAlice() const;

private:
    bool hasSessionKey_;
    CryptographBob crypto_;
    QString userName_;
};


#endif //SERVER_USER_H
