#include <QApplication>
#include <QPushButton>

#include "Manager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Manager man;
    return QApplication::exec();
}
