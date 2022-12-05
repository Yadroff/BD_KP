#ifndef COMMONHEADER_H
#define COMMONHEADER_H

#include <QString>

const QString SEPARATOR = QString(QChar(7));
const quint16 UDP_SERVER_PORT = 44444;
const quint16 TCP_SERVER_PORT = 7777;

#define COMMAND_SEND_KEY "SEND KEY"
#define COMMAND_SEND_MESSAGE "SEND MESSAGE"
#define COMMAND_LOGIN "LOGIN"

#endif // COMMONHEADER_H
