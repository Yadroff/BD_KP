//
// Created by yadroff on 04.12.22.
//

#ifndef CLIENT_SENDERRECEIVER_H
#define CLIENT_SENDERRECEIVER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedPointer>

#include "CommonHeader.h"
#include "CryptographAlice.h"

class SenderReceiver : public QObject {
Q_OBJECT
public:
    SenderReceiver(QSharedPointer<CryptographAlice> &crypto, const QString &ip, const long long &keyToBob);

    ~SenderReceiver() override;

public slots:

    void send(const QString &str);

private:
    QSharedPointer<QTcpSocket> socket_;
    QSharedPointer<CryptographAlice> crypto_;

    void parse(const QJsonDocument &);

private slots:

    void readData();

signals:

};


#endif //CLIENT_SENDERRECEIVER_H
