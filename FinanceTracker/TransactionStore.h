#ifndef TRANSACTIONSTORE_H
#define TRANSACTIONSTORE_H

#include "TransactionData.h"
#include <QList>

namespace TransactionStore {
// 모든 거래 데이터를 담는 전역 리스트
inline QList<TransactionData> allTransactions;
}

#endif // TRANSACTIONSTORE_H
