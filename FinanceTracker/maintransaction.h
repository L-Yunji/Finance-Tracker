#ifndef MAINTRANSACTION_H
#define MAINTRANSACTION_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QToolButton>
#include <QMenu>
#include <QString>
#include <QGraphicsDropShadowEffect>
#include <QMap>
#include <QDebug>
#include <QImageReader>
#include "detailtransaction.h"
#include "TransactionData.h"

class MainTransaction : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainTransaction(QWidget *parent = nullptr);
    ~MainTransaction();

private:
    QLabel *curMoneyTitle;
    QLabel *curMoney;

    QPushButton *getBtn;
    QPushButton *sendBtn;

    QLabel *listHistoryTitle;
    //QPushButton *filterBtn;
    QToolButton *filterBtn;
    QString currentFilter;

    QLabel *foodIc;
    QLabel *foodTitle;
    QLabel *shoppingIc;
    QLabel *detailHisDate;
    QLabel *detailHisMoney;

    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *historyListLayout;

    void loadTransactionHistory();
    void updateCurrentBalance();
    void showDetailWindow(const TransactionData &data);

    bool eventFilter(QObject *watched, QEvent *event) override;

    QWidget* createHistoryItem(const TransactionData &data);

public slots:
    void refreshTransactionList();

    void filterAll();
    void filterDeposit();
    void filterWithdrawal();


};

#endif // MAINTRANSACTION_H
