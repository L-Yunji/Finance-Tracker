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

    QVBoxLayout *textLayout = new QVBoxLayout;
    QLabel *labelDate = new QLabel(data.dateTime);
    labelDate->setStyleSheet("color: gray; font-size: 12px;");
    QLabel *labelTitle = new QLabel(data.category);
    labelTitle->setStyleSheet("font-size: 14px; font-weight: bold;");
    textLayout->setSpacing(0);
    textLayout->addWidget(labelDate);
    textLayout->addWidget(labelTitle);
    layout->addLayout(textLayout);

    QVBoxLayout *amountLayout = new QVBoxLayout;
    amountLayout->setSpacing(0);
    amountLayout->setAlignment(Qt::AlignRight);

    QString typeText = data.isExpense ? "출금" : "입금";
    QColor typeColor = data.isExpense ? QColor("#1E40FF") : QColor("#D72638");

    QLabel *labelType = new QLabel(typeText);
    labelType->setStyleSheet(QString("color: %1; font-size: 12px; font-weight: bold;").arg(typeColor.name()));
    labelType->setAlignment(Qt::AlignRight);

    QLabel *labelAmount = new QLabel(data.amount + "원");
    labelAmount->setStyleSheet("font-size: 14px; font-weight: bold;");
    labelAmount->setAlignment(Qt::AlignRight);

    amountLayout->addWidget(labelType);
    amountLayout->addWidget(labelAmount);

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
