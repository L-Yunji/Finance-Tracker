#ifndef TRANSACTIONDATA_H
#define TRANSACTIONDATA_H

#include <QString>

struct TransactionData {
    QString amount;     // 금액 (예: "25000")
    QString category;   // 카테고리 (예: "식비")
    QString dateTime;   // 시간 (예: "2024-04-14 18:00")
    bool isExpense = true;  // 출금이면 true, 입금이면 false
};

#endif // TRANSACTIONDATA_H
