#include "window.h"
#include "ui_window.h"
#include "MessageLabel.h"
#include "MessageMenu.h"

#include <qtestsupport_core.h>
#include <iostream>

#include <QCryptographicHash>
#include <QScrollBar>
#include <QFile>
#include <QDir>

QString userName;

Window::Window(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Window), loadGif_(":Resources/loading.gif") {
    ui->setupUi(this);
    initUI();
    thread_ = QSharedPointer<QThread>(new QThread(this));
    crypto_ = QSharedPointer<CryptographAlice>(new CryptographAlice);
    connector_ = QSharedPointer<Connector>(new Connector);
    connector_->moveToThread(thread_.get());
    connect(connector_.get(), SIGNAL(finished(QString)), this, SLOT(connectToServer(QString)));
    connect(thread_.get(), SIGNAL(started()), connector_.get(), SLOT(run()));
    thread_->start();
    ui->channels->setSortingEnabled(true);
    isNewDialog_ = false;
}

Window::~Window() {
    connector_->setRun(false);
    if (thread_->isRunning()) {
        thread_->quit();
        thread_->wait();
    }
    connector_.clear();
    thread_.clear();
    crypto_.clear();
    delete ui;
    QDir dir(PATH_TO_CACHE);
    dir.setNameFilters(QStringList() << QString("*") + CACHE_SUFFIX);
    dir.setFilter(QDir::Files);
            foreach(QString dirFile, dir.entryList()) {
            dir.remove(dirFile);
        }
}

void Window::connectToServer(const QString &servAddress) {
    std::cout << servAddress.toStdString() << std::endl;
    connector_->setRun(false);
    if (servAddress == CONNECT_ERROR_MESSAGE) {
        std::cout << QTime::currentTime().toString().toStdString() << " ERROR: CAN NOT CONNECT TO SERVER" << std::endl;
        QMessageBox::critical(this, "ERROR", "Can not connect to server");
        if (ui->stackedWidget->currentWidget() == ui->login) {
            ui->login_status_label->setText("CONNECTION FAIL");
        } else {
            ui->signup_status_label->setText("CONNECTION FAIL");
        }
        thread_->quit();
        return;
    }
    if (ui->stackedWidget->currentWidget() == ui->login) {
        ui->login_status_label->setText("CONNECTED TO SERVER");
    } else {
        ui->signup_status_label->setText("CONNECTED TO SERVER");
    }
    auto _stringList = servAddress.split(SEPARATOR);
    QString ip = _stringList.last();
    auto keyToBob = crypto_->getKeyToBob();
    sender_ = QSharedPointer<SenderReceiver>(new SenderReceiver(crypto_, ip, keyToBob));
    sender_->moveToThread(thread_.get());
    connect(sender_.get(), SIGNAL(serverDisconnect()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(needSend(QString)), sender_.get(), SLOT(send(QString)));
    connect(sender_.get(), SIGNAL(serverMessage(QJsonDocument)), this, SLOT(parseServerMessage(QJsonDocument)));
}

void Window::setLogin(const QString &login) {
    userName = login;
}

void Window::on_login_button_clicked() {
    if (ui->login_username->text().isEmpty() or ui->login_password->text().isEmpty()) {
        ui->login_error->setText("You have empty fields");
        return;
    }
    if (!ui->login_username->text().isSimpleText() or !ui->login_password->text().isSimpleText()) {
        ui->login_status_label->setText("Username and Password should be simple text!");
        return;
    }
    auto password = QString(QCryptographicHash::hash(ui->login_password->text().append(SALT).toUtf8(),
                                                     QCryptographicHash::Sha512).toHex());
    userName = ui->login_username->text();
    emit needSend(COMMAND_LOGIN + SEPARATOR + ui->login_username->text() + SEPARATOR + password);
}


void Window::on_login_signup_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->signup);
    ui->signup_status_label->setText(ui->login_status_label->text());
}


void Window::on_signup_login_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->login);
    ui->login_status_label->setText(ui->signup_status_label->text());
}


