#include "maintransaction.h"
#include "logindialog.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ✅ 1. DB 연결
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("transactions.db");

    if (!db.open()) {
        qDebug() << "❌ 데이터베이스 열기 실패:" << db.lastError().text();
        return -1;
    }

    // ✅ 2. users 테이블 확인 및 생성
    QSqlQuery checkQuery;
    checkQuery.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='users'");

    if (!checkQuery.next()) {
        QSqlQuery createQuery;
        if (!createQuery.exec("CREATE TABLE users ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "username TEXT UNIQUE,"
                              "password TEXT)")) {
            qDebug() << "❌ users 테이블 생성 실패:" << createQuery.lastError().text();
        } else {
            qDebug() << "✅ users 테이블 생성 완료";
        }
    } else {
        qDebug() << "ℹ️ users 테이블 이미 존재함";
    }

    // ✅ 3. 로그인 후 MainTransaction 이동
    LoginDialog login;
    QObject::connect(&login, &LoginDialog::loginSuccess, [&](const QString &username) {
        MainTransaction *w = new MainTransaction(username);
        w->show();
    });

    login.exec();

    return a.exec();
}
