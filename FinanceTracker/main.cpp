#include "maintransaction.h"
#include "logindialog.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ✅ DB 연결 시작
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("transactions.db");

    if (!db.open()) {
        qDebug() << "데이터베이스 열기 실패:" << db.lastError().text();
        return -1;
    }

    // ✅ 로그인 후 MainTransaction으로 이동
    LoginDialog login;
    QObject::connect(&login, &LoginDialog::loginSuccess, [&](const QString &username) {
        MainTransaction *w = new MainTransaction(username);
        w->show();
    });

    login.exec();

    return a.exec();
}