void Window::on_signup_button_clicked() {
    if (ui->signup_username->text().isEmpty()) {
        ui->signup_error->setText("Username is empty");
        return;
    }
    if (ui->signup_name->text().isEmpty()) {
        ui->signup_error->setText("Name is empty");
        return;
    }
    if (ui->signup_surname->text().isEmpty()) {
        ui->signup_error->setText("Surname is empty");
        return;
    }
    if (ui->signup_password->text().isEmpty()) {
        ui->signup_error->setText("Password is empty");
        return;
    }
    if (ui->signup_confirm->text().isEmpty()) {
        ui->signup_error->setText("Confirm password is empty");
        return;
    }
    if (!checkLettersOnly(ui->signup_name->text())) {
        ui->signup_error->setText("Invalid format of name");
        return;
    }
    if (!checkLettersOnly(ui->signup_surname->text())) {
        ui->signup_error->setText("Invalid format of surname");
        return;
    }
    if (!checkUsername(ui->signup_username->text())) {
        ui->signup_error->setText("Invalid format of username");
        return;
    }
    if (!ui->signup_password->text().isSimpleText()) {
        ui->signup_error->setText("Invalid format of password");
        return;
    }
    if (!ui->signup_confirm->text().isSimpleText()) {
        ui->signup_error->setText("Invalid format of surname");
        return;
    }
    if (ui->signup_password->text() != ui->signup_confirm->text()) {
        ui->signup_error->setText("Password != Confirm password");
        return;
    }
    auto password = QString(QCryptographicHash::hash(ui->signup_password->text().append(SALT).toUtf8(),
                                                     QCryptographicHash::Sha512).toHex());
    QString query = COMMAND_REGIST + SEPARATOR + ui->signup_username->text() + SEPARATOR +
                    ui->signup_name->text() + SEPARATOR + ui->signup_surname->text() + SEPARATOR + password;
    std::cout << query.toStdString() << std::endl;
    userName = ui->signup_username->text();
    emit needSend(query);
}

bool Window::checkLettersOnly(const QString &str) {
    for (int i = 0; i < str.size(); ++i) {
        QChar c = str[i];
        if (!(c.isLetter() or (str[i] == ' ' or c == '-') and i != 0)) {
            return false;
        }
    }
    return true;
}

bool Window::checkUsername(const QString &str) {
    for (int i = 0; i < str.size(); ++i) {
        QChar c = str[i];
        if (!(c.isLetterOrNumber() or (str[i] == ',' or c == '-' or c == '.') and i != 0)) {
            return false;
        }
    }
    return true;
}

