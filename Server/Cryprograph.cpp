#include "Cryptograph.h"

#include <QTime>

long long powMod(long long a, long long b, long long p) {
    long long res = 1;
    while (b)
        if (b & 1)
            res = int(res * 1ll * a % p), --b;
        else
            a = int(a * 1ll * a % p), b >>= 1;
    return res;
}

CryptographBob::CryptographBob() : generator(QTime::currentTime().msecsSinceStartOfDay()), a(0), sessionKey_(0) {
    b = generator.bounded(static_cast<long long>(2), p - 1);
}

long long CryptographBob::getKeyToAlice() const {
    return powMod(g, b, p);
}

long long CryptographBob::getSessionKey() const {
    return sessionKey_;
}

void CryptographBob::setKeyFromAlice(const long long int &AliceNumber) {
    a = AliceNumber;
    sessionKey_ = powMod(a, b, p);
}
