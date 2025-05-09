#ifndef MAINTRANSACTION_H
#define MAINTRANSACTION_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include "TransactionData.h"
#include <QToolButton>
#include <QMenu>
#include <QString>
#include <QGraphicsDropShadowEffect>
#include <QMap>
#include <QDebug>
#include <QImageReader>
#include <QFileDialog>
#include <QLineEdit>

class MainTransaction : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainTransaction(const QString &username, QWidget *parent = nullptr);
    ~MainTransaction();

private:
    QLabel *curMoneyTitle;
    QLabel *curMoney;

    QPushButton *getBtn;
    QPushButton *sendBtn;
    QPushButton *exportBtn;

    QLabel *listHistoryTitle;
    QToolButton *filterBtn;
    QString currentFilter;
    QString currentUsername;

    QLabel *foodIc;
    QLabel *foodTitle;
    QLabel *shoppingIc;
    QLabel *detailHisDate;
    QLabel *detailHisMoney;

    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *historyListLayout;
    QLabel *emptyMessageLabel;

    void loadTransactionHistory();
    void updateCurrentBalance();
    void showDetailWindow(const TransactionData &data);

    bool eventFilter(QObject *watched, QEvent *event) override;

    QWidget* createHistoryItem(const TransactionData &data);
    QLineEdit *searchInput; // 멤버 변수 선언
protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void refreshTransactionList();
    void exportToCSV();
    void filterAll();
    void filterDeposit();
    void filterWithdrawal();
    void filterTransactionList(const QString &keyword);


};

#endif // MAINTRANSACTION_H
