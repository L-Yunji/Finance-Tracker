#include "detailtransaction.h"
#include "TransactionData.h"
#include "TransactionStore.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QMenu>
#include <QPushButton>
#include <QLocale>

DetailTransaction::DetailTransaction(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(360, 640);
    setWindowTitle("상세 거래 내역");
    setupUI();
}

void DetailTransaction::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 32, 32, 32);
    mainLayout->setSpacing(20);
    this->setStyleSheet("background-color: white;");

    // QPushButton + QMenu로 대체한 카테고리 선택
    categoryDropdownBtn = new QPushButton("카테고리 선택 ▼", this);
    categoryDropdownBtn->setCursor(Qt::PointingHandCursor);
    categoryDropdownBtn->setStyleSheet(R"(
    QPushButton {
        background-color: white;
        border-radius: 12px;
        padding: 8px 0px;
        font-size: 24px;
        font-weight: bold;
        color: #030303;
        text-align: left;
    }
    QPushButton::menu-indicator {
        width: 0px;
        height: 0px;
        image: none;
        subcontrol-position: right center;
        subcontrol-origin: content;
    }
)");


    categoryMenu = new QMenu(this);
    categoryMenu->setStyleSheet(R"(
    QMenu {
        background-color: white;
        border: none;
        border-radius: 12px;
        padding: 4px;
    }
    QMenu::item {
        background-color: white;
        color: #030303;
        font-size: 14px;
        padding: 10px 16px;
        border-radius: 8px;
    }
    QMenu::item:selected {
        background-color: #B3D5FF;
        color: white;
    }
    )");

    mainLayout->addWidget(categoryDropdownBtn);

    // 카테고리 항목 설정
    auto setCategoryItems = [=](const QStringList& items) {
        categoryMenu->clear();
        for (const QString& item : items) {
            QAction* action = categoryMenu->addAction(item);
            connect(action, &QAction::triggered, this, [=]() {
                selectedCategory = item;
                categoryDropdownBtn->setText(item + " ▾");
            });
        }
    };

    // 초기 입금 카테고리 세팅 (수정 시 아래 setTransaction에서 재설정됨)
    setCategoryItems({"월급", "용돈", "기타"});
    categoryDropdownBtn->setMenu(categoryMenu);

    // 메모 입력창
    memoEdit = new QLineEdit(this);
    memoEdit->setPlaceholderText("메모 입력");
    memoEdit->setStyleSheet(R"(
        QLineEdit {
            font-size: 12px;
            border: none;
        }
    )");
    mainLayout->addWidget(memoEdit);

    // 구분선
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    // 거래 정보
    QVBoxLayout *detailLayout = new QVBoxLayout;
    detailLayout->setAlignment(Qt::AlignTop);

    QFont labelFont;
    labelFont.setPointSize(12);
    QString labelStyle = "color: #4F4F4F; font-size: 12px;";

    QLabel *labelDateTitle = new QLabel("거래 일시:");
    labelDateTitle->setStyleSheet(labelStyle);
    labelDateTitle->setFont(labelFont);

    dateLabel = new QLabel("");
    dateLabel->setStyleSheet(labelStyle);
    dateLabel->setFont(labelFont);

    QHBoxLayout *dateLayout = new QHBoxLayout;
    dateLayout->addWidget(labelDateTitle);
    dateLayout->addStretch();
    dateLayout->addWidget(dateLabel);
    detailLayout->addLayout(dateLayout);

    QLabel *labelAmountTitle = new QLabel("거래 금액:");
    labelAmountTitle->setStyleSheet(labelStyle);
    labelAmountTitle->setFont(labelFont);

    amountLabel = new QLabel("");
    amountLabel->setStyleSheet(labelStyle);
    amountLabel->setFont(labelFont);

    QHBoxLayout *amountLayout = new QHBoxLayout;
    amountLayout->addWidget(labelAmountTitle);
    amountLayout->addStretch();
    amountLayout->addWidget(amountLabel);
    detailLayout->addLayout(amountLayout);

    QLabel *labelTypeTitle = new QLabel("거래 유형:");
    labelTypeTitle->setStyleSheet(labelStyle);
    labelTypeTitle->setFont(labelFont);

    balanceLabel = new QLabel("");
    balanceLabel->setTextFormat(Qt::RichText);
    balanceLabel->setFont(labelFont);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(labelTypeTitle);
    typeLayout->addStretch();
    typeLayout->addWidget(balanceLabel);
    detailLayout->addLayout(typeLayout);

    mainLayout->addLayout(detailLayout);

    // 버튼 영역
    updateBtn = new QPushButton("수정하기", this);
    updateBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #B3D5FF;
            color: #1E40FF;
            font-weight: bold;
            font-size: 16px;
            border-radius: 10px;
        }
        QPushButton:hover {
            background-color: #1517D5;
            color: white;
        }
    )");
    updateBtn->setFixedHeight(45);
    connect(updateBtn, &QPushButton::clicked, this, &DetailTransaction::onUpdateClicked);

    deleteBtn = new QPushButton("삭제하기", this);
    deleteBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #D5D6DA;
            color: white;
            font-size: 16px;
            font-weight: bold;
            border-radius: 10px;
        }
        QPushButton:hover {
            background-color: #030303;
            color: white;
        }
    )");
    deleteBtn->setFixedHeight(45);
    connect(deleteBtn, &QPushButton::clicked, this, &DetailTransaction::onDeleteClicked);

    mainLayout->addWidget(updateBtn);
    mainLayout->addWidget(deleteBtn);
}

