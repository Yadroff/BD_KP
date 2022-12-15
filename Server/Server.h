#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QtSql>
#include <QSqlDatabase>
#include <QSharedPointer>

#include <iostream>

#include "IncludesToServer.h"

class Server : public QObject {
Q_OBJECT
public:
    Server();

    ~Server() override;

private:
    QTcpServer *server_;
    bool isListen_ = false; // статус
    QHash<QTcpSocket *, QSharedPointer<User> > clients_;// пользователи
    QSqlDatabase database_;
    QSharedPointer<QThread> threadBroadcast_;
    QSharedPointer<Sender> senderBroadcast_;
    const unsigned short SERVER_PORT = 7777;
    QHash<QString, QSharedPointer<User>> users_;

    QJsonDocument parse(const QJsonDocument &doc, QSharedPointer<User> &user);

    void test(QSharedPointer<User> &user);

private slots:

    void newUser();

    void shutDownServer();

    void readData();

    void disconnectUser();

};


#endif //SERVER_SERVER_H
