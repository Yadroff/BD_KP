#ifndef SERVER_CRYPTOGRAPH_H
#define SERVER_CRYPTOGRAPH_H

#include <QRandomGenerator>

long long powMod(long long a, long long b, long long p);

class CryptographBob {
public:
    CryptographBob();

    explicit CryptographBob(const long long &AliceNumber);

    void setKeyFromAlice(const long long &AliceNumber);

    [[nodiscard]] long long getKeyToAlice() const;

    [[nodiscard]] long long getSessionKey() const;

private:
    static const long long g = 5;
    static const long long p = 1000000000 + 7;
    long long a;
    long long b;
    QRandomGenerator generator;
    long long sessionKey_{};
};


#endif //SERVER_CRYPTOGRAPH_H
