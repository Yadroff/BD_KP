#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <QTcpSocket>
#include <QThread>

#include "Connector.h"
#include "SenderReceiver.h"
#include "CryptographAlice.h"
#include "CommonHeader.h"


class Manager : public QObject {
Q_OBJECT
public:
    Manager();

private:
    QSharedPointer<QThread> thread_;
    QSharedPointer<CryptographAlice> crypto_;
    QSharedPointer<Connector> connector_;
    QSharedPointer<SenderReceiver> sender_;
    QString userName_;
//    const QString SEPARATOR = QString(QChar(7));
private slots:

    void setLogin(const QString &);

    void connectToServer(const QString &);

//    void loginFail(const QString &);

//    void login(const QString &);
};


#endif //CLIENT_MANAGER_H
