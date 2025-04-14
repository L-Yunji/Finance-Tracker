#ifndef MAINTRANSACTION_H
#define MAINTRANSACTION_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

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
    QPushButton *filterBtn;

    QLabel *foodIc;
    QLabel *foodTitle;
    QLabel *shoppingIc;
    QLabel *detailHisDate;
    QLabel *detailHisMoney;

    // 거래 내역 카드 component
    QWidget* createHistoryItem(
        const QString &iconPath,
        const QString &date,
        const QString &title,
        const QString &type,
        const QString &amount,
        const QColor &typeColor
        );

};
#endif // MAINTRANSACTION_H
