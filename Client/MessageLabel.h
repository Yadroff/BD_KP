#ifndef CLIENT_MESSAGE_LABEL_H
#define CLIENT_MESSAGE_LABEL_H

#include <QLabel>

#include "Message.h"

class MessageLabel : public QLabel {
Q_OBJECT
public:
    MessageLabel(int id, bool isSender, const Qt::WindowFlags &f = Qt::WindowFlags(),
                 QWidget *parent = nullptr);

    [[nodiscard]] int getId() const;

    bool isSender() const;

signals:

    void clicked();

private:
    int id_;
    bool isSender_;
protected:
    void mousePressEvent(QMouseEvent *event);
};


#endif //CLIENT_MESSAGE_LABEL_H
