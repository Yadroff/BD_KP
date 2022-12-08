#include <QJsonObject>
#include <iostream>
#include "CommandReplyKey.h"

CommandReplyKey::CommandReplyKey(QSharedPointer<User> &user, const long long &key) : user_(user), key_(key) {
    std::cout << QTime::currentTime().toString().toStdString() << " COMMAND REPLY KEY: CREATE" << std::endl;
}

QJsonDocument CommandReplyKey::exec() {
    user_->setKeyFromAlice(key_);
    auto _ansKey = user_->getKeyToAlice();
    QJsonDocument ans;
    QJsonObject obj;
    obj["Command"] = COMMAND_SEND_KEY;
    obj["Key"] = _ansKey;
    ans.setObject(obj);
    std::cout << QTime::currentTime().toString().toStdString() << " COMMAND REPLY KEY: EXEC" << std::endl;
    return ans;
}

CommandReplyKey::~CommandReplyKey() {
    std::cout << QTime::currentTime().toString().toStdString() << " COMMAND REPLY KEY: DELETE" << std::endl;
}
