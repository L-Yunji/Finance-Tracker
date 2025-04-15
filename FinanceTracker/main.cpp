#include "maintransaction.h"
#include "TransactionData.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<TransactionData>("TransactionData");

    MainTransaction w;
    w.show();
    return a.exec();
}
