#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "userdbmanger.h"

bool UserDBManager::initDB() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");
    if (!db.open()) {
        qDebug() << "DB 열기 실패:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "username TEXT UNIQUE NOT NULL, "
                      "password TEXT NOT NULL)");
}

bool UserDBManager::registerUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);
    return query.exec();
}

bool UserDBManager::loginUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);
    if (query.exec() && query.next()) return true;
    return false;
}

void UserDBManager::printAllUsers()
{
    QSqlQuery query("SELECT username, password FROM users");
    qDebug() << "[저장된 사용자 목록]";
    while (query.next()) {
        QString username = query.value(0).toString();
        QString password = query.value(1).toString();
        qDebug() << "아이디:" << username << ", 비밀번호:" << password;
    }
}
