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
#include "commands/CommandExit.h"


class Server : public QObject {
Q_OBJECT
public:
    Server();

    ~Server() override;

private:
    QTcpServer *server_;
    bool isListen_ = false; // статус
    QMap<QTcpSocket *, QSharedPointer<User> > clients_;// пользователи
    QSqlDatabase database_;
    QSharedPointer<QThread> threadBroadcast_;
    QSharedPointer<Sender> senderBroadcast_;
    const unsigned short SERVER_PORT = 7777;
    QHash<QString, QSharedPointer<User>> users_;

    QJsonDocument parse(const QJsonDocument &doc, QSharedPointer<User> &user);

private slots:

    void newUser();

    void shutDownServer();

    void readData();

    void disconnectUser();

};


#endif //SERVER_SERVER_H
