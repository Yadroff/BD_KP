#ifndef CLIENT_CRYPTOGRAPHALICE_H
#define CLIENT_CRYPTOGRAPHALICE_H

#include <QRandomGenerator>

long long powMod(long long a, long long b, long long p);

class CryptographAlice {
public:
    CryptographAlice();

    [[nodiscard]] long long getKeyToBob() const;

    void setKeyFromBob(const long long &Bob);

    [[nodiscard]] long long getSessionKey() const;

    [[nodiscard]] QByteArray encode(const QByteArray &arr) const;

    QByteArray decode(const QByteArray &arr) const;
private:
    static const long long g = 5;
    static const long long p = 1000000000 + 7;
    QRandomGenerator generator_;
    long long a;
    long long b;
    long long sessionKey;
};


#endif //CLIENT_CRYPTOGRAPHALICE_H
