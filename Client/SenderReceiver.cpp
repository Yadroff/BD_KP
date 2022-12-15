#include <iostream>
#include "SenderReceiver.h"

#include <QThread>

SenderReceiver::SenderReceiver(QSharedPointer<CryptographAlice> &crypto, const QString &ip, const long long &keyToBob)
        : socket_(new QTcpSocket(this)), crypto_(crypto) {
    socket_->connectToHost(ip, TCP_SERVER_PORT);
    connect(socket_.get(), SIGNAL(readyRead()), this, SLOT(readData()));
    // создаем JSON с ключом
    QJsonDocument doc;
    QJsonObject obj;
    obj["Command"] = COMMAND_SEND_KEY;
    obj["Key"] = keyToBob;
    doc.setObject(obj);
    auto arr = doc.toJson(QJsonDocument::Indented);
    std::cout << QTime::currentTime().toString().toStdString() << " SEND: " << arr.toStdString() << std::endl;
    socket_->write(arr);
//    socket_->waitForReadyRead();
    connect(socket_.get(), SIGNAL(disconnected()), this, SLOT(sendDisconnect()));
}

SenderReceiver::~SenderReceiver() {
    std::cout << QTime::currentTime().toString().toStdString() << " SENDER_RECEIVER: DELETE" << std::endl;
    if (socket_->isOpen()) {
        socket_->close();
    }
}

void SenderReceiver::send(const QString &str) {
    QJsonDocument doc = parseClientSend(str);
    socket_->write(crypto_->encode(doc.toJson(QJsonDocument::Indented)));
}

void SenderReceiver::readData() {
    auto arr = socket_->readAll();
    arr = crypto_->decode(arr);
    QJsonParseError parser;
    auto json = QJsonDocument::fromJson(arr, &parser);
    if (parser.error != QJsonParseError::NoError) {
        std::cout << QTime::currentTime().toString().toStdString() << " SENDER_RECEIVER: ERROR: CAN_NOT_PARSE_TO_JSON\n"
                  << parser.errorString().toStdString() << std::endl;
        std::cout << arr.toStdString() << std::endl;
        return;
    }
    std::cout << QTime::currentTime().toString().toStdString() << " SENDER_RECEIVER: MESSAGE_FROM SERVER\n"
              << QString(json.toJson(QJsonDocument::Indented)).toStdString() << std::endl;
    parseServerAnswer(json);
}

void SenderReceiver::parseServerAnswer(const QJsonDocument &doc) {
    QJsonObject obj = doc.object();
    QString command = obj["Command"].toString();
    if (command == COMMAND_SEND_KEY) {
        crypto_->setKeyFromBob(obj["Key"].toInteger());
    } else {
        emit serverMessage(doc);
    }
}

void SenderReceiver::sendDisconnect() {
    emit serverDisconnect();
}

QJsonDocument SenderReceiver::parseClientSend(const QString &str) {
    QJsonDocument doc;
    QJsonObject obj;
    auto arguments = str.split(SEPARATOR);
    QString command = arguments[0];
    obj["Command"] = command;
    doc.setObject(obj);
    if (command == COMMAND_LOGIN) {
        if (arguments.size() != COMMAND_LOGIN_SIZE) {
            std::cout << QTime::currentTime().toString().toStdString() << " ERROR: WRONG ARGUMENTS TO COMMAND "
                      << command.toStdString() << std::endl;
            return doc;
        }
        obj["Login"] = arguments[1];
        obj["Password"] = arguments[2];
        doc.setObject(obj);
        return doc;
    } else if (command == COMMAND_REGIST) {
        if (arguments.size() != COMMAND_REGIST_SIZE) {
            std::cout << QTime::currentTime().toString().toStdString() << " ERROR: WRONG ARGUMENTS TO COMMAND "
                      << command.toStdString() << std::endl;
            return doc;
        }
        obj["Login"] = arguments[1];
        obj["Name"] = arguments[2];
        obj["Surname"] = arguments[3];
        obj["Password"] = arguments[4];
        doc.setObject(obj);
        return doc;
    } else if (command == COMMAND_SEND_MESSAGE) {
        if (arguments.size() != COMMAND_SEND_MESSAGE_SIZE) {
            std::cout << QTime::currentTime().toString().toStdString() << " ERROR: WRONG ARGUMENTS TO COMMAND "
                      << command.toStdString() << std::endl;
            return doc;
        }
        obj["Channel"] = arguments[1];
        obj["Message"] = arguments[2];
        obj["Date"] = arguments[3];
        doc.setObject(obj);
        return doc;
    } else if (command == COMMAND_READ_MESSAGES) {
        if (arguments.size() != COMMAND_GET_MESSAGES_SIZE) {
            std::cout << QTime::currentTime().toString().toStdString() << " ERROR: WRONG ARGUMENTS TO COMMAND "
                      << command.toStdString() << std::endl;
            return doc;
        }
        obj["Channel"] = arguments[1];
        obj["Date"] = arguments[2];
        doc.setObject(obj);
        return doc;
    } else if (command == COMMAND_SEARCH) {
        if (arguments.size() != COMMAND_SEARCH_SIZE) {
            std::cout << QTime::currentTime().toString().toStdString() << " ERROR: WRONG ARGUMENTS TO COMMAND "
                      << command.toStdString() << std::endl;
            return doc;
        }
        obj["Name"] = arguments[1];
        doc.setObject(obj);
        return doc;
    } else {
        std::cout << QTime::currentTime().toString().toStdString() << " ERROR: WRONG COMMAND: "
                  << command.toStdString() << std::endl;
        return doc;
    }
}
