#include <iostream>
#include "SenderReceiver.h"

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
}

SenderReceiver::~SenderReceiver() {
    std::cout << QTime::currentTime().toString().toStdString() << " SENDER_RECEIVER: DELETE" << std::endl;
    if (socket_->isOpen()) {
        socket_->close();
    }
}

void SenderReceiver::send(const QString &str) {
}

void SenderReceiver::readData() {
    auto arr = socket_->readAll();
    arr = crypto_->decode(arr);
    QJsonParseError parser;
    auto json = QJsonDocument::fromJson(arr, &parser);
    if (parser.error != QJsonParseError::NoError) {
        std::cout << QTime::currentTime().toString().toStdString() << " SENDER_RECEIVER: ERROR: CAN_NOT_PARSE_TO_JSON\n"
                  << parser.errorString().toStdString() << std::endl;
        return;
    }
    std::cout << QTime::currentTime().toString().toStdString() << "SENDER_RECEIVER: MESSAGE_FROM SERVER\n"
              << QString(json.toJson(QJsonDocument::Indented)).toStdString() << std::endl;
    parse(json);
}

void SenderReceiver::parse(const QJsonDocument &doc) {
    QJsonObject obj = doc.object();
    QString command = obj["Command"].toString();
    if (command == COMMAND_SEND_KEY) {
        crypto_->setKeyFromBob(obj["Key"].toInteger());
        socket_->write(crypto_->encode(QString("Hello, world!").toUtf8()));
    }
}
