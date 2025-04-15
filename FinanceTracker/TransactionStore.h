#ifndef TRANSACTIONSTORE_H
#define TRANSACTIONSTORE_H

#include "TransactionData.h"
#include <QList>
#include <QString>

namespace TransactionStore {
extern QList<TransactionData> allTransactions;

void loadFromDB(const QString &username);
void addTransaction(const QString &username, const TransactionData &data);
bool updateTransaction(int id, const QString &category, const QString &memo);
}

#endif // TRANSACTIONSTORE_H
