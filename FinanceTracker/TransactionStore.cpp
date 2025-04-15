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
            dateTime TEXT,
            memo TEXT
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
    query.prepare("SELECT id, category, amount, isExpense, dateTime, memo FROM transactions WHERE username = ?");
    query.addBindValue(username);
    qDebug() << "username으로 거래 불러오기 시작:" << username;
    if (!query.exec()) {
        qDebug() << "불러오기 오류:" << query.lastError().text();
        return;
    }
    int count = 0;
    while (query.next()) {
        TransactionData data;
        data.id = query.value(0).toInt();
        data.category = query.value(1).toString();
        data.amount = query.value(2).toString();
        data.isExpense = query.value(3).toBool();
        data.dateTime = query.value(4).toString();
        data.memo = query.value(5).toString();
        allTransactions.append(data);
        count++;
    }
    qDebug() << "불러온 거래 수:" << count;

}

void addTransaction(const QString &username, const TransactionData &data) {
    QSqlQuery query;
    query.prepare("INSERT INTO transactions (username, category, amount, isExpense, dateTime) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(data.category);
    query.addBindValue(data.amount);
    query.addBindValue(data.isExpense);
    query.addBindValue(data.dateTime);
    query.addBindValue(data.memo);

    if (!query.exec()) {
        qDebug() << "저장 오류:" << query.lastError().text();
    }

    allTransactions.append(data);
}

bool updateTransaction(int id, const QString &category, const QString &memo) {
    QSqlQuery query;
    query.prepare("UPDATE transactions SET category = ?, memo = ? WHERE id = ?");
    query.addBindValue(category);
    query.addBindValue(memo);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "거래 수정 실패:" << query.lastError().text();
        return false;
    }
    return true;
}


}
