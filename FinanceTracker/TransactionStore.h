#ifndef TRANSACTIONSTORE_H
#define TRANSACTIONSTORE_H

#include "TransactionData.h"
#include <QList>
#include <QString>

namespace TransactionStore {

// 사용자별 거래 내역을 DB에서 불러오기
void loadFromDB(const QString &username);

// 현재 사용자 거래 내역 저장
void saveToDB(const QString &username);

// 거래 추가 (자동 저장)
void addTransaction(const QString &username, const TransactionData &data);

// 현재 사용자 거래 리스트
inline QList<TransactionData> allTransactions;

} // namespace

#endif // TRANSACTIONSTORE_H
