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

class MainTransaction : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainTransaction(QWidget *parent = nullptr);
    ~MainTransaction();

private:
    // UI 요소 선언
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

    // scroll 영역 component
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *historyListLayout; // 거래 리스트를 저장할 레이아웃
    void loadTransactionHistory(); // 거래 내역 불러오기 함수 선언
    void updateCurrentBalance();  // 현재 잔액 업데이트 함수

    // 거래 내역 카드 component
    QWidget* createHistoryItem(
        const QString &date,
        const QString &title,
        const QString &type,
        const QString &amount,
        const QColor &typeColor
        );

public slots:
    void refreshTransactionList();

    void filterAll();
    void filterDeposit();
    void filterWithdrawal();


};
#endif // MAINTRANSACTION_H