void Window::parseServerMessage(const QJsonDocument &doc) {
    QJsonObject obj = doc.object();
    QString command = obj["Command"].toString();
    if (command == COMMAND_REGIST) {
        QString res = obj["Result"].toString();
        if (res != "SUCCESS") {
            ui->stackedWidget->setCurrentWidget(ui->signup);
            ui->signup_status_label->setText(res);
            return;
        }
        ui->stackedWidget->setCurrentWidget(ui->main);
        ui->username_label->setText(userName);
    } else if (command.contains(COMMAND_LOGIN)) {
        QString res = obj["Result"].toString();
        if (res != "SUCCESS") {
            ui->stackedWidget->setCurrentWidget(ui->login);
            ui->signup_status_label->setText(res);
            return;
        }
        loginSuccess(obj);
    } else if (command == COMMAND_READ_MESSAGES) {
        QString res = obj["Result"].toString();
        if (res != "SUCCESS") {
            ui->status_text_label->setText(res);
            ui->status_label->movie()->stop();
            return;
        }
        readMessagesSuccess(obj);
    } else if (command == COMMAND_SEARCH) {
        QString res = obj["Result"].toString();
        if (res != "SUCCESS") {
            ui->status_text_label->setText(res);
            ui->status_label->movie()->stop();
            return;
        }
        searchSuccess(obj);
    } else if (command == COMMAND_NOTIFY_NEW_MESSAGE) {
        QString channel = obj["Channel"].toString();
        if (!channels_[channel]->hasMessages()) {
            QStringList list = channel.split(DIALOG_SEPARATOR);
            list.removeOne(userName);
            QString channelName = list[0];
            QMessageBox::information(this, "New message", QString("You have new message from %1").arg(channelName));
            return;
        }
        QDateTime date = QDateTime::fromString(obj["Date"].toString(), DATE_FORMAT);
        date.setTimeSpec(Qt::UTC);
        date = date.toLocalTime();
        QString content = obj["Message"].toString();
        QString sender = obj["Sender"].toString();
        QDateTime edit;
        if (obj.contains("Edit")) {
            edit = QDateTime::fromString(obj["Edit"].toString(), DATE_FORMAT);
            edit.setTimeSpec(Qt::UTC);
            edit = edit.toLocalTime();
        }
        int id = obj["MessageID"].toInt();
        auto mess = new Message(id, channel, sender, content, date);
        channels_[channel]->addMessage(QSharedPointer<Message>(mess));
        QStringList list = channel.split(DIALOG_SEPARATOR);
        list.removeOne(userName);
        QString channelName = list[0];
        if (currentChannel_ == channel) {
            addMessage(id, sender, content, date, edit);
//            ui->scrollArea
        } else {
            QMessageBox::information(this, "New message", QString("You have new message from %1").arg(channelName));
        }
    } else if (command == COMMAND_CREATE_DIALOG) {
        if (obj["Result"] != "SUCCESS") {
            QMessageBox::critical(this, "Create dialog", "ERROR");
            return;
        }
        QString channelName = obj["Channel"].toString();
        QStringList _list = channelName.split(DIALOG_SEPARATOR);
        _list.removeOne(userName);
        QString secondUser = _list.first();
        QDateTime _lastMessage = QDateTime::currentDateTimeUtc();
        channels_.insert(channelName, QSharedPointer<Channel>(new Channel(channelName, _lastMessage, ROOTS_OWNER)));
        Channel::channels[secondUser] = QPair<QDateTime, int>(_lastMessage, ROOTS_OWNER);
        auto *channelItem = new ChannelItem(channelName, secondUser);
        ui->channels->addItem(channelItem);
    }
}

void Window::initUI() {
    ui->stackedWidget->setCurrentWidget(ui->login);
    ui->login_status_label->setText("CONNECT TO SERVER");
    ui->send_button->hide();
    ui->input->hide();
    ui->input_frame_box->hide();
    ui->input_frame->hide();
    ui->status_text_label->hide();
    ui->scrollArea->setWidget(ui->scrollAreaWidgetContents_4);
    ui->scrollAreaWidgetContents_4->show();
    ui->join_channel_button->hide();
    ui->add_layout->setAlignment(Qt::AlignTop);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    hideDialog();
}


void Window::on_add_button_clicked() {
//    dialog_->clearWidget()
    ui->add_line->clear();
    hideDialog();
    ui->contact_widget->setCurrentWidget(ui->add_page);
    isNewDialog_ = false;
}


void Window::on_contact_button_clicked() {
//    hideInput();
//    ui->contact_label_2->clear();
    hideDialog();
    eraseLayout(ui->add_layout);
    ui->contact_widget->setCurrentWidget(ui->view_page);
    isNewDialog_ = false;
}


void Window::on_add_line_returnPressed() {
    if (ui->add_line->text().isEmpty()) {
        return;
    }
    QString query = COMMAND_SEARCH + SEPARATOR + ui->add_line->text();
    std::cout << QTime::currentTime().toString().toStdString() << query.toStdString() << std::endl;
    emit needSend(query);
}

