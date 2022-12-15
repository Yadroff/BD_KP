#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QThread>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QMovie>

#include "Connector.h"
#include "SenderReceiver.h"
#include "CryptographAlice.h"
#include "CommonHeader.h"
#include "Channel.h"

namespace Ui {
    class Window;
}

class Window : public QMainWindow {
Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);

    ~Window() override;

private:
    Ui::Window *ui;
    QSharedPointer<QThread> thread_;
    QSharedPointer<CryptographAlice> crypto_;
    QSharedPointer<Connector> connector_;
    QSharedPointer<SenderReceiver> sender_;
    QString userName_;
    QSet<QString> channels_;
    QMovie loadGif_;
    bool isNewDialog_;

    static bool checkUsername(const QString &str);

    static bool checkLettersOnly(const QString &str);

    void initUI();

    void hideInput();

    void showInput();

    QString user_style = "background: #73739b;"
                         "border-radius: 5px;"
                         "color: black;"
                         "font:10pt \"Open Sans\";";

    QString contact_style = "background: white;"
                            "border-radius: 5px;"
                            "color: black;"
                            "font:10pt \"Open Sans\";";

    QString user_bubble_style = "background: white;"
                                "border-radius: 20;"
                                "color:  #73739b;"
                                "font: 20pt \"Open Sans\";";

    QString contact_bubble_style = "background: #73739b;"
                                   "border-radius: 20;"
                                   "color: white;"
                                   "font: 20pt \"Open Sans\";";

    QString currentOpenChannel_ = "";

    QString lastSender_;

    QDateTime lastMessage_;

    void eraseLayout(QLayout *);

private slots:

    void setLogin(const QString &);

    void connectToServer(const QString &);

    void on_login_button_clicked();

    void on_login_signup_clicked();

    void on_signup_login_clicked();

    void on_signup_button_clicked();

    void on_channels_itemClicked(QListWidgetItem *item);

    void parseServerMessage(const QJsonDocument &doc);

    void newMessage(const QString &sender, const QString &text, const QDateTime &date);

    void on_add_button_clicked();

    void on_contact_button_clicked();

    void on_add_line_returnPressed();

    void search_user_clicked();

    void on_send_button_clicked();

signals:

    void needSend(QString);
};

#endif // WINDOW_H
