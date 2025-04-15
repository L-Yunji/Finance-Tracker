#include "TransactionStore.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace TransactionStore {

QList<TransactionData> allTransactions;

void ensureTableExists() {
    QSqlQuery query;
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            category TEXT,
            amount TEXT,
            isExpense INTEGER,
            dateTime TEXT
        )
    )");

    if (query.lastError().isValid()) {
        qDebug() << "테이블 생성 오류:" << query.lastError().text();
    }
}

void loadFromDB(const QString &username) {
    allTransactions.clear();
    ensureTableExists();

    QSqlQuery query;
    query.prepare("SELECT category, amount, isExpense, dateTime FROM transactions WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec()) {
        qDebug() << "불러오기 오류:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        TransactionData data;
        data.category = query.value(0).toString();
        data.amount = query.value(1).toString();
        data.isExpense = query.value(2).toInt();
        data.dateTime = query.value(3).toString();
        allTransactions.append(data);
    }
}

void addTransaction(const QString &username, const TransactionData &data) {
    QSqlQuery query;
    query.prepare("INSERT INTO transactions (username, category, amount, isExpense, dateTime) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(data.category);
    query.addBindValue(data.amount);
    query.addBindValue(data.isExpense);
    query.addBindValue(data.dateTime);

    if (!query.exec()) {
        qDebug() << "저장 오류:" << query.lastError().text();
    }

    allTransactions.append(data);
}

}
