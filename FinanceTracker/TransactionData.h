#ifndef TRANSACTIONDATA_H
#define TRANSACTIONDATA_H

#include <QString>
#include <QMetaType>

struct TransactionData {
    int id = -1;        // 고유 ID (DB에서 받아옴)
    QString amount;     // 금액 (예: "25000")
    QString category;   // 카테고리 (예: "식비")
    QString dateTime;   // 시간 (예: "2024-04-14 18:00")
    bool isExpense = true;  // 출금이면 true, 입금이면 false
    QString memo = "";      // 메모 (선택 사항)
};

Q_DECLARE_METATYPE(TransactionData)

#endif // TRANSACTIONDATA_H
