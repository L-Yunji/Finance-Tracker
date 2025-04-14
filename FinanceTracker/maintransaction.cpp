#include "maintransaction.h"

MainTransaction::MainTransaction(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(360, 640);
    setStyleSheet("background-color: white;");

    // 루트 위젯 & 레이아웃
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 전체 여백 설정
    mainLayout->setContentsMargins(32,32,32,32); // 화면 전체 패딩

    // 현재 잔액 영역
    QVBoxLayout *layoutBalanceSection = new QVBoxLayout();
    layoutBalanceSection->setContentsMargins(5, 5, 5, 5);
    layoutBalanceSection->setSpacing(0);

    curMoneyTitle = new QLabel("현재 잔액");
    curMoneyTitle->setStyleSheet("font-size: 18px;");
    curMoneyTitle->setFixedHeight(38);
    curMoneyTitle->setContentsMargins(0, 0, 0, 0);
    curMoney = new QLabel("₩2,010,000");
    curMoney->setStyleSheet("font-size: 32px; font-weight: bold;");
    curMoney->setFixedHeight(38);  // 너무 크지 않게 제한

    layoutBalanceSection->addWidget(curMoneyTitle);
    layoutBalanceSection->addWidget(curMoney);
    mainLayout->addLayout(layoutBalanceSection);

    // 버튼 두 개
    QHBoxLayout *btnLayout = new QHBoxLayout();
    getBtn = new QPushButton("가져오기");
    sendBtn = new QPushButton("보내기");
    getBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #DDEBFF;
        color: #1E40FF;
        font-size: 16px;
        font-weight: bold;
        border: none;
        border-radius: 12px;
        padding: 10px 20px;
    }
    QPushButton:hover {
        background-color: #cddfff;
    }
)");

    sendBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #DDEBFF;
        color: #1E40FF;
        font-size: 16px;
        font-weight: bold;
        border: none;
        border-radius: 12px;
        padding: 10px 20px;
    }
    QPushButton:hover {
        background-color: #cddfff;
    }
)");

    btnLayout->addWidget(getBtn);
    btnLayout->addWidget(sendBtn);
    mainLayout->addLayout(btnLayout);

    // 히스토리 헤더
    mainLayout->addSpacing(12);
    QHBoxLayout *headerLayout = new QHBoxLayout();
    listHistoryTitle = new QLabel("거래 내역");
    listHistoryTitle->setStyleSheet("font-size: 18px;");
    filterBtn = new QPushButton("전체");

    filterBtn->setStyleSheet(R"(
    QPushButton {
        background-color: white;
        color: #333333;
        border: 1px solid #D0D0D0;
        border-radius: 12px;
        font-size: 14px;
        font-weight: bold;
        padding: 6px 12px;
    }
    QPushButton:hover {
        background-color: #f0f0f0;
    }
)");

    headerLayout->addWidget(listHistoryTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(filterBtn);
    mainLayout->addLayout(headerLayout);

    // 거래 항목 1줄
    // 거래 리스트용 레이아웃 만들기
    QVBoxLayout *historyListLayout = new QVBoxLayout;
    historyListLayout->setSpacing(8);

    // 거래 1건 추가
    historyListLayout->addWidget(createHistoryItem(
        "2024-10-04 오전 11시",  // 날짜
        "네이버 쇼핑",   // 제목
        "출금",  // 출금/입금
        "13,500원",   // 금액
        QColor("#1E40FF")   // 파란색
        ));

    // mainLayout에 넣어줘야 보임!
    mainLayout->addLayout(historyListLayout);



    // 중앙 위젯 세팅
    setCentralWidget(centralWidget);
}

QWidget* MainTransaction::createHistoryItem(
    const QString &date,
    const QString &title,
    const QString &type,
    const QString &amount,
    const QColor &typeColor
    ) {
    QWidget *itemWidget = new QWidget;
    itemWidget->setFixedHeight(64);
    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    // 1. 날짜 + 제목 (위/아래 텍스트)
    QVBoxLayout *textLayout = new QVBoxLayout;
    QLabel *labelDate = new QLabel(date);
    labelDate->setStyleSheet("color: gray; font-size: 12px;");
    QLabel *labelTitle = new QLabel(title);
    labelTitle->setStyleSheet("font-size: 14px; font-weight: bold;");
    textLayout->setSpacing(0);
    textLayout->addWidget(labelDate);
    textLayout->addWidget(labelTitle);
    layout->addLayout(textLayout);

    // 2. 오른쪽에 출금/금액
    QLabel *labelType = new QLabel(type);
    labelType->setStyleSheet(QString("color: %1; font-weight: bold;").arg(typeColor.name()));

    QLabel *labelAmount = new QLabel(amount);
    labelAmount->setStyleSheet("font-size: 14px; font-weight: bold;");

    layout->addStretch(); // 중간에 공간 줌
    layout->addWidget(labelType);
    layout->addSpacing(8);
    layout->addWidget(labelAmount);

    return itemWidget;
}


MainTransaction::~MainTransaction() {

}