void Window::search_user_clicked() {
    hideDialog();
    showDialog();
    QString senderName = ((QPushButton *) sender())->text();
//    std::cout << senderName.toStdString() << std::endl;
    ui->contact_label_2->setText(senderName);
    currentChannel_ = qMin(userName, senderName) + DIALOG_SEPARATOR + qMax(userName, senderName);
    QString channelName = qMin(senderName, userName) + DIALOG_SEPARATOR + qMax(senderName, userName);
    if (Channel::channels.contains(senderName)) {
        int roots = Channel::channels[senderName].second;
        if (roots == ROOTS_BANNED) {
            QMessageBox::information(this, QString("Dialog with %1").arg(senderName),
                                     "You are banned at this channel!");
            return;
        }
        if (roots < ROOTS_ADMINISTRATOR) {
            hideInput();
        } else {
            ui->input_frame->show();
        }
        changeChannel(channelName);
        return;
    }
    isNewDialog_ = true;
    showInput();
}

void Window::hideInput() {
    ui->input_frame->hide();
    ui->input->hide();
    ui->send_button->hide();
}

void Window::showInput() {
    ui->input_frame->show();
    ui->input_frame_box->show();
    ui->input->show();
    ui->send_button->show();
}

void Window::on_send_button_clicked() {
    if (ui->input->toPlainText().isEmpty()) {
        return;
    }
    QString query;
    if (isNewDialog_) {
        query = COMMAND_CREATE_DIALOG + SEPARATOR + ui->contact_label_2->text();
        emit needSend(query);
    }
    QTest::qWait(1000);
    QString channelName = ui->contact_label_2->text();
    if (!channels_.contains(channelName)) {
        channelName = qMin(userName, channelName) + DIALOG_SEPARATOR + qMax(userName, channelName);
    }
    query = COMMAND_SEND_MESSAGE + SEPARATOR + channelName + SEPARATOR + ui->input->toPlainText()
            + SEPARATOR + QDateTime::currentDateTimeUtc().toString(DATE_FORMAT);
    std::cout << QTime::currentTime().toString().toStdString() << " " << query.toStdString() << std::endl;
    emit needSend(query);
    ui->input->clear();
}

void Window::loginSuccess(const QJsonObject &obj) {
    ui->stackedWidget->setCurrentWidget(ui->main);
    ui->username_label->setText(userName);
    QJsonArray channels = obj["Channels"].toArray();
    QJsonObject channel;
    channels_.reserve(channels.size());
    for (auto &&item: channels) {
        channel = item.toObject();
        QString channelName = channel["Name"].toString();
        QStringList channelNameList = channelName.split(DIALOG_SEPARATOR);
        if (channelNameList.size() == 2) {
            // убираем лишний никнейм в диалоге
            channelNameList.removeOne(userName);
        }
        auto _lastMessage = QDateTime::fromString(channel["Last Message"].toString(), DATE_FORMAT);
        int _rights = channel["Rights"].toInt();
        channels_.insert(channelName, QSharedPointer<Channel>(new Channel(channelName, _lastMessage, _rights)));
        Channel::channels[channelNameList[0]] = QPair<QDateTime, int>(_lastMessage, _rights);
        auto *channelItem = new ChannelItem(channelName, channelNameList[0]);
        ui->channels->addItem(channelItem);
    }
    ui->channels->sortItems();
}

void Window::readMessagesSuccess(const QJsonObject &obj) {
    QJsonArray messages = obj["Messages"].toArray();
    QString sender;
    QString text;
    QDateTime time;
    QDateTime edit;
    QString channel = obj["Channel"].toString();
    int id;
    for (auto &&item: messages) {
        auto message = item.toObject();
        id = message["ID"].toInt();
        sender = message["Sender"].toString();
        text = message["Text"].toString();
        time = QDateTime::fromString(message["Date"].toString(), DATE_FORMAT).toLocalTime();
        time.setTimeSpec(Qt::UTC);
        time = time.toLocalTime();
        if (message.contains("Edit")) {
            edit = QDateTime::fromString(message["Edit"].toString(), DATE_FORMAT).toLocalTime();
            edit.setTimeSpec(Qt::UTC);
            edit = edit.toLocalTime();
        }
        channels_[channel]->addMessage(QSharedPointer<Message>(new Message(id, channel, sender, text, time, edit)));
    }
    ui->scrollArea->update();
    ui->status_text_label->clear();
    ui->status_label->movie()->stop();
    displayMessages();
}


