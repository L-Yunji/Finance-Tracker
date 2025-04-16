#include "maintransaction.h"
#include "TransactionStore.h"
#include "addtransaction.h"
#include "detailtransaction.h"
#include <QMouseEvent>
#include <QVariant>
#include <QToolButton>
#include <QMenu>

MainTransaction::MainTransaction(const QString &username, QWidget *parent)
    : QMainWindow(parent), currentUsername(username)
{
    setFixedSize(360, 640);
    setStyleSheet("background-color: white;");
    setWindowTitle("가계부");

    TransactionStore::loadFromDB(username);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(32,32,32,32);

    QVBoxLayout *layoutBalanceSection = new QVBoxLayout();
    layoutBalanceSection->setContentsMargins(5, 5, 5, 5);
    layoutBalanceSection->setSpacing(0);

    QHBoxLayout *topRow = new QHBoxLayout();
    topRow->addStretch();

    exportBtn = new QPushButton("내보내기");
    exportBtn->setFixedHeight(30);
    exportBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #E0E0E0;
        border: none;
        border-radius: 12px;
        padding: 4px 12px;
        font-size: 12px;
        font-weight: bold;
        color: #333;
    }
    QPushButton:hover {
        background-color: #D0D0D0;
    }
)");
    topRow->addWidget(exportBtn);
    layoutBalanceSection->addLayout(topRow);

    curMoneyTitle = new QLabel("현재 잔액");
    curMoneyTitle->setStyleSheet("font-size: 18px; color: #4F4F4F;");
    curMoneyTitle->setFixedHeight(38);
    curMoneyTitle->setContentsMargins(0, 0, 0, 0);

    curMoney = new QLabel("₩0");
    curMoney->setStyleSheet("font-size: 32px; font-weight: bold; color: #030303;");
    curMoney->setFixedHeight(60);
    curMoney->setMinimumWidth(320);
    curMoney->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 좌측 정렬

    layoutBalanceSection->addWidget(curMoneyTitle);
    layoutBalanceSection->addWidget(curMoney);
    mainLayout->addLayout(layoutBalanceSection);

    connect(exportBtn, &QPushButton::clicked, this, &MainTransaction::exportToCSV);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    getBtn = new QPushButton("수입");
    sendBtn = new QPushButton("지출");
    getBtn->setCursor(Qt::PointingHandCursor);
    sendBtn->setCursor(Qt::PointingHandCursor);
    getBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #DDEBFF;
        color: #3033A2;
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
        color: #3033A2;
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

    connect(sendBtn, &QPushButton::clicked, this, [=]() {
        AddTransaction *addWin = new AddTransaction(true, currentUsername);
        connect(addWin, &AddTransaction::transactionAdded, this, &MainTransaction::refreshTransactionList);
        addWin->move(this->x() + 30, this->y() + 30);
        addWin->show();
    });
    connect(getBtn, &QPushButton::clicked, this, [=]() {
        AddTransaction *addWin = new AddTransaction(false, currentUsername);
        connect(addWin, &AddTransaction::transactionAdded, this, &MainTransaction::refreshTransactionList);
        addWin->move(this->x() + 30, this->y() + 30);
        addWin->show();
    });

    mainLayout->addSpacing(12);
    QHBoxLayout *headerLayout = new QHBoxLayout();
    listHistoryTitle = new QLabel("가계 내역");
    listHistoryTitle->setStyleSheet("font-size: 14px; color: #4F4F4F;");
    filterBtn = new QToolButton(this);
    filterBtn->setText("전체");
    filterBtn->setPopupMode(QToolButton::InstantPopup);
    filterBtn->setCursor(Qt::PointingHandCursor);

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
        font-size: 12px;
        padding: 8px 12px;
        border-radius: 4px;
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

    connect(filterMenu, &QMenu::aboutToShow, [=]() {
        filterMenu->setFixedWidth(filterBtn->width());
    });

    filterMenu->addAction("전체", this, &MainTransaction::filterAll);
    filterMenu->addAction("입금", this, &MainTransaction::filterDeposit);
    filterMenu->addAction("출금", this, &MainTransaction::filterWithdrawal);
    filterBtn->setMenu(filterMenu);

    filterBtn->setStyleSheet(R"(
    QToolButton {
        background-color: transparent;
        border: 1px solid #D5D6DA;
        border-radius: 12px;
        padding: 8px 16px;
        font-size: 14px;
        color: #030303;
    }
    QToolButton:hover {
        background-color: #cddfff;
    }
    QToolButton:pressed {
        background-color: #030303;
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
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setAlignment(Qt::AlignTop);
    scrollLayout->setSpacing(8);
    scrollLayout->setContentsMargins(0, 0, 0, 0);

    emptyMessageLabel = new QLabel("거래내역을 입력해주세요", scrollContent);
    emptyMessageLabel->setAlignment(Qt::AlignCenter);
    emptyMessageLabel->setStyleSheet("font-size: 14px; color: #888888; padding-top: 80px;");
    emptyMessageLabel->setVisible(false);
    scrollLayout->addWidget(emptyMessageLabel);

    historyListLayout = new QVBoxLayout();
    scrollLayout->addLayout(historyListLayout);

    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    currentFilter = "전체";
    loadTransactionHistory();
    updateCurrentBalance();
    setCentralWidget(centralWidget);
}

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

void MainTransaction::loadTransactionHistory()
{
    QLayoutItem *child;
    while ((child = historyListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    int count = 0;
    for (const TransactionData &data : TransactionStore::allTransactions) {
        if (currentFilter == "전체" ||
            (currentFilter == "입금" && !data.isExpense) ||
            (currentFilter == "출금" && data.isExpense)) {
            historyListLayout->addWidget(createHistoryItem(data));
            count++;
        }
    }

    if (count == 0) {
        emptyMessageLabel->show();
    } else {
        emptyMessageLabel->hide();
    }
}

QWidget* MainTransaction::createHistoryItem(const TransactionData &data)
{
    QWidget *itemWidget = new QWidget;
    itemWidget->setFixedHeight(64);
    itemWidget->setCursor(Qt::PointingHandCursor);

    // 🔹 클릭 이벤트 처리를 위한 정보 저장
    itemWidget->setProperty("transaction", QVariant::fromValue(data));
    itemWidget->installEventFilter(this);

    QHBoxLayout *layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    // 🔸 왼쪽 레이아웃: 아이콘 + 날짜 + 카테고리
    QHBoxLayout *leftLayout = new QHBoxLayout;
    leftLayout->setSpacing(12);

    QLabel *iconLabel = new QLabel;
    iconLabel->setFixedSize(40, 40);

    static QMap<QString, QString> categoryIconMap = {
        { "식비",    "food.png" },
        { "교통",    "transport.png" },
        { "쇼핑",    "shopping.png" },
        { "월급",    "salary.png" },
        { "용돈",    "allowance.png" },
        { "기타",    "default.png" }
    };

    QPixmap iconPixmap;
    if (categoryIconMap.contains(data.category)) {
        iconPixmap = QPixmap(categoryIconMap.value(data.category));
    } else {
        iconPixmap = QPixmap(":/icons/icons/default.png");
    }

    if (!iconPixmap.isNull()) {
        iconLabel->setPixmap(iconPixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    leftLayout->addWidget(iconLabel);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setSpacing(2);

    QLabel *dateLabel = new QLabel(data.dateTime);
    dateLabel->setStyleSheet("font-size: 12px; color: gray;");
    textLayout->addWidget(dateLabel);

    QLabel *categoryLabel = new QLabel(data.category);
    categoryLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #030303;");
    textLayout->addWidget(categoryLabel);

    leftLayout->addLayout(textLayout);
    layout->addLayout(leftLayout);

    // 🔸 오른쪽 레이아웃: 출금/입금 + 금액
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setSpacing(0);
    rightLayout->setAlignment(Qt::AlignRight);

    QString typeText = data.isExpense ? "출금" : "입금";
    QColor typeColor = data.isExpense ? QColor("#1E40FF") : QColor("#D72638");

    QLabel *typeLabel = new QLabel(typeText);
    typeLabel->setStyleSheet(QString("font-size: 12px; font-weight: bold; color: %1;").arg(typeColor.name()));
    typeLabel->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(typeLabel);

    // 천 단위 쉼표 넣기
    QLocale locale = QLocale::system();
    QString formattedAmount = locale.toString(data.amount.toLongLong());

    QLabel *amountLabel = new QLabel(formattedAmount + "원");
    amountLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    amountLabel->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(amountLabel);

    layout->addStretch();
    layout->addLayout(rightLayout);

    return itemWidget;
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
    dtWin->move(this->x() + 30, this->y() + 30);
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

void MainTransaction::exportToCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "CSV 파일로 저장", "transactions.csv", "CSV Files (*.csv)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "오류", "파일을 열 수 없습니다.");
        return;
    }

    QTextStream out(&file);

    out << "날짜,카테고리,타입,금액\n";

    for (const TransactionData &data : TransactionStore::allTransactions) {
        QString type = data.isExpense ? "출금" : "입금";
        out << QString("%1,%2,%3,%4\n")
                   .arg(data.dateTime)
                   .arg(data.category)
                   .arg(type)
                   .arg(data.amount);
    }

    file.close();
    QMessageBox::information(this, "완료", "CSV 파일로 내보내기가 완료되었습니다.");
}

MainTransaction::~MainTransaction() {}
