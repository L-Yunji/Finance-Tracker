#ifndef TRANSACTIONSTORE_H
#define TRANSACTIONSTORE_H

#include "TransactionData.h"
#include <QList>
#include <QString>

namespace TransactionStore {
extern QList<TransactionData> allTransactions;

void loadFromDB(const QString &username);
void addTransaction(const QString &username, const TransactionData &data);
}

#endif // TRANSACTIONSTORE_H
