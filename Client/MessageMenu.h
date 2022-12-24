#ifndef CLIENT_MESSAGE_MENU_H
#define CLIENT_MESSAGE_MENU_H

#include <QMenu>

class MessageMenu : public QMenu {
Q_OBJECT
public:
    explicit MessageMenu(QWidget *parent = nullptr);

private:
signals:
public slots:

    void showMenu(const QPoint &pos);
};


#endif //CLIENT_MESSAGE_MENU_H
