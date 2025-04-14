#include "maintransaction.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainTransaction w;
    w.show();
    return a.exec();
}
