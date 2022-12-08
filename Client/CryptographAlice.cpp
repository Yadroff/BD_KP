#include "CryptographAlice.h"

#include <QTime>
#include <iostream>

long long powMod(long long a, long long b, long long p) {
    long long res = 1;
    while (b)
        if (b & 1)
            res = int(res * 1ll * a % p), --b;
        else
            a = int(a * 1ll * a % p), b >>= 1;
    return res;
}

CryptographAlice::CryptographAlice() : generator_(QTime::currentTime().msecsSinceStartOfDay()), b(0), sessionKey(0) {
    a = generator_.bounded(static_cast<long long>(2), p - 1);
}

long long CryptographAlice::getKeyToBob() const {
    std::cout << QTime::currentTime().toString().toStdString() << " CRYPTOGRAPH: GET_KEY_TO_BOB" << std::endl;
    return powMod(g, a, p);
}

void CryptographAlice::setKeyFromBob(const long long &Bob) {
    b = Bob;
    sessionKey = powMod(Bob, a, p);
}

long long CryptographAlice::getSessionKey() const {
    return sessionKey;
}

QByteArray CryptographAlice::encode(const QByteArray &arr) const {
    QByteArray res(arr);
    if (sessionKey == 0) {
        return res;
    }
    long long key = sessionKey;
    QString str(res);
    long long _long_size = sizeof(long long);
    long long ost = res.size() % _long_size;
    if (ost != 0) {
        str.resize(str.size() + _long_size - ost, '\0');
        res = str.toUtf8();
    }
//    std::cout << res.size() % _long_size << std::endl;
//    std::cout << key << std::endl;
    auto *array = new long long[res.size() / _long_size];
    memcpy(array, res.data(), _long_size);
    for (int i = 0; i < res.size() / _long_size; ++i) {
        array[i] = array[i] ^ key;
    }
    memcpy(res.data(), array, _long_size);
    return res;
}

QByteArray CryptographAlice::decode(const QByteArray &arr) const {
    if (sessionKey == 0) {
        return arr;
    }
    QByteArray res(arr);
    long long key = sessionKey;
    long long _long_size = sizeof(long long);
//    std::cout << res.size() % _long_size << std::endl;
    auto *array = new long long[arr.size() / _long_size];
    memcpy(array, res, _long_size);
    for (int i = 0; i < arr.size() / _long_size; ++i) {
        array[i] = array[i] ^ key;
    }
    memcpy(res.data(), array, _long_size);
    QString str(res);
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '\0') {
            str.resize(i);
            break;
        }
    }
    return str.toUtf8();
}
