#include "Sender.h"

#include <QThread>
#include <iostream>

Sender::Sender(QObject *parent)
    : QObject{parent}
{
    runFlag_ = true;
    socket_ = new QUdpSocket(this);
}

void Sender::setRun(bool flag)
{
    runFlag_ = flag;
}

void Sender::run()
{
    std::cout << "SENDER: START SENDING" << std::endl;
    while (runFlag_){
        socket_->writeDatagram(DATAGRAM_TO_CLIENT, QHostAddress::Broadcast, SERVER_PORT);
        QThread::sleep(2);
    }
    std::cout << "SERVER: END SENDING" << std::endl;
    emit finished();
}