void Window::on_channels_itemClicked(QListWidgetItem *item) {
    auto channelItem = (ChannelItem *) item;
    QString channelName = channelItem->getChannelName();
    if (currentChannel_ == channelName) {
        return;
    }
    eraseLayout(ui->output);
    int roots = channels_[channelName]->getRoots();
    currentChannel_ = channelName;
    if (roots == ROOTS_BANNED) {
        QMessageBox::information(this, QString("ChannelItem %1").arg(channelName), "You are banned at this channel!");
        return;
    }
    if (roots < ROOTS_ADMINISTRATOR) {
        hideInput();
    } else {
        showInput();
    }
    if (channels_[channelName]->hasMessages()) {
        changeChannel(channelName);
        return;
    }
    QString query = COMMAND_READ_MESSAGES + SEPARATOR + channelName + SEPARATOR +
                    QDateTime::currentDateTimeUtc().toString(DATE_FORMAT);
    std::cout << query.toStdString() << std::endl;
    ui->contact_label_2->setText(channelItem->text());
    ui->status_text_label->setText("Getting messages\n from server");
    ui->status_label->setMovie(&loadGif_);
    ui->status_text_label->show();
    ui->status_label->show();
    loadGif_.start();
    emit needSend(query);
}

void Window::searchSuccess(const QJsonObject &obj) {
    QJsonArray users = obj["Users"].toArray();
    if (!users.isEmpty()) {
        auto *button = new QLabel("Users");
//            button->setStyleSheet(user_bubble_style);
        auto *layout = new QHBoxLayout;
        layout->setAlignment(Qt::AlignHCenter);
        layout->addWidget(button);
        ui->add_layout->addLayout(layout);
        for (auto &&item: users) {
            QString name = item.toString();
            if (name == userName) {
                continue;
            }
            auto *_button = new QPushButton(name);
            _button->setStyleSheet("border: none; text-align: left;");
            connect(_button, SIGNAL(clicked()), this, SLOT(search_user_clicked()));
            auto *_layout = new QHBoxLayout;
            _layout->setAlignment(Qt::AlignLeft);
            _layout->addWidget(_button);
            ui->add_layout->addLayout(_layout);
        }
    }
    QJsonArray channels = obj["Channels"].toArray();
    if (!channels.isEmpty()) {
        auto *button = new QLabel("Channels");
//            button->setStyleSheet(contact_bubble_style);
        auto *layout = new QHBoxLayout;
        layout->setAlignment(Qt::AlignHCenter);
        layout->addWidget(button);
        ui->add_layout->addLayout(layout);
        for (auto &&item: channels) {
            QStringList channelNameList = item.toString().split(DIALOG_SEPARATOR);
            if (channelNameList.size() == 2) {
                // убираем лишний никнейм в диалоге
                channelNameList.removeOne(userName);
            }
            QString name = channelNameList[0];
            if (users.contains(name)) {
                continue;
            }
            auto *_button = new QPushButton(name);
            _button->setFlat(true);
            _button->setStyleSheet("border: none; text-align: left;");
            connect(_button, SIGNAL(clicked()), this, SLOT(search_channel_clicked()));
            auto *_layout = new QHBoxLayout;
            _layout->setAlignment(Qt::AlignLeft);
            _layout->addWidget(_button);
            ui->add_layout->addLayout(_layout);
        }
    }
}

void Window::search_channel_clicked() {

}

void Window::eraseLayout(QLayout *mainLayout) {
    while (mainLayout->count() > 0) {
        QLayoutItem *item = mainLayout->takeAt(0);
        QWidget *widget = item->widget();
        if (widget) {
            delete widget;
        } else {
            QLayout *layout = item->layout();
            if (layout) {
                eraseLayout(layout);
            } else {
                QSpacerItem *si = item->spacerItem();
                if (si) {
                    delete si;
                }
            }
        }
        delete item;
    }
}

