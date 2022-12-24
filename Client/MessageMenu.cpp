#include "MessageMenu.h"

MessageMenu::MessageMenu(QWidget *parent) : QMenu(parent) {
    addAction("Action1");
}

void MessageMenu::showMenu(const QPoint &pos) {
    exec(QCursor::pos());
}
