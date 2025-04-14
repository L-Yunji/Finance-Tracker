#include "addtransaction.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    AddTransaction w;
    w.show();
    return a.exec();
}