void Window::displayMessages() {
    if (currentChannel_.isEmpty()) {
        return;
    }
    setMessages(channels_[currentChannel_]->getAllMessages());
}

void Window::hideDialog() {
    ui->join_channel_button->hide();
    hideInput();
    eraseLayout(ui->output);
    ui->contact_label_2->clear();
    ui->contact_status_2->clear();
    lastSender_ = "";
    lastSendMessage_ = QDateTime();
    currentChannel_ = "";
}

void Window::showDialog() {
    ui->input_frame->show();
    ui->scrollArea->show();
    ui->scrollAreaWidgetContents_4->show();
    ui->contact_frame->show();
    showInput();
}

void Window::setMessages(const QJsonDocument &messagesDoc) {
    auto messages = messagesDoc.array();
    for (auto &&message: messages) {
        QJsonObject obj = message.toObject();
        QDateTime sendDate = QDateTime::fromString(obj["Send"].toString(), DATE_FORMAT);
        QString sender = obj["Sender"].toString();
        QString content = obj["Content"].toString();
        QDateTime editDate = QDateTime::fromString(obj["Edit"].toString(), DATE_FORMAT);
        int messageID = obj["ID"].toInt();
        addMessage(messageID, sender, content, sendDate, editDate);
    }
}

void Window::changeChannel(const QString &channelName) {
    showDialog();
    lastSender_ = "";
    lastSendMessage_ = QDateTime();
    currentChannel_ = channelName;
    ui->contact_label_2->setText(channelName.split(DIALOG_SEPARATOR).first());
    displayMessages();
}

void Window::addMessage(int messageID, const QString &sender, const QString &content, const QDateTime &sendDate,
                        const QDateTime &editDate) {
    if (sendDate.date() != lastSendMessage_.date()) {
        auto *dateLayout = new QHBoxLayout;
        dateLayout->setAlignment(Qt::AlignHCenter);
        auto *dateLabel = new QLabel(sendDate.date().toString("d, MMMM"));
        dateLayout->addWidget(dateLabel);
        ui->output->addLayout(dateLayout);
        lastSendMessage_ = sendDate;
    }
    bool clientIsSender = sender == userName;
    if (sender != lastSender_) {
        auto *senderLayout = new QHBoxLayout;
        senderLayout->setAlignment(Qt::AlignHCenter);
        auto *senderLabel = new QLabel(sender);
        senderLabel->setStyleSheet(clientIsSender ? user_bubble_style : contact_bubble_style);
        senderLayout->addWidget(senderLabel);
        ui->output->addLayout(senderLayout);
        lastSender_ = sender;
    }
    auto *messageLayout = new QHBoxLayout();
    messageLayout->setAlignment((clientIsSender) ? Qt::AlignRight : Qt::AlignLeft);
    QString textLabel = QString::fromLatin1(
            R"(<font face=\"Open Sans\";> %1 </font>
                <h1 align =\"right\"><font size=\"8\" face=\"Open Sans\" style=italic;> %2 </font></h1>)").arg(
            content, sendDate.time().toString(TIME_FORMAT));
    if (editDate.isValid()) {
        textLabel.append(QString("<font:8pt \"Open Sans\" italic;> %1 </font>").arg(
                "Edit " + editDate.toString(DATE_FORMAT)));
    }
    auto *messageLabel = new MessageLabel(messageID, clientIsSender);
    messageLabel->setText(textLabel);
    messageLabel->setStyleSheet((clientIsSender) ? user_style : contact_style);
    messageLayout->addWidget(messageLabel);
    auto menu = new MessageMenu;
//        connect(messageLabel, SIGNAL(clicked()), this, SLOT(message_click()));
    connect(messageLabel, SIGNAL(customContextMenuRequested(QPoint)), menu, SLOT(showMenu(QPoint)));
    ui->output->addLayout(messageLayout);
}
