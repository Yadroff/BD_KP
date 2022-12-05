#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QUdpSocket>

const QByteArray DATAGRAM_TO_CLIENT = "SERVER SEND";

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(QObject *parent = nullptr);
    void setRun(bool flag);
private:
    bool runFlag_;
    static const unsigned short SERVER_PORT = 44444;

    QUdpSocket *socket_;
public slots:
    void run();
signals:
    void finished();
};

#endif // SENDER_H
