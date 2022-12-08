#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QtSql>
#include <QSqlDatabase>
#include <QSharedPointer>

#include <iostream>

#include "Sender.h"
#include "User.h"

#include "commands/Command.h"
#include "commands/CommandReplyKey.h"
#include "commands/CommandLogin.h"
#include "commands/CommandRegist.h"


class Server : public QObject {
Q_OBJECT
public:
    Server();

    ~Server() override;

private:
    QTcpServer *server_;
    QMap<long long, QSharedPointer<QTcpSocket>> clients_; // [clientID, socket]
    bool isListen_ = false; // статус
    QMap<QSharedPointer<QTcpSocket>, QSharedPointer<User> > users_;// пользователи
    QSqlDatabase database_;
    QSharedPointer<QThread> threadBroadcast_;
    QSharedPointer<Sender> senderBroadcast_;
    const unsigned short SERVER_PORT = 7777;

    QJsonDocument parse(const QJsonDocument &doc, QSharedPointer<User> &user);

private slots:

    void newUser();

    void shutDownServer();

    void readData();

    void disconnectUser();

};


#endif //SERVER_SERVER_H
