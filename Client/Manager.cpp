//
// Created by yadroff on 04.12.22.
//

#include "Manager.h"
#include <qtestsupport_core.h>
#include <iostream>

Manager::Manager() {
    //TODO: виджет загрузки

//    QTest::qWait(1000);
    thread_ = QSharedPointer<QThread>(new QThread(this));
    crypto_ = QSharedPointer<CryptographAlice>(new CryptographAlice);
    connector_ = QSharedPointer<Connector>(new Connector);
    connector_->moveToThread(thread_.get());
    connect(connector_.get(), SIGNAL(finished(QString)), this, SLOT(connectToServer(QString)));
    connect(thread_.get(), SIGNAL(started()), connector_.get(), SLOT(run()));
    thread_->start();
}

void Manager::setLogin(const QString &newLogin) {
    userName_ = newLogin;
}

void Manager::connectToServer(const QString &servAddress) {
    std::cout << servAddress.toStdString() << std::endl;
    connector_->setRun(false);
    connector_->deleteLater();
    auto _stringList = servAddress.split(SEPARATOR);
    if (_stringList.size() != 4) {
        std::cout << QTime::currentTime().toString().toStdString() << " ERROR: CAN NOT CONNECT TO SERVER" << std::endl;
        //TODO: вывести ошибку
        return;
    }
    QString ip = _stringList.last();
    auto keyToBob = crypto_->getKeyToBob();
    sender_ = QSharedPointer<SenderReceiver>(new SenderReceiver(crypto_, ip, keyToBob));
    sender_->moveToThread(thread_.get());
}
