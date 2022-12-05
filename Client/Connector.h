#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QUdpSocket>
#include <QTime>

#include "CommonHeader.h"

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(QObject *parent = nullptr);
    ~Connector();
    void setRun(bool run);
private:
    QString serverAddress_;
    unsigned short serverPort_;
    QUdpSocket *socket_;
    bool runFlag_;
    QTime start_;
    static const int LOGIN_TIME = 10000;
private slots:
    void readData();
public slots:
    void run();
signals:
    void finished(const QString &);
};

#endif // CONNECTOR_H
