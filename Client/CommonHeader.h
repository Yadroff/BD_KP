#ifndef COMMONHEADER_H
#define COMMONHEADER_H

#include <QObject>
#include <QUdpSocket>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSharedPointer>
#include <QLayout>


const QString SEPARATOR = QString(QChar(7));
const QString DIALOG_SEPARATOR = QString(QChar(8));
const QString SALT = QString(QChar(15));
const quint16 UDP_SERVER_PORT = 44444;
const quint16 TCP_SERVER_PORT = 7777;

extern QString userName;


#define COMMAND_SEND_KEY "SEND KEY"
#define COMMAND_SEND_MESSAGE "SEND MESSAGE"
#define COMMAND_LOGIN "LOGIN"
#define COMMAND_REGIST "REGIST"
#define COMMAND_READ_MESSAGES "READ MESSAGES"
#define CONNECT_ERROR_MESSAGE "CAN_NOT_CONNECT_TO_SERVER"
#define COMMAND_SEARCH "SEARCH"
#define COMMAND_CREATE_DIALOG "CREATE DIALOG"
#define COMMAND_NOTIFY_NEW_MESSAGE "NOTIFY NEW MESSAGE"


#define DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#define TIME_FORMAT "hh:mm"
#define PATH_TO_CACHE "./Cache/"
#define CACHE_SUFFIX ".cache"

#define ROOTS_OWNER 4
#define ROOTS_ADMINISTRATOR 3
#define ROOTS_MODERATOR 2
#define ROOTS_USER 1
#define ROOTS_BANNED 0

#endif // COMMONHEADER_H
