#include "maintransaction.h"
#include "addtransaction.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainTransaction w;
    w.show();
    //AddTransaction AddW;
    //AddW.show();
    return a.exec();
}
