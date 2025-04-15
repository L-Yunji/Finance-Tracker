#ifndef DETAILTRANSACTION_H
#define DETAILTRANSACTION_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "TransactionData.h"

class DetailTransaction : public QWidget
{
    Q_OBJECT

public:
    explicit DetailTransaction(QWidget *parent = nullptr);
    void setTransaction(const TransactionData &data);

private:
    // 수정 가능한 필드
    QComboBox *categoryComboBox;
    QLineEdit *memoEdit;

    // 읽기 전용 필드
    QLabel *dateLabel;
    QLabel *amountLabel;
    QLabel *balanceLabel;

    // 버튼
    QPushButton *updateBtn;
    QPushButton *deleteBtn;

private:
    QPushButton* categoryDropdownBtn;
    QMenu* categoryMenu;
    QString selectedCategory;

    void setupUI();

private slots:
    void onUpdateClicked();
    void onDeleteClicked();

signals:
    void transactionUpdated();
    void transactionDeleted();

private:
    TransactionData currentTransaction;


};

#endif // DETAILTRANSACTION_H
