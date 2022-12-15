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
    database_ = QSqlDatabase::addDatabase("QSQLITE");
    database_.setDatabaseName(PATH_TO_DATABASE);
    if (database_.open()) {
        std::cout << QTime::currentTime().toString().toStdString() << " DATABASE: OPEN" << std::endl;
        CommandInitDB com;
        if (com.exec().object()["Result"] != "SUCCESS") {
            std::cout << QTime::currentTime().toString().toStdString() << " DATABASE: ERROR: CAN NOT CREATE TEMP TABLE"
                      << std::endl;
        }
    } else {
        std::cout << QTime::currentTime().toString().toStdString() << " DATABASE: ERROR: CAN NOT OPEN" << std::endl;
    }
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
            auto _client = server_->nextPendingConnection();
            clients_[_client] = QSharedPointer<User>(new User(_client));
            connect(_client, SIGNAL(readyRead()), this, SLOT(readData()));
            connect(_client, SIGNAL(disconnected()), this, SLOT(disconnectUser()));
        }
    }
}

void Server::shutDownServer() {
    if (!isListen_) {
        return;
    }
    CommandExit com("");
            foreach(auto item, clients_.keys()) {
            com.setNick(clients_[item]->getUserName());
            com.exec();
            clients_.remove(item);
            std::cout << QTime::currentTime().toString().toStdString() << " SERVER: OPERATION: REMOVE "
                      << item->socketDescriptor() << " CLIENT" << std::endl;
            item->close();
            item->deleteLater();
        }
    server_->close();
    std::cout << QDateTime::currentDateTimeUtc().toLocalTime().toString().toStdString() << "SERVER: STATE: CLOSED"
              << std::endl;
    isListen_ = false;
}

void Server::readData() {
    auto _client = (QTcpSocket *) sender();
    if (_client->bytesAvailable()) {
        QByteArray _readBuff = _client->readAll();
        auto _user = clients_[_client];
        _readBuff = _user->decode(_readBuff);
        std::cout << QTime::currentTime().toString().toStdString() << " SERVER: GET MESSAGE\n"
                  << _readBuff.toStdString() << std::endl;
        QJsonDocument doc;
        QJsonParseError parser;
        doc = QJsonDocument::fromJson(_readBuff, &parser);
        if (parser.error != QJsonParseError::NoError) {
            std::cout << QTime::currentTime().toString().toStdString()
                      << " SERVER: ERROR: PARSE BYTES ARRAY TO JSON\n"
                      << parser.errorString().toStdString() << std::endl;
        }
        QJsonDocument ans;
        ans = parse(doc, _user);
        QByteArray res = ans.toJson(QJsonDocument::Indented);
        std::cout << QTime::currentTime().toString().toStdString() << " SERVER: REPLY\n" << res.toStdString()
                  << std::endl;
        _client->write(_user->encode(res));
        _user->setHasSessionKey(true);
    }
}

void Server::disconnectUser() {
    auto client = (QTcpSocket *) sender();
    if (client) {
        auto userName = clients_[client]->getUserName();
        if (!userName.isEmpty()) {
            CommandExit cmd(userName);
            users_.remove(userName);
            std::cout << cmd.exec().toJson(QJsonDocument::Indented).toStdString() << std::endl;
        }
        clients_.remove(client);
        client->deleteLater();
    }
    std::cout << QTime::currentTime().toString().toStdString() << " SERVER: OPERATION DISCONNECT" << std::endl;
}

QJsonDocument Server::parse(const QJsonDocument &doc, QSharedPointer<User> &user) {
    auto obj = doc.object();
    auto command = obj["Command"].toString();
    QJsonDocument res;
    if (command == COMMAND_SEND_KEY) {
        CommandReplyKey com(user, obj["Key"].toInteger());
        res = com.exec();
    } else if (command == COMMAND_LOGIN) {
        QString login = obj["Login"].toString();
        CommandLogin com(user, login, obj["Password"].toString());
        res = com.exec();
        if (res.object()["Result"].toString() == "SUCCESS") {
            user->setUserName(login);
            users_.insert(login, user);
            test(user);
        }
    } else if (command == COMMAND_REGIST) {
        CommandRegist com(user, obj["Login"].toString(), obj["Name"].toString(), obj["Surname"].toString(),
                          obj["Password"].toString());
        res = com.exec();
    } else if (command == COMMAND_SEND_MESSAGE) {
        QString channelName = obj["Channel"].toString();
        QString sender = user->getUserName();
        CommandSendMessage com(user, channelName, obj["Message"].toString(),
                               QDateTime::fromString(obj["Date"].toString(), DATE_FORMAT));

        res = com.exec();
        if (res.object()["Result"].toString() == "SUCCESS") {
            //TODO: добавить отправку уведомления о сообщении другим пользователям канала в сети
            CommandGetUsersInChannel getOnlineUsers(channelName);
            NotifyNewMessage notifyNewMessage(users_, getOnlineUsers.exec(), channelName, sender);
        }
    } else if (command == COMMAND_READ_MESSAGES) {
        CommandReadMessages com(obj["Channel"].toString(), QDateTime::fromString(obj["Date"].toString(), DATE_FORMAT));
        res = com.exec();
    } else if (command == COMMAND_SEARCH) {
        CommandSearch com(user, obj["Name"].toString());
        res = com.exec();
    } else {
        std::cout << QTime::currentTime().toString().toStdString()
                  << " SERVER: ERROR: UNKNOWN COMMAND FROM USER" << std::endl;
        obj["Result"] = "ERROR: UNKNOWN COMMAND";
        res.setObject(obj);
    }
    return res;
}

void Server::test(QSharedPointer<User> &user) {
    /*   if (user->getUserName() == "Yadroff") {
           CommandCreateDialog com(user, "Kuli4OK");
           std::cout << com.exec().toJson().toStdString() << std::endl;
       }*/
}
