#include "window.h"
#include "ui_window.h"

#include <qtestsupport_core.h>
#include <iostream>

#include <QCryptographicHash>
#include <QFile>


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
    userName_ = login;
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
    userName_ = ui->login_username->text();
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
    userName_ = ui->signup_username->text();
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
        ui->username_label->setText(userName_);
    } else if (command.contains(COMMAND_LOGIN)) {
        QString res = obj["Result"].toString();
        if (res != "SUCCESS") {
            ui->stackedWidget->setCurrentWidget(ui->login);
            ui->signup_status_label->setText(res);
            return;
        }
        ui->stackedWidget->setCurrentWidget(ui->main);
        ui->username_label->setText(userName_);
        QJsonArray channels = obj["Channels"].toArray();
        QJsonObject channel;
        channels_.reserve(channels.size());
        for (auto &&item: channels) {
            channel = item.toObject();
            QString channelName = channel["Name"].toString();
            channels_.insert(channelName);
            QStringList channelNameList = channelName.split(DIALOG_SEPARATOR);
            if (channelNameList.size() == 2) {
                // убираем лишний никнейм в диалоге
                channelNameList.removeOne(userName_);
            }
            auto _lastMessage = QDateTime::fromString(channel["Last Message"].toString(), DATE_FORMAT);
            int _rights = channel["Rights"].toInt();
            std::cout << _lastMessage.toString(DATE_FORMAT).toStdString() << std::endl;
            Channel::channels[channelNameList[0]] = QPair<QDateTime, int>(_lastMessage, _rights);
            auto *channelItem = new Channel(channelName, channelNameList[0]);
            ui->channels->addItem(channelItem);
        }
        ui->channels->sortItems();
    } else if (command == COMMAND_READ_MESSAGES) {
        QString res = obj["Result"].toString();
        if (res != "SUCCESS") {
            ui->status_text_label->setText(res);
            ui->status_label->movie()->stop();
            return;
        }
        QJsonArray messages = obj["Messages"].toArray();
        QString sender;
        QString text;
        QDateTime time;
        for (auto &&item: messages) {
            auto message = item.toObject();
            sender = message["Sender"].toString();
            text = message["Text"].toString();
            time = QDateTime::fromString(message["Date"].toString(), DATE_FORMAT).toLocalTime();
            newMessage(sender, text, time);
        }
    } else if (command == COMMAND_SEARCH) {
        QString res = obj["Result"].toString();
        if (res != "SUCCESS") {
            ui->status_text_label->setText(res);
            ui->status_label->movie()->stop();
            return;
        }
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
                if (name == userName_) {
                    continue;
                }
                auto *_button = new QPushButton(name);
//                _button->setStyleSheet(user_bubble_style);
                connect(_button, SIGNAL(clicked()), this, SLOT(search_user_clicked()));
                auto *_layout = new QHBoxLayout;
                _layout->setAlignment(Qt::AlignHCenter);
                _layout->addWidget(_button);
                ui->add_layout->addLayout(_layout);
            }
        }
        QJsonArray channels = obj["Channels"].toArray();
        if (!channels.isEmpty()) {
            auto *button = new QLabel("Channels");
            button->setStyleSheet(contact_bubble_style);
            auto *layout = new QHBoxLayout;
            layout->setAlignment(Qt::AlignHCenter);
            layout->addWidget(button);
            ui->add_layout->addLayout(layout);
            for (auto &&item: channels) {
                QStringList channelNameList = item.toString().split(DIALOG_SEPARATOR);
                if (channelNameList.size() == 2) {
                    // убираем лишний никнейм в диалоге
                    channelNameList.removeOne(userName_);
                }
                QString name = channelNameList[0];
                if (users.contains(name)) {
                    continue;
                }
                auto *_button = new QPushButton(name);
                _button->setStyleSheet(contact_bubble_style);
                auto *_layout = new QHBoxLayout;
                _layout->setAlignment(Qt::AlignHCenter);
                _layout->addWidget(_button);
                ui->add_layout->addLayout(_layout);
            }
        }
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
}


void Window::on_channels_itemClicked(QListWidgetItem *item) {
    if (currentOpenChannel_ == item->text()) {
        return;
    }
    eraseLayout(ui->output);
    ui->output->update();
    lastSender_ = "";
    lastMessage_ = QDateTime();
    QString channelName = item->text();
    int roots = Channel::channels[channelName].second;
    if (roots == ROOTS_BANNED) {
        QMessageBox::information(this, QString("Channel %1").arg(channelName), "You are banned at this channel!");
        return;
    }
    if (roots < ROOTS_ADMINISTRATOR) {
        hideInput();
    } else {
        showInput();
    }
//    ui.contact
    if (!channels_.contains(item->text())) {
        for (auto &channel: channels_) {
            if (channel.contains(channelName)) {
                channelName = channel;
                break;
            }
        }
    }
    QString query = COMMAND_READ_MESSAGES + SEPARATOR + channelName + SEPARATOR +
                    QDateTime::currentDateTimeUtc().toString(DATE_FORMAT);
    std::cout << query.toStdString() << std::endl;
    ui->status_text_label->setText("Getting messages\n from server");
    ui->status_label->setMovie(&loadGif_);
    ui->status_text_label->show();
    ui->status_label->show();
    loadGif_.start();
    emit needSend(query);
}

void Window::newMessage(const QString &sender, const QString &text, const QDateTime &date) {
    if (date.date() != lastMessage_.date()) {
        auto *dateLayout = new QHBoxLayout;
        dateLayout->setAlignment(Qt::AlignHCenter);
        auto *dateLabel = new QLabel(date.date().toString("d, MMMM"));
        dateLayout->addWidget(dateLabel);
        ui->output->addLayout(dateLayout);
        lastMessage_ = date;
    }
    bool clientIsSender = sender == userName_;
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
    auto *messageLabel = new QLabel(text + "\t" + date.time().toString("hh:mm"));
    messageLabel->setStyleSheet((clientIsSender) ? user_style : contact_style);
    messageLayout->addWidget(messageLabel);
    ui->output->addLayout(messageLayout);
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

void Window::on_add_button_clicked() {
    eraseLayout(ui->output);
    eraseLayout(ui->add_layout);
    ui->add_line->clear();
    hideInput();
    ui->contact_label_2->clear();
    ui->contact_widget->setCurrentWidget(ui->add_page);
    isNewDialog_ = false;
}


void Window::on_contact_button_clicked() {
    eraseLayout(ui->output);
    hideInput();
    ui->contact_label_2->clear();
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
    QString senderName = ((QPushButton *) sender())->text();
//    std::cout << senderName.toStdString() << std::endl;
    ui->input_frame->show();
    QString channelName = qMin(senderName, userName_) + DIALOG_SEPARATOR + qMax(senderName, userName_);
    if (Channel::channels.contains(channelName)) {
        int roots = Channel::channels[channelName].second;
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
        QString query = COMMAND_READ_MESSAGES + SEPARATOR + channelName + SEPARATOR +
                        QDateTime::currentDateTimeUtc().toString(DATE_FORMAT);
        std::cout << query.toStdString() << std::endl;
        ui->status_text_label->setText("Getting messages\n from server");
        ui->status_label->setMovie(&loadGif_);
        ui->status_text_label->show();
        ui->status_label->show();
        loadGif_.start();
        emit needSend(query);
        return;
    }
    ui->contact_label_2->setText(senderName);
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
    if (isNewDialog_) {
        //TODO: создать диалог
//        QString query = ;
    }
}

