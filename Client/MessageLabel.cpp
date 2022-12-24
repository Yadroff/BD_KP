#include "MessageLabel.h"

MessageLabel::MessageLabel(int id, bool isSender, const Qt::WindowFlags &f, QWidget *parent) :
        QLabel(parent, f), isSender_(isSender), id_(id) {
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void MessageLabel::mousePressEvent(QMouseEvent *event) {
    emit clicked();
}

int MessageLabel::getId() const {
    return id_;
}

bool MessageLabel::isSender() const {
    return isSender_;
}
