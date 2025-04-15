#include "maintransaction.h"
#include "TransactionStore.h"
#include "addtransaction.h"

MainTransaction::MainTransaction(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(360, 640);
    setStyleSheet("background-color: white;");
    setWindowTitle("가계부");

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
    curMoney = new QLabel("₩0");
    curMoney->setStyleSheet("font-size: 32px; font-weight: bold;");
    curMoney->setFixedHeight(38);  // 너무 크지 않게 제한

    layoutBalanceSection->addWidget(curMoneyTitle);
    layoutBalanceSection->addWidget(curMoney);
    mainLayout->addLayout(layoutBalanceSection);

    // 버튼 두 개
    QHBoxLayout *btnLayout = new QHBoxLayout();
    getBtn = new QPushButton("수입");
    sendBtn = new QPushButton("지출");
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
    filterBtn = new QToolButton(this);
    filterBtn->setText("전체");
    filterBtn->setPopupMode(QToolButton::InstantPopup);
    // 2. 드롭다운 메뉴(QMenu) 스타일 적용 및 효과
    QMenu *filterMenu = new QMenu(filterBtn);
    filterMenu->setStyleSheet(R"(
    QMenu {
        background-color: white;
        border: none;
        border-radius: 12px;
        padding: 0;
        margin: 0;
    }
    QMenu::item {
        background-color: white;
        color: #030303;
        font-size: 14px;
        padding: 10px 16px;
        border-radius: 8px;
        margin: 2px;
    }
    QMenu::item:selected {
        background-color: #B3D5FF;
        color: white;
    }
    QMenu::separator {
        height: 1px;
        background: #D5D6DA;
        margin: 4px 0;
    }
)");

    // 3. 메뉴가 표시되기 전, 버튼의 width와 동일하게 QMenu의 width 조정
    connect(filterMenu, &QMenu::aboutToShow, [=]() {
        filterMenu->setFixedWidth(filterBtn->width());
    });

    // 메뉴 액션 추가
    filterMenu->addAction("전체", this, &MainTransaction::filterAll);
    filterMenu->addAction("입금", this, &MainTransaction::filterDeposit);
    filterMenu->addAction("출금", this, &MainTransaction::filterWithdrawal);
    filterBtn->setMenu(filterMenu);

    filterBtn->setStyleSheet(R"(
    QToolButton {
        background-color: #D5D6DA;
        border: none;
        border-radius: 12px;
        padding: 8px 16px;
        font-size: 14px;
        font-weight: bold;
        color: #030303;
    }
    QToolButton:hover {
        background-color: #B3D5FF;
    }
    QToolButton:pressed {
        background-color: #000000;
        color: white;
    }
    QToolButton::menu-indicator {
        subcontrol-origin: padding;
        subcontrol-position: center right;
        image: none;
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

    currentFilter = "전체";
    // 저장된 거래 내역 불러오기
    loadTransactionHistory();
    updateCurrentBalance();
    // 중앙 위젯 세팅
    setCentralWidget(centralWidget);
}

QWidget* MainTransaction::createHistoryItem(
    const QString &date,
    const QString &category,
    const QString &type,
    const QString &amount,
    const QColor &typeColor
    )
{
    // 기본 위젯 및 메인 레이아웃 생성
    QWidget *itemWidget = new QWidget;
    itemWidget->setFixedHeight(64);
    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    // 왼쪽 영역: 아이콘 및 날짜/카테고리 텍스트 영역을 포함하는 수평 레이아웃
    QHBoxLayout *leftLayout = new QHBoxLayout;
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(12);

    // 1. 아이콘 레이블 (가장 왼쪽)
    QLabel *iconLabel = new QLabel;
    iconLabel->setFixedSize(40,40);
    static QMap<QString, QString> categoryIconMap = {
        { "식비",    "food.png" },
        { "교통",    "transport.png" },
        { "쇼핑",    "shopping.png" },
        { "월급",    "salary.png" },
        { "용돈",    "allowance.png" },
        { "기타",    "default.png" }
    };

    QPixmap iconPixmap;
    if (categoryIconMap.contains(category))
        iconPixmap = QPixmap(categoryIconMap.value(category));
    else
        iconPixmap = QPixmap(":/icons/icons/default.png");

    if (!iconPixmap.isNull()) {
        iconLabel->setPixmap(iconPixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // iconPixmap = QPixmap("default.png");
    // iconLabel->setPixmap(iconPixmap);
    leftLayout->addWidget(iconLabel);

    // 2. 날짜와 카테고리 텍스트를 위한 수직 레이아웃
    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setSpacing(2);
    textLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *dateLabel = new QLabel(date);
    dateLabel->setStyleSheet("font-size: 12px; color: gray;");
    textLayout->addWidget(dateLabel);

    QLabel *categoryLabel = new QLabel(category);
    categoryLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #030303;");
    textLayout->addWidget(categoryLabel);

    leftLayout->addLayout(textLayout);
    layout->addLayout(leftLayout);

    // 오른쪽 영역: 거래 유형 및 금액 표시 (수직 레이아웃)
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setContentsMargins(0,0,0,0);
    rightLayout->setSpacing(0);
    rightLayout->setAlignment(Qt::AlignRight);

    QLabel *typeLabel = new QLabel(type);
    typeLabel->setStyleSheet(QString("font-size: 12px; font-weight: bold; color: %1;")
                                 .arg(typeColor.name()));
    typeLabel->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(typeLabel);

    QLabel *amountLabel = new QLabel(amount);
    amountLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    amountLabel->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(amountLabel);

    layout->addStretch(); // 좌측과 우측 영역 사이에 여백
    layout->addLayout(rightLayout);

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
        if (currentFilter == "전체" ||
            (currentFilter == "입금" && !data.isExpense) ||
            (currentFilter == "출금" && data.isExpense)) {

            QString type = data.isExpense ? "출금" : "입금";
            QColor color = data.isExpense ? QColor("#1E40FF") : QColor("#D72638");

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

// 새로 추가: 필터 관련 슬롯 구현
void MainTransaction::filterAll() {
    currentFilter = "전체";
    filterBtn->setText("전체");
    refreshTransactionList();
}

void MainTransaction::filterDeposit() {
    currentFilter = "입금";
    filterBtn->setText("입금");
    refreshTransactionList();
}

void MainTransaction::filterWithdrawal() {
    currentFilter = "출금";
    filterBtn->setText("출금");
    refreshTransactionList();
}



MainTransaction::~MainTransaction() {

}
