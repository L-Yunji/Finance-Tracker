#include "maintransaction.h"
#include "TransactionStore.h"
#include "addtransaction.h"

MainTransaction::MainTransaction(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(360, 640);
    setStyleSheet("background-color: white;");
    setWindowTitle("계좌 관리");

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

    // sendBtn 클릭 시 AddTransaction 창 띄우기
    connect(sendBtn, &QPushButton::clicked, this, [=]() {
        AddTransaction *addWin = new AddTransaction(true); // true = 출금

        connect(addWin, &AddTransaction::transactionAdded, this, &MainTransaction::refreshTransactionList);

        addWin->move(this->x() + 30, this->y() + 30);  // 약간 옆에 띄움
        addWin->show();
    });
    connect(getBtn, &QPushButton::clicked, this, [=]() {
        AddTransaction *addWin = new AddTransaction(false);  // false = 입금
        connect(addWin, &AddTransaction::transactionAdded, this, &MainTransaction::refreshTransactionList);
        addWin->move(this->x() + 30, this->y() + 30);
        addWin->show();
    });


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

    // 거래 리스트용 레이아웃 만들기
    // 스크롤 영역 생성
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setFixedHeight(360);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    // 내부 콘텐츠 위젯 + 레이아웃
    QWidget *scrollContent = new QWidget;

    historyListLayout = new QVBoxLayout(scrollContent); // 멤버 변수로 초기화
    historyListLayout->setAlignment(Qt::AlignTop);
    historyListLayout->setSpacing(8);
    historyListLayout->setContentsMargins(0, 0, 0, 0);

    // 연결
    scrollArea->setWidget(scrollContent);

    // 스크롤 영역만 mainLayout에 추가!
    mainLayout->addWidget(scrollArea);

    // 저장된 거래 내역 불러오기
    loadTransactionHistory();
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

    // 1. 날짜 + 제목
    QVBoxLayout *textLayout = new QVBoxLayout;
    QLabel *labelDate = new QLabel(date);
    labelDate->setStyleSheet("color: gray; font-size: 12px;");
    QLabel *labelTitle = new QLabel(title);
    labelTitle->setStyleSheet("font-size: 14px; font-weight: bold;");
    textLayout->setSpacing(0);
    textLayout->addWidget(labelDate);
    textLayout->addWidget(labelTitle);
    layout->addLayout(textLayout);

    // 2. 오른쪽에 출금/입금 + 금액
    QVBoxLayout *amountLayout = new QVBoxLayout;
    amountLayout->setSpacing(0);
    amountLayout->setAlignment(Qt::AlignRight);

    QLabel *labelType = new QLabel(type);
    labelType->setStyleSheet(QString("color: %1; font-size: 12px; font-weight: bold;").arg(typeColor.name()));
    labelType->setAlignment(Qt::AlignRight);

    QLabel *labelAmount = new QLabel(amount);
    labelAmount->setStyleSheet("font-size: 14px; font-weight: bold;");
    labelAmount->setAlignment(Qt::AlignRight);

    amountLayout->addWidget(labelType);
    amountLayout->addWidget(labelAmount);

    layout->addStretch(); // 좌우 사이 간격 벌려줌
    layout->addLayout(amountLayout);

    return itemWidget;

}

void MainTransaction::loadTransactionHistory()
{
    // 기존 리스트 정리
    QLayoutItem *child;
    while ((child = historyListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    // 저장된 거래 내역 모두 출력
    for (const TransactionData &data : TransactionStore::allTransactions) {
        QString type = data.isExpense ? "출금" : "입금";
        QColor color = data.isExpense ? QColor("#1E40FF") : QColor("#3A9D23");

        QWidget *item = createHistoryItem(
            data.dateTime,
            data.category,
            type,
            data.amount + "원",
            color
            );

        historyListLayout->addWidget(item);
    }
}
void MainTransaction::updateCurrentBalance()
{
    long long total = 0;
    for (const TransactionData &data : TransactionStore::allTransactions) {
        long long amount = data.amount.toLongLong();
        total += data.isExpense ? -amount : amount;
    }

    QLocale locale = QLocale::system();
    QString formatted = locale.toString(total);
    curMoney->setText("₩" + formatted);
}

void MainTransaction::refreshTransactionList() {
    loadTransactionHistory();
    updateCurrentBalance();
}




MainTransaction::~MainTransaction() {

}
