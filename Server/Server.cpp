#include "Server.h"

Server::Server() {
    senderBroadcast_ = QSharedPointer<Sender>(new Sender());
    threadBroadcast_ = QSharedPointer<QThread>(new QThread(this));
    senderBroadcast_->moveToThread(threadBroadcast_.get());
    connect(threadBroadcast_.get(), SIGNAL(started()), senderBroadcast_.get(), SLOT(run()));
    connect(senderBroadcast_.get(), SIGNAL(finished()), senderBroadcast_.get(), SLOT(deleteLater()));
    connect(senderBroadcast_.get(), SIGNAL(finished()), threadBroadcast_.get(), SLOT(quit()));
    connect(threadBroadcast_.get(), SIGNAL(finished()), threadBroadcast_.get(), SLOT(deleteLater()));
    threadBroadcast_->start();
    server_ = new QTcpServer(this);
    connect(server_, SIGNAL(newConnection()), this, SLOT(newUser()));
    if (!server_->listen(QHostAddress::Any, SERVER_PORT)) {
        perror("SERVER DOESN'T LISTEN");
    } else {
        isListen_ = true;
        std::cout << QTime::currentTime().toString().toStdString() << " SERVER: STATE: SERVER STARTED" << std::endl;
    }
//    database_ = QSqlDatabase::addDatabase("QPSQL");
}

Server::~Server() {
    senderBroadcast_->setRun(false);
    QThread::msleep(200);
    if (database_.isOpen()) {
        database_.close();
        std::cout << QTime::currentTime().toString().toStdString() << " DATABASE: OPERATION: CLOSE" << std::endl;
    }
    shutDownServer();
}

void Server::newUser() {
    if (isListen_) {
        while (server_->hasPendingConnections()) {
            std::cout << QTime::currentTime().toString().toStdString() << " SERVER: OPERATION: NEW USER" << std::endl;
            auto _client = QSharedPointer<QTcpSocket>(server_->nextPendingConnection());
            auto _id = _client->socketDescriptor();
            clients_[_id] = _client;
            users_[_client] = QSharedPointer<User>(new User());
            connect(_client.get(), SIGNAL(readyRead()), this, SLOT(readData()));
            connect(_client.get(), SIGNAL(disconnected()), this, SLOT(disconnectUser()));
        }
    }
}

void Server::shutDownServer() {
    if (!isListen_) {
        return;
    }
            foreach(auto item, clients_.keys()) {
            clients_[item]->close();
            clients_.remove(item);
            std::cout << QTime::currentTime().toString().toStdString() << " SERVER: OPERATION: REMOVE " << item
                      << " CLIENT"
                      << std::endl;
        }
    server_->close();
    std::cout << QTime::currentTime().toString().toStdString() << "SERVER: STATE: CLOSED" << std::endl;
    isListen_ = false;
}

void Server::readData() {
            foreach(auto _client, clients_.values()) {
            if (_client->bytesAvailable()) {
                QByteArray _readBuff = _client->readAll();
                auto _user = users_[_client];
                _readBuff = _user->decode(_readBuff);
                std::cout << _readBuff.toStdString() << std::endl;
                QJsonDocument doc;
                QJsonParseError parser;
                doc = QJsonDocument::fromJson(_readBuff, &parser);
                if (parser.error != QJsonParseError::NoError) {
                    std::cout << QTime::currentTime().toString().toStdString()
                              << " SERVER: ERROR: PARSE BYTES ARRAY TO JSON\n"
                              << parser.errorString().toStdString() << std::endl;
                }
                QJsonDocument ans;
                auto obj = doc.object();
                auto command = obj["Command"].toString();
                if (command == COMMAND_SEND_KEY) {
                    CommandReplyKey com(_user, obj["Key"].toInteger());
                    ans = com.exec();
                } else if (command == COMMAND_SEND_MESSAGE) {

                } else {
                    std::cout << QTime::currentTime().toString().toStdString()
                              << " SERVER: ERROR: UNKNOWN COMMAND FROM USER " << _client->socketDescriptor()
                              << std::endl;
                }
                QByteArray res = ans.toJson(QJsonDocument::Indented);
                std::cout << QTime::currentTime().toString().toStdString() << " SERVER: REPLY\n" << res.toStdString()
                          << std::endl;
                _client->write(_user->encode(res));
                _user->setHasSessionKey(true);
            }
        }
}

void Server::disconnectUser() {
    auto _client = QSharedPointer<QTcpSocket>(qobject_cast<QTcpSocket *>(sender()));
    std::cout << "OK" << std::endl;
    long long _id = -1;
    if (_client) {
        for (auto it = clients_.begin(); it != clients_.end(); ++it) {
            if (it.value() == _client) {
                _id = it.key();
                clients_.erase(it);
                break;
            }
        }
    }
    std::cout << QTime::currentTime().toString().toStdString() << " SERVER: OPERATION DISCONNECT " << _id << std::endl;
}
