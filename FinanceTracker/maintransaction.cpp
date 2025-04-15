#include "maintransaction.h"
#include "TransactionStore.h"
#include "addtransaction.h"
#include "detailtransaction.h"
#include <QMouseEvent>
#include <QVariant>

MainTransaction::MainTransaction(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(360, 640);
    setStyleSheet("background-color: white;");
    setWindowTitle("계좌 관리");

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(32,32,32,32);

    QVBoxLayout *layoutBalanceSection = new QVBoxLayout();
    layoutBalanceSection->setContentsMargins(5, 5, 5, 5);
    layoutBalanceSection->setSpacing(0);

    curMoneyTitle = new QLabel("현재 잔액");
    curMoneyTitle->setStyleSheet("font-size: 18px;");
    curMoneyTitle->setFixedHeight(38);
    curMoneyTitle->setContentsMargins(0, 0, 0, 0);
    curMoney = new QLabel("₩2,010,000");
    curMoney->setStyleSheet("font-size: 32px; font-weight: bold;");
    curMoney->setFixedHeight(38);

    layoutBalanceSection->addWidget(curMoneyTitle);
    layoutBalanceSection->addWidget(curMoney);
    mainLayout->addLayout(layoutBalanceSection);

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
        cursor: pointer;
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
        cursor: pointer;
    }
    QPushButton:hover {
        background-color: #cddfff;
    }
    )");

    btnLayout->addWidget(getBtn);
    btnLayout->addWidget(sendBtn);
    mainLayout->addLayout(btnLayout);

    connect(sendBtn, &QPushButton::clicked, this, [=]() {
        AddTransaction *addWin = new AddTransaction(true);
        connect(addWin, &AddTransaction::transactionAdded, this, &MainTransaction::refreshTransactionList);
        addWin->move(this->x() + 30, this->y() + 30);
        addWin->show();
    });
    connect(getBtn, &QPushButton::clicked, this, [=]() {
        AddTransaction *addWin = new AddTransaction(false);
        connect(addWin, &AddTransaction::transactionAdded, this, &MainTransaction::refreshTransactionList);
        addWin->move(this->x() + 30, this->y() + 30);
        addWin->show();
    });

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

    filterBtn = new QPushButton("전체");
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

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setFixedHeight(360);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *scrollContent = new QWidget;
    historyListLayout = new QVBoxLayout(scrollContent);
    historyListLayout->setAlignment(Qt::AlignTop);
    historyListLayout->setSpacing(8);
    historyListLayout->setContentsMargins(0, 0, 0, 0);
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    TransactionData sample1;
    sample1.amount = "12000";
    sample1.category = "식비";
    sample1.dateTime = "2025-04-14 11:30";
    sample1.memo = "편의점";
    sample1.isExpense = true;

    TransactionData sample2;
    sample2.amount = "500000";
    sample2.category = "월급";
    sample2.dateTime = "2025-04-13 09:00";
    sample2.memo = "4월 급여";
    sample2.isExpense = false;

    TransactionStore::allTransactions.append(sample1);
    TransactionStore::allTransactions.append(sample2);

    loadTransactionHistory();
    setCentralWidget(centralWidget);
}

QWidget* MainTransaction::createHistoryItem(const TransactionData &data)
{
    QWidget *itemWidget = new QWidget;
    itemWidget->setFixedHeight(64);
    itemWidget->installEventFilter(this);
    itemWidget->setProperty("transaction", QVariant::fromValue(data));

    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    // 왼쪽 영역: 아이콘 및 날짜/카테고리 텍스트 영역을 포함하는 수평 레이아웃
    QHBoxLayout *leftLayout = new QHBoxLayout;
    leftLayout->setSpacing(8);

    // 1. 아이콘 레이블 (가장 왼쪽)
    QLabel *iconLabel = new QLabel;
    static QMap<QString, QString> categoryIconMap = {
        // { "식비",    ":/icons/icons/food.png" },
        // { "교통",    ":/icons/icons/transport.png" },
        // { "쇼핑",    ":/icons/icons/shopping.png" },
        // { "월급",    ":/icons/icons/salary.png" },
        // { "용돈",    ":/icons/icons/allowance.png" },
        // { "기타",    ":/icons/icons/default.png" }
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
    rightLayout->setSpacing(0);
    rightLayout->setAlignment(Qt::AlignRight);

    layout->addStretch();
    layout->addLayout(amountLayout);

    itemWidget->setCursor(Qt::PointingHandCursor);

    // 클릭 이벤트 연결
    itemWidget->installEventFilter(this);
    itemWidget->setProperty("transaction", QVariant::fromValue(data));

    return itemWidget;
}

void MainTransaction::loadTransactionHistory()
{
    QLayoutItem *child;
    while ((child = historyListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    for (const TransactionData &data : TransactionStore::allTransactions) {
        historyListLayout->addWidget(createHistoryItem(data));
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
void MainTransaction::showDetailWindow(const TransactionData &data)
{
    DetailTransaction *dtWin = new DetailTransaction();
    dtWin->setTransaction(data);
    dtWin->setAttribute(Qt::WA_DeleteOnClose);

    // 현재 창 위치 기준으로 약간 오른쪽 아래로 띄우기
    int offsetX = 30;
    int offsetY = 30;
    dtWin->move(this->x() + offsetX, this->y() + offsetY);

    dtWin->show();
    connect(dtWin, &DetailTransaction::transactionUpdated, this, &MainTransaction::refreshTransactionList);
    connect(dtWin, &DetailTransaction::transactionDeleted, this, &MainTransaction::refreshTransactionList);

}

bool MainTransaction::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QWidget *clickedWidget = qobject_cast<QWidget *>(watched);
        if (clickedWidget && clickedWidget->property("transaction").isValid()) {
            TransactionData data = clickedWidget->property("transaction").value<TransactionData>();
            showDetailWindow(data);
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

MainTransaction::~MainTransaction() {}
