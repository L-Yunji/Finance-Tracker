#include "maintransaction.h"
#include "detailtransaction.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     MainTransaction w;
     w.show();
    //DetailTransaction wT;
    //wT.show();
    return a.exec();
}
