#ifndef CLIENT_SENDERRECEIVER_H
#define CLIENT_SENDERRECEIVER_H

#include <QObject>
#include <QTcpSocket>


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

    void parseServerAnswer(const QJsonDocument &doc);

    QJsonDocument parseClientSend(const QString &str);

    static const int COMMAND_LOGIN_SIZE = 3;
    static const int COMMAND_REGIST_SIZE = 5;
    static const int COMMAND_GET_MESSAGES_SIZE = 3;
    static const int COMMAND_SEND_MESSAGE_SIZE = 4;
    static const int COMMAND_SEARCH_SIZE = 2;

private slots:

    void readData();

    void sendDisconnect();

signals:

    void serverDisconnect();

    void serverMessage(const QJsonDocument &doc);
};


#endif //CLIENT_SENDERRECEIVER_H