void DetailTransaction::setTransaction(const TransactionData &data)
{
    QStringList categories = data.isExpense ? QStringList{"식비", "교통", "쇼핑", "기타"}
                                            : QStringList{"월급", "용돈", "기타"};

    selectedCategory = data.category;
    categoryDropdownBtn->setText(data.category + " ▾");

    categoryMenu->clear();
    for (const QString& item : categories) {
        QAction* action = categoryMenu->addAction(item);
        connect(action, &QAction::triggered, this, [=]() {
            selectedCategory = item;
            categoryDropdownBtn->setText(item);
        });
    }

    memoEdit->setText(data.memo);
    dateLabel->setText(data.dateTime);

    QLocale locale = QLocale::system();
    QString formattedAmount = locale.toString(data.amount.toLongLong());
    amountLabel->setText(formattedAmount + "원");

    QString typeText = data.isExpense ? "출금" : "입금";
    QString typeColor = data.isExpense ? "#1E40FF" : "#E53935";

    balanceLabel->setText("<span style='color:" + typeColor + "; font-size:12px; font-weight: bold;'>" + typeText + "</span>");
    currentTransaction = data;
}

void DetailTransaction::onUpdateClicked()
{
    qDebug() << "수정 버튼 클릭!";
    QString newCategory = selectedCategory;  // 현재 선택된 카테고리 저장 변수
    QString newMemo = memoEdit->text();
    // 1. DB에 업데이트
    bool success = TransactionStore::updateTransaction(currentTransaction.id, newCategory, newMemo);
    if (!success) {
        QMessageBox::warning(this, "수정 실패", "DB 업데이트에 실패했습니다.");
        return;
    }
    // 2. 메모리 상 거래 정보도 업데이트
    for (TransactionData &item : TransactionStore::allTransactions) {
        if (item.id == currentTransaction.id) {
            item.category = newCategory;
            item.memo = newMemo;
            break;
        }
    }
    // 3. 현재 창 닫기 + 메인 화면 갱신 트리거
    emit transactionUpdated();
    this->close();
}


void DetailTransaction::onDeleteClicked()
{
    for (int i = 0; i < TransactionStore::allTransactions.size(); ++i) {
        const TransactionData &item = TransactionStore::allTransactions[i];
        if (item.dateTime == currentTransaction.dateTime && item.amount == currentTransaction.amount) {
            TransactionStore::allTransactions.removeAt(i);
            emit transactionDeleted();
            this->close();
            return;
        }
    }
}
